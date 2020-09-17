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
	// TODO: Symbol::getFlags returns a bitmask and we must treat it so
	// Callers of CppType must treat return value as a bit mask too
	if(ts::SymbolFlags::RegularEnum == jsymType->getFlags() || ts::SymbolFlags::ConstEnum == jsymType->getFlags()) {
		return ecpptypeENUM;
	}
	if(ts::SymbolFlags::Class == jsymType->getFlags() ||
	ts::SymbolFlags::Interface == jsymType->getFlags() ||
	(ts::SymbolFlags::Interface | ts::SymbolFlags::FunctionScopedVariable) == jsymType->getFlags() ||
	ts::SymbolFlags::ValueModule == jsymType->getFlags() ||
	(ts::SymbolFlags::ValueModule | ts::SymbolFlags::Interface) == jsymType->getFlags() ||
	(ts::SymbolFlags::NamespaceModule | ts::SymbolFlags::Interface) == jsymType->getFlags() ||
	ts::SymbolFlags::NamespaceModule == jsymType->getFlags()) {
		return ecpptypeCLASS;
	}

	if(ts::SymbolFlags::TypeAlias==jsymType->getFlags()) {
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

		auto const jtype = (*g_ojtsTypeChecker)->getTypeFromTypeNode(ts::TypeAliasDeclaration(jsymType->declarations()[0])->type());
		auto const ojuniontype = tc::js::ts_ext::isUnion(jtype);
		if((ojuniontype && tc::all_of((*ojuniontype)->types(), IsObject))
		|| IsObject(jtype)) {
			return ecpptypeTYPEALIAS;
		}
	}
	return ecpptypeIGNORE;
}

namespace {
    std::string MangleSymbolName(ts::Symbol jsymType) noexcept {
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
        return strMangled;
    }
}

SJsEnumOption::SJsEnumOption(ts::Symbol jsymOption) noexcept
    : m_jsymOption([&]() noexcept {
        _ASSERTEQUAL(jsymOption->getFlags(), ts::SymbolFlags::EnumMember);
        return jsymOption;
    }())
    , m_strJsName(tc::explicit_cast<std::string>(jsymOption->getName()))
    , m_strCppifiedName(CppifyName(jsymOption))
    , m_jtsEnumMember([&]() noexcept {
        auto const jarrDeclaration = jsymOption->declarations();
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

SJsEnum::SJsEnum(ts::Symbol jsymEnum) noexcept
    : m_jsym(jsymEnum)
    , m_strQualifiedName(FullyQualifiedName(m_jsym))
    , m_strMangledName(MangleSymbolName(m_jsym))
    , m_vecjsenumoption(tc::make_vector(tc::transform(
        *tc::js::ts_ext::Symbol(m_jsym)->exports(),
        [](ts::Symbol jsymOption) noexcept {
            return SJsEnumOption(jsymOption);
        }
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

SJsVariableLike::SJsVariableLike(ts::Symbol jsymName) noexcept
    : m_jsym(jsymName)
    , m_strJsName(tc::explicit_cast<std::string>(jsymName->getName()))
    , m_strCppifiedName(CppifyName(jsymName))
    , m_jdeclVariableLike([&]() noexcept {
        _ASSERT(1 <= jsymName->declarations()->length());
        if (1 < jsymName->declarations()->length()) {
            // There may be multiple declarations, but they should have the same type (structurally).
            // We strenghten this requirement even more for now.
            auto const jdeclFirst = jsymName->declarations()[0];
            auto const nModifierFlagsFirst = ts::getCombinedModifierFlags(jdeclFirst);
            auto const jtypeFirst = (*g_ojtsTypeChecker)->getTypeOfSymbolAtLocation(jsymName, jdeclFirst);
            tc::for_each(jsymName->declarations(), [&](ts::Declaration const jdeclCurrent) {
                auto const nModifierFlagsCurrent = ts::getCombinedModifierFlags(jdeclCurrent);
                auto const jtypeCurrent = (*g_ojtsTypeChecker)->getTypeOfSymbolAtLocation(jsymName, jdeclCurrent);
                if (nModifierFlagsCurrent != nModifierFlagsFirst || !jtypeCurrent.getEmval().strictlyEquals(jtypeFirst.getEmval())) {
                    tc::append(std::cerr, "JSVariableLike of symbol '", m_strJsName, "' has ", tc::as_dec(jsymName->declarations()->length()), " conflicting declarations\n");
                    _ASSERTFALSE;
                }
            });
        }
        return jsymName->declarations()[0];
    }())
    , m_jtypeDeclared((*g_ojtsTypeChecker)->getTypeOfSymbolAtLocation(jsymName, m_jdeclVariableLike))
    , m_bReadonly(ts::getCombinedModifierFlags(m_jdeclVariableLike) & ts::ModifierFlags::Readonly)
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

SJsFunctionLike::SJsFunctionLike(ts::Symbol jsymFunctionLike, ts::Declaration const jdeclFunctionLike) noexcept
    : m_jsym(jsymFunctionLike)
    , m_strCppifiedName(CppifyName(m_jsym))
    , m_jtsSignatureDeclaration([&]() noexcept -> ts::SignatureDeclaration {
        if (auto const jotsMethodSignature = tc::js::ts_ext::isMethodSignature(jdeclFunctionLike)) { // In interfaces.
            return *jotsMethodSignature;
        }
        if (auto const jotsMethodDeclaration = tc::js::ts_ext::isMethodDeclaration(jdeclFunctionLike)) { // In classes.
            return *jotsMethodDeclaration;
        }
        if (auto const jotsConstructorDeclaration = tc::js::ts_ext::isConstructorDeclaration(jdeclFunctionLike)) {
            return *jotsConstructorDeclaration;
        }
        if (auto const jotsFunctionDeclaration = tc::js::ts_ext::isFunctionDeclaration(jdeclFunctionLike)) {
            return *jotsFunctionDeclaration;
        }
        _ASSERTFALSE;
    }())
    , m_jtsSignature(*(*g_ojtsTypeChecker)->getSignatureFromDeclaration(m_jtsSignatureDeclaration))
    , m_joptarrunkTypeParameter(m_jtsSignature->getTypeParameters())
    , m_vecjsvariablelikeParameters(tc::make_vector(tc::transform(
        m_jtsSignature->getParameters(),
        [&](ts::Symbol jsymParameter) noexcept {
            return SJsVariableLike(jsymParameter);
        }
    )))
{
    if (ts::ModifierFlags::None != ts::getCombinedModifierFlags(jdeclFunctionLike) &&
        ts::ModifierFlags::Export != ts::getCombinedModifierFlags(jdeclFunctionLike) &&
        ts::ModifierFlags::Ambient != ts::getCombinedModifierFlags(jdeclFunctionLike)) {
        tc::append(std::cerr,
            "Unknown getCombinedModifierFlags for jdeclFunctionLike ",
            tc::explicit_cast<std::string>(m_jsym->getName()),
            ": ",
            tc::as_dec(static_cast<int>(ts::getCombinedModifierFlags(jdeclFunctionLike))),
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

SJsClass::SJsClass(ts::Symbol jsymClass) noexcept
    : m_jsym(jsymClass)
    , m_strQualifiedName(FullyQualifiedName(m_jsym))
    , m_strMangledName(MangleSymbolName(m_jsym))
    , m_bHasImplicitDefaultConstructor(false)
{
    SJsScope::Initialize(
        tc_conditional_range(
            m_jsym->getFlags() & ts::SymbolFlags::Module,
            (*g_ojtsTypeChecker)->getExportsOfModule(m_jsym),
            tc::make_empty_range<ts::Symbol>()
        )
    );

    if(auto ojarrsymMembers = tc::js::ts_ext::Symbol(m_jsym)->members()) { 
        auto vecjsymMembers = tc::explicit_cast<std::vector<ts::Symbol>>(*ojarrsymMembers);
        m_vecjsfunctionlikeMethod = tc::make_vector(tc::join(tc::transform(
            tc::filter(vecjsymMembers, [](ts::Symbol jsymMember) noexcept {
                return ts::SymbolFlags::Method == jsymMember->getFlags() || ts::SymbolFlags::Constructor == jsymMember->getFlags();
            }),
            [&](ts::Symbol jsymMethod) noexcept {
                return tc::transform(
                    jsymMethod->declarations(),
                    [=](ts::Declaration jdeclMethod) noexcept {
                        return SJsFunctionLike(jsymMethod, jdeclMethod);
                    }
                );
            }
        )));

        m_vecjsvariablelikeProperty = tc::make_vector(tc::transform(
            tc::filter(vecjsymMembers, [](ts::Symbol jsymMember) noexcept {
                _ASSERT(!(ts::SymbolFlags::Property & jsymMember->getFlags()) || !(~(ts::SymbolFlags::Property|ts::SymbolFlags::Optional) & jsymMember->getFlags()));
                return ts::SymbolFlags::Property & jsymMember->getFlags();
            }),
            [](ts::Symbol jsymProperty) noexcept {
                return SJsVariableLike(jsymProperty);
            }
        ));
    }

     if((*g_ojtsTypeChecker)->getDeclaredTypeOfSymbol(m_jsym)->isClass()) {
        // See logic at `src/compiler/transformers/es2015.ts`, `addConstructor`, `transformConstructorBody` and `createDefaultConstructorBody`:
        // if no constructrs are defined, we add a "default" constructor with no parameters.
        m_bHasImplicitDefaultConstructor = !tc::find_first_if<tc::return_bool>(
            m_vecjsfunctionlikeMethod,
            [](SJsFunctionLike const& jsfunctionlikeMethod) noexcept {
                return ts::SymbolFlags::Constructor == jsfunctionlikeMethod.m_jsym->getFlags();
            }
        );
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
    , m_jtypenode(ts::TypeAliasDeclaration(jsym->declarations()[0])->type())
    , m_jtype((*g_ojtsTypeChecker)->getTypeFromTypeNode(m_jtypenode))
    , m_strQualifiedName(FullyQualifiedName(m_jsym))
    , m_strMangledName(MangleSymbolName(m_jsym))
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
        (*g_ojtsTypeChecker)->getTypeFromTypeNode(m_jtypenode), 
        /*bUseTypeAlias*/ [&]() noexcept {
            if (auto const ojsymAlias = m_jtype->aliasSymbol())
            {
                return !tc::equal(tc::explicit_cast<std::string>((*ojsymAlias)->name()), tc::explicit_cast<std::string>(m_jsym->name()));
            }
            return false;
        }()
    );
}
