#pragma once
#include "mangle.h"
#include "walk_symbol.h"

enum ECppType {
	ecpptypeIGNORE = 0,
	ecpptypeENUM = 1,
	ecpptypeCLASS = 2,
	ecpptypeTYPEALIAS = 4
};
BITMASK_OPS(ECppType);
ECppType CppType(tc::js::ts::Symbol jsymType) noexcept;
bool IsIgnoredSymbol(std::string const& strFullyQualified) noexcept;

DEFINE_ENUM(ENameContext, enamectx, (NONE)(ENUM)(CLASS)(TYPEALIAS)(FUNCTION));
struct SJsEnumOption final {
	tc::js::ts::Symbol m_jsym;
	std::string m_strJsName;
	std::string m_strCppifiedName;
	tc::js::ts::EnumMember m_jenummember;
	std::variant<std::monostate, double, std::string> m_vardblstrValue;

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
	// enum E {
	//  a = 0,
	//  b = 1,
	//  c = 1
	// }
	// has 3 enum options but only two sub types.
	int m_cTypes;
	bool m_bIsIntegral;

	SJsEnum(tc::js::ts::Symbol jsymEnum) noexcept;
	SJsEnum(SJsEnum&&) noexcept = default;
	SJsEnum& operator=(SJsEnum&&) noexcept = default;

	void Initialize() & noexcept;
};
static_assert(std::is_nothrow_move_constructible<SJsEnum>::value);
static_assert(std::is_nothrow_move_assignable<SJsEnum>::value);

struct SJsVariableLike {
	tc::js::ts::Symbol m_jsym;
	std::string m_strJsName;
	std::string m_strCppifiedName;

  protected:
	tc::js::ts::Declaration m_jdeclVariableLike; // There may be multiple declarations, we ensure they do not conflict.
	std::optional<SMangledType> mutable m_omtType; // cached

  public:
	tc::js::ts::Type m_jtypeDeclared;
	bool m_bReadonly;

	SJsVariableLike(tc::js::ts::Symbol jsymName) noexcept;
	SJsVariableLike(SJsVariableLike&&) noexcept = default;
	SJsVariableLike& operator=(SJsVariableLike&&) noexcept = default;

	bool IsVoid() const& noexcept;

	// FIXME: We must generalize this
	// https://github.com/think-cell/tcjs/issues/3
	// All data structures should point to their parent (e.g. parameter -> function -> class -> class)
	// In circumstances where we want to mangle type parameters as types, we need to walk this list
	// and resolve the type parameter to a type.
	template<typename RngTypeParam>
	SMangledType MangleType(RngTypeParam const& rngtypeparam) const& noexcept {
		if(!m_omtType) {
			m_omtType = ::MangleType(m_jtypeDeclared);
		}

		if(ts::TypeFlags::TypeParameter == m_jtypeDeclared->getFlags()) {
			if(auto const ittypeparam =
				   tc::find_first<tc::return_element_or_null>(
					   tc::transform(rngtypeparam, TC_MEMBER(.m_strName)), m_omtType->ExpandType()
				   )
					   .element_base())
			{
				if(etypeparamTYPE != ittypeparam->m_etypeparam && etypeparamKEYOF != ittypeparam->m_etypeparam) {
					return {ittypeparam->Type()};
				}
			}
		}
		return *m_omtType;
	}
};

struct SJsParameter : SJsVariableLike {
	bool m_bVariadic = false; // test(...param: any[]) is really a method with variadic arguments

	SJsParameter(tc::js::ts::Symbol jsymName) noexcept;
	SJsParameter(SJsParameter&&) noexcept = default;
	SJsParameter& operator=(SJsParameter&&) noexcept = default;
};

static_assert(std::is_nothrow_move_constructible<SJsVariableLike>::value);
static_assert(std::is_nothrow_move_assignable<SJsVariableLike>::value);

struct STypeParameter final {
	ETypeParameter m_etypeparam;
	std::string m_strName;
	tc::jst::optional<tc::js::ts::Type> m_ojtype;

	STypeParameter(tc::js::ts::TypeParameterDeclaration) noexcept;
	std::string Type() const& noexcept;
};

struct SJsFunctionLike final {
	tc::js::ts::Symbol m_jsym;
	tc::js::ts::Signature m_jsignature;
	std::vector<SJsParameter> m_vecjsparam;
	std::vector<STypeParameter> m_vectypeparam;

	std::string m_strCppifiedName;
	bool m_bIndexSignature;

  private:
	// Cached
	std::string mutable m_strCanonizedParameterCppTypes;
	std::string const& CanonizedParameterCppTypes() const& noexcept;

	SJsFunctionLike(tc::js::ts::Symbol jsym, tc::js::ts::SignatureDeclaration jsigndecl, bool bIndexSignature) noexcept;

  public:
	SJsFunctionLike(tc::js::ts::Symbol jsym, tc::js::ts::SignatureDeclaration jsigndecl) noexcept;
	SJsFunctionLike(tc::js::ts::Symbol jsym, tc::js::ts::IndexSignatureDeclaration jsigndecl) noexcept;

	SJsFunctionLike(SJsFunctionLike&&) noexcept = default;
	SJsFunctionLike& operator=(SJsFunctionLike&&) noexcept = default;

	std::string CppifiedParametersWithCommentsDecl() const& noexcept;
	std::string CppifiedParametersWithCommentsDef() const& noexcept;

	static bool LessCppSignature(SJsFunctionLike const& a, SJsFunctionLike const& b) noexcept;
};
static_assert(std::is_nothrow_move_constructible<SJsFunctionLike>::value);
static_assert(std::is_nothrow_move_assignable<SJsFunctionLike>::value);

template<typename Rng>
void MergeWithSameCppSignatureInplace(Rng& rngjsfunctionlikeFuncs) noexcept {
	tc::sort_unique_inplace(rngjsfunctionlikeFuncs, &SJsFunctionLike::LessCppSignature);
	// TODO: add comments about skipped overloads;
}

struct SJsClass;
struct SJsTypeAlias;

struct SJsScope {
	std::vector<SJsEnum> m_vecjsenumExport;
	std::vector<SJsClass> m_vecjsclassExport;
	std::vector<SJsTypeAlias> m_vecjstypealiasExport;
	std::vector<SJsFunctionLike> m_vecjsfunctionlikeExport;
	std::vector<SJsVariableLike> m_vecjsvariablelikeExport;

	template<typename Rng>
	SJsScope(Rng&& rngjsym) noexcept;
	SJsScope(SJsScope&&) noexcept = default;
	SJsScope& operator=(SJsScope&&) noexcept = default;

  protected:
	SJsScope() noexcept = default;

	template<typename Rng>
	void Initialize(Rng&& rngjsym) & noexcept;
};
static_assert(std::is_nothrow_move_constructible<SJsScope>::value);
static_assert(std::is_nothrow_move_assignable<SJsScope>::value);

struct SJsClass final
	: public SJsScope
	, public boost::intrusive::set_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>> {
	tc::js::ts::Symbol m_jsym;
	std::string m_strQualifiedName;
	std::string m_strCppifiedName;
	std::string m_strMangledName;

	std::vector<SJsFunctionLike> m_vecjsfunctionlikeCtor;
	std::vector<SJsFunctionLike> m_vecjsfunctionlikeMethod;
	std::vector<SJsVariableLike> m_vecjsvariablelikeProperty;
	std::vector<SJsClass const*> m_vecpjsclassSortDependency;
	std::vector<SMangledType> m_vecmtBaseClass; // Also contains implemented interfaces, superset of m_vecpjsclassBaseClass

	std::vector<STypeParameter> m_vectypeparam;

	bool m_bHasImplicitDefaultConstructor;
	tc::js::ts::Type m_jtype;

	SJsClass(tc::js::ts::Symbol jsymClass) noexcept;
	SJsClass(SJsClass&&) noexcept = default;
	SJsClass& operator=(SJsClass&&) noexcept = default;

	void Initialize() & noexcept;
	void ResolveBaseClassesAndSortDependencies() & noexcept;
};
static_assert(std::is_nothrow_move_constructible<SJsClass>::value);
static_assert(std::is_nothrow_move_assignable<SJsClass>::value);

struct SJsTypeAlias final
	: public boost::intrusive::set_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>> {
	tc::js::ts::Symbol m_jsym;
	tc::js::ts::TypeNode m_jtypenode;
	tc::js::ts::Type m_jtype;

	std::string m_strQualifiedName;
	std::string m_strCppifiedName;
	std::string m_strMangledName;

	std::vector<STypeParameter> m_vectypeparam;

	SJsTypeAlias(tc::js::ts::Symbol jsym) noexcept;
	SJsTypeAlias(SJsTypeAlias&&) noexcept = default;
	SJsTypeAlias& operator=(SJsTypeAlias&&) noexcept = default;

	void Initialize() & noexcept;

	SMangledType MangleType() const& noexcept;
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

using SetJsEnum = boost::intrusive::
	set<SJsEnum, boost::intrusive::constant_time_size<false>, boost::intrusive::key_of_value<FQualifiedNameExtractorT<SJsEnum>>>;
extern SetJsEnum g_setjsenum;

using SetJsClass = boost::intrusive::
	set<SJsClass, boost::intrusive::constant_time_size<false>, boost::intrusive::key_of_value<FQualifiedNameExtractorT<SJsClass>>>;
extern SetJsClass g_setjsclass;

using SetJsTypeAlias = boost::intrusive::set<
	SJsTypeAlias,
	boost::intrusive::constant_time_size<false>,
	boost::intrusive::key_of_value<FQualifiedNameExtractorT<SJsTypeAlias>>>;
extern SetJsTypeAlias g_setjstypealias;

template<typename Rng>
SJsScope::SJsScope(Rng&& rngjsym) noexcept {
	Initialize(std::forward<Rng>(rngjsym));
}

template<typename Rng>
void SJsScope::Initialize(Rng&& rngjsym) & noexcept {
	tc::for_each(rngjsym, [&](tc::js::ts::Symbol const& jsymType) noexcept {
		// We assume that ts::Symbol::exports() returns a symbol list without duplicates.
		auto const str = FullyQualifiedName(jsymType);
		if(!IsBootstrapType(str) && !IsIgnoredSymbol(str)) {
			auto const ecpptype = CppType(jsymType);
			if(ecpptypeENUM & ecpptype) {
				m_vecjsenumExport.emplace_back(jsymType);
			}
			if(ecpptypeCLASS & ecpptype) {
				m_vecjsclassExport.emplace_back(jsymType);
			}
			if(ecpptypeTYPEALIAS & ecpptype) {
				m_vecjstypealiasExport.emplace_back(jsymType);
			}
		}
	});
	tc::for_each(m_vecjsenumExport, TC_MEMBER(.Initialize()));
	tc::for_each(m_vecjsclassExport, TC_MEMBER(.Initialize()));
	tc::for_each(m_vecjstypealiasExport, TC_MEMBER(.Initialize()));

	tc::for_each(
		tc::filter(
			rngjsym,
			[](tc::js::ts::Symbol jsymExport) noexcept {
				return static_cast<bool>(tc::js::ts::SymbolFlags::Function & jsymExport->getFlags());
			}
		),
		[&](tc::js::ts::Symbol jsymFunction) noexcept {
			tc::for_each(*jsymFunction->declarations(), [=](tc::js::ts::Declaration jdeclFunction) noexcept {
				if(auto const ojsfuncdecl = tc::js::ts::isFunctionDeclaration(jdeclFunction)) {
					tc::cont_emplace_back(m_vecjsfunctionlikeExport, SJsFunctionLike(jsymFunction, *ojsfuncdecl));
				}
			});
		}
	);

	m_vecjsvariablelikeExport = tc::make_vector(tc::filter(
		tc::transform(
			tc::filter(
				rngjsym,
				[](tc::js::ts::Symbol jsymExport) noexcept {
					// tc::js::ts::SymbolFlags::FunctionScopedVariable|tc::js::ts::SymbolFlags::Interface is not turned
					// into a variable but is merged into a class see e.g. XmlHttpRequest in lib.dom.d.ts
					return (static_cast<bool>(tc::js::ts::SymbolFlags::FunctionScopedVariable & jsymExport->getFlags())
							|| static_cast<bool>(tc::js::ts::SymbolFlags::BlockScopedVariable & jsymExport->getFlags()))
						   && !static_cast<bool>(tc::js::ts::SymbolFlags::Interface & jsymExport->getFlags())
						   && !static_cast<bool>(tc::js::ts::SymbolFlags::TypeAlias & jsymExport->getFlags());
				}
			),
			[](tc::js::ts::Symbol jsymVariable) noexcept { return SJsVariableLike(jsymVariable); }
		),
		[](auto const& jsvariablelike) noexcept { return !jsvariablelike.IsVoid(); }
	));

	// Merge variable redeclarations inplace
	tc::sort_accumulate_each_unique_range(
		m_vecjsvariablelikeExport,
		[](SJsVariableLike const& a, SJsVariableLike const& b) { return a.m_strJsName < b.m_strJsName; },
		[](SJsVariableLike const& first, SJsVariableLike const& current) {
			_ASSERT(current.m_jsym.getEmval().strictlyEquals(first.m_jsym.getEmval()));
		}
	);
}