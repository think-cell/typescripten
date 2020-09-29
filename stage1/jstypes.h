#pragma once
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
ECppType CppType(tc::js::ts::Symbol jsymType) noexcept;

DEFINE_ENUM(ENameContext, enamectx, (NONE)(ENUM)(CLASS)(TYPEALIAS)(FUNCTION));
std::string CppifyName(tc::js::ts::Symbol jsymSymbol, ENameContext enamectx) noexcept;

struct SJsEnumOption final {
    tc::js::ts::Symbol m_jsym;
    std::string m_strJsName;
    std::string m_strCppifiedName;
    tc::js::ts::EnumMember m_jtsEnumMember;
    std::optional<std::variant<double, std::string>> m_ovardblstrValue;

    SJsEnumOption() = delete;
    SJsEnumOption(tc::js::ts::Symbol jsym) noexcept;    
    SJsEnumOption(SJsEnumOption&&) noexcept = default;
    SJsEnumOption& operator=(SJsEnumOption&&) noexcept = default;
};
static_assert(std::is_nothrow_move_constructible<SJsEnumOption>::value);
static_assert(std::is_nothrow_move_assignable<SJsEnumOption>::value);

struct SJsEnum final : public boost::intrusive::set_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>> {
    tc::js::ts::Symbol m_jsym;
    std::string m_strQualifiedName;
    std::string m_strCppifiedName;
    std::string m_strMangledName;

    std::vector<SJsEnumOption> m_vecjsenumoption;
    bool m_bIsIntegral;

    SJsEnum(tc::js::ts::Symbol jsymEnum) noexcept;
    SJsEnum(SJsEnum&&) noexcept = default;
    SJsEnum& operator=(SJsEnum&&) noexcept = default;

    void Initialize() noexcept;
};
static_assert(std::is_nothrow_move_constructible<SJsEnum>::value);
static_assert(std::is_nothrow_move_assignable<SJsEnum>::value);

struct SJsVariableLike final {
    tc::js::ts::Symbol m_jsym;
    std::string m_strJsName;
    std::string m_strCppifiedName;

private:
    tc::js::ts::Declaration m_jdeclVariableLike; // There may be multiple declarations, we ensure they do not conflict.
    std::optional<SMangledType> mutable m_omtType; // cached

public:
    tc::js::ts::Type m_jtypeDeclared;
    bool m_bReadonly;

    SJsVariableLike(tc::js::ts::Symbol jsymName) noexcept;
    SJsVariableLike(SJsVariableLike&&) noexcept = default;
    SJsVariableLike& operator=(SJsVariableLike&&) noexcept = default;

    SMangledType const& MangleType() const noexcept;
};
static_assert(std::is_nothrow_move_constructible<SJsVariableLike>::value);
static_assert(std::is_nothrow_move_assignable<SJsVariableLike>::value);

struct SJsFunctionLike final {
    tc::js::ts::Symbol m_jsym;
    std::string m_strCppifiedName;
    tc::js::ts::Signature m_jsignature;
    std::vector<SJsVariableLike> m_vecjsvariablelikeParameters;

private:
    // Cached
    std::string mutable m_strCanonizedParameterCppTypes;
    std::string const& CanonizedParameterCppTypes() const noexcept;

public:
    SJsFunctionLike(tc::js::ts::Symbol jsym, tc::js::ts::SignatureDeclaration jsigndecl) noexcept;
    SJsFunctionLike(SJsFunctionLike&&) noexcept = default;
    SJsFunctionLike& operator=(SJsFunctionLike&&) noexcept = default;

    std::string CppifiedParametersWithCommentsDecl() const noexcept;
    std::string CppifiedParametersWithCommentsDef() const noexcept;
   
    static bool LessCppSignature(SJsFunctionLike const& a, SJsFunctionLike const& b) noexcept;
};
static_assert(std::is_nothrow_move_constructible<SJsFunctionLike>::value);
static_assert(std::is_nothrow_move_assignable<SJsFunctionLike>::value);

template<typename Rng>
void MergeWithSameCppSignatureInplace(Rng& rngjsfunctionlikeFuncs) noexcept {
    tc::sort_unique_inplace(
        rngjsfunctionlikeFuncs,
        &SJsFunctionLike::LessCppSignature
    );
    // TODO: add comments about skipped overloads;
}

struct SJsClass;
struct SJsTypeAlias;

struct SJsScope {
    std::vector<SJsEnum> m_vecvarjsExportEnum;
    std::vector<SJsClass> m_vecvarjsExportClass;
    std::vector<SJsTypeAlias> m_vecvarjsExportTypeAlias;
    std::vector<SJsFunctionLike> m_vecjsfunctionlikeExportFunction;
    std::vector<SJsVariableLike> m_vecjsvariablelikeExportVariable;

    template<typename Rng>
    SJsScope(Rng&& rngjsym) noexcept;
    SJsScope(SJsScope&&) noexcept = default;
    SJsScope& operator=(SJsScope&&) noexcept = default;

protected:
    SJsScope() noexcept = default;

    template<typename Rng>
    void Initialize(Rng&& rngjsym) noexcept;
};
static_assert(std::is_nothrow_move_constructible<SJsScope>::value);
static_assert(std::is_nothrow_move_assignable<SJsScope>::value);

struct SJsClass final : public SJsScope, public boost::intrusive::set_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>> { 
    tc::js::ts::Symbol m_jsym;
    std::string m_strQualifiedName;
    std::string m_strCppifiedName;
    std::string m_strMangledName;

    std::vector<SJsFunctionLike> m_vecjsfunctionlikeCtor;
    std::vector<SJsFunctionLike> m_vecjsfunctionlikeMethod;
    std::vector<SJsVariableLike> m_vecjsvariablelikeProperty;
    std::vector<SJsClass const*> m_vecpjsclassBase;
    std::vector<tc::js::ts::Symbol> m_vecjsymBaseUnknown;

    bool m_bHasImplicitDefaultConstructor;

    SJsClass(tc::js::ts::Symbol jsymClass) noexcept;
    SJsClass(SJsClass&&) noexcept = default;
    SJsClass& operator=(SJsClass&&) noexcept = default;
    
    void Initialize() noexcept;
    void ResolveBaseClasses() noexcept;
};
static_assert(std::is_nothrow_move_constructible<SJsClass>::value);
static_assert(std::is_nothrow_move_assignable<SJsClass>::value);

struct SJsTypeAlias final : public boost::intrusive::set_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>> {
    tc::js::ts::Symbol m_jsym;
    tc::js::ts::TypeNode m_jtypenode;
    tc::js::ts::Type m_jtype;

    std::string m_strQualifiedName;
    std::string m_strCppifiedName;
    std::string m_strMangledName;

    SJsTypeAlias(tc::js::ts::Symbol jsym) noexcept;
    SJsTypeAlias(SJsTypeAlias&&) noexcept = default;
    SJsTypeAlias& operator=(SJsTypeAlias&&) noexcept = default;

    void Initialize() noexcept;

    SMangledType MangleType() const noexcept;
};
static_assert(std::is_nothrow_move_constructible<SJsTypeAlias>::value);
static_assert(std::is_nothrow_move_assignable<SJsTypeAlias>::value);

template<typename JsXXX>
struct FQualifiedNameExtractorT {
    using type = std::string;
    type operator()(JsXXX const& js) const& noexcept {
        return js.m_strQualifiedName;
    }
};

using SetJsEnum = boost::intrusive::set<
    SJsEnum,
    boost::intrusive::constant_time_size<false>,
    boost::intrusive::key_of_value<FQualifiedNameExtractorT<SJsEnum>>
>;
extern SetJsEnum g_setjsenum;

using SetJsClass = boost::intrusive::set<
    SJsClass,
    boost::intrusive::constant_time_size<false>,
    boost::intrusive::key_of_value<FQualifiedNameExtractorT<SJsClass>>
>;
extern SetJsClass g_setjsclass;

using SetJsTypeAlias = boost::intrusive::set<
    SJsTypeAlias,
    boost::intrusive::constant_time_size<false>,
    boost::intrusive::key_of_value<FQualifiedNameExtractorT<SJsTypeAlias>>
>;
extern SetJsTypeAlias g_setjstypealias;

template<typename Rng>
SJsScope::SJsScope(Rng&& rngjsym) noexcept {
    Initialize(std::forward<Rng>(rngjsym));
}

template<typename Rng>
void SJsScope::Initialize(Rng&& rngjsym) noexcept {
     tc::for_each(rngjsym, [&](tc::js::ts::Symbol const& jsymType) noexcept {
        // Use tc::cont_must_emplace to make sure we only create a single enum/class/typealias per symbol.
        // We assume that ts::Symbol::exports() returns a symbol list without duplicates.
        auto const ecpptype = CppType(jsymType);
        if(ecpptypeENUM&ecpptype) {
            m_vecvarjsExportEnum.emplace_back(jsymType);
        }
        if(ecpptypeCLASS&ecpptype) {
            m_vecvarjsExportClass.emplace_back(jsymType);
        }
        if(ecpptypeTYPEALIAS&ecpptype) {
            m_vecvarjsExportTypeAlias.emplace_back(jsymType); 
        }
    });
    tc::for_each(m_vecvarjsExportEnum, TC_MEMBER(.Initialize()));
    tc::for_each(m_vecvarjsExportClass, TC_MEMBER(.Initialize()));
    tc::for_each(m_vecvarjsExportTypeAlias, TC_MEMBER(.Initialize()));
    
    tc::for_each(
        tc::filter(
            rngjsym,
            [](tc::js::ts::Symbol jsymExport) noexcept {
                return static_cast<bool>(tc::js::ts::SymbolFlags::Function & jsymExport->getFlags());
            }
        ),
        [&](tc::js::ts::Symbol jsymFunction) noexcept {
            tc::for_each(
                jsymFunction->declarations(),
                [=](tc::js::ts::Declaration jdeclFunction) noexcept {
                    if(auto const ojsfuncdecl = tc::js::ts::isFunctionDeclaration(jdeclFunction)) {
                        tc::cont_emplace_back(m_vecjsfunctionlikeExportFunction, SJsFunctionLike(jsymFunction, *ojsfuncdecl));
                    }
                }
            );
        }
    );

    m_vecjsvariablelikeExportVariable = tc::make_vector(tc::transform(
        tc::filter(
            rngjsym,
            [](tc::js::ts::Symbol jsymExport) noexcept {
                // TODO: Leave equality check here for now. 
                // tc::js::ts::SymbolFlags::FunctionScopedVariable|tc::js::ts::SymbolFlags::Interface is not turned into a variable but is 
                // merged into a class
                return
                    tc::js::ts::SymbolFlags::FunctionScopedVariable == jsymExport->getFlags() ||
                    tc::js::ts::SymbolFlags::BlockScopedVariable == jsymExport->getFlags();
            }
        ),
        [&](tc::js::ts::Symbol jsymVariable) noexcept {
            return SJsVariableLike(jsymVariable);
        }
    ));

    // Merge variable redeclarations inplace 
    tc::sort_accumulate_each_unique_range(
        m_vecjsvariablelikeExportVariable,
        [](SJsVariableLike const& a, SJsVariableLike const& b) { return a.m_strJsName < b.m_strJsName; },
        [](SJsVariableLike const& first, SJsVariableLike const& current) {
            _ASSERT(current.m_jsym.getEmval().strictlyEquals(first.m_jsym.getEmval()));
        }
    );
}