#pragma once
/**
 * This file re-uses parts of TypeScript compiler API definitions which are
 * available at https://github.com/microsoft/TypeScript/blob/v3.5.3/lib/typescript.d.ts
 * Lots of modifications and original additions includes.
 * Original copyright for parts of the file is retained below.
 */

/*! *****************************************************************************
Copyright (c) Microsoft Corporation. All rights reserved.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the
License at http://www.apache.org/licenses/LICENSE-2.0

THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY IMPLIED
WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE,
MERCHANTABLITY OR NON-INFRINGEMENT.

See the Apache Version 2.0 License for specific language governing permissions
and limitations under the License.
***************************************************************************** */

#include <emscripten/val.h>
#include <optional>
#include "explicit_cast.h"
#include "range_defines.h"
#include "js_types.h"
#include "js_callback.h"
#include "js_bootstrap.h"

namespace tc::js {
namespace globals::enums_adl {
struct _jsenums_ts {
	enum class SyntaxKind {
		Unknown = 0,
		EndOfFileToken = 1,
		SingleLineCommentTrivia = 2,
		MultiLineCommentTrivia = 3,
		NewLineTrivia = 4,
		WhitespaceTrivia = 5,
		ShebangTrivia = 6,
		ConflictMarkerTrivia = 7,
		NumericLiteral = 8,
		BigIntLiteral = 9,
		StringLiteral = 10,
		JsxText = 11,
		JsxTextAllWhiteSpaces = 12,
		RegularExpressionLiteral = 13,
		NoSubstitutionTemplateLiteral = 14,
		TemplateHead = 15,
		TemplateMiddle = 16,
		TemplateTail = 17,
		OpenBraceToken = 18,
		CloseBraceToken = 19,
		OpenParenToken = 20,
		CloseParenToken = 21,
		OpenBracketToken = 22,
		CloseBracketToken = 23,
		DotToken = 24,
		DotDotDotToken = 25,
		SemicolonToken = 26,
		CommaToken = 27,
		LessThanToken = 28,
		LessThanSlashToken = 29,
		GreaterThanToken = 30,
		LessThanEqualsToken = 31,
		GreaterThanEqualsToken = 32,
		EqualsEqualsToken = 33,
		ExclamationEqualsToken = 34,
		EqualsEqualsEqualsToken = 35,
		ExclamationEqualsEqualsToken = 36,
		EqualsGreaterThanToken = 37,
		PlusToken = 38,
		MinusToken = 39,
		AsteriskToken = 40,
		AsteriskAsteriskToken = 41,
		SlashToken = 42,
		PercentToken = 43,
		PlusPlusToken = 44,
		MinusMinusToken = 45,
		LessThanLessThanToken = 46,
		GreaterThanGreaterThanToken = 47,
		GreaterThanGreaterThanGreaterThanToken = 48,
		AmpersandToken = 49,
		BarToken = 50,
		CaretToken = 51,
		ExclamationToken = 52,
		TildeToken = 53,
		AmpersandAmpersandToken = 54,
		BarBarToken = 55,
		QuestionToken = 56,
		ColonToken = 57,
		AtToken = 58,
		/** Only the JSDoc scanner produces BacktickToken. The normal scanner produces NoSubstitutionTemplateLiteral and related kinds. */
		BacktickToken = 59,
		EqualsToken = 60,
		PlusEqualsToken = 61,
		MinusEqualsToken = 62,
		AsteriskEqualsToken = 63,
		AsteriskAsteriskEqualsToken = 64,
		SlashEqualsToken = 65,
		PercentEqualsToken = 66,
		LessThanLessThanEqualsToken = 67,
		GreaterThanGreaterThanEqualsToken = 68,
		GreaterThanGreaterThanGreaterThanEqualsToken = 69,
		AmpersandEqualsToken = 70,
		BarEqualsToken = 71,
		CaretEqualsToken = 72,
		Identifier = 73,
		BreakKeyword = 74,
		CaseKeyword = 75,
		CatchKeyword = 76,
		ClassKeyword = 77,
		ConstKeyword = 78,
		ContinueKeyword = 79,
		DebuggerKeyword = 80,
		DefaultKeyword = 81,
		DeleteKeyword = 82,
		DoKeyword = 83,
		ElseKeyword = 84,
		EnumKeyword = 85,
		ExportKeyword = 86,
		ExtendsKeyword = 87,
		FalseKeyword = 88,
		FinallyKeyword = 89,
		ForKeyword = 90,
		FunctionKeyword = 91,
		IfKeyword = 92,
		ImportKeyword = 93,
		InKeyword = 94,
		InstanceOfKeyword = 95,
		NewKeyword = 96,
		NullKeyword = 97,
		ReturnKeyword = 98,
		SuperKeyword = 99,
		SwitchKeyword = 100,
		ThisKeyword = 101,
		ThrowKeyword = 102,
		TrueKeyword = 103,
		TryKeyword = 104,
		TypeOfKeyword = 105,
		VarKeyword = 106,
		VoidKeyword = 107,
		WhileKeyword = 108,
		WithKeyword = 109,
		ImplementsKeyword = 110,
		InterfaceKeyword = 111,
		LetKeyword = 112,
		PackageKeyword = 113,
		PrivateKeyword = 114,
		ProtectedKeyword = 115,
		PublicKeyword = 116,
		StaticKeyword = 117,
		YieldKeyword = 118,
		AbstractKeyword = 119,
		AsKeyword = 120,
		AnyKeyword = 121,
		AsyncKeyword = 122,
		AwaitKeyword = 123,
		BooleanKeyword = 124,
		ConstructorKeyword = 125,
		DeclareKeyword = 126,
		GetKeyword = 127,
		InferKeyword = 128,
		IsKeyword = 129,
		KeyOfKeyword = 130,
		ModuleKeyword = 131,
		NamespaceKeyword = 132,
		NeverKeyword = 133,
		ReadonlyKeyword = 134,
		RequireKeyword = 135,
		NumberKeyword = 136,
		ObjectKeyword = 137,
		SetKeyword = 138,
		StringKeyword = 139,
		SymbolKeyword = 140,
		TypeKeyword = 141,
		UndefinedKeyword = 142,
		UniqueKeyword = 143,
		UnknownKeyword = 144,
		FromKeyword = 145,
		GlobalKeyword = 146,
		BigIntKeyword = 147,
		OfKeyword = 148,
		QualifiedName = 149,
		ComputedPropertyName = 150,
		TypeParameter = 151,
		Parameter = 152,
		Decorator = 153,
		PropertySignature = 154,
		PropertyDeclaration = 155,
		MethodSignature = 156,
		MethodDeclaration = 157,
		Constructor = 158,
		GetAccessor = 159,
		SetAccessor = 160,
		CallSignature = 161,
		ConstructSignature = 162,
		IndexSignature = 163,
		TypePredicate = 164,
		TypeReference = 165,
		FunctionType = 166,
		ConstructorType = 167,
		TypeQuery = 168,
		TypeLiteral = 169,
		ArrayType = 170,
		TupleType = 171,
		OptionalType = 172,
		RestType = 173,
		UnionType = 174,
		IntersectionType = 175,
		ConditionalType = 176,
		InferType = 177,
		ParenthesizedType = 178,
		ThisType = 179,
		TypeOperator = 180,
		IndexedAccessType = 181,
		MappedType = 182,
		LiteralType = 183,
		ImportType = 184,
		ObjectBindingPattern = 185,
		ArrayBindingPattern = 186,
		BindingElement = 187,
		ArrayLiteralExpression = 188,
		ObjectLiteralExpression = 189,
		PropertyAccessExpression = 190,
		ElementAccessExpression = 191,
		CallExpression = 192,
		NewExpression = 193,
		TaggedTemplateExpression = 194,
		TypeAssertionExpression = 195,
		ParenthesizedExpression = 196,
		FunctionExpression = 197,
		ArrowFunction = 198,
		DeleteExpression = 199,
		TypeOfExpression = 200,
		VoidExpression = 201,
		AwaitExpression = 202,
		PrefixUnaryExpression = 203,
		PostfixUnaryExpression = 204,
		BinaryExpression = 205,
		ConditionalExpression = 206,
		TemplateExpression = 207,
		YieldExpression = 208,
		SpreadElement = 209,
		ClassExpression = 210,
		OmittedExpression = 211,
		ExpressionWithTypeArguments = 212,
		AsExpression = 213,
		NonNullExpression = 214,
		MetaProperty = 215,
		SyntheticExpression = 216,
		TemplateSpan = 217,
		SemicolonClassElement = 218,
		Block = 219,
		VariableStatement = 220,
		EmptyStatement = 221,
		ExpressionStatement = 222,
		IfStatement = 223,
		DoStatement = 224,
		WhileStatement = 225,
		ForStatement = 226,
		ForInStatement = 227,
		ForOfStatement = 228,
		ContinueStatement = 229,
		BreakStatement = 230,
		ReturnStatement = 231,
		WithStatement = 232,
		SwitchStatement = 233,
		LabeledStatement = 234,
		ThrowStatement = 235,
		TryStatement = 236,
		DebuggerStatement = 237,
		VariableDeclaration = 238,
		VariableDeclarationList = 239,
		FunctionDeclaration = 240,
		ClassDeclaration = 241,
		InterfaceDeclaration = 242,
		TypeAliasDeclaration = 243,
		EnumDeclaration = 244,
		ModuleDeclaration = 245,
		ModuleBlock = 246,
		CaseBlock = 247,
		NamespaceExportDeclaration = 248,
		ImportEqualsDeclaration = 249,
		ImportDeclaration = 250,
		ImportClause = 251,
		NamespaceImport = 252,
		NamedImports = 253,
		ImportSpecifier = 254,
		ExportAssignment = 255,
		ExportDeclaration = 256,
		NamedExports = 257,
		ExportSpecifier = 258,
		MissingDeclaration = 259,
		ExternalModuleReference = 260,
		JsxElement = 261,
		JsxSelfClosingElement = 262,
		JsxOpeningElement = 263,
		JsxClosingElement = 264,
		JsxFragment = 265,
		JsxOpeningFragment = 266,
		JsxClosingFragment = 267,
		JsxAttribute = 268,
		JsxAttributes = 269,
		JsxSpreadAttribute = 270,
		JsxExpression = 271,
		CaseClause = 272,
		DefaultClause = 273,
		HeritageClause = 274,
		CatchClause = 275,
		PropertyAssignment = 276,
		ShorthandPropertyAssignment = 277,
		SpreadAssignment = 278,
		EnumMember = 279,
		UnparsedPrologue = 280,
		UnparsedPrepend = 281,
		UnparsedText = 282,
		UnparsedInternalText = 283,
		UnparsedSyntheticReference = 284,
		SourceFile = 285,
		Bundle = 286,
		UnparsedSource = 287,
		InputFiles = 288,
		JSDocTypeExpression = 289,
		JSDocAllType = 290,
		JSDocUnknownType = 291,
		JSDocNullableType = 292,
		JSDocNonNullableType = 293,
		JSDocOptionalType = 294,
		JSDocFunctionType = 295,
		JSDocVariadicType = 296,
		JSDocComment = 297,
		JSDocTypeLiteral = 298,
		JSDocSignature = 299,
		JSDocTag = 300,
		JSDocAugmentsTag = 301,
		JSDocClassTag = 302,
		JSDocCallbackTag = 303,
		JSDocEnumTag = 304,
		JSDocParameterTag = 305,
		JSDocReturnTag = 306,
		JSDocThisTag = 307,
		JSDocTypeTag = 308,
		JSDocTemplateTag = 309,
		JSDocTypedefTag = 310,
		JSDocPropertyTag = 311,
		SyntaxList = 312,
		NotEmittedStatement = 313,
		PartiallyEmittedExpression = 314,
		CommaListExpression = 315,
		MergeDeclarationMarker = 316,
		EndOfDeclarationMarker = 317,
		Count = 318,
		FirstAssignment = 60,
		LastAssignment = 72,
		FirstCompoundAssignment = 61,
		LastCompoundAssignment = 72,
		FirstReservedWord = 74,
		LastReservedWord = 109,
		FirstKeyword = 74,
		LastKeyword = 148,
		FirstFutureReservedWord = 110,
		LastFutureReservedWord = 118,
		FirstTypeNode = 164,
		LastTypeNode = 184,
		FirstPunctuation = 18,
		LastPunctuation = 72,
		FirstToken = 0,
		LastToken = 148,
		FirstTriviaToken = 2,
		LastTriviaToken = 7,
		FirstLiteralToken = 8,
		LastLiteralToken = 14,
		FirstTemplateToken = 14,
		LastTemplateToken = 17,
		FirstBinaryOperator = 28,
		LastBinaryOperator = 72,
		FirstNode = 149,
		FirstJSDocNode = 289,
		LastJSDocNode = 311,
		FirstJSDocTagNode = 300,
		LastJSDocTagNode = 311,
	};

	enum class SymbolFlags {
		None = 0,
		FunctionScopedVariable = 1,
		BlockScopedVariable = 2,
		Property = 4,
		EnumMember = 8,
		Function = 16,
		Class = 32,
		Interface = 64,
		ConstEnum = 128,
		RegularEnum = 256,
		ValueModule = 512,
		NamespaceModule = 1024,
		TypeLiteral = 2048,
		ObjectLiteral = 4096,
		Method = 8192,
		Constructor = 16384,
		GetAccessor = 32768,
		SetAccessor = 65536,
		Signature = 131072,
		TypeParameter = 262144,
		TypeAlias = 524288,
		ExportValue = 1048576,
		Alias = 2097152,
		Prototype = 4194304,
		ExportStar = 8388608,
		Optional = 16777216,
		Transient = 33554432,
		Assignment = 67108864,
		ModuleExports = 134217728,
		Enum = 384,
		Variable = 3,
		Value = 67220415,
		Type = 67897832,
		Namespace = 1920,
		Module = 1536,
		Accessor = 98304,
		FunctionScopedVariableExcludes = 67220414,
		BlockScopedVariableExcludes = 67220415,
		ParameterExcludes = 67220415,
		PropertyExcludes = 0,
		EnumMemberExcludes = 68008959,
		FunctionExcludes = 67219887,
		ClassExcludes = 68008383,
		InterfaceExcludes = 67897736,
		RegularEnumExcludes = 68008191,
		ConstEnumExcludes = 68008831,
		ValueModuleExcludes = 110735,
		NamespaceModuleExcludes = 0,
		MethodExcludes = 67212223,
		GetAccessorExcludes = 67154879,
		SetAccessorExcludes = 67187647,
		TypeParameterExcludes = 67635688,
		TypeAliasExcludes = 67897832,
		AliasExcludes = 2097152,
		ModuleMember = 2623475,
		ExportHasLocal = 944,
		BlockScoped = 418,
		PropertyOrAccessor = 98308,
		ClassMember = 106500,
	};

	enum class TypeFlags {
		Any = 1,
		Unknown = 2,
		String = 4,
		Number = 8,
		Boolean = 16,
		Enum = 32,
		BigInt = 64,
		StringLiteral = 128,
		NumberLiteral = 256,
		BooleanLiteral = 512,
		EnumLiteral = 1024,
		BigIntLiteral = 2048,
		ESSymbol = 4096,
		UniqueESSymbol = 8192,
		Void = 16384,
		Undefined = 32768,
		Null = 65536,
		Never = 131072,
		TypeParameter = 262144,
		Object = 524288,
		Union = 1048576,
		Intersection = 2097152,
		Index = 4194304,
		IndexedAccess = 8388608,
		Conditional = 16777216,
		Substitution = 33554432,
		NonPrimitive = 67108864,
		Literal = 2944,
		Unit = 109440,
		StringOrNumberLiteral = 384,
		PossiblyFalsy = 117724,
		StringLike = 132,
		NumberLike = 296,
		BigIntLike = 2112,
		BooleanLike = 528,
		EnumLike = 1056,
		ESSymbolLike = 12288,
		VoidLike = 49152,
		UnionOrIntersection = 3145728,
		StructuredType = 3670016,
		TypeVariable = 8650752,
		InstantiableNonPrimitive = 58982400,
		InstantiablePrimitive = 4194304,
		Instantiable = 63176704,
		StructuredOrInstantiable = 66846720,
		Narrowable = 133970943,
		NotUnionOrUnit = 67637251,
	};

	enum class ObjectFlags {
		Class = 1,
		Interface = 2,
		Reference = 4,
		Tuple = 8,
		Anonymous = 16,
		Mapped = 32,
		Instantiated = 64,
		ObjectLiteral = 128,
		EvolvingArray = 256,
		ObjectLiteralPatternWithComputedProperties = 512,
		ContainsSpread = 1024,
		ReverseMapped = 2048,
		JsxAttributes = 4096,
		MarkerType = 8192,
		JSLiteral = 16384,
		FreshLiteral = 32768,
		ArrayLiteral = 65536,
		ClassOrInterface = 3,
	};

	enum class ModuleKind {
		None = 0,
		CommonJS = 1,
		AMD = 2,
		UMD = 3,
		System = 4,
		ES2015 = 5,
		ESNext = 6
	};

	enum class ScriptTarget {
		ES3 = 0,
		ES5 = 1,
		ES2015 = 2,
		ES2016 = 3,
		ES2017 = 4,
		ES2018 = 5,
		ES2019 = 6,
		ES2020 = 7,
		ESNext = 8,
		JSON = 100,
		Latest = 8
	};

	enum class SignatureKind {
		Call = 0,
		Construct = 1
	};

	enum class ModifierFlags {
		None = 0,
		Export = 1,
		Ambient = 2,
		Public = 4,
		Private = 8,
		Protected = 16,
		Static = 32,
		Readonly = 64,
		Abstract = 128,
		Async = 256,
		Default = 512,
		Const = 2048,
		HasComputedFlags = 536870912,
		AccessibilityModifier = 28,
		ParameterPropertyModifier = 92,
		NonPublicAccessibilityModifier = 24,
		TypeScriptModifier = 2270,
		ExportDefault = 513,
		All = 3071
	};
};

// Manually implemented instead of DEFINE_CONTIGUOUS_ENUM + enumset to avoid manually checking that all enums are indeed contiguous.
inline _jsenums_ts::SymbolFlags operator|(_jsenums_ts::SymbolFlags a, _jsenums_ts::SymbolFlags b) { return static_cast<_jsenums_ts::SymbolFlags>(static_cast<int>(a) | static_cast<int>(b)); }
inline bool operator&(_jsenums_ts::SymbolFlags a, _jsenums_ts::SymbolFlags b) { return static_cast<int>(a) & static_cast<int>(b); }

inline _jsenums_ts::TypeFlags operator|(_jsenums_ts::TypeFlags a, _jsenums_ts::TypeFlags b) { return static_cast<_jsenums_ts::TypeFlags>(static_cast<int>(a) | static_cast<int>(b)); }
inline bool operator&(_jsenums_ts::TypeFlags a, _jsenums_ts::TypeFlags b) { return static_cast<int>(a) & static_cast<int>(b); }

inline _jsenums_ts::ObjectFlags operator|(_jsenums_ts::ObjectFlags a, _jsenums_ts::ObjectFlags b) { return static_cast<_jsenums_ts::ObjectFlags>(static_cast<int>(a) | static_cast<int>(b)); }
inline bool operator&(_jsenums_ts::ObjectFlags a, _jsenums_ts::ObjectFlags b) { return static_cast<int>(a) & static_cast<int>(b); }

inline _jsenums_ts::ModifierFlags operator|(_jsenums_ts::ModifierFlags a, _jsenums_ts::ModifierFlags b) { return static_cast<_jsenums_ts::ModifierFlags>(static_cast<int>(a) | static_cast<int>(b)); }
inline bool operator&(_jsenums_ts::ModifierFlags a, _jsenums_ts::ModifierFlags b) { return static_cast<int>(a) & static_cast<int>(b); }
} // namespace globals::enums_adl

// We have to specialize IsJsIntegralEnum before these types are used below.
template<> struct IsJsIntegralEnum<globals::enums_adl::_jsenums_ts::SyntaxKind> : std::true_type {};
template<> struct IsJsIntegralEnum<globals::enums_adl::_jsenums_ts::ModuleKind> : std::true_type {};
template<> struct IsJsIntegralEnum<globals::enums_adl::_jsenums_ts::ScriptTarget> : std::true_type {};
template<> struct IsJsIntegralEnum<globals::enums_adl::_jsenums_ts::SignatureKind> : std::true_type {};
template<> struct IsJsIntegralEnum<globals::enums_adl::_jsenums_ts::SymbolFlags> : std::true_type {};
template<> struct IsJsIntegralEnum<globals::enums_adl::_jsenums_ts::TypeFlags> : std::true_type {};
template<> struct IsJsIntegralEnum<globals::enums_adl::_jsenums_ts::ObjectFlags> : std::true_type {};
template<> struct IsJsIntegralEnum<globals::enums_adl::_jsenums_ts::ModifierFlags> : std::true_type {};

namespace globals {
namespace no_adl {
struct _jsdefs_ts : enums_adl::_jsenums_ts {
	struct _js_TextRange;
	struct _js_Node;
	struct _js_Declaration;
	struct _js_CallSignatureDeclaration;
	struct _js_MethodSignature;
	struct _js_MethodDeclaration;
	struct _js_ConstructorDeclaration;
	struct _js_Identifier;
	struct _js_NamedDeclaration;
	struct _js_Signature;
	struct _js_DeclarationStatement;
	struct _js_FunctionDeclaration;
	struct _js_ClassDeclaration;
	struct _js_EnumDeclaration;
	struct _js_VariableDeclaration;
	struct _js_VariableDeclarationList;
	struct _js_VariableStatement;
	struct _js_InterfaceDeclaration;
	struct _js_TypeAliasDeclaration;
	struct _js_ModuleDeclaration;
	struct _js_PropertyDeclaration;
	struct _js_EnumMember;
	struct _js_SourceFileLike;
	struct _js_SourceFile;
	struct _js_Program;
	struct _js_EmitResult;
	struct _js_TypeChecker;
	struct _js_Symbol;
	struct _js_Type;
	struct _js_TypeParameter;
	struct _js_UnionOrIntersectionType;
	struct _js_UnionType;
	struct _js_IntersectionType;
	struct _js_ObjectType;
	struct _js_InterfaceType;
	struct _js_TypeReference;
	struct _js_GenericType;
	struct _js_DiagnosticRelatedInformation;
	struct _js_Diagnostic;
	struct _js_SymbolTable;
	struct _js_CompilerOptions;
	struct _js_LineAndCharacter;
	struct _js_CompilerHost;
	struct _js_FormatDiagnosticsHost;

	using TextRange = js_ref<_js_TextRange>;
	using Node = js_ref<_js_Node>;
	using Declaration = js_ref<_js_Declaration>;
	using CallSignatureDeclaration = js_ref<_js_CallSignatureDeclaration>;
	using MethodSignature = js_ref<_js_MethodSignature>;
	using MethodDeclaration = js_ref<_js_MethodDeclaration>;
	using ConstructorDeclaration = js_ref<_js_ConstructorDeclaration>;
	using Identifier = js_ref<_js_Identifier>;
	using NamedDeclaration = js_ref<_js_NamedDeclaration>;
	using Signature = js_ref<_js_Signature>;
	using DeclarationStatement = js_ref<_js_DeclarationStatement>;
	using FunctionDeclaration = js_ref<_js_FunctionDeclaration>;
	using ClassDeclaration = js_ref<_js_ClassDeclaration>;
	using EnumDeclaration = js_ref<_js_EnumDeclaration>;
	using VariableDeclaration = js_ref<_js_VariableDeclaration>;
	using VariableDeclarationList = js_ref<_js_VariableDeclarationList>;
	using VariableStatement = js_ref<_js_VariableStatement>;
	using InterfaceDeclaration = js_ref<_js_InterfaceDeclaration>;
	using TypeAliasDeclaration = js_ref<_js_TypeAliasDeclaration>;
	using ModuleDeclaration = js_ref<_js_ModuleDeclaration>;
	using PropertyDeclaration = js_ref<_js_PropertyDeclaration>;
	using EnumMember = js_ref<_js_EnumMember>;
	using SourceFileLike = js_ref<_js_SourceFileLike>;
	using SourceFile = js_ref<_js_SourceFile>;
	using Program = js_ref<_js_Program>;
	using EmitResult = js_ref<_js_EmitResult>;
	using TypeChecker = js_ref<_js_TypeChecker>;
	using Symbol = js_ref<_js_Symbol>;
	using Type = js_ref<_js_Type>;
	using TypeParameter = js_ref<_js_TypeParameter>;
	using UnionOrIntersectionType = js_ref<_js_UnionOrIntersectionType>;
	using UnionType = js_ref<_js_UnionType>;
	using IntersectionType = js_ref<_js_IntersectionType>;
	using ObjectType = js_ref<_js_ObjectType>;
	using InterfaceType = js_ref<_js_InterfaceType>;
	using TypeReference = js_ref<_js_TypeReference>;
	using GenericType = js_ref<_js_GenericType>;
	using DiagnosticRelatedInformation = js_ref<_js_DiagnosticRelatedInformation>;
	using Diagnostic = js_ref<_js_Diagnostic>;
	using SymbolTable = js_ref<_js_SymbolTable>;
	using CompilerOptions = js_ref<_js_CompilerOptions>;
	using LineAndCharacter = js_ref<_js_LineAndCharacter>;
	using CompilerHost = js_ref<_js_CompilerHost>;
	using FormatDiagnosticsHost = js_ref<_js_FormatDiagnosticsHost>;

	using SignatureDeclaration = js_union<MethodSignature, MethodDeclaration, ConstructorDeclaration, CallSignatureDeclaration, FunctionDeclaration>;
	using BaseType = js_union<ObjectType, IntersectionType>;

	struct _js_TextRange : virtual IObject {
	};

	struct _js_Node : virtual _js_TextRange {
		auto kind() noexcept { return _getProperty<SyntaxKind>("kind"); }
		void kind(SyntaxKind v) noexcept { _setProperty<SyntaxKind>("kind", v); }

		auto getChildren() noexcept { return _call<Array<SourceFile>>("getChildren"); }
	};

	struct _js_Declaration : virtual _js_Node {
	};

	struct _js_Identifier : virtual _js_Declaration {
		js_string text() noexcept { return _getProperty<js_string>("text"); }
	};

	struct _js_NamedDeclaration : virtual _js_Declaration {
	};

	struct _js_CallSignatureDeclaration : virtual _js_NamedDeclaration {
	};

	struct _js_MethodSignature : virtual _js_NamedDeclaration {
	};

	struct _js_MethodDeclaration : virtual _js_NamedDeclaration {
	};

	struct _js_ConstructorDeclaration : virtual _js_NamedDeclaration {
	};

	struct _js_Signature : virtual IObject {
		auto getTypeParameters() noexcept { return _call<js_optional<ReadonlyArray<js_unknown>>>("getTypeParameters"); }
		auto getParameters() noexcept { return _call<ReadonlyArray<Symbol>>("getParameters"); }
		auto getReturnType() noexcept { return _call<Type>("getReturnType"); }
	};

	struct _js_DeclarationStatement : virtual _js_NamedDeclaration {
		auto name() noexcept { return _getProperty<js_optional<Declaration>>("name"); }
	};

	struct _js_FunctionDeclaration : virtual _js_DeclarationStatement {
	};

	struct _js_ClassDeclaration : virtual _js_DeclarationStatement {
	};

	struct _js_EnumDeclaration : virtual _js_DeclarationStatement {
	};

	struct _js_VariableDeclaration : virtual _js_NamedDeclaration {
		auto name() noexcept { return _getProperty<Node>("name"); }
	};

	struct _js_VariableDeclarationList : virtual _js_Node {
		auto declarations() noexcept { return _getProperty<ReadonlyArray<VariableDeclaration>>("declarations"); }
	};

	struct _js_VariableStatement : virtual _js_Node {
		auto declarationList() noexcept { return _getProperty<VariableDeclarationList>("declarationList"); }
	};

	struct _js_InterfaceDeclaration : virtual _js_DeclarationStatement {
	};

	struct _js_TypeAliasDeclaration : virtual _js_DeclarationStatement {
	};

	struct _js_ModuleDeclaration : virtual _js_DeclarationStatement {
		auto name() noexcept { return _getProperty<Identifier>("name"); }
	};

	struct _js_PropertyDeclaration : virtual _js_NamedDeclaration {
		auto name() noexcept { return _getProperty<Identifier /* | StringLiteral | NumericLiteral | ComputedPropertyName */>("name"); }
	};

	struct _js_EnumMember : virtual _js_Declaration {
	};

	struct _js_SourceFileLike : virtual IObject {
		auto getLineAndCharacterOfPosition(int pos) noexcept {
			return _call<LineAndCharacter>("getLineAndCharacterOfPosition", tc::explicit_cast<double>(pos));
		}
	};

	struct _js_SourceFile : virtual _js_Declaration, /* deduced */ virtual _js_SourceFileLike {
		auto fileName() noexcept { return _getProperty<js_string>("fileName"); }
		void fileName(js_string v) noexcept { _setProperty("fileName", v); }
	};

	struct _js_Program : virtual IObject {
		auto getSourceFiles() noexcept { return _call<ReadonlyArray<SourceFile>>("getSourceFiles"); }

		auto emit() noexcept { return _call<EmitResult>("emit"); }

		auto getTypeChecker() noexcept { return _call<TypeChecker>("getTypeChecker"); }
	};

	struct _js_EmitResult : virtual IObject {
		auto emitSkipped() noexcept { return _getProperty<bool>("emitSkipped"); }
		void emitSkipped(bool v) noexcept { _setProperty("emitSkipped", v); }

		auto diagnostics() noexcept { return _getProperty<ReadonlyArray<Diagnostic>>("diagnostics"); }
		void diagnostics(ReadonlyArray<Diagnostic> v) noexcept { _setProperty("diagnostics", v); }
	};

	struct _js_TypeChecker : virtual IObject {
		auto getDeclaredTypeOfSymbol(Symbol symbol) noexcept { return _call<Type>("getDeclaredTypeOfSymbol", symbol); }
		auto getTypeOfSymbolAtLocation(Symbol symbol, Node node) noexcept { return _call<Type>("getTypeOfSymbolAtLocation", symbol, node); }
		auto getPropertiesOfType(Type type) noexcept { return _call<Array<Symbol>>("getPropertiesOfType", type); }
		auto getRootSymbols(Symbol type) noexcept { return _call<ReadonlyArray<Symbol>>("getRootSymbols", type); }
		auto getAugmentedPropertiesOfType(Type type) noexcept { return _call<Array<Symbol>>("getAugmentedPropertiesOfType", type); }
		auto getSymbolsInScope(Node location, SymbolFlags meaning) noexcept { return _call<Array<Symbol>>("getSymbolsInScope", location, meaning); }
		auto getSymbolAtLocation(Node node) noexcept { return _call<js_optional<Symbol>>("getSymbolAtLocation", node); }
		auto getExportsOfModule(Symbol moduleSymbol) noexcept { return _call<Array<Symbol>>("getExportsOfModule", moduleSymbol); }
		auto getExportSymbolOfSymbol(Symbol symbol) noexcept { return _call<Symbol>("getExportSymbolOfSymbol", symbol); }
		auto getSignaturesOfType(Type type, SignatureKind kind) noexcept { return _call<ReadonlyArray<Signature>>("getSignaturesOfType", type, kind); }
		auto getSignatureFromDeclaration(SignatureDeclaration declaration) noexcept { return _call<js_optional<Signature>>("getSignatureFromDeclaration", declaration); }
		auto signatureToString(Signature signature) noexcept { return _call<js_string>("signatureToString", signature); }
		auto getFullyQualifiedName(Symbol symbol) noexcept { return _call<js_string>("getFullyQualifiedName", symbol); }
		auto getConstantValue(EnumMember node) noexcept { return _call<js_union<js_string, double, js_undefined>>("getConstantValue", node); }
		auto typeToString(Type type) noexcept { return _call<js_string>("typeToString", type); }
		auto getBaseTypes(InterfaceType type) noexcept { return _call<Array<BaseType>>("getBaseTypes", type); }
	};

	struct _js_Symbol : virtual IObject {
		auto getName() noexcept { return _call<js_string>("getName"); }

		auto getFlags() noexcept { return _call<SymbolFlags>("getFlags"); }

		auto members() noexcept { return _getProperty<js_optional<SymbolTable>>("members"); }

		auto exports() noexcept { return _getProperty<js_optional<SymbolTable>>("exports"); }

		auto globalExports() noexcept { return _getProperty<js_optional<SymbolTable>>("globalExports"); }

		auto valueDeclaration() noexcept { return _getProperty<js_optional<Declaration>>("valueDeclaration"); }

		auto declarations() noexcept { return _getProperty<Array<Declaration>>("declarations"); }

		auto parent() noexcept { return _getProperty<js_optional<Symbol>>("parent"); } // @internal
	};


	struct _js_Type : virtual IObject {
		auto flags() noexcept { return _getProperty<TypeFlags>("flags"); }

		auto getSymbol() noexcept { return _call<js_union<js_undefined, Symbol>>("getSymbol"); }

		auto getProperties() noexcept { return _call<Array<Symbol>>("getProperties"); }

		auto constraint() noexcept { return _getProperty<js_union<js_undefined, Type>>("constraint"); }

		auto isUnion() noexcept {
			std::optional<UnionType> result;
			if (_call<bool>("isUnion"))
				result.emplace(_this<UnionType>());
			return result;
		}

		auto isClassOrInterface() noexcept {
			std::optional<InterfaceType> result;
			if (_call<bool>("isClassOrInterface"))
				result.emplace(_this<InterfaceType>());
			return result;
		}
	};

	struct _js_TypeParameter : virtual _js_Type {
	};

	struct _js_UnionOrIntersectionType : virtual _js_Type {
		auto types() noexcept { return _getProperty<Array<Type>>("types"); }
	};

	struct _js_UnionType : virtual _js_UnionOrIntersectionType {
		auto types() noexcept { return _getProperty<Array<Type>>("types"); }
	};

	struct _js_IntersectionType : virtual _js_UnionOrIntersectionType {
	};

	struct _js_ObjectType : virtual _js_Type {
		auto objectFlags() noexcept { return _getProperty<ObjectFlags>("objectFlags"); }
	};

	struct _js_InterfaceType : virtual _js_ObjectType {
		auto typeParameters() noexcept { return _getProperty<js_union<js_undefined, Array<TypeParameter>>>("typeParameters"); }
		auto outerTypeParameters() noexcept { return _getProperty<js_union<js_undefined, Array<TypeParameter>>>("outerTypeParameters"); }
		auto localTypeParameters() noexcept { return _getProperty<js_union<js_undefined, Array<TypeParameter>>>("localTypeParameters"); }
		auto thisType() noexcept { return _getProperty<js_union<js_undefined, TypeParameter>>("thisType"); }
	};

	struct _js_TypeReference : virtual _js_ObjectType {
		auto target() noexcept { return _getProperty<GenericType>("target"); }
		auto typeArguments() noexcept { return _getProperty<ReadonlyArray<Type>>("typeArguments"); }
	};

	struct 	_js_GenericType : virtual _js_InterfaceType, virtual _js_TypeReference {
	};

	struct _js_DiagnosticRelatedInformation : virtual IObject {
		auto file() noexcept { return _getProperty<js_optional<SourceFile>>("file"); }
		void file(js_optional<SourceFile> v) noexcept { _setProperty("file", v); }

		auto start() noexcept { return _getProperty<js_optional<double>>("start"); }
		void start(js_optional<double> v) noexcept { _setProperty("start", v); }

		/* string | DiagnosticMessageChain; */
		auto messageText() noexcept { return _getProperty<js_unknown>("messageText"); }
		void messageText(js_unknown v) noexcept { _setProperty("messageText", v); }
	};

	struct _js_SymbolTable : virtual IObject {
		template<typename Fn>
		auto operator()(Fn fn) noexcept {
			return _call<void>("forEach", js_lambda_wrap([&](Symbol value, js_unknown, js_unknown) noexcept {
				fn(tc_move(value));
			}));
		}
	};

	struct _js_Diagnostic : virtual _js_DiagnosticRelatedInformation {
	};

	struct _js_CompilerOptions : virtual IObject {
		auto noEmitOnError() noexcept { return _getProperty<js_optional<bool>>("noEmitOnError"); }
		void noEmitOnError(js_optional<bool> v) noexcept { _setProperty("noEmitOnError", v); }

		auto module() noexcept { return _getProperty<js_optional<bool>>("module"); }
		void module(js_optional<ModuleKind> v) noexcept { _setProperty("module", v); }

		auto strict() noexcept { return _getProperty<js_optional<bool>>("strict"); }
		void strict(js_optional<bool> v) noexcept { _setProperty("strict", v); }

		auto target() noexcept { return _getProperty<js_optional<ScriptTarget>>("target"); }
		void target(js_optional<ScriptTarget> v) noexcept { _setProperty("target", v); }

		static auto _tcjs_construct() noexcept { return emscripten::val::object(); }
	};

	struct _js_LineAndCharacter : virtual IObject {
		auto line() noexcept { return tc::explicit_cast<int>(_getProperty<double>("line")); }
		void line(int v) noexcept { _setProperty("line", tc::explicit_cast<double>(v)); }

		auto character() noexcept { return tc::explicit_cast<int>(_getProperty<double>("character")); }
		void character(int v) noexcept { _setProperty("character", tc::explicit_cast<double>(v)); }
	};

	struct _js_FormatDiagnosticsHost : virtual IObject {
	};

	struct _js_CompilerHost : virtual /*derived*/ _js_FormatDiagnosticsHost {
	};
};

struct _js_ts : virtual IObject, _jsdefs_ts {
	auto isVariableStatement(Node node) noexcept {
		std::optional<VariableStatement> result;
		if (_call<bool>("isVariableStatement", node))
			result.emplace(node);
		return result;
	}

	auto isInterfaceDeclaration(Node node) noexcept {
		std::optional<InterfaceDeclaration> result;
		if (_call<bool>("isInterfaceDeclaration", node))
			result.emplace(node);
		return result;
	}

	auto isFunctionDeclaration(Node node) noexcept {
		std::optional<FunctionDeclaration> result;
		if (_call<bool>("isFunctionDeclaration", node))
			result.emplace(node);
		return result;
	}

	auto isClassDeclaration(Node node) noexcept {
		std::optional<ClassDeclaration> result;
		if (_call<bool>("isClassDeclaration", node))
			result.emplace(node);
		return result;
	}

	auto isEnumDeclaration(Node node) noexcept {
		std::optional<EnumDeclaration> result;
		if (_call<bool>("isEnumDeclaration", node))
			result.emplace(node);
		return result;
	}

	auto isTypeAliasDeclaration(Node node) noexcept {
		std::optional<TypeAliasDeclaration> result;
		if (_call<bool>("isTypeAliasDeclaration", node))
			result.emplace(node);
		return result;
	}

	auto isModuleDeclaration(Node node) noexcept {
		std::optional<ModuleDeclaration> result;
		if (_call<bool>("isModuleDeclaration", node))
			result.emplace(node);
		return result;
	}

	auto isPropertyDeclaration(Node node) noexcept {
		std::optional<PropertyDeclaration> result;
		if (_call<bool>("isPropertyDeclaration", node))
			result.emplace(node);
		return result;
	}

	auto isMethodSignature(Node node) noexcept {
		std::optional<MethodSignature> result;
		if (_call<bool>("isMethodSignature", node))
			result.emplace(node);
		return result;
	}

	auto isMethodDeclaration(Node node) noexcept {
		std::optional<MethodDeclaration> result;
		if (_call<bool>("isMethodDeclaration", node))
			result.emplace(node);
		return result;
	}

	auto isConstructorDeclaration(Node node) noexcept {
		std::optional<ConstructorDeclaration> result;
		if (_call<bool>("isConstructorDeclaration", node))
			result.emplace(node);
		return result;
	}

	auto isEnumMember(Node node) noexcept {
		std::optional<EnumMember> result;
		if (_call<bool>("isEnumMember", node))
			result.emplace(node);
		return result;
	}

	auto getLineAndCharacterOfPosition(SourceFileLike sourceFile, int position) noexcept {
		return _call<LineAndCharacter>("getLineAndCharacterOfPosition", sourceFile, tc::explicit_cast<double>(position));
	}

	auto createProgram(ReadonlyArray<js_string> rootNames, CompilerOptions compilerOptions) noexcept {
		return _call<Program>("createProgram", rootNames, compilerOptions);
	}

	auto getPreEmitDiagnostics(Program program) noexcept {
		return _call<ReadonlyArray<Diagnostic>>("getPreEmitDiagnostics", program);
	}

	auto createCompilerHost(CompilerOptions options) noexcept {
		return _call<CompilerHost>("createCompilerHost", options);
	}

	auto formatDiagnosticsWithColorAndContext(ReadonlyArray<Diagnostic> diagnostics, FormatDiagnosticsHost host) noexcept {
		return _call<js_string>("formatDiagnosticsWithColorAndContext", diagnostics, host);
	}

	auto flattenDiagnosticMessageText(js_unknown messageText, js_string newLine) noexcept {
		return _call<js_string>("flattenDiagnosticMessageText", messageText, newLine);
	}

	auto getCombinedModifierFlags(js_ref<_js_Declaration> node) noexcept {
		return _call<ModifierFlags>("getCombinedModifierFlags", node);
	}

	auto forEachChild(Node node, js_function<void(Node)> func) noexcept {
		return _call<void>("forEachChild", node, func);
	}

	static emscripten::val _tcjs_construct() noexcept { return emscripten::val::global("ts"); }
};

struct ts : js_ref<_js_ts>, _jsdefs_ts {
};
} // namespace no_adl
using no_adl::ts;
} // namespace globals
} // namespace tc::js
