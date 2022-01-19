#include "../../precompiled.h"
#include "MyLib.d.h"

int main() {
    using namespace tc::js;
    static_assert(std::is_enum_v<MyLib::SyntaxKind>);
    static_assert(std::is_same_v<decltype(std::declval<MyLib::Token<MyLib::SyntaxKind::Unknown>>()->kind()), MyLib::SyntaxKind>);

    // TODO: Literal enum union not yet supported, takes any enum value
    static_assert(std::is_same_v<decltype(std::declval<MyLib::TestEnum>()->func(MyLib::SyntaxKind::SingleLineCommentTrivia)), void>);
    static_assert(std::is_same_v<decltype(std::declval<MyLib::TestEnum2<MyLib::SyntaxKind::Unknown>>()->kind()), MyLib::SyntaxKind>);

    static_assert(std::is_same_v<
        decltype(
            std::declval<MyLib::NodeListOf<MyLib::Node>>()->forEach(
                std::declval<tc::jst::function<void(MyLib::Node, double, MyLib::NodeListOf<MyLib::Node>)>>(), 
                tc::js::undefined{}
            )
        ), 
        void
    >);

    static_assert(std::is_same_v<
        decltype(
            std::declval<MyLib::NodeListOf<MyLib::DerivedNode>>()->forEach(
                std::declval<tc::jst::function<void(MyLib::Node, double, MyLib::NodeListOf<MyLib::DerivedNode>)>>(), 
                tc::js::undefined{}
            )
        ), 
        void
    >);

    // TODO: MyLib::NodeListOf operator [] not supported

    static_assert(std::is_same_v<decltype(std::declval<MyLib::A>()->p()), Record<string, double>>);

    static_assert(std::is_same_v<
        decltype(
            std::declval<MyLib::NodeListOf<MyLib::DerivedNode>>()->forEach(
                std::declval<tc::jst::function<void(MyLib::Node, double, MyLib::NodeListOf<MyLib::DerivedNode>)>>(), 
                tc::js::undefined{}
            )
        ), 
        void
    >);

    static_assert(std::is_same_v<
        decltype(
            std::declval<MyLib::Collection<MyLib::Node>>()->item_generic(std::declval<double>())
        ),
        tc::jst::union_t<MyLib::Node, null>
    >);

    static_assert(std::is_same_v<
        MyLib::TestAlias<MyLib::Node>,
        tc::jst::union_t<MyLib::Node, double, string>
    >);
}
