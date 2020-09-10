#include "precompiled.h"
#include "typescript.d.bootstrap.h"
#include "mangle.h"
#include "walk_symbol.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/key.hpp>

enum ECppType {
    ecpptypeIGNORE = 0,
    ecpptypeENUM = 1,
    ecpptypeCLASS = 2,
    ecpptypeTYPEALIAS = 4
};
BITMASK_OPS(ECppType);
ECppType CppType(tc::js::ts::TypeChecker jtsTypeChecker, tc::js::ts::Symbol jsymType) noexcept;

struct SJsEnumOption {
    tc::js::ts::Symbol m_jsymOption;
    std::string m_strJsName;
    std::string m_strCppifiedName;
    tc::js::ts::EnumMember m_jtsEnumMember;
    std::optional<std::variant<double, std::string>> m_ovardblstrValue;

    SJsEnumOption(tc::js::ts::TypeChecker const jtsTypeChecker, tc::js::ts::Symbol const jsymOption) noexcept;
};

struct SJsEnum {
    tc::js::ts::Symbol m_jsymEnum;
    std::string m_strMangledName;
    std::vector<SJsEnumOption> m_vecjsenumoption;
    bool m_bIsIntegral;

    SJsEnum(tc::js::ts::TypeChecker const jtsTypeChecker, tc::js::ts::Symbol const jsymEnum) noexcept;
};

struct SJsVariableLike {
    tc::js::ts::Symbol m_jsymName;
    std::string m_strJsName;
    std::string m_strCppifiedName;
private:
    tc::js::ts::Declaration m_jdeclVariableLike; // There may be multiple declarations, we ensure they do not conflict.
public:
    tc::js::ts::Type m_jtypeDeclared;
    SMangledType m_mtType;
    bool m_bReadonly;

    SJsVariableLike(tc::js::ts::TypeChecker const jtsTypeChecker, tc::js::ts::Symbol const jsymName) noexcept;
};

struct SJsFunctionLike {
    tc::js::ts::Symbol m_jsymFunctionLike;
    std::string m_strCppifiedName;
    tc::js::ts::Declaration m_jdeclFunctionLike;
    tc::js::ts::SignatureDeclaration m_jtsSignatureDeclaration;
    tc::js::ts::Signature m_jtsSignature;
    tc::jst::js_union<tc::js::Array<tc::js::ts::TypeParameter>, tc::jst::js_undefined> m_joptarrunkTypeParameter;
    std::vector<SJsVariableLike> m_vecjsvariablelikeParameters;
    std::string m_strCppifiedParametersWithCommentsDecl;
    std::string m_strCppifiedParametersWithCommentsDef;
    std::string m_strCanonizedParameterCppTypes;

    SJsFunctionLike(tc::js::ts::TypeChecker const jtsTypeChecker, tc::js::ts::Symbol const jsymFunctionLike, tc::js::ts::Declaration const jdeclFunctionLike) noexcept;

    static bool LessCppSignature(SJsFunctionLike const& a, SJsFunctionLike const& b) noexcept;
};

template<typename Rng>
void MergeWithSameCppSignatureInplace(Rng& rngjsfunctionlikeFuncs) noexcept {
    tc::sort_unique_inplace(
        rngjsfunctionlikeFuncs,
        &SJsFunctionLike::LessCppSignature
    );
    // TODO: add comments about skipped overloads;
}

struct SJsScope {
    std::vector<tc::js::ts::Symbol> m_vecjsymExportType;
    std::vector<SJsFunctionLike> m_vecjsfunctionlikeExportFunction;
    std::vector<SJsVariableLike> m_vecjsvariablelikeExportVariable;

    template<typename Rng>
    SJsScope(tc::js::ts::TypeChecker const jtsTypeChecker, Rng&& rngjsym) noexcept;
    void WalkSymbols(tc::js::ts::TypeChecker jtsTypeChecker) const noexcept;

protected:
    SJsScope() noexcept = default;

    template<typename Rng>
    void Initialize(tc::js::ts::TypeChecker const jtsTypeChecker, Rng&& rngjsym) noexcept;
};

struct SJsClass final : SJsScope { 
    tc::js::ts::Symbol m_jsymClass;
    std::string m_strMangledName;
    std::vector<SJsFunctionLike> m_vecjsfunctionlikeMethod;
    std::vector<SJsVariableLike> m_vecjsvariablelikeProperty;
    std::vector<tc::js::ts::Symbol> m_vecjsymBaseClass;
    bool m_bHasImplicitDefaultConstructor;

    SJsClass(tc::js::ts::TypeChecker const jtsTypeChecker, tc::js::ts::Symbol const jsymClass) noexcept;
};

struct SJsTypeAlias final {
    tc::js::ts::TypeNode m_jtypenode;
    tc::js::ts::Type m_jtype;

    std::string m_strMangledName;
    SMangledType m_mt;

    SJsTypeAlias(tc::js::ts::TypeChecker jtsTypeChecker, tc::js::ts::Symbol jsym) noexcept;
};

using SetJsEnum = 
	boost::multi_index_container<
		SJsEnum,
		boost::multi_index::indexed_by<	
			boost::multi_index::ordered_unique<
				boost::multi_index::key<&SJsEnum::m_strMangledName>
			>
		>
	>;
extern SetJsEnum g_setjsenum;

using SetJsClass = 
	boost::multi_index_container<
		SJsClass,
		boost::multi_index::indexed_by<	
			boost::multi_index::sequenced <>,
			boost::multi_index::ordered_unique<
				boost::multi_index::key<&SJsClass::m_strMangledName>
			>
		>
	>;
extern SetJsClass g_setjsclass;

using SetJsTypeAlias = 
	boost::multi_index_container<
		SJsTypeAlias,
		boost::multi_index::indexed_by<	
            boost::multi_index::sequenced <>,
			boost::multi_index::ordered_unique<
				boost::multi_index::key<&SJsTypeAlias::m_strMangledName>
			>
		>
	>;
extern SetJsTypeAlias g_setjstypealias;

template<typename Rng>
SJsScope::SJsScope(tc::js::ts::TypeChecker const jtsTypeChecker, Rng&& rngjsym) noexcept {
    Initialize(jtsTypeChecker, std::forward<Rng>(rngjsym));
}

template<typename Rng>
void SJsScope::Initialize(tc::js::ts::TypeChecker const jtsTypeChecker, Rng&& rngjsym) noexcept {
    m_vecjsymExportType = tc::make_vector(tc::filter(
        rngjsym,
        [&](tc::js::ts::Symbol jsymExport) noexcept {
            return ecpptypeIGNORE!=CppType(jtsTypeChecker, jsymExport);
        }
    ));

    m_vecjsfunctionlikeExportFunction = tc::make_vector(tc::join(tc::transform(
        tc::filter(
            rngjsym,
            [](tc::js::ts::Symbol const jsymExport) noexcept {
                return tc::js::ts::SymbolFlags::Function == jsymExport->getFlags();
            }
        ),
        [&](tc::js::ts::Symbol const jsymFunction) noexcept {
            return tc::transform(
                jsymFunction->declarations(),
                [&jtsTypeChecker, jsymFunction](tc::js::ts::Declaration const jdeclFunction) noexcept {
                    return SJsFunctionLike(jtsTypeChecker, jsymFunction, jdeclFunction);
                }
            );
        }
    )));
    MergeWithSameCppSignatureInplace(m_vecjsfunctionlikeExportFunction);

    m_vecjsvariablelikeExportVariable = tc::make_vector(tc::transform(
        tc::filter(
            rngjsym,
            [](tc::js::ts::Symbol const jsymExport) noexcept {
                return
                    tc::js::ts::SymbolFlags::FunctionScopedVariable == jsymExport->getFlags() ||
                    tc::js::ts::SymbolFlags::BlockScopedVariable == jsymExport->getFlags();
            }
        ),
        [&](tc::js::ts::Symbol const jsymVariable) noexcept {
            return SJsVariableLike(jtsTypeChecker, jsymVariable);
        }
    ));

    // Merge variable redeclarations inplace 
    tc::sort_accumulate_each_unique_range(
        m_vecjsvariablelikeExportVariable,
        [](SJsVariableLike const& a, SJsVariableLike const& b) { return a.m_strJsName < b.m_strJsName; },
        [](SJsVariableLike const& first, SJsVariableLike const& current) {
            _ASSERT(current.m_jsymName.getEmval().strictlyEquals(first.m_jsymName.getEmval()));
        }
    );
}