#include "precompiled.h"
#include "typescript.d.bootstrap.h"
#include "mangle.h"
#include "walk_symbol.h"

using tc::jst::create_js_object;
using tc::jst::js_string;
using tc::jst::js_optional;
using tc::jst::js_unknown;
using tc::js::console;
using tc::js::ts;
using tc::js::Array;
using tc::js::ReadonlyArray;

std::string RetrieveSymbolFromCpp(ts::Symbol jsymSymbol) noexcept {
	std::string strSymbolName = tc::explicit_cast<std::string>(jsymSymbol->getName());
	if ('"' == strSymbolName.front() && '"' == strSymbolName.back()) {
		strSymbolName = strSymbolName.substr(1, strSymbolName.length() - 2);
	}
	return tc::explicit_cast<std::string>(
		tc_conditional_range(
			!jsymSymbol->parent(),
			tc::concat("emscripten::val::global(\"", strSymbolName, "\")"),
			tc::concat(
				RetrieveSymbolFromCpp(*jsymSymbol->parent()),
				"[\"", strSymbolName, "\"]"
			)
		)
	);
}

std::string CppifyName(ts::Symbol jsymSymbol) noexcept {
	std::string strSourceName = tc::explicit_cast<std::string>(jsymSymbol->getName());
	if ('"' == strSourceName.front() && '"' == strSourceName.back()) {
		strSourceName = strSourceName.substr(1, strSourceName.length() - 2);
	}
	std::string strResult;
	bool bLastIsUnderscore = false;
	tc::for_each(
		tc::transform(strSourceName, [&](char c) noexcept {
			if ('-' == c) return '_';
			if ('$' == c) return '_';
			if ('_' == c) return c;
			if ('a' <= c && c <= 'z') return c;
			if ('A' <= c && c <= 'Z') return c;
			if ('0' <= c && c <= '9') return c;
			tc::append(std::cerr, "Cannot convert JS name to C++ name: '", strSourceName, "'\n");
			_ASSERTFALSE;
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
	return strResult;
}

struct SJsEnumOption {
	ts::Symbol m_jsymOption;
	std::string m_strJsName;
	std::string m_strCppifiedName;
	ts::EnumMember m_jtsEnumMember;
	std::optional<std::variant<double, std::string>> m_ovardblstrValue;

	SJsEnumOption(ts::TypeChecker const jtsTypeChecker, ts::Symbol const jsymOption) noexcept
		: m_jsymOption([&]() noexcept {
			_ASSERTEQUAL(jsymOption->getFlags(), ts::SymbolFlags::EnumMember);
			return jsymOption;
		}())
		, m_strJsName(tc::explicit_cast<std::string>(jsymOption->getName()))
		, m_strCppifiedName(CppifyName(jsymOption))
		, m_jtsEnumMember([&]() noexcept {
			auto const jarrDeclaration = jsymOption->declarations();
			_ASSERTEQUAL(jarrDeclaration->length(), 1);
			return *ts()->isEnumMember(jarrDeclaration[0]);
		}())
		, m_ovardblstrValue([&]() noexcept -> decltype(m_ovardblstrValue) {
			_ASSERTEQUAL(ts()->getCombinedModifierFlags(m_jtsEnumMember), ts::ModifierFlags::None);
			auto const junionOptionValue = jtsTypeChecker->getConstantValue(m_jtsEnumMember);
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
};

struct SJsEnum {
	ts::Symbol m_jsymEnum;
	std::string m_strMangledName;
	std::vector<SJsEnumOption> m_vecjsenumoption;
	bool m_bIsIntegral;

	SJsEnum(ts::TypeChecker const jtsTypeChecker, ts::Symbol const jsymEnum) noexcept
		: m_jsymEnum(jsymEnum)
		, m_strMangledName(MangleSymbolName(jtsTypeChecker, jsymEnum))
		, m_vecjsenumoption(tc::make_vector(tc::transform(
			*jsymEnum->exports(),
			[&jtsTypeChecker](ts::Symbol const jsymOption) noexcept {
				return SJsEnumOption(jtsTypeChecker, jsymOption);
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
	{
	}
};

struct SJsVariableLike {
	ts::Symbol m_jsymName;
	std::string m_strJsName;
	std::string m_strCppifiedName;
private:
	ts::Declaration m_jdeclVariableLike; // There may be multiple declarations, we ensure they do not conflict.
public:
	ts::Type m_jtypeDeclared;
	SMangledType m_mtType;
	bool m_bReadonly;

	SJsVariableLike(ts::TypeChecker const jtsTypeChecker, ts::Symbol const jsymName) noexcept
		: m_jsymName(jsymName)
		, m_strJsName(tc::explicit_cast<std::string>(jsymName->getName()))
		, m_strCppifiedName(CppifyName(jsymName))
		, m_jdeclVariableLike([&]() noexcept {
			_ASSERT(1 <= jsymName->declarations()->length());
			if (1 < jsymName->declarations()->length()) {
				// There may be multiple declarations, but they should have the same type (structurally).
				// We strenghten this requirement even more for now.
				auto const jdeclFirst = jsymName->declarations()[0];
				auto const nModifierFlagsFirst = ts()->getCombinedModifierFlags(jdeclFirst);
				auto const jtypeFirst = jtsTypeChecker->getTypeOfSymbolAtLocation(jsymName, jdeclFirst);
				tc::for_each(jsymName->declarations(), [&](ts::Declaration const jdeclCurrent) {
					auto const nModifierFlagsCurrent = ts()->getCombinedModifierFlags(jdeclCurrent);
					auto const jtypeCurrent = jtsTypeChecker->getTypeOfSymbolAtLocation(jsymName, jdeclCurrent);
					if (nModifierFlagsCurrent != nModifierFlagsFirst || !jtypeCurrent.getEmval().strictlyEquals(jtypeFirst.getEmval())) {
						tc::append(std::cerr, "JSVariableLike of symbol '", m_strJsName, "' has ", tc::as_dec(jsymName->declarations()->length()), " conflicting declarations\n");
						_ASSERTFALSE;
					}
				});
			}
			return jsymName->declarations()[0];
		}())
		, m_jtypeDeclared(jtsTypeChecker->getTypeOfSymbolAtLocation(jsymName, m_jdeclVariableLike))
		, m_mtType(MangleType(jtsTypeChecker, m_jtypeDeclared))
		, m_bReadonly(ts()->getCombinedModifierFlags(m_jdeclVariableLike) & ts::ModifierFlags::Readonly)
	{
		ts::ModifierFlags const nModifierFlags = ts()->getCombinedModifierFlags(m_jdeclVariableLike);
		if (ts::ModifierFlags::None != nModifierFlags &&
			ts::ModifierFlags::Export != nModifierFlags &&
			ts::ModifierFlags::Readonly != nModifierFlags &&
			ts::ModifierFlags::Ambient != ts()->getCombinedModifierFlags(m_jdeclVariableLike)) {
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
};

struct SJsFunctionLike {
	ts::Symbol m_jsymFunctionLike;
	std::string m_strCppifiedName;
	ts::Declaration m_jdeclFunctionLike;
	ts::SignatureDeclaration m_jtsSignatureDeclaration;
	ts::Signature m_jtsSignature;
	js_optional<ReadonlyArray<js_unknown>> m_joptarrunkTypeParameter;
	std::vector<SJsVariableLike> m_vecjsvariablelikeParameters;
	std::string m_strCppifiedParametersWithComments;
	std::string m_strCanonizedParameterCppTypes;

	SJsFunctionLike(ts::TypeChecker const jtsTypeChecker, ts::Symbol const jsymFunctionLike, ts::Declaration const jdeclFunctionLike) noexcept
		: m_jsymFunctionLike(jsymFunctionLike)
		, m_strCppifiedName(CppifyName(m_jsymFunctionLike))
		, m_jdeclFunctionLike(jdeclFunctionLike)
		, m_jtsSignatureDeclaration([&]() noexcept -> ts::SignatureDeclaration {
			if (auto const jotsMethodSignature = ts()->isMethodSignature(jdeclFunctionLike)) { // In interfaces.
				return *jotsMethodSignature;
			}
			if (auto const jotsMethodDeclaration = ts()->isMethodDeclaration(jdeclFunctionLike)) { // In classes.
				return *jotsMethodDeclaration;
			}
			if (auto const jotsConstructorDeclaration = ts()->isConstructorDeclaration(jdeclFunctionLike)) {
				return *jotsConstructorDeclaration;
			}
			if (auto const jotsFunctionDeclaration = ts()->isFunctionDeclaration(jdeclFunctionLike)) {
				return *jotsFunctionDeclaration;
			}
			_ASSERTFALSE;
		}())
		, m_jtsSignature(*jtsTypeChecker->getSignatureFromDeclaration(m_jtsSignatureDeclaration))
		, m_joptarrunkTypeParameter(m_jtsSignature->getTypeParameters())
		, m_vecjsvariablelikeParameters(tc::make_vector(tc::transform(
			m_jtsSignature->getParameters(),
			[&](ts::Symbol const jsymParameter) noexcept {
				return SJsVariableLike(jtsTypeChecker, jsymParameter);
			}
		)))
		, m_strCppifiedParametersWithComments(tc::explicit_cast<std::string>(tc::join_separated(
			tc::transform(m_vecjsvariablelikeParameters, [](SJsVariableLike const& jsvariablelikeParameter) noexcept {
				return tc::concat(jsvariablelikeParameter.m_mtType.m_strWithComments, " ", jsvariablelikeParameter.m_strCppifiedName);
			}),
			", "
		)))
		, m_strCanonizedParameterCppTypes(tc::explicit_cast<std::string>(tc::join_separated(
			tc::transform(m_vecjsvariablelikeParameters, [](SJsVariableLike const& jsvariablelikeParameter) noexcept {
				return jsvariablelikeParameter.m_mtType.m_strCppCanonized;
			}),
			", "
		)))
	{
		if (ts::ModifierFlags::None != ts()->getCombinedModifierFlags(jdeclFunctionLike) &&
			ts::ModifierFlags::Export != ts()->getCombinedModifierFlags(jdeclFunctionLike) &&
			ts::ModifierFlags::Ambient != ts()->getCombinedModifierFlags(jdeclFunctionLike)) {
			tc::append(std::cerr,
				"Unknown getCombinedModifierFlags for jdeclFunctionLike ",
				tc::explicit_cast<std::string>(m_jsymFunctionLike->getName()),
				": ",
				tc::as_dec(static_cast<int>(ts()->getCombinedModifierFlags(jdeclFunctionLike))),
				"\n"
			);
			_ASSERTFALSE;
		}
	}

	static bool LessCppSignature(SJsFunctionLike const& a, SJsFunctionLike const& b) noexcept {
		if (a.m_strCppifiedName != b.m_strCppifiedName) {
			return a.m_strCppifiedName < b.m_strCppifiedName;
		}
		return a.m_strCanonizedParameterCppTypes < b.m_strCanonizedParameterCppTypes;
	}
};

template<typename Rng>
void MergeWithSameCppSignatureInplace(Rng& rngjsfunctionlikeFuncs) noexcept {
	tc::sort_unique_inplace(
		rngjsfunctionlikeFuncs,
		&SJsFunctionLike::LessCppSignature
	);
	// TODO: add comments about skipped overloads;
}

template<typename Rng>
void MergeVariableRedeclarationsInplace(Rng& rngjsvariablelikeVars) noexcept {
	tc::sort_accumulate_each_unique_range(
		rngjsvariablelikeVars,
		[](SJsVariableLike const& a, SJsVariableLike const& b) { return a.m_strJsName < b.m_strJsName; },
		[](SJsVariableLike const& first, SJsVariableLike const& current) {
			_ASSERT(current.m_jsymName.getEmval().strictlyEquals(first.m_jsymName.getEmval()));
		}
	);
}

struct SJsClass {
	ts::Symbol m_jsymClass;
	std::string m_strMangledName;
	std::vector<ts::Symbol> m_vecjsymExportOfModule;
	std::vector<ts::Symbol> m_vecjsymExportType;
	std::vector<SJsFunctionLike> m_vecjsfunctionlikeExportFunction;
	std::vector<SJsVariableLike> m_vecjsvariablelikeExportVariable;
	std::vector<ts::Symbol> m_vecjsymMember;
	std::vector<SJsFunctionLike> m_vecjsfunctionlikeMethod;
	std::vector<SJsVariableLike> m_vecjsvariablelikeProperty;
	std::vector<ts::Symbol> m_vecjsymBaseClass;
	bool m_bHasImplicitDefaultConstructor;

	SJsClass(ts::TypeChecker const jtsTypeChecker, ts::Symbol const jsymClass) noexcept
		: m_jsymClass(jsymClass)
		, m_strMangledName(MangleSymbolName(jtsTypeChecker, jsymClass))
		, m_vecjsymExportOfModule(tc::make_vector(
			tc_conditional_range(
				jsymClass->getFlags() & ts::SymbolFlags::Module,
				jtsTypeChecker->getExportsOfModule(jsymClass),
				tc::make_empty_range<ts::Symbol>()
			)
		))
		, m_vecjsymExportType(tc::make_vector(tc::filter(
			m_vecjsymExportOfModule,
			[](ts::Symbol const jsymExport) noexcept {
				return IsEnumInCpp(jsymExport) || IsClassInCpp(jsymExport);
			}
		)))
		, m_vecjsfunctionlikeExportFunction(tc::make_vector(tc::join(tc::transform(
			tc::filter(
				m_vecjsymExportOfModule,
				[](ts::Symbol const jsymExport) noexcept {
					return ts::SymbolFlags::Function == jsymExport->getFlags();
				}
			),
			[&](ts::Symbol const jsymFunction) noexcept {
				return tc::transform(
					jsymFunction->declarations(),
					[&jtsTypeChecker, jsymFunction](ts::Declaration const jdeclFunction) noexcept {
						return SJsFunctionLike(jtsTypeChecker, jsymFunction, jdeclFunction);
					}
				);
			}
		))))
		, m_vecjsvariablelikeExportVariable(tc::make_vector(tc::transform(
			tc::filter(
				m_vecjsymExportOfModule,
				[](ts::Symbol const jsymExport) noexcept {
					return
						ts::SymbolFlags::FunctionScopedVariable == jsymExport->getFlags() ||
						ts::SymbolFlags::BlockScopedVariable == jsymExport->getFlags();
				}
			),
			[&](ts::Symbol const jsymVariable) noexcept {
				return SJsVariableLike(jtsTypeChecker, jsymVariable);
			}
		)))
		, m_vecjsymMember([&]() noexcept {
			if (jsymClass->members()) {
				return tc::explicit_cast<std::vector<ts::Symbol>>(*jsymClass->members());
			} else {
				return std::vector<ts::Symbol>();
			}
		}())
		, m_vecjsfunctionlikeMethod(tc::make_vector(tc::join(tc::transform(
			tc::filter(m_vecjsymMember, [](ts::Symbol const jsymMember) noexcept {
				return ts::SymbolFlags::Method == jsymMember->getFlags() || ts::SymbolFlags::Constructor == jsymMember->getFlags();
			}),
			[&](ts::Symbol const jsymMethod) noexcept {
				return tc::transform(
					jsymMethod->declarations(),
					[&jtsTypeChecker, jsymMethod](ts::Declaration const jdeclMethod) noexcept {
						return SJsFunctionLike(jtsTypeChecker, jsymMethod, jdeclMethod);
					}
				);
			}
		))))
		, m_vecjsvariablelikeProperty(tc::make_vector(tc::transform(
			tc::filter(m_vecjsymMember, [](ts::Symbol const jsymMember) noexcept {
				return ts::SymbolFlags::Property == jsymMember->getFlags();
			}),
			[&](ts::Symbol const jsymProperty) noexcept {
				return SJsVariableLike(jtsTypeChecker, jsymProperty);
			}
		)))
		, m_vecjsymBaseClass()
		, m_bHasImplicitDefaultConstructor(false)
	{
		MergeWithSameCppSignatureInplace(m_vecjsfunctionlikeExportFunction);
		MergeWithSameCppSignatureInplace(m_vecjsfunctionlikeMethod);
		MergeVariableRedeclarationsInplace(m_vecjsvariablelikeExportVariable);
		// MergeVariableRedeclarationsInplace(m_vecjsvariablelikeProperty); // Properties cannot be redeclared.
		if (auto jointerfacetypeClass = jtsTypeChecker->getDeclaredTypeOfSymbol(jsymClass)->isClassOrInterface()) {
			tc::for_each(jtsTypeChecker->getBaseTypes(*jointerfacetypeClass),
				[&](ts::BaseType const jtsBaseType) noexcept {
					if (auto const jointerfacetypeBase = tc::reluctant_implicit_cast<ts::Type>(jtsBaseType)->isClassOrInterface()) {
						tc::cont_emplace_back(m_vecjsymBaseClass, *(*jointerfacetypeBase)->getSymbol());
					}
				}
			);
			// See `utilities.ts:getAllSuperTypeNodes`. `extends` clause for classes is already covered by `getBaseTypes()`, as well as `implements` clause for interfaces.
			// The only missing part is `implements` for classes.
			tc::for_each(jsymClass->declarations(), [&](ts::Declaration jdeclClass) {
				if (auto joclassdeclarationClass = ts()->isClassDeclaration(jdeclClass)) {
					auto jorarrHeritageClause = (*joclassdeclarationClass)->heritageClauses();
					if (jorarrHeritageClause) {
						tc::for_each(*jorarrHeritageClause, [&](ts::HeritageClause jtsHeritageClause) {
							if (jtsHeritageClause->token() == ts::SyntaxKind::ImplementsKeyword) {
								tc::for_each(jtsHeritageClause->types(), [&](ts::Node jnodeImplementsType) {
									auto jtypeImplements = jtsTypeChecker->getTypeAtLocation(jnodeImplementsType);
									auto josymImplements = jtypeImplements->getSymbol();
									if (!josymImplements) {
										tc::append(std::cerr,
											"Unable to determine 'implements' symbol for class '",
											tc::explicit_cast<std::string>(jsymClass->getName()),
											"' for type '",
											tc::explicit_cast<std::string>(jtsTypeChecker->typeToString(jtypeImplements)),
											"'\n"
										);
										_ASSERTFALSE;
									}
									tc::cont_emplace_back(m_vecjsymBaseClass, *josymImplements);
								});
							}
						});
					}
				}
			});

			if ((*jointerfacetypeClass)->isClass()) {
				// See logic at `src/compiler/transformers/es2015.ts`, `addConstructor`, `transformConstructorBody` and `createDefaultConstructorBody`:
				// if no constructrs are defined, we add a "default" constructor with no parameters.
				m_bHasImplicitDefaultConstructor = !tc::find_first_if<tc::return_bool>(
					m_vecjsfunctionlikeMethod,
					[](SJsFunctionLike const& jsfunctionlikeMethod) noexcept {
						return ts::SymbolFlags::Constructor == jsfunctionlikeMethod.m_jsymFunctionLike->getFlags();
					}
				);
			}
		} else {
			// Do nothing: e.g. namespaces.
		}
	}
};

int main(int argc, char* argv[]) {
	_ASSERT(2 <= argc);

	ts::CompilerOptions const jtsCompilerOptions(create_js_object);
	jtsCompilerOptions->strict(true);
	jtsCompilerOptions->target(ts::ScriptTarget::ES5);
	jtsCompilerOptions->module(ts::ModuleKind::CommonJS);

	auto const rngstrFileNames = tc::make_iterator_range(argv + 1, argv + argc);
	ts::Program const jtsProgram = ts()->createProgram(ReadonlyArray<js_string>(create_js_object, rngstrFileNames), jtsCompilerOptions);

	ts::TypeChecker const jtsTypeChecker = jtsProgram->getTypeChecker();

	{
		auto const jtsReadOnlyArrayDiagnostics = ts()->getPreEmitDiagnostics(jtsProgram);
		if (jtsReadOnlyArrayDiagnostics->length()) {
			console::log(ts()->formatDiagnosticsWithColorAndContext(jtsReadOnlyArrayDiagnostics, ts()->createCompilerHost(jtsCompilerOptions)));
			return 1;
		}
	}

	std::vector<ts::Symbol> vecjsymExportedModule;
	std::vector<ts::Symbol> vecjsymExportedSymbol;
	tc::for_each(jtsProgram->getSourceFiles(),
		[&](ts::SourceFile const& jtsSourceFile) noexcept {
			if (!tc::find_unique<tc::return_bool>(rngstrFileNames, tc::explicit_cast<std::string>(jtsSourceFile->fileName()))) {
				return;
			}
			auto const josymSourceFile = jtsTypeChecker->getSymbolAtLocation(jtsSourceFile);
			if (!josymSourceFile) {
				tc::append(std::cerr, "Module not found for ", tc::explicit_cast<std::string>(jtsSourceFile->fileName()), ", treating at as a global library\n");
				tc::append(vecjsymExportedSymbol, ListSourceFileTopLevel(jtsTypeChecker, jtsSourceFile));
				return;
			}
			tc::cont_emplace_back(vecjsymExportedModule, *josymSourceFile);
		}
	);

	tc::for_each(
		vecjsymExportedModule,
		[&](ts::Symbol const& jsymSourceFile) noexcept {
			tc::append(std::cerr, "Module name is ", tc::explicit_cast<std::string>(jsymSourceFile->getName()), "\n");
			WalkSymbol(jtsTypeChecker, 0, jsymSourceFile);
		}
	);

	tc::for_each(
		vecjsymExportedSymbol,
		[&](ts::Symbol const& jsymSymbol) noexcept {
			tc::append(std::cerr, "Global exported symbol is ", tc::explicit_cast<std::string>(jsymSymbol->getName()), "\n");
			WalkSymbol(jtsTypeChecker, 0, jsymSymbol);
		}
	);

	tc::append(std::cerr, "\n========== GENERATED CODE ==========\n");

	{
		tc::for_each(g_vecjsymEnum, [&](ts::Symbol const jsymEnum) noexcept {
			g_usstrAllowedMangledTypes.insert(MangleSymbolName(jtsTypeChecker, jsymEnum));
		});

		tc::for_each(g_vecjsymClass, [&](ts::Symbol const jsymClass) noexcept {
			g_usstrAllowedMangledTypes.insert(MangleSymbolName(jtsTypeChecker, jsymClass));
		});

		auto SortClasses = [&jtsTypeChecker](std::vector<SJsClass> vecjsclassOriginal) {
			std::map<std::string, SJsClass const*> mstrjsclassClasses;
			tc::for_each(vecjsclassOriginal, [&](SJsClass const& jsclassClass) {
				mstrjsclassClasses.emplace(jsclassClass.m_strMangledName, &jsclassClass);
			});

			std::vector<SJsClass> vecjsclassResult;
			std::unordered_set<std::string> usstrInResult;

			auto dfs = [&](SJsClass const& jsclassClass, auto const& dfs) {
				if (usstrInResult.count(jsclassClass.m_strMangledName)) {
					return;
				}
				usstrInResult.insert(jsclassClass.m_strMangledName);
				tc::for_each(jsclassClass.m_vecjsymBaseClass, [&](ts::Symbol const& jsymBaseClass) {
					auto strMangledBaseClass = MangleSymbolName(jtsTypeChecker, jsymBaseClass);
					if (mstrjsclassClasses.count(strMangledBaseClass)) {
						dfs(*mstrjsclassClasses[strMangledBaseClass], dfs);
					}
				});
				vecjsclassResult.emplace_back(jsclassClass);
			};
			tc::for_each(vecjsclassOriginal, [&](SJsClass const& jsclassCls) {
				dfs(jsclassCls, dfs);
			});
			return vecjsclassResult;
		};

		auto vecjsenumEnum = tc::make_vector(tc::transform(g_vecjsymEnum, [&](ts::Symbol const jsymEnum) noexcept {
			return SJsEnum(jtsTypeChecker, jsymEnum);
		}));

		auto vecjsclassClass = SortClasses(tc::make_vector(tc::transform(g_vecjsymClass, [&](ts::Symbol const jsymClass) noexcept {
			return SJsClass(jtsTypeChecker, jsymClass);
		})));

		tc::append(std::cout,
			"namespace tc::js_defs {\n",
			tc::join(tc::transform(vecjsenumEnum, [](SJsEnum const& jsenumEnum) noexcept {
				// We have to mark enums as IsJsIntegralEnum before using in js interop.
				return tc::concat(
					"enum class _enum", jsenumEnum.m_strMangledName, " {\n",
					tc::join(
						tc::transform(jsenumEnum.m_vecjsenumoption, [&jsenumEnum](SJsEnumOption const& jsenumoption) noexcept {
							return tc_conditional_range(
								jsenumoption.m_ovardblstrValue,
								tc::concat(
									"	", jsenumoption.m_strCppifiedName,
									tc_conditional_range(
										jsenumEnum.m_bIsIntegral,
										tc::concat(" = ", tc::as_dec(tc::explicit_cast<int>(std::get<double>(*jsenumoption.m_ovardblstrValue))))
									),
									",\n"
								),
								tc::concat(
									"	/*", jsenumoption.m_strJsName, " = ??? */\n"
								)
							);
						})
					),
					"};\n"
				);
			})),
			"} // namespace tc::js_defs\n",
			"namespace tc::jst {\n",
			tc::join(tc::transform(vecjsenumEnum, [](SJsEnum const& jsenumEnum) noexcept {
				// Enums are declared outside of the _jsall class because we have to mark them as IsJsIntegralEnum
				// before using in js interop.
				return tc_conditional_range(
					jsenumEnum.m_bIsIntegral,
					tc::concat(
						"template<> struct IsJsIntegralEnum<js_defs::_enum", jsenumEnum.m_strMangledName, "> : std::true_type {};\n"
					),
					tc::concat(
						"template<> struct IsJsHeterogeneousEnum<js_defs::_enum", jsenumEnum.m_strMangledName, "> : std::true_type {\n",
						"	static inline auto const& Values() {\n",
						"		using E = js_defs::_enum", jsenumEnum.m_strMangledName, ";\n",
						"		static tc::unordered_map<E, jst::js_unknown> vals{\n",
						tc::join_separated(
							tc::transform(
								tc::filter(jsenumEnum.m_vecjsenumoption, TC_MEMBER(.m_ovardblstrValue)),
								[](SJsEnumOption const& jsenumoption) noexcept {
									_ASSERT(jsenumoption.m_ovardblstrValue);
									return tc::concat(
										"			{E::", jsenumoption.m_strCppifiedName, ", ",
										tc::visit(*jsenumoption.m_ovardblstrValue,
											[](double dblValue) {
												// TODO: std::to_string because of floating-point numbers. May be not enough precision.
												return tc::explicit_cast<std::string>(tc::concat("js_unknown(", std::to_string(dblValue), ")"));
											},
											[](std::string const& strValue) {
												return tc::explicit_cast<std::string>(tc::concat("js_string(\"", strValue, "\")"));
											}
										),
										"}"
									);
								}
							),
							",\n"
						),
						"\n",
						"		};\n",
						"		return vals;\n",
						"	}\n",
						"};\n"
					)
				);
			})),
			"} // namespace tc::jst\n",
			"namespace tc::js_defs {\n",
			"	using namespace jst; // no ADL\n",
			tc::join(tc::transform(vecjsenumEnum, [](SJsEnum const& jsenumEnum) noexcept {
				return tc::concat(
					"	using ", jsenumEnum.m_strMangledName, " = _enum", jsenumEnum.m_strMangledName, ";\n"
				);
			})),
			tc::join(tc::transform(vecjsclassClass, [](SJsClass const& jsclassClass) noexcept {
				return tc::concat(
					"	struct _impl", jsclassClass.m_strMangledName, ";\n",
					"	using ", jsclassClass.m_strMangledName, " = js_ref<_impl", jsclassClass.m_strMangledName, ">;\n"
				);
			})),
			tc::join(tc::transform(vecjsclassClass, [&jtsTypeChecker](SJsClass const& jsclassClass) noexcept {
				std::vector<std::string> vecstrBaseClass, vecstrUnknownBaseClass;
				 tc::for_each(
					jsclassClass.m_vecjsymBaseClass,
					[&](ts::Symbol const jsymBaseClass) noexcept {
						auto strMangledName = MangleSymbolName(jtsTypeChecker, jsymBaseClass);
						if (g_usstrAllowedMangledTypes.count(strMangledName)) {
							tc::cont_emplace_back(vecstrBaseClass, strMangledName);
						} else {
							tc::cont_emplace_back(vecstrUnknownBaseClass, strMangledName);
						}
					}
				);
				return tc::explicit_cast<std::string>(tc::concat(
					"	struct _impl", jsclassClass.m_strMangledName,
					" : ",
					tc_conditional_range(
						tc::empty(vecstrBaseClass),
						"virtual IObject",
						tc::join_separated(
							tc::transform(vecstrBaseClass,
								[](std::string const& strMangledName) noexcept {
									return tc::concat("virtual _impl", strMangledName);
								}
							),
							", "
						)
					),
					tc::join(tc::transform(
						vecstrUnknownBaseClass,
						[](std::string const& strMangledName) noexcept {
							return tc::concat("/* : ", strMangledName, " */");
						}
					)),
					" {\n",
					"		struct _tcjs_definitions {\n",
					tc::join(tc::transform(
						jsclassClass.m_vecjsymExportType,
						[&jtsTypeChecker](ts::Symbol const jsymExport) noexcept {
							return tc::concat(
								"			using ",
								CppifyName(jsymExport),
								" = ",
								MangleSymbolName(jtsTypeChecker, jsymExport),
								";\n"
							);
						}
					)),
					tc::join(tc::transform(
						jsclassClass.m_vecjsfunctionlikeExportFunction,
						[](SJsFunctionLike const& jsfunctionlikeFunction) noexcept {
							return tc::concat(
								"			static auto ", jsfunctionlikeFunction.m_strCppifiedName, "(", jsfunctionlikeFunction.m_strCppifiedParametersWithComments, ") noexcept;\n"
							);
						}
					)),
					tc::join(tc::transform(
						jsclassClass.m_vecjsvariablelikeExportVariable,
						[](SJsVariableLike const& jsvariablelikeVariable) noexcept {
							return tc::concat(
								"			static auto ", jsvariablelikeVariable.m_strCppifiedName, "() noexcept;\n",
								tc_conditional_range(
									jsvariablelikeVariable.m_bReadonly,
									"",
									tc::concat(
										"			static void ", jsvariablelikeVariable.m_strCppifiedName, "(", jsvariablelikeVariable.m_mtType.m_strWithComments, " v) noexcept;\n"
									)
								)
							);
						}
					)),
					"		};\n",
					tc::join(tc::transform(
						jsclassClass.m_vecjsvariablelikeProperty,
						[](SJsVariableLike const& jsvariablelikeProperty) noexcept {
							return tc::concat(
								"		auto ", jsvariablelikeProperty.m_strCppifiedName, "() noexcept;\n",
								tc_conditional_range(
									jsvariablelikeProperty.m_bReadonly,
									"",
									tc::concat(
										"		void ", jsvariablelikeProperty.m_strCppifiedName, "(", jsvariablelikeProperty.m_mtType.m_strWithComments, " v) noexcept;\n"
									)
								)
							);
						}
					)),
					tc::join(tc::transform(
						jsclassClass.m_vecjsfunctionlikeMethod,
						[](SJsFunctionLike const& jsfunctionlikeMethod) noexcept {
							if (ts::SymbolFlags::Method == jsfunctionlikeMethod.m_jsymFunctionLike->getFlags()) {
								return tc::explicit_cast<std::string>(tc::concat(
									"		auto ", jsfunctionlikeMethod.m_strCppifiedName, "(", jsfunctionlikeMethod.m_strCppifiedParametersWithComments, ") noexcept;\n"
								));
							} else if (ts::SymbolFlags::Constructor == jsfunctionlikeMethod.m_jsymFunctionLike->getFlags()) {
								return tc::explicit_cast<std::string>(tc::concat(
									"		static auto _tcjs_construct(", jsfunctionlikeMethod.m_strCppifiedParametersWithComments, ") noexcept;\n"
								));
							} else {
								_ASSERTFALSE;
							}
						}
					)),
					tc_conditional_range(
						jsclassClass.m_bHasImplicitDefaultConstructor,
						"		static auto _tcjs_construct() noexcept;\n"
					),
					"	};\n"
				));
			})),
			tc::join(tc::transform(vecjsclassClass, [&jtsTypeChecker](SJsClass const& jsclassClass) noexcept {
				auto strClassNamespace = tc::explicit_cast<std::string>(tc::concat(
					"_impl", jsclassClass.m_strMangledName, "::"
				));
				auto strClassInstanceRetrieve = RetrieveSymbolFromCpp(jsclassClass.m_jsymClass);
				return tc::explicit_cast<std::string>(tc::concat(
					tc::join(tc::transform(
						jsclassClass.m_vecjsfunctionlikeExportFunction,
						[&jtsTypeChecker, &strClassNamespace](SJsFunctionLike const& jsfunctionlikeFunction) noexcept {
							auto const rngchCallArguments = tc::join_separated(
								tc::transform(
									jsfunctionlikeFunction.m_jtsSignature->getParameters(),
									[](ts::Symbol const jsymParameter) noexcept {
										return CppifyName(jsymParameter);
									}
								),
								", "
							);
							auto const rngchFunctionCall = tc::concat(
								RetrieveSymbolFromCpp(jsfunctionlikeFunction.m_jsymFunctionLike), "(", rngchCallArguments, ")"
							);
							return tc::explicit_cast<std::string>(tc::concat(
								"	inline auto ", strClassNamespace, "_tcjs_definitions::",
									jsfunctionlikeFunction.m_strCppifiedName, "(", jsfunctionlikeFunction.m_strCppifiedParametersWithComments, ") noexcept {\n",
								tc_conditional_range(
									ts::TypeFlags::Void == jsfunctionlikeFunction.m_jtsSignature->getReturnType()->flags(),
									tc::concat("		", rngchFunctionCall, ";\n"),
									tc::concat(
										"		return ", rngchFunctionCall, ".template as<", MangleType(jtsTypeChecker, jsfunctionlikeFunction.m_jtsSignature->getReturnType()).m_strWithComments, ">();\n"
									)
								),
								"	}\n"
							));
						}
					)),
					tc::join(tc::transform(
						jsclassClass.m_vecjsvariablelikeExportVariable,
						[&strClassNamespace, &strClassInstanceRetrieve](SJsVariableLike const& jsvariablelikeVariable) noexcept {
							return tc::concat(
								"	inline auto ", strClassNamespace, "_tcjs_definitions::", jsvariablelikeVariable.m_strCppifiedName, "() noexcept ",
								"{ return ", strClassInstanceRetrieve, "[\"", jsvariablelikeVariable.m_strJsName, "\"].template as<", jsvariablelikeVariable.m_mtType.m_strWithComments, ">(); }\n",
								tc_conditional_range(
									jsvariablelikeVariable.m_bReadonly,
									"",
									tc::concat(
										"	inline void ", strClassNamespace, "_tcjs_definitions::", jsvariablelikeVariable.m_strCppifiedName, "(", jsvariablelikeVariable.m_mtType.m_strWithComments, " v) noexcept ",
										"{ ", strClassInstanceRetrieve, ".set(\"", jsvariablelikeVariable.m_strJsName, "\", v); }\n"
									)
								)
							);
						}
					)),
					tc::join(tc::transform(
						jsclassClass.m_vecjsvariablelikeProperty,
						[&strClassNamespace](SJsVariableLike const& jsvariablelikeProperty) noexcept {
							return tc::concat(
								"	inline auto ", strClassNamespace, jsvariablelikeProperty.m_strCppifiedName, "() noexcept ",
								"{ return _getProperty<", jsvariablelikeProperty.m_mtType.m_strWithComments, ">(\"", jsvariablelikeProperty.m_strJsName, "\"); }\n",
								tc_conditional_range(
									jsvariablelikeProperty.m_bReadonly,
									"",
									tc::concat(
										"	inline void ", strClassNamespace, jsvariablelikeProperty.m_strCppifiedName, "(", jsvariablelikeProperty.m_mtType.m_strWithComments, " v) noexcept ",
										"{ _setProperty(\"", jsvariablelikeProperty.m_strJsName, "\", v); }\n"
									)
								)
							);
						}
					)),
					tc::join(tc::transform(
						jsclassClass.m_vecjsfunctionlikeMethod,
						[&jtsTypeChecker, &jsclassClass, &strClassNamespace](SJsFunctionLike const& jsfunctionlikeMethod) noexcept {
							auto const rngstrArguments = tc::transform(
								jsfunctionlikeMethod.m_jtsSignature->getParameters(),
								[](ts::Symbol const jsymParameter) noexcept {
									return CppifyName(jsymParameter);
								}
							);
							if (ts::SymbolFlags::Method == jsfunctionlikeMethod.m_jsymFunctionLike->getFlags()) {
								auto const rngchCallArguments = tc::join_separated(
									tc::concat(
										tc::single(tc::concat("\"", tc::explicit_cast<std::string>(jsfunctionlikeMethod.m_jsymFunctionLike->getName()), "\"")),
										rngstrArguments
									),
									", "
								);
								return tc::explicit_cast<std::string>(tc::concat(
									"	inline auto ", strClassNamespace, jsfunctionlikeMethod.m_strCppifiedName, "(", jsfunctionlikeMethod.m_strCppifiedParametersWithComments, ") noexcept {\n",
									"		return _call<", MangleType(jtsTypeChecker, jsfunctionlikeMethod.m_jtsSignature->getReturnType()).m_strWithComments, ">(", rngchCallArguments, ");\n",
									"	}\n"
								));
							} else if (ts::SymbolFlags::Constructor == jsfunctionlikeMethod.m_jsymFunctionLike->getFlags()) {
								auto const rngchCallArguments = tc::join_separated(rngstrArguments, ", ");
								return tc::explicit_cast<std::string>(tc::concat(
									"	inline auto ", strClassNamespace, "_tcjs_construct(", jsfunctionlikeMethod.m_strCppifiedParametersWithComments, ") noexcept {\n",
									"		return ", jsclassClass.m_strMangledName, "(", RetrieveSymbolFromCpp(jsclassClass.m_jsymClass), ".new_(", rngchCallArguments, "));\n",
									"	}\n"
								));
							} else {
								_ASSERTFALSE;
							}
						}
					)),
					tc_conditional_range(
						jsclassClass.m_bHasImplicitDefaultConstructor,
						tc::concat(
							"	inline auto ", strClassNamespace, "_tcjs_construct() noexcept {\n",
							"		return ", jsclassClass.m_strMangledName, "(", RetrieveSymbolFromCpp(jsclassClass.m_jsymClass), ".new_());\n",
							"	}\n"
						)
					)
				));
			}))
		);

		std::vector<ts::Symbol> vecjsymGlobalType;
		std::vector<SJsFunctionLike> vecjsfunctionlikeGlobalFunction;
		std::vector<SJsVariableLike> vecjsvariablelikeGlobalVariable;
		std::vector<ts::Symbol> vecjsymGlobalUnknown;

		tc::for_each(vecjsymExportedSymbol, [&](ts::Symbol const& jsymExported) noexcept {
			if (IsClassInCpp(jsymExported) || IsEnumInCpp(jsymExported)) {
				tc::cont_emplace_back(vecjsymGlobalType, jsymExported);
			} else if (ts::SymbolFlags::Function == jsymExported->getFlags()) {
				tc::append(vecjsfunctionlikeGlobalFunction, tc::transform(
					jsymExported->declarations(),
					[&](ts::Declaration const jdeclFunction) {
						return SJsFunctionLike(jtsTypeChecker, jsymExported, jdeclFunction);
					}
				));
			} else if (ts::SymbolFlags::FunctionScopedVariable == jsymExported->getFlags() ||
				ts::SymbolFlags::BlockScopedVariable == jsymExported->getFlags()) {
				tc::cont_emplace_back(vecjsvariablelikeGlobalVariable, jtsTypeChecker, jsymExported);
			} else {
				tc::cont_emplace_back(vecjsymGlobalUnknown, jsymExported);
			}
		});

		MergeWithSameCppSignatureInplace(vecjsfunctionlikeGlobalFunction);
		MergeVariableRedeclarationsInplace(vecjsvariablelikeGlobalVariable);

		tc::append(std::cout,
			tc::join(tc::transform(
				vecjsfunctionlikeGlobalFunction,
				[&jtsTypeChecker](SJsFunctionLike& jsfunctionlikeFunction) {
					// TODO: deduplicate following code into SJsFunctionLike.
					auto const rngchCallArguments = tc::join_separated(
						tc::transform(
							jsfunctionlikeFunction.m_jtsSignature->getParameters(),
							[](ts::Symbol const jsymParameter) noexcept {
								return CppifyName(jsymParameter);
							}
						),
						", "
					);
					auto const rngchFunctionCall = tc::concat(
						RetrieveSymbolFromCpp(jsfunctionlikeFunction.m_jsymFunctionLike), "(", rngchCallArguments, ")"
					);
					return tc::explicit_cast<std::string>(tc::concat(
						"	inline auto ",
							jsfunctionlikeFunction.m_strCppifiedName, "(", jsfunctionlikeFunction.m_strCppifiedParametersWithComments, ") noexcept {\n",
						tc_conditional_range(
							ts::TypeFlags::Void == jsfunctionlikeFunction.m_jtsSignature->getReturnType()->flags(),
							tc::concat("		", rngchFunctionCall, ";\n"),
							tc::concat(
								"		return ", rngchFunctionCall, ".template as<", MangleType(jtsTypeChecker, jsfunctionlikeFunction.m_jtsSignature->getReturnType()).m_strWithComments, ">();\n"
							)
						),
						"	}\n"
					));
				}
			)),
			tc::join(tc::transform(
				vecjsvariablelikeGlobalVariable,
				[&](SJsVariableLike const& jsvariablelikeVariable) noexcept {
					// TODO: deduplicate following code into SJsVariableLike.
					return tc::explicit_cast<std::string>(tc::concat(
						"	inline auto ", jsvariablelikeVariable.m_strCppifiedName, "() noexcept ",
						"{ return emscripten::val::global(\"", jsvariablelikeVariable.m_strJsName, "\").template as<", jsvariablelikeVariable.m_mtType.m_strWithComments, ">(); }\n",
						tc_conditional_range(
							jsvariablelikeVariable.m_bReadonly,
							"",
							tc::concat(
								"	inline void ", jsvariablelikeVariable.m_strCppifiedName, "(", jsvariablelikeVariable.m_mtType.m_strWithComments, " v) noexcept ",
								"{ emscripten::val::global().set(\"", jsvariablelikeVariable.m_strJsName, "\", v); }\n"
							)
						)
					));
				}
			)),
			"}; // namespace tc::js_defs\n",
			"namespace tc::js {\n",
			tc::join(tc::transform(
				vecjsymExportedModule,
				[&jtsTypeChecker](ts::Symbol const& jsymSourceFile) noexcept {
					_ASSERT(IsClassInCpp(jsymSourceFile));
					return tc::concat(
						"using ", CppifyName(jsymSourceFile), " = js_defs::", MangleSymbolName(jtsTypeChecker, jsymSourceFile), ";\n"
					);
				}
			))
		);

		tc::append(std::cout,
			tc::join(tc::transform(
				vecjsymGlobalType,
				[&jtsTypeChecker](ts::Symbol const& jsymType) noexcept {
					return tc::explicit_cast<std::string>(tc::concat(
						"	using ", CppifyName(jsymType), " = js_defs::", MangleSymbolName(jtsTypeChecker, jsymType), ";\n"
					));
				}
			)),
			tc::join(tc::transform(
				vecjsfunctionlikeGlobalFunction,
				[](SJsFunctionLike& jsfunctionlikeFunction) {
					return tc::concat("	using js_defs::", jsfunctionlikeFunction.m_strCppifiedName, ";\n");
				}
			)),
			tc::join(tc::transform(
				vecjsvariablelikeGlobalVariable,
				[](SJsVariableLike const& jsvariablelikeVariable) noexcept {
					return tc::concat("	using js_defs::", jsvariablelikeVariable.m_strCppifiedName, ";\n");
				}
			)),
			tc::join(tc::transform(
				vecjsymGlobalUnknown,
				[&jtsTypeChecker](ts::Symbol const& jsymUnknown) noexcept {
					return tc::explicit_cast<std::string>(tc::concat(
						"	/* ", tc::explicit_cast<std::string>(jtsTypeChecker->getFullyQualifiedName(jsymUnknown)), " flags=", tc::as_dec(static_cast<int>(jsymUnknown->getFlags())), "*/\n"
					));
				}
			)),
			"} // namespace tc::js\n"
		);
	}
	return 0;
}
