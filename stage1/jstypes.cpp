#include "jstypes.h"

using tc::js::Array;
using tc::js::console;
using tc::js::ReadonlyArray;
using tc::js::ts;
using tc::jst::optional;
using tc::js::string;
using tc::js::any;

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
                [](tc::js::ts::Declaration decl) noexcept {
                    return tc::bool_cast(tc::js::ts::isTypeAliasDeclaration(decl));
                }
            )
        )->type());
		auto const ojuniontype = jtype->isUnion();
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
        auto const strQualified = FullyQualifiedName(jsymType);
        tc::for_each(strQualified, [&](char c) noexcept {
            switch (c) {
            case '_': tc::append(strMangled, "_u"); break;
            case ',': tc::append(strMangled, "_c"); break;
            case '.': tc::append(strMangled, "_d"); break;
            case '-': tc::append(strMangled, "_m"); break;
            case '"': tc::append(strMangled, "_q"); break;
            default:
                if(!tc::isasciidigit(c) && !tc::isasciilower(c) && !tc::isasciiupper(c)) {
                    tc::append(std::cerr, "Invalid character in symbol name ", strQualified, "\n");
                    _ASSERTFALSE;
                }
                tc::cont_emplace_back(strMangled, c); break;
            }
        });
        return MakeUniqueName(jsymType, tc_move(strMangled), enamectx);
    }
}

tc::ptr_range<char const> StripQuotes(std::string const& str) noexcept {
    _ASSERT(!tc::empty(str));
    auto strNoQuotes = tc::as_pointers(str);
    if ('"' == tc::front(strNoQuotes) && '"' == tc::back(strNoQuotes)) {
        tc::drop_first_inplace(strNoQuotes);
        tc::drop_last_inplace(strNoQuotes);
    }
    return strNoQuotes;
}

namespace {
   constexpr char const* c_apszReserved[] = {
       "alignas", "alignof", "and", "and_eq", "asm", "auto", 
       "bitand", "bitor", "bool", "break", 
       "case", "catch", "char", "char16_t", "char32_t", "class", "compl", "const", "const_cast", "constexpr", "continue", 
       "decltype", "default", "delete", "do", "double", "dynamic_cast", 
       "else", "enum", "explicit", "export", "extern", 
       "false", "float", "for", "friend", 
       "goto", 
       "if", "inline", "int", 
       "long", 
       "mutable", 
       "namespace", "new", "noexcept", "not", "not_eq", "nullptr", 
       "operator", "or", "or_eq", 
       "private", "protected", "public", 
       "register", "reinterpret_cast", "return", 
       "short", "signed", "sizeof", "static", "static_assert", "static_cast", "struct", "switch", 
       "template", "this", "thread_local", "throw", "true", "try", "typedef", "typeid", "typename", 
       "union", "unsigned", "using", 
       "virtual", "void", "volatile", 
       "wchar_t", "while", 
       "xor", "xor_eq"
    };
    static_assert(tc::is_sorted(c_apszReserved, tc::lessfrom3way(tc::fn_lexicographical_compare_3way())));
}

// CppifyName returns the user-visible name that we export from namespaces/classes
std::string CppifyName(ts::Symbol jsym, ENameContext enamectx) noexcept {
    // TODO: https://en.cppreference.com/w/cpp/language/identifiers
    // JavaScript identifiers are actually Unicode and C++ compilers support Unicode, 
    // so we are needlessly restrictive here. 
    std::string strResult;
    tc::for_each(
        StripQuotes(tc::explicit_cast<std::string>(jsym->getName())),
        [&](char c) noexcept {
            if(tc::isasciidigit(c) || tc::isasciilower(c) || tc::isasciiupper(c)) {
                tc::cont_emplace_back(strResult, c);
            } else if(!tc::empty(strResult) && '_' == tc::back(strResult)) {
                tc::append(strResult, "u_");
            } else {
                tc::append(strResult, "_");
            }
        }
    );

    if(tc::binary_find_unique<tc::return_bool>(c_apszReserved, strResult, tc::lessfrom3way(tc::fn_lexicographical_compare_3way()))) {
        _ASSERT('_'!=tc::back(strResult));
        tc::append(strResult, "_");
    }
    _ASSERT(!tc::empty(strResult));
    return MakeUniqueName(jsym, tc_move(strResult), enamectx);
}

SJsEnumOption::SJsEnumOption(ts::Symbol jsym) noexcept
    : m_jsym([&]() noexcept {
        _ASSERTEQUAL(jsym->getFlags(), ts::SymbolFlags::EnumMember);
        return jsym;
    }())
    , m_strJsName(tc::explicit_cast<std::string>(jsym->getName()))
    , m_strCppifiedName(CppifyName(jsym, enamectxNONE))
    , m_jenummember([&]() noexcept {
        auto const jarrDeclaration = jsym->declarations();
        _ASSERTEQUAL(jarrDeclaration->length(), 1);
        return ts::EnumMember(jarrDeclaration[0]);
    }())
{    
    _ASSERTEQUAL(ts::getCombinedModifierFlags(m_jenummember), ts::ModifierFlags::None);
    auto const junionOptionValue = (*g_ojtsTypeChecker)->getConstantValue(m_jenummember);
    if (junionOptionValue.getEmval().isNumber()) {
        m_vardblstrValue = junionOptionValue.get<double>();
    } else if (junionOptionValue.getEmval().isString()) {
        m_vardblstrValue = tc::explicit_cast<std::string>(junionOptionValue.get<string>());
    } else {
        _ASSERT(junionOptionValue.getEmval().isUndefined());
    }
}

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
    , m_bIsIntegral(tc::all_of(
        m_vecjsenumoption,
        [](SJsEnumOption const& opt) noexcept {
            return tc::visit(opt.m_vardblstrValue,
                [](double dblValue) { return static_cast<int>(dblValue) == dblValue; },
                [](std::string const&) { return false; },
                [](std::monostate const&) { return true; }
            );
        }
    ))
{}

void SJsEnum::Initialize() & noexcept {
    tc::intrusive_cont_must_insert(g_setjsenum, *this);
}

SJsVariableLike::SJsVariableLike(ts::Symbol jsym) noexcept
    : m_jsym(jsym)
    , m_strJsName(tc::explicit_cast<std::string>(m_jsym->getName()))
    , m_strCppifiedName(CppifyName(m_jsym, enamectxNONE))
    , m_jdeclVariableLike([&]() noexcept {
        auto rngvariabledecl = tc::filter(m_jsym->declarations(), [](ts::Declaration decl) noexcept { 
            return tc::js::ts::isVariableDeclaration(decl) || tc::js::ts::isParameter(decl) || tc::js::ts::isPropertySignature(decl) || tc::js::ts::isPropertyDeclaration(decl);;
        });
        _ASSERT(!tc::empty(rngvariabledecl));
        {
            // There may be multiple declarations, but they should have the same type (structurally).
            // We strengthen this requirement even more for now.
            auto const jdeclFirst = tc::front(rngvariabledecl);
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
        return tc::front(rngvariabledecl);
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

SMangledType const& SJsVariableLike::MangleType() const& noexcept {
    if (!m_omtType) {
        m_omtType = ::MangleType(m_jtypeDeclared);
    }
    return *m_omtType;
}

STypeParameter::STypeParameter(ts::TypeParameterDeclaration typeparamdecl) noexcept
    : m_strName(tc::explicit_cast<std::string>(tc::js::string(typeparamdecl->name()->escapedText())))
{   
    // FIXME: Support type constraints, double and enum template arguments
    // , std::enable_if_t<std::is_base_of<A, K>::value>* = nullptr
    if(auto const otypenode = typeparamdecl->constraint()) {
        if(!(ts::SyntaxKind::NumberKeyword == (*otypenode)->kind()
        || (ts::SyntaxKind::TypeReference == (*otypenode)->kind()
            && [&]() noexcept {
                auto jtype = (*g_ojtsTypeChecker)->getTypeFromTypeNode(*otypenode);
                auto jsym = jtype->symbol();
                if(ts::SymbolFlags::None!=((ts::SymbolFlags::RegularEnum|ts::SymbolFlags::Class|ts::SymbolFlags::Interface)&jsym->getFlags())) {
                    return true;
                } else {
                    tc::append(std::cerr, "Cannot find symbol for type parameter declaration (", tc::as_dec(static_cast<int>(jsym->getFlags())) , ")\n");
                    return false;
                }
            }()
        )
        || ts::SyntaxKind::TypeOperator == (*otypenode)->kind() // e.g. keyof declaration
        || ts::SyntaxKind::UnionType == (*otypenode)->kind()
        || ts::SyntaxKind::IndexedAccessType == (*otypenode)->kind()
        )) {
            tc::append(std::cerr, "Unsupported type parameter declaration ", tc::explicit_cast<std::string>(typeparamdecl->getFullText()), " (", tc::as_dec(static_cast<int>((*otypenode)->kind())), ")\n");
            _ASSERTFALSE;
        }
    }
}

SJsFunctionLike::SJsFunctionLike(ts::Symbol jsym, ts::SignatureDeclaration jsigndecl) noexcept
    : m_jsym(jsym)
    , m_strCppifiedName(CppifyName(m_jsym, enamectxFUNCTION))
    , m_jsignature(*(*g_ojtsTypeChecker)->getSignatureFromDeclaration(jsigndecl))
    , m_vecjsvariablelikeParameters(tc::make_vector(tc::transform(
        m_jsignature->getParameters(),
        [](ts::Symbol const jsymParameter) noexcept {
            return SJsVariableLike(jsymParameter);
        }
    )))
    , m_vectypeparam(tc::make_vector(tc::transform(ts::getEffectiveTypeParameterDeclarations(jsigndecl), tc::fn_explicit_cast<STypeParameter>())))
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

std::string SJsFunctionLike::CppifiedParametersWithCommentsDecl() const& noexcept {
    // Trailing function arguments of type 'x | undefined' can be defaulted to undefined
    auto const itjsvariablelike = tc::find_last_if<tc::return_border_after_or_begin>(m_vecjsvariablelikeParameters, [](auto const& jsvariablelike) noexcept {
        if(auto ounion = jsvariablelike.m_jtypeDeclared->isUnion()) {
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
                return tc::concat(jsvariablelikeParameter.MangleType().m_strWithComments, " ", jsvariablelikeParameter.m_strCppifiedName, " = tc::js::undefined()");
            })
        ),
        ", "
    ));
}

std::string SJsFunctionLike::CppifiedParametersWithCommentsDef() const& noexcept {
    return tc::explicit_cast<std::string>(tc::join_separated(
        tc::transform(m_vecjsvariablelikeParameters, [&](SJsVariableLike const& jsvariablelikeParameter) noexcept {
            return tc::concat(jsvariablelikeParameter.MangleType().m_strWithComments, " ", jsvariablelikeParameter.m_strCppifiedName);
        }),
        ", "
    ));
}

std::string const& SJsFunctionLike::CanonizedParameterCppTypes() const& noexcept {
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
        tc::for_each(
            tc::filter(*ojarrsymMembers, [](ts::Symbol jsymMember) noexcept {
                // TODO: Support optional member functions
                return static_cast<bool>((ts::SymbolFlags::Method|ts::SymbolFlags::Constructor) & jsymMember->getFlags())
                    && !static_cast<bool>(ts::SymbolFlags::Optional & jsymMember->getFlags());
            }),
            [&](ts::Symbol jsymMethod) noexcept {
                tc::for_each(jsymMethod->declarations(),
                    [&](ts::Declaration jdecl) noexcept {
                        if (auto const jotsMethodSignature = ts::isMethodSignature(jdecl)) { // In interfaces.
                            tc::cont_emplace_back(m_vecjsfunctionlikeMethod, SJsFunctionLike(jsymMethod, *jotsMethodSignature));
                        } else if (auto const jotsMethodDeclaration = ts::isMethodDeclaration(jdecl)) { // In classes.
                            tc::cont_emplace_back(m_vecjsfunctionlikeMethod, SJsFunctionLike(jsymMethod, *jotsMethodDeclaration));
                        } else if (auto const jotsConstructorDeclaration = ts::isConstructorDeclaration(jdecl)) {
                             tc::cont_emplace_back(m_vecjsfunctionlikeCtor, SJsFunctionLike(jsymMethod, *jotsConstructorDeclaration));
                        }
                    }
                );
            }
        );

        m_vecjsvariablelikeProperty = tc::make_vector(tc::transform(
            tc::filter(*ojarrsymMembers, [](ts::Symbol jsymMember) noexcept {
                _ASSERT(!static_cast<bool>(ts::SymbolFlags::Property & jsymMember->getFlags()) || !static_cast<bool>(~(ts::SymbolFlags::Property|ts::SymbolFlags::Optional) & jsymMember->getFlags()));
                return static_cast<bool>(ts::SymbolFlags::Property & jsymMember->getFlags());
            }),
            [](ts::Symbol jsymProperty) noexcept {
                return SJsVariableLike(jsymProperty);
            }
        ));
    }

    // We must get the type parameters via the declaration
    // (*g_ojtsTypeChecker)->symbolToTypeParameterDeclarations(m_jsym) seems to return new nodes, i.e., syntax nodes
    // without a text position in the source file. These cannot be transformed into types. 
    
    if(auto ojdeclwithtypeparam = [&]() noexcept -> std::optional<ts::DeclarationWithTypeParameters> {
        auto jdecl = tc::front(m_jsym->declarations());
        if(auto ojclassdecl = ts::isClassLike(jdecl)) {
            return ts::DeclarationWithTypeParameters(*ojclassdecl);
        } else if(auto ojinterfacedecl = ts::isInterfaceDeclaration(jdecl)) {
            return ts::DeclarationWithTypeParameters(*ojinterfacedecl);
        } else {
            return std::nullopt;
        }
    }()) {
        m_vectypeparam = tc::make_vector(tc::transform(ts::getEffectiveTypeParameterDeclarations(*ojdeclwithtypeparam), tc::fn_explicit_cast<STypeParameter>()));
    }
    // FIXME: Add assert that all declarations have the same type parameters

    if(static_cast<bool>(ts::SymbolFlags::Class&m_jsym->getFlags())) {
        // See logic at `src/compiler/transformers/es2015.ts`, `addConstructor`, `transformConstructorBody` and `createDefaultConstructorBody`:
        // if no constructrs are defined, we add a "default" constructor with no parameters.
        m_bHasImplicitDefaultConstructor = tc::empty(m_vecjsfunctionlikeCtor);
    } else if(static_cast<bool>(ts::SymbolFlags::Interface&m_jsym->getFlags())) {
        // In Typescript, symbols can be "types" or "values". The types do not exist in JavaScript. The values do. 
        // A typescript class is both because the class constructor exists in JavaScript.
        // An interface is just a type. 
        // The "value" belonging to an interface is sometimes declared separately in a variable, e.g.,

        // interface CSSConditionRule extends CSSGroupingRule {
        //     conditionText: string;
        // }

        // declare var CSSConditionRule: {
        //     prototype: CSSConditionRule;
        //     new(): CSSConditionRule;
        // };

        // Generate a constructor if we find such a value declaration
        if(auto ojdecl = m_jsym->valueDeclaration()) {
            if(auto ojvardecl = ts::isVariableDeclaration(*ojdecl)) {
                if(auto ojtypenode = (*ojvardecl)->type()) {
                    auto jstypeVariable = (*g_ojtsTypeChecker)->getTypeFromTypeNode(*ojtypenode);
                    tc::for_each(*(tc::js::ts_ext::Symbol(jstypeVariable->symbol())->members()), [&](ts::Symbol jsymMember) noexcept {
                         tc::for_each(jsymMember->declarations(), [&](ts::Declaration jdecl) noexcept {
                             if(auto ojctorsignature = ts::isConstructSignatureDeclaration(jdecl)) {
                                  tc::cont_emplace_back(m_vecjsfunctionlikeCtor, SJsFunctionLike(jsymMember, *ojctorsignature));
                             }
                         });
                    });
                }
            }
        }

        // Generate default constructor interfaces containing only optional properties, e.g., 
        // interface TypeAcquisition {
        //     enableAutoDiscovery?: boolean;
        //     enable?: boolean;
        //     include?: string[];
        //     exclude?: string[];
        //     [option: string]: string[] | boolean | undefined;
        // }
        auto jstype = (*g_ojtsTypeChecker)->getDeclaredTypeOfSymbol(m_jsym);
        m_bHasImplicitDefaultConstructor = tc::empty(m_vecjsfunctionlikeCtor) 
            && tc::all_of(
                // Type::getProperties will include properties (and member functions) of base classes
                tc::transform(jstype->getProperties(), [](ts::Symbol jsymMember) noexcept {
                    return ((ts::SymbolFlags::Property|ts::SymbolFlags::Optional) == ((ts::SymbolFlags::Property|ts::SymbolFlags::Optional) & jsymMember->getFlags()))
                    || tc::all_of(jsymMember->declarations(), [](auto const& jsdecl) noexcept {
                        if(auto ojsindexdecl = ts::isIndexSignatureDeclaration(jsdecl)) {
                            if(auto ojstypenode = (*ojsindexdecl)->type()) {
                                if(auto ojsuniontypenode = ts::isUnionTypeNode(*ojstypenode)) {
                                    return tc::any_of(tc::js::ts_ext::MakeReadOnlyArray<ts::TypeNode>((*ojsuniontypenode)->types()), [](auto const& typenode) noexcept { return ts::SyntaxKind::UndefinedKeyword==typenode->kind(); }); 
                                }
                            }
                        }
                        return false;
                    });
                })
            );
    }
}

void SJsClass::Initialize() & noexcept {
    tc::intrusive_cont_must_insert(g_setjsclass, *this);
}

void SJsClass::ResolveBaseClasses() & noexcept {
    auto AddBaseClass = [this](ts::Symbol jsymBase) noexcept {
        if(auto ojsclass = tc::cont_find<tc::return_element_or_null>(g_setjsclass, FullyQualifiedName(jsymBase))) {
            tc::cont_emplace_back(m_vecpjsclassBase, std::addressof(*ojsclass));
        } else {
            tc::cont_emplace_back(m_vecjsymBaseUnknown, jsymBase);
        }
    };

    if (auto jointerfacetypeClass = (*g_ojtsTypeChecker)->getDeclaredTypeOfSymbol(m_jsym)->isClassOrInterface()) {
        tc::for_each((*g_ojtsTypeChecker)->getBaseTypes(*jointerfacetypeClass),
            [&](tc::js::ts::BaseType jtsBaseType) noexcept {
                if (auto const jointerfacetypeBase = tc::reluctant_implicit_cast<ts::Type>(jtsBaseType)->isClassOrInterface()) {
                    AddBaseClass(*(*jointerfacetypeBase)->getSymbol());
                }
            }
        );
        // See `utilities.ts:getAllSuperTypeNodes`. `extends` clause for classes is already covered by `getBaseTypes()`, as well as `implements` clause for interfaces.
        // The only missing part is `implements` for classes.
        tc::for_each(m_jsym->declarations(), [&](ts::Declaration jdeclClass) noexcept {
            if (auto joclassdeclarationClass = ts::isClassDeclaration(jdeclClass)) {
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

tc::js::ts::TypeAliasDeclaration TypeAliasDeclaration(ts::Symbol jsym) noexcept {
    return tc::js::ts::TypeAliasDeclaration(
        tc::find_first_if<tc::return_value>(
            jsym->declarations(), 
            [](tc::js::ts::Declaration decl) noexcept {
                return tc::bool_cast(tc::js::ts::isTypeAliasDeclaration(decl));
            }
        )
    );
}

SJsTypeAlias::SJsTypeAlias(ts::Symbol jsym) noexcept 
    : m_jsym(jsym)
    , m_jtypenode(TypeAliasDeclaration(jsym)->type())
    , m_jtype((*g_ojtsTypeChecker)->getTypeFromTypeNode(m_jtypenode))
    , m_strQualifiedName(FullyQualifiedName(m_jsym))
    , m_strCppifiedName(CppifyName(m_jsym, enamectxTYPEALIAS))
    , m_strMangledName(MangleSymbolName(m_jsym, enamectxTYPEALIAS))
    , m_vectypeparam(tc::make_vector(tc::transform(ts::getEffectiveTypeParameterDeclarations(TypeAliasDeclaration(jsym)), tc::fn_explicit_cast<STypeParameter>())))
{}

void SJsTypeAlias::Initialize() & noexcept {
    tc::intrusive_cont_must_insert(g_setjstypealias, *this);
}

SMangledType SJsTypeAlias::MangleType() const& noexcept {
    // If a type alias is defined by a union, we have to define it by the corresponding union_t<...> declaration
    // type FooBar = number | string;
    // using FooBar = union_t<js_number, string>;

    // If is type alias is defined as another type or type alias, we do not want to repeat the union_t declaration
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
