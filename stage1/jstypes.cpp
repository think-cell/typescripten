#include "jstypes.h"

using tc::js::Array;
using tc::js::console;
using tc::js::ReadonlyArray;
using tc::js::ts;
using tc::jst::js_optional;
using tc::jst::js_string;
using tc::jst::js_unknown;

extern std::optional<tc::js::ts::TypeChecker> g_ojtsTypeChecker;

ECppType CppType(ts::Symbol jsymType) noexcept {
    ECppType ecpptype = ecpptypeIGNORE;
	if(static_cast<bool>((ts::SymbolFlags::RegularEnum|ts::SymbolFlags::ConstEnum) & jsymType->getFlags())) {
		ecpptype |= ecpptypeENUM;
	}

	if(static_cast<bool>((ts::SymbolFlags::Class|ts::SymbolFlags::Interface|ts::SymbolFlags::ValueModule|ts::SymbolFlags::NamespaceModule) & jsymType->getFlags())) {
		ecpptype |= ecpptypeCLASS;
	}

	if(static_cast<bool>(ts::SymbolFlags::TypeAlias&jsymType->getFlags())) {
		// Emit type aliases as using declarations if the type alias only references types, not e.g. literals
		// A type alias of a single or a union of literals should be transformed into tag structs in C++
		auto IsObject = [](auto jtypeInternal) noexcept {
			switch(jtypeInternal->flags()) {
			case ts::TypeFlags::Any:
			case ts::TypeFlags::Unknown:
			case ts::TypeFlags::String:
			case ts::TypeFlags::Number:
			case ts::TypeFlags::Boolean:
			case ts::TypeFlags::Enum:
			case ts::TypeFlags::BigInt:
			case ts::TypeFlags::Undefined:
			case ts::TypeFlags::Object:
				return true;
			default:
				return false;
			}
		};

		auto const jtype = (*g_ojtsTypeChecker)->getTypeFromTypeNode(tc::js::ts::TypeAliasDeclaration(
            tc::find_first_if<tc::return_value>(
                jsymType->declarations(), 
                [](tc::js::ts::Declaration decl) noexcept -> bool {
                    return tc::js::ts::isTypeAliasDeclaration(decl);
                }
            )
        )->type());
		auto const ojuniontype = tc::js::ts_ext::isUnion(jtype);
		if((ojuniontype && tc::all_of((*ojuniontype)->types(), IsObject))
		|| IsObject(jtype)) {
			ecpptype |= ecpptypeTYPEALIAS;
		}
	}
	return ecpptype;
}

namespace {
    // Symbols in typescript can be declared twice in some circumstances. Make the names unique 
    // if this happens.
    std::string MakeUniqueName(ts::Symbol jsym, std::string strName, ENameContext enamectx) noexcept {
        auto const ecpptype = CppType(jsym);
        switch_no_default(enamectx) {
        case enamectxNONE: 
        case enamectxFUNCTION: 
            // Functions are not renamed. All other contexts check for collision with function name
            break;
        case enamectxENUM: 
            _ASSERT(!static_cast<bool>(ts::SymbolFlags::Function & jsym->getFlags())); // enum and function names cannot collide
            if(ecpptype!=ecpptypeENUM) {
                tc::append(strName, "_enum");
            }
            break;
        case enamectxCLASS:
            if(ecpptype!=ecpptypeCLASS || static_cast<bool>(ts::SymbolFlags::Function & jsym->getFlags())) {
                tc::append(strName, "_class");
            }
            break;
        case enamectxTYPEALIAS:
            if(ecpptype!=ecpptypeTYPEALIAS || static_cast<bool>(ts::SymbolFlags::Function & jsym->getFlags())) {
                tc::append(strName, "_alias");
            }
            break;
        }
        return strName;
    }

    // MangleSymbolName returns the name used internally that should be globally unique 
    // (that is why it is based on the fully qualified name).
    // We define all classes in a single namespace, sorted in a C++ compatible declaration order,
    // and export them with using declarations.
    std::string MangleSymbolName(ts::Symbol jsymType, ENameContext enamectx) noexcept {
        std::string strMangled = "_js_j";
        tc::for_each(FullyQualifiedName(jsymType), [&](char c) noexcept {
            switch (c) {
            case '_': tc::append(strMangled, "_u"); break;
            case ',': tc::append(strMangled, "_c"); break;
            case '.': tc::append(strMangled, "_d"); break;
            case '-': tc::append(strMangled, "_m"); break;
            case '"': tc::append(strMangled, "_q"); break;
            default: tc::cont_emplace_back(strMangled, c); break;
            }
        });
       return MakeUniqueName(jsymType, strMangled, enamectx);
    }
}

// CppifyName returns the user-visible name that we export from namespaces/classes
std::string CppifyName(ts::Symbol jsymSymbol, ENameContext enamectx) noexcept {
    std::string strSourceName = tc::explicit_cast<std::string>(jsymSymbol->getName());
    if ('"' == strSourceName.front() && '"' == strSourceName.back()) {
        strSourceName = strSourceName.substr(1, strSourceName.length() - 2);
    }
    // TODO: https://en.cppreference.com/w/cpp/language/identifiers
    std::string strResult;
    bool bLastIsUnderscore = false;
    tc::for_each(
        tc::transform(strSourceName, [&](char c) noexcept {
            if(('a' <= c && c <= 'z')
            || ('A' <= c && c <= 'Z')
            || ('0' <= c && c <= '9')
            || '_' == c) {
                return c;
            } else {
                return '_';
            }
        }),
        [&](char c) noexcept {
            if (bLastIsUnderscore && c == '_') {
                tc::append(strResult, "u_u"); // ABC__DEF --> ABC_u_uDEF
                return;
            }
            bLastIsUnderscore = c == '_';
            strResult += c;
        }
    );
    static std::unordered_set<std::string> usstrCppKeywords = {"alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor", "bool", "break", "case", "catch", "char", "char16_t", "char32_t", "class", "compl", "const", "constexpr", "const_cast", "continue", "decltype", "default", "delete", "do", "double", "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "float", "for", "friend", "goto", "if", "inline", "int", "long", "mutable", "namespace", "new", "noexcept", "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private", "protected", "public", "register", "reinterpret_cast", "return", "short", "signed", "sizeof", "static", "static_assert", "static_cast", "struct", "switch", "template", "this", "thread_local", "throw", "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while", "xor", "xor_eq"};
    if (usstrCppKeywords.count(strResult)) {
        tc::append(strResult, "_");
    }
    _ASSERT(!strResult.empty());
    return MakeUniqueName(jsymSymbol, strResult, enamectx);
}

SJsEnumOption::SJsEnumOption(ts::Symbol jsym) noexcept
    : m_jsym([&]() noexcept {
        _ASSERTEQUAL(jsym->getFlags(), ts::SymbolFlags::EnumMember);
        return jsym;
    }())
    , m_strJsName(tc::explicit_cast<std::string>(jsym->getName()))
    , m_strCppifiedName(CppifyName(jsym, enamectxNONE))
    , m_jtsEnumMember([&]() noexcept {
        auto const jarrDeclaration = jsym->declarations();
        _ASSERTEQUAL(jarrDeclaration->length(), 1);
        return ts::EnumMember(jarrDeclaration[0]);
    }())
    , m_ovardblstrValue([&]() noexcept -> decltype(m_ovardblstrValue) {
        _ASSERTEQUAL(ts::getCombinedModifierFlags(m_jtsEnumMember), ts::ModifierFlags::None);
        auto const junionOptionValue = (*g_ojtsTypeChecker)->getConstantValue(m_jtsEnumMember);
        if (junionOptionValue.getEmval().isNumber()) {
            return junionOptionValue.get<double>();
        } else if (junionOptionValue.getEmval().isString()) {
            return tc::explicit_cast<std::string>(junionOptionValue.get<js_string>());
        } else if (junionOptionValue.getEmval().isUndefined()) {
            return std::nullopt;  // Uncomputed value.
        } else {
            _ASSERTFALSE;
        }
    }())
{}

SJsEnum::SJsEnum(ts::Symbol jsym) noexcept
    : m_jsym(jsym)
    , m_strQualifiedName(FullyQualifiedName(m_jsym))
    , m_strCppifiedName(CppifyName(m_jsym, enamectxENUM))
    , m_strMangledName(MangleSymbolName(m_jsym, enamectxENUM))
    , m_vecjsenumoption(tc::make_vector(tc::transform(
        tc::filter(
            *tc::js::ts_ext::Symbol(m_jsym)->exports(),
            [](ts::Symbol const jsymExport) noexcept {
                return static_cast<bool>(ts::SymbolFlags::EnumMember&jsymExport->getFlags());
            }
        ),
        tc::fn_explicit_cast<SJsEnumOption>()
    )))
    , m_bIsIntegral(!tc::find_first_if<tc::return_bool>(
        m_vecjsenumoption,
        [](SJsEnumOption const& opt) noexcept {
            return opt.m_ovardblstrValue &&
                tc::visit(*opt.m_ovardblstrValue,
                    [](double dblValue) { return static_cast<int>(dblValue) != dblValue; },
                    [](std::string const&) { return true; }
                );
        }
    ))
{}

void SJsEnum::Initialize() noexcept {
    tc::cont_must_insert(g_setjsenum, *this);
}

SJsVariableLike::SJsVariableLike(ts::Symbol jsym) noexcept
    : m_jsym(jsym)
    , m_strJsName(tc::explicit_cast<std::string>(m_jsym->getName()))
    , m_strCppifiedName(CppifyName(m_jsym, enamectxNONE))
    , m_jdeclVariableLike([&]() noexcept {
        auto rngvariabledecl = tc::filter(tc::make_vector(m_jsym->declarations()), [](ts::Declaration decl) noexcept { // TODO: https://github.com/think-cell/tcjs/issues/5
            return tc::js::ts::isVariableDeclaration(decl) || tc::js::ts::isParameter(decl) || tc::js::ts::isPropertySignature(decl) || tc::js::ts::isPropertyDeclaration(decl);;
        });
        _ASSERT(!tc::empty(rngvariabledecl));
        {
            // There may be multiple declarations, but they should have the same type (structurally).
            // We strengthen this requirement even more for now.
            auto const jdeclFirst = tc_front(rngvariabledecl);
            auto const nModifierFlagsFirst = ts::getCombinedModifierFlags(jdeclFirst);
            auto const jtypeFirst = (*g_ojtsTypeChecker)->getTypeOfSymbolAtLocation(m_jsym, jdeclFirst);
            tc::for_each(rngvariabledecl, [&](ts::Declaration const jdeclCurrent) {
                auto const nModifierFlagsCurrent = ts::getCombinedModifierFlags(jdeclCurrent);
                auto const jtypeCurrent = (*g_ojtsTypeChecker)->getTypeOfSymbolAtLocation(m_jsym, jdeclCurrent);
                if (nModifierFlagsCurrent != nModifierFlagsFirst || !jtypeCurrent.getEmval().strictlyEquals(jtypeFirst.getEmval())) {
                    tc::append(std::cerr, "JSVariableLike of symbol '", m_strJsName, "' has ", tc::as_dec(m_jsym->declarations()->length()), " conflicting declarations\n");
                    _ASSERTFALSE;
                }
            });
        }
        return tc_front(rngvariabledecl);
    }())
    , m_jtypeDeclared((*g_ojtsTypeChecker)->getTypeOfSymbolAtLocation(m_jsym, m_jdeclVariableLike))
    , m_bReadonly(static_cast<bool>(ts::getCombinedModifierFlags(m_jdeclVariableLike) & ts::ModifierFlags::Readonly))
{
    ts::ModifierFlags const nModifierFlags = ts::getCombinedModifierFlags(m_jdeclVariableLike);
    if (ts::ModifierFlags::None != nModifierFlags &&
        ts::ModifierFlags::Export != nModifierFlags &&
        ts::ModifierFlags::Readonly != nModifierFlags &&
        ts::ModifierFlags::Ambient != ts::getCombinedModifierFlags(m_jdeclVariableLike)) {
        tc::append(std::cerr,
            "Unknown getCombinedModifierFlags for jdeclVariableLike ",
            m_strJsName,
            ": ",
            tc::as_dec(static_cast<int>(nModifierFlags)),
            "\n"
        );
        _ASSERTFALSE;
    }
}

SMangledType const& SJsVariableLike::MangleType() const noexcept {
    if (!m_omtType) {
        m_omtType = ::MangleType(m_jtypeDeclared);
    }
    return *m_omtType;
}

SJsFunctionLike::SJsFunctionLike(ts::Symbol jsym, ts::SignatureDeclaration jsigndecl) noexcept
    : m_jsym(jsym)
    , m_strCppifiedName(CppifyName(m_jsym, enamectxFUNCTION))
    , m_jsignature(*(*g_ojtsTypeChecker)->getSignatureFromDeclaration(jsigndecl))
    , m_vecjsvariablelikeParameters(tc::make_vector(tc::transform(
        m_jsignature->getParameters(),
        [&](ts::Symbol const jsymParameter) noexcept {
            return SJsVariableLike(jsymParameter);
        }
    )))
{
    if (ts::ModifierFlags::None != ts::getCombinedModifierFlags(jsigndecl) &&
        ts::ModifierFlags::Export != ts::getCombinedModifierFlags(jsigndecl) &&
        ts::ModifierFlags::Ambient != ts::getCombinedModifierFlags(jsigndecl)) {
        tc::append(std::cerr,
            "Unknown getCombinedModifierFlags for jsigndecl ",
            tc::explicit_cast<std::string>(m_jsym->getName()),
            ": ",
            tc::as_dec(static_cast<int>(ts::getCombinedModifierFlags(jsigndecl))),
            "\n"
        );
        _ASSERTFALSE;
    }
}

std::string SJsFunctionLike::CppifiedParametersWithCommentsDecl() const noexcept {
    // Trailing function arguments of type 'x | undefined' can be defaulted to undefined
    auto const itjsvariablelike = tc::find_last_if<tc::return_border_after_or_begin>(m_vecjsvariablelikeParameters, [](auto const& jsvariablelike) noexcept {
        if(auto ounion = tc::js::ts_ext::isUnion(jsvariablelike.m_jtypeDeclared)) {
            return !tc::find_first_if<tc::return_bool>((*ounion)->types(), [](auto const& type) noexcept {
                return ts::TypeFlags::Undefined==type->flags();
            });
        }
        return true;
    });
    return tc::explicit_cast<std::string>(tc::join_separated(
        tc::concat(
            tc::transform(tc::take(m_vecjsvariablelikeParameters, itjsvariablelike), [](SJsVariableLike const& jsvariablelikeParameter) noexcept {
                return tc::concat(jsvariablelikeParameter.MangleType().m_strWithComments, " ", jsvariablelikeParameter.m_strCppifiedName);
            }),
            tc::transform(tc::drop(m_vecjsvariablelikeParameters, itjsvariablelike), [](SJsVariableLike const& jsvariablelikeParameter) noexcept {
                return tc::concat(jsvariablelikeParameter.MangleType().m_strWithComments, " ", jsvariablelikeParameter.m_strCppifiedName, " = js_undefined()");
            })
        ),
        ", "
    ));
}

std::string SJsFunctionLike::CppifiedParametersWithCommentsDef() const noexcept {
    return tc::explicit_cast<std::string>(tc::join_separated(
        tc::transform(m_vecjsvariablelikeParameters, [&](SJsVariableLike const& jsvariablelikeParameter) noexcept {
            return tc::concat(jsvariablelikeParameter.MangleType().m_strWithComments, " ", jsvariablelikeParameter.m_strCppifiedName);
        }),
        ", "
    ));
}

std::string const& SJsFunctionLike::CanonizedParameterCppTypes() const noexcept {
    if(tc::empty(m_strCanonizedParameterCppTypes)) {
        m_strCanonizedParameterCppTypes = tc::explicit_cast<std::string>(tc::join_separated(
            tc::transform(m_vecjsvariablelikeParameters, [&](SJsVariableLike const& jsvariablelikeParameter) noexcept {
                return jsvariablelikeParameter.MangleType().m_strCppCanonized;
            }),
            ", "
        ));
    }
    return m_strCanonizedParameterCppTypes;
}

/*static*/ bool SJsFunctionLike::LessCppSignature(SJsFunctionLike const& a, SJsFunctionLike const& b) noexcept {
    if (a.m_strCppifiedName != b.m_strCppifiedName) {
        return a.m_strCppifiedName < b.m_strCppifiedName;
    }
    return a.CanonizedParameterCppTypes() < b.CanonizedParameterCppTypes();
}

SJsClass::SJsClass(ts::Symbol jsym) noexcept
    : m_jsym(jsym)
    , m_strQualifiedName(FullyQualifiedName(m_jsym))
    , m_strCppifiedName(CppifyName(m_jsym, enamectxCLASS))
    , m_strMangledName(MangleSymbolName(m_jsym, enamectxCLASS))
    , m_bHasImplicitDefaultConstructor(false)
{
    SJsScope::Initialize(
        tc_conditional_range(
            static_cast<bool>(ts::SymbolFlags::Module & m_jsym->getFlags()),
            (*g_ojtsTypeChecker)->getExportsOfModule(m_jsym),
            tc::make_empty_range<ts::Symbol>()
        )
    );

    if(auto ojarrsymMembers = tc::js::ts_ext::Symbol(m_jsym)->members()) { 
        auto vecjsymMembers = tc::explicit_cast<std::vector<ts::Symbol>>(*ojarrsymMembers);
        tc::for_each(
            tc::filter(vecjsymMembers, [](ts::Symbol jsymMember) noexcept {
                // TODO: Support optional member functions
                return static_cast<bool>((ts::SymbolFlags::Method|ts::SymbolFlags::Constructor) & jsymMember->getFlags())
                    && !static_cast<bool>(ts::SymbolFlags::Optional & jsymMember->getFlags());
            }),
            [&](ts::Symbol jsymMethod) noexcept {
                tc::for_each(jsymMethod->declarations(),
                    [&](ts::Declaration jdecl) noexcept {
                        if (auto const jotsMethodSignature = tc::js::ts_ext::isMethodSignature(jdecl)) { // In interfaces.
                            tc::cont_emplace_back(m_vecjsfunctionlikeMethod, SJsFunctionLike(jsymMethod, *jotsMethodSignature));
                        } else if (auto const jotsMethodDeclaration = tc::js::ts_ext::isMethodDeclaration(jdecl)) { // In classes.
                            tc::cont_emplace_back(m_vecjsfunctionlikeMethod, SJsFunctionLike(jsymMethod, *jotsMethodDeclaration));
                        } else if (auto const jotsConstructorDeclaration = tc::js::ts_ext::isConstructorDeclaration(jdecl)) {
                             tc::cont_emplace_back(m_vecjsfunctionlikeCtor, SJsFunctionLike(jsymMethod, *jotsConstructorDeclaration));
                        }
                    }
                );
            }
        );

        m_vecjsvariablelikeProperty = tc::make_vector(tc::transform(
            tc::filter(vecjsymMembers, [](ts::Symbol jsymMember) noexcept {
                _ASSERT(!static_cast<bool>(ts::SymbolFlags::Property & jsymMember->getFlags()) || !static_cast<bool>(~(ts::SymbolFlags::Property|ts::SymbolFlags::Optional) & jsymMember->getFlags()));
                return static_cast<bool>(ts::SymbolFlags::Property & jsymMember->getFlags());
            }),
            [](ts::Symbol jsymProperty) noexcept {
                return SJsVariableLike(jsymProperty);
            }
        ));
    }

     if((*g_ojtsTypeChecker)->getDeclaredTypeOfSymbol(m_jsym)->isClass()) {
        // See logic at `src/compiler/transformers/es2015.ts`, `addConstructor`, `transformConstructorBody` and `createDefaultConstructorBody`:
        // if no constructrs are defined, we add a "default" constructor with no parameters.
        m_bHasImplicitDefaultConstructor = tc::empty(m_vecjsfunctionlikeCtor);
    }
}

void SJsClass::Initialize() noexcept {
    tc::cont_must_insert(g_setjsclass, *this);
}

void SJsClass::ResolveBaseClasses() noexcept {
    auto AddBaseClass = [this](ts::Symbol jsymBase) noexcept {
        if(auto ojsclass = tc::cont_find<tc::return_element_or_null>(g_setjsclass, FullyQualifiedName(jsymBase))) {
            tc::cont_emplace_back(m_vecpjsclassBase, std::addressof(*ojsclass));
        } else {
            tc::cont_emplace_back(m_vecjsymBaseUnknown, jsymBase);
        }
    };

    if (auto jointerfacetypeClass = tc::js::ts_ext::isClassOrInterface((*g_ojtsTypeChecker)->getDeclaredTypeOfSymbol(m_jsym))) {
        tc::for_each((*g_ojtsTypeChecker)->getBaseTypes(*jointerfacetypeClass),
            [&](tc::js::ts::BaseType jtsBaseType) noexcept {
                if (auto const jointerfacetypeBase = tc::js::ts_ext::isClassOrInterface(tc::reluctant_implicit_cast<ts::Type>(jtsBaseType))) {
                    AddBaseClass(*(*jointerfacetypeBase)->getSymbol());
                }
            }
        );
        // See `utilities.ts:getAllSuperTypeNodes`. `extends` clause for classes is already covered by `getBaseTypes()`, as well as `implements` clause for interfaces.
        // The only missing part is `implements` for classes.
        tc::for_each(m_jsym->declarations(), [&](ts::Declaration jdeclClass) {
            if (auto joclassdeclarationClass = tc::js::ts_ext::isClassDeclaration(jdeclClass)) {
                if (auto jorarrHeritageClause = tc::js::ts_ext::ClassLikeDeclaration(*joclassdeclarationClass)->heritageClauses()) {
                    tc::for_each(*jorarrHeritageClause, [&](ts::HeritageClause jtsHeritageClause) noexcept {
                        if(ts::SyntaxKind::ImplementsKeyword == jtsHeritageClause->token()) {
                            tc::for_each(tc::js::ts_ext::HeritageClause(jtsHeritageClause)->types(), [&](ts::Node jnodeImplementsType) noexcept {
                                auto jtypeImplements = (*g_ojtsTypeChecker)->getTypeAtLocation(jnodeImplementsType);
                                if(auto josymImplements = jtypeImplements->getSymbol()) {
                                    AddBaseClass(*josymImplements);
                                } else {
                                    tc::append(std::cerr,
                                        "Unable to determine 'implements' symbol for class '",
                                        tc::explicit_cast<std::string>(m_jsym->getName()),
                                        "' for type '",
                                        tc::explicit_cast<std::string>((*g_ojtsTypeChecker)->typeToString(jtypeImplements)),
                                        "'\n"
                                    );
                                    _ASSERTFALSE;
                                }
                            });
                        }
                    });
                }
            }
        });
    } else {
        // Do nothing: e.g. namespaces.
    }
}

SJsTypeAlias::SJsTypeAlias(ts::Symbol jsym) noexcept 
    : m_jsym(jsym)
    , m_jtypenode(tc::js::ts::TypeAliasDeclaration(
        tc::find_first_if<tc::return_value>(
            jsym->declarations(), 
            [](tc::js::ts::Declaration decl) noexcept -> bool {
                return tc::js::ts::isTypeAliasDeclaration(decl);
            }
        )
      )->type())
    , m_jtype((*g_ojtsTypeChecker)->getTypeFromTypeNode(m_jtypenode))
    , m_strQualifiedName(FullyQualifiedName(m_jsym))
    , m_strCppifiedName(CppifyName(m_jsym, enamectxTYPEALIAS))
    , m_strMangledName(MangleSymbolName(m_jsym, enamectxTYPEALIAS))
{}

void SJsTypeAlias::Initialize() noexcept {
    tc::cont_must_insert(g_setjstypealias, *this);
}

SMangledType SJsTypeAlias::MangleType() const noexcept {
    // If a type alias is defined by a union, we have to define it by the corresponding js_union<...> declaration
    // type FooBar = number | string;
    // using FooBar = js_union<js_number, js_string>;

    // If is type alias is defined as another type or type alias, we do not want to repeat the js_union declaration
    // but try to keep referencing the type alias, so we call MangleType with bUseTypeAlias = true
    // type FooBar2 = FooBar;
    // using FooBar2 = FooBar;
    //
    // This does not work in all cases, see https://github.com/microsoft/TypeScript/issues/28197
    return ::MangleType(
        m_jtype, 
        /*bUseTypeAlias*/ [&]() noexcept {
            if (auto const ojsymAlias = m_jtype->aliasSymbol())
            {
                return !tc::equal(tc::explicit_cast<std::string>((*ojsymAlias)->name()), tc::explicit_cast<std::string>(m_jsym->name()));
            }
            return false;
        }()
    );
}
