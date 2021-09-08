#include "../../precompiled.h"
#include "MyLib.d.h"

int main() {
	 static_assert(std::is_same<decltype(std::declval<tc::js::Test const&>()->isBar(std::declval<tc::js::Foo const&>())), std::optional<tc::js::Bar>>::value);
	 static_assert(std::is_same<decltype(std::declval<tc::js::Test const&>()->isTypeAlias2()), std::optional<tc::js::FooBarNumber>>::value);
	 static_assert(std::is_same<decltype(std::declval<tc::js::Test const&>()->isTypeAlias(0.0, std::declval<tc::js::any>())), std::optional<tc::js::FooBarNumber>>::value);

	 static_assert(std::is_same<decltype(std::declval<tc::js::Test const&>()->isFunction(std::declval<tc::js::any>())), std::optional<tc::jst::function<double (tc::js::string)> > >::value);

	 static_assert(std::is_same<decltype(tc::js::g(std::declval<tc::js::any>())), std::optional<tc::js::FooBarNumber>>::value);
}
