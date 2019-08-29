#pragma once
// TODO: add notices according to Apache License at https://github.com/microsoft/TypeScript/blob/master/LICENSE.txt as this may be a derivative work.

#include <emscripten/val.h>
#include <optional>
#include "explicit_cast.h"
#include "range_defines.h"
#include "js_types.h"
#include "js_callback.h"
#include "js_bootstrap.h"

namespace tc::js {
namespace globals::no_adl {
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
};
} // namespace globals::no_adl

// We have to specialize IsJsIntegralEnum before these types are used below.
template<> struct IsJsIntegralEnum<globals::no_adl::_jsenums_ts::SyntaxKind> : std::true_type {};
template<> struct IsJsIntegralEnum<globals::no_adl::_jsenums_ts::ModuleKind> : std::true_type {};
template<> struct IsJsIntegralEnum<globals::no_adl::_jsenums_ts::ScriptTarget> : std::true_type {};
template<> struct IsJsIntegralEnum<globals::no_adl::_jsenums_ts::SignatureKind> : std::true_type {};

namespace globals {
namespace no_adl {
struct _jsdefs_ts : _jsenums_ts {
    struct _js_TextRange;
    struct _js_Node;
    struct _js_Declaration;
    struct _js_Identifier;
    struct _js_NamedDeclaration;
    struct _js_Signature;
    struct _js_DeclarationStatement;
    struct _js_FunctionDeclaration;
    struct _js_VariableStatement;
    struct _js_InterfaceDeclaration;
    struct _js_TypeAliasDeclaration;
    struct _js_ModuleDeclaration;
    struct _js_SourceFileLike;
    struct _js_SourceFile;
    struct _js_Program;
    struct _js_EmitResult;
    struct _js_TypeChecker;
    struct _js_Symbol;
    struct _js_Type;
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
    using Identifier = js_ref<_js_Identifier>;
    using NamedDeclaration = js_ref<_js_NamedDeclaration>;
    using Signature = js_ref<_js_Signature>;
    using DeclarationStatement = js_ref<_js_DeclarationStatement>;
    using FunctionDeclaration = js_ref<_js_FunctionDeclaration>;
    using VariableStatement = js_ref<_js_VariableStatement>;
    using InterfaceDeclaration = js_ref<_js_InterfaceDeclaration>;
    using TypeAliasDeclaration = js_ref<_js_TypeAliasDeclaration>;
    using ModuleDeclaration = js_ref<_js_ModuleDeclaration>;
    using SourceFileLike = js_ref<_js_SourceFileLike>;
    using SourceFile = js_ref<_js_SourceFile>;
    using Program = js_ref<_js_Program>;
    using EmitResult = js_ref<_js_EmitResult>;
    using TypeChecker = js_ref<_js_TypeChecker>;
    using Symbol  = js_ref<_js_Symbol>;
    using Type = js_ref<_js_Type>;
    using DiagnosticRelatedInformation = js_ref<_js_DiagnosticRelatedInformation>;
    using Diagnostic = js_ref<_js_Diagnostic>;
    using SymbolTable = js_ref<_js_SymbolTable>;
    using CompilerOptions = js_ref<_js_CompilerOptions>;
    using LineAndCharacter = js_ref<_js_LineAndCharacter>;
    using CompilerHost = js_ref<_js_CompilerHost>;
    using FormatDiagnosticsHost = js_ref<_js_FormatDiagnosticsHost>;

    struct _js_TextRange : virtual IObject {
    };

    struct _js_Node : virtual _js_TextRange {
        auto kind() { return _getProperty<SyntaxKind>("kind"); }
        void kind(SyntaxKind v) { _setProperty<SyntaxKind>("kind", v); }

        auto getChildren() { return _call<Array<SourceFile>>("getChildren"); }
    };

    struct _js_Declaration : virtual _js_Node {
    };

    struct _js_Identifier : virtual _js_Declaration {
        js_string text() { return _getProperty<js_string>("text"); }
    };

    struct _js_NamedDeclaration : virtual _js_Declaration {
    };

    struct _js_Signature : virtual IObject {
    };

    struct _js_DeclarationStatement : virtual _js_NamedDeclaration {
    };

    struct _js_FunctionDeclaration : virtual _js_DeclarationStatement {
    };

    struct _js_VariableStatement : virtual _js_Node {
    };

    struct _js_InterfaceDeclaration : virtual _js_DeclarationStatement {
    };

    struct _js_TypeAliasDeclaration : virtual _js_DeclarationStatement {
    };

    struct _js_ModuleDeclaration : virtual _js_DeclarationStatement {
        auto name() {
            auto result = _getProperty<Identifier>("name");
            _ASSERT(!result.getEmval()["text"].isUndefined());
            return result;
        }
    };

    struct _js_SourceFileLike : virtual IObject {
        auto getLineAndCharacterOfPosition(int pos) {
            return _call<LineAndCharacter>("getLineAndCharacterOfPosition", tc::explicit_cast<double>(pos));
        }
    };

    struct _js_SourceFile : virtual _js_Declaration, /* deduced */ virtual _js_SourceFileLike {
        auto fileName() { return _getProperty<js_string>("fileName"); }
        void fileName(js_string v) { _setProperty("fileName", v); }
    };

    struct _js_Program : virtual IObject {
        auto getSourceFiles() { return _call<ReadonlyArray<SourceFile>>("getSourceFiles"); }

        auto emit() { return _call<EmitResult>("emit"); }

        auto getTypeChecker() { return _call<TypeChecker>("getTypeChecker"); }
    };

    struct _js_EmitResult : virtual IObject {
        auto emitSkipped() { return _getProperty<bool>("emitSkipped"); }
        void emitSkipped(bool v) { _setProperty("emitSkipped", v); }

        auto diagnostics() { return _getProperty<ReadonlyArray<Diagnostic>>("diagnostics"); }
        void diagnostics(ReadonlyArray<Diagnostic> v) { _setProperty("diagnostics", v); }
    };

    struct _js_TypeChecker : virtual IObject {
        auto getDeclaredTypeOfSymbol(Symbol symbol) { return _call<Type>("getDeclaredTypeOfSymbol", symbol); }
        auto getPropertiesOfType(Type type) { return _call<Array<Symbol>>("getPropertiesOfType", type); }
        auto getRootSymbols(Symbol type) { return _call<ReadonlyArray<Symbol>>("getRootSymbols", type); }
        auto getAugmentedPropertiesOfType(Type type) { return _call<Array<Symbol>>("getAugmentedPropertiesOfType", type); }
        auto getSymbolsInScope(Node location, int /*SymbolFlags*/ meaning) { return _call<Array<Symbol>>("getSymbolsInScope", location, tc::explicit_cast<double>(meaning)); }
        auto getSymbolAtLocation(Node node) { return _call<js_optional<Symbol>>("getSymbolAtLocation", node); }
        auto getExportsOfModule(Symbol moduleSymbol) { return _call<Array<Symbol>>("getExportsOfModule", moduleSymbol); }
        auto getExportSymbolOfSymbol(Symbol symbol) { return _call<Symbol>("getExportSymbolOfSymbol", symbol); }
        auto getSignaturesOfType(Type type, SignatureKind kind) { return _call<ReadonlyArray<Signature>>("getSignaturesOfType", type, kind); }
        auto signatureToString(Signature signature) { return _call<js_string>("signatureToString", signature); }
        auto getFullyQualifiedName(Symbol symbol) { return _call<js_string>("getFullyQualifiedName", symbol); }
    };

    struct _js_Symbol : virtual IObject {
        auto getName() { return _call<js_string>("getName"); }

        auto getFlags() { return tc::explicit_cast<int>(_call<double /*SymbolFlags*/>("getFlags")); }

        auto members() { return _getProperty<js_optional<SymbolTable>>("members"); }

        auto exports() { return _getProperty<js_optional<SymbolTable>>("exports"); }

        auto globalExports() { return _getProperty<js_optional<SymbolTable>>("globalExports"); }

        auto valueDeclaration() { return _getProperty<js_optional<Declaration>>("valueDeclaration"); }
    };


    struct _js_Type : virtual IObject {
        auto flags() { return tc::explicit_cast<int>(_getProperty<double /*TypeFlags*/>("flags")); }

        auto symbol() { return _getProperty<Symbol>("symbol"); }

        auto getProperties() { return _call<Array<Symbol>>("getProperties"); }
    };

    struct _js_DiagnosticRelatedInformation : virtual IObject {
        auto file() { return _getProperty<js_optional<SourceFile>>("file"); }
        void file(js_optional<SourceFile> v) { _setProperty("file", v); }

        auto start() { return _getProperty<js_optional<double>>("start"); }
        void start(js_optional<double> v) { _setProperty("start", v); }

        /* string | DiagnosticMessageChain; */
        auto messageText() { return _getProperty<js_unknown>("messageText"); }
        void messageText(js_unknown v) { _setProperty("messageText", v); }
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
        auto noEmitOnError() { return _getProperty<js_optional<bool>>("noEmitOnError"); }
        void noEmitOnError(js_optional<bool> v) { _setProperty("noEmitOnError", v); }

        auto module() { return _getProperty<js_optional<bool>>("module"); }
        void module(js_optional<ModuleKind> v) { _setProperty("module", v); }

        auto strict() { return _getProperty<js_optional<bool>>("strict"); }
        void strict(js_optional<bool> v) { _setProperty("strict", v); }

        auto target() { return _getProperty<js_optional<ScriptTarget>>("target"); }
        void target(js_optional<ScriptTarget> v) { _setProperty("target", v); }

        static auto _construct() noexcept { return emscripten::val::object(); }
    };

    struct _js_LineAndCharacter : virtual IObject {
        auto line() { return tc::explicit_cast<int>(_getProperty<double>("line")); }
        void line(int v) { _setProperty("line", tc::explicit_cast<double>(v)); }

        auto character() { return tc::explicit_cast<int>(_getProperty<double>("character")); }
        void character(int v) { _setProperty("character", tc::explicit_cast<double>(v)); }
    };

    struct _js_FormatDiagnosticsHost : virtual IObject {
    };

    struct _js_CompilerHost : virtual /*derived*/ _js_FormatDiagnosticsHost {
    };
};

struct _js_ts : virtual IObject, _jsdefs_ts {
    auto isVariableStatement(Node node) {
        std::optional<VariableStatement> result;
        if (_call<bool>("isVariableStatement", node))
            result.emplace(node);
        return result;
    }

    auto isInterfaceDeclaration(Node node) {
        std::optional<InterfaceDeclaration> result;
        if (_call<bool>("isInterfaceDeclaration", node))
            result.emplace(node);
        return result;
    }

    auto isFunctionDeclaration(Node node) {
        std::optional<FunctionDeclaration> result;
        if (_call<bool>("isFunctionDeclaration", node))
            result.emplace(node);
        return result;
    }

    auto isTypeAliasDeclaration(Node node) {
        std::optional<TypeAliasDeclaration> result;
        if (_call<bool>("isTypeAliasDeclaration", node))
            result.emplace(node);
        return result;
    }

    auto isModuleDeclaration(Node node) {
        std::optional<ModuleDeclaration> result;
        if (_call<bool>("isModuleDeclaration", node))
            result.emplace(node);
        return result;
    }

    auto getLineAndCharacterOfPosition(SourceFileLike sourceFile, int position) {
        return _call<LineAndCharacter>("getLineAndCharacterOfPosition", sourceFile, tc::explicit_cast<double>(position));
    }

    auto createProgram(ReadonlyArray<js_string> rootNames, CompilerOptions compilerOptions) {
        return _call<Program>("createProgram", rootNames, compilerOptions);
    }

    auto getPreEmitDiagnostics(Program program) {
        return _call<ReadonlyArray<Diagnostic>>("getPreEmitDiagnostics", program);
    }

    auto createCompilerHost(CompilerOptions options) {
        return _call<CompilerHost>("createCompilerHost", options);
    }

    auto formatDiagnosticsWithColorAndContext(ReadonlyArray<Diagnostic> diagnostics, FormatDiagnosticsHost host) {
        return _call<js_string>("formatDiagnosticsWithColorAndContext", diagnostics, host);
    }

    auto flattenDiagnosticMessageText(js_unknown messageText, js_string newLine) {
        return _call<js_string>("flattenDiagnosticMessageText", messageText, newLine);
    }

   static emscripten::val _construct() { return emscripten::val::global("ts"); }
};

struct ts : js_ref<_js_ts>, _jsdefs_ts {
};
} // namespace no_adl
using no_adl::ts;
} // namespace globals
} // namespace tc::js
