#pragma once

std::tuple<SMangledType, SJsEnum const*> LookupEnumFromLiteral(ts::Type jtype) noexcept;
SMangledType CommentType(std::string strCppType, tc::js::ts::Type const jtypeRoot) noexcept;

template<typename Rng, typename RngType>
SMangledType MangleTypeArguments(
	std::string const& strWithComments, std::string const& strCppCanonized, Rng const& atypeArguments, RngType const& rngetype
) noexcept {
	auto MangleEnumLiteral = [](ts::Type jtypeLit) noexcept -> SMangledType {
		auto jsym = SymbolOrAliasSymbol(jtypeLit);
		auto tupmtojsenum = LookupEnumFromLiteral(jtypeLit);

		if(std::get<1>(tupmtojsenum)) {
			auto const& jsenum = *std::get<1>(tupmtojsenum);
			auto const strName = tc::explicit_cast<std::string>(jsym->getName());
			if(auto ojsenumoption =
				   tc::find_first<tc::return_element_or_null>(
					   tc::transform(jsenum.m_vecjsenumoption, TC_MEMBER(.m_strJsName)), strName
				   )
					   .element_base())
			{
				return {tc::make_str(jsenum.m_strMangledName, "::", ojsenumoption->m_strCppifiedName)};
			}
		}
		return std::get<0>(tupmtojsenum);
	};
	if(tc::size(atypeArguments) != tc::size(rngetype)) {
		tc::append(std::cerr, "Type ", strCppCanonized," has ", tc::as_dec(tc::size(atypeArguments)), " type arguments and ",
			tc::as_dec(tc::size(rngetype)), " type parameters.\n"
		);
		tc::append(std::cerr, "Type Arguments: ", tc::join_separated(
			tc::transform(atypeArguments, [](auto jtype) noexcept {
				auto ojsym = OptSymbolOrAliasSymbol(jtype);
				return tc::concat(
					ojsym ? FullyQualifiedName(*ojsym) : "[no symbol]",
					", flags = ", tc::as_dec(static_cast<int>(jtype->getFlags()))
				);
			}),
			", "
		), 
		"\n");
		std::exit(EXIT_FAILURE);
	}

	auto const vecmt = tc::make_vector(tc::transform(
		tc::zip(atypeArguments, rngetype),
		[&](ts::Type jtype, ETypeParameter etypeparam) noexcept -> SMangledType {
			// Type arguments are the only place where we do not want to replace enum literals with enum types
			if(etypeparamENUM == etypeparam) {
				if((ts::TypeFlags::NumberLiteral | ts::TypeFlags::EnumLiteral) == jtype->getFlags()) {
					return MangleEnumLiteral(jtype);
				} else if(auto ojuniontype = jtype->isUnion()) {
					_ASSERT(tc::all_of((*ojuniontype)->types(), [](auto jtypeUnionOption) noexcept {
						return (ts::TypeFlags::NumberLiteral | ts::TypeFlags::EnumLiteral) == jtypeUnionOption->getFlags();
					}));
					// Here we replace an expression like Token<Enum.a | Enum.b | Enum.c> with Token<Enum.a> which is of course not true.
					// Token<Enum.a | Enum.b | Enum.c> means that Token takes a non-type parameter with one of these three values.
					return CommentType(MangleEnumLiteral(tc::front((*ojuniontype)->types())).m_strCppCanonized, jtype);
				}
				// TypeFlags::TypeParameter
			} else if(etypeparamNUMBER == etypeparam) {
				_ASSERTFALSE;
			}
			return MangleType(jtype);
		}
	));

	return {
		tc::make_str<char>(strCppCanonized, "<", tc::join_separated(tc::transform(vecmt, TC_MEMBER(.ExpandType())), ", "), ">"),
		tc::make_str<char>(
			strWithComments, "<", tc::join_separated(tc::transform(vecmt, TC_MEMBER(.m_strWithComments)), ", "), ">"
		)};
}

template<typename Rng>
SMangledType MangleClassOrInterface(std::string const& strFullyQualifiedName, Rng const& rngtypearg) noexcept {
	if(auto ojsclass = tc::cont_find<tc::return_element_or_null>(g_setjsclass, strFullyQualifiedName)) {
		// When jinterfaceRoot refers to interface C declared as
		// 	interface C<K, T> {}
		// (*jointerfacetypeRoot)->typeParameters() returns the uninstantiated generic type parameters K, T
		// We don't want to output them.
		if(tc::empty(rngtypearg)) {
			return {ojsclass->m_strMangledName};
		} else {
			return MangleTypeArguments(
				ojsclass->m_strMangledName,
				ojsclass->m_strMangledName,
				rngtypearg,
				tc::transform(ojsclass->m_vectypeparam, TC_MEMBER(.m_etypeparam))
			);
		} 
	} else if(IsBootstrapType(strFullyQualifiedName)) {
		auto str = tc::make_str("_js_j", strFullyQualifiedName);
		if(tc::empty(rngtypearg)) {
			return {str};
		} else {
			return MangleTypeArguments(str, str, rngtypearg, tc::repeat_n(tc::size(rngtypearg), etypeparamTYPE));
		}
	} else {
		return {mangling_error, tc::make_str("tc::js::any /*UnknownClassOrInterface=", strFullyQualifiedName, "*/")};
	}
}

template<typename Rng>
SMangledType MangleClassOrInterface(ts::InterfaceType jinterfacetypeRoot, Rng const& rngtypearg) noexcept {
	auto const strName = FullyQualifiedName(*jinterfacetypeRoot->getSymbol());
	_ASSERT(!jinterfacetypeRoot->outerTypeParameters()); // TODO: Not supported
	return ::MangleClassOrInterface(strName, rngtypearg);
}

SMangledType MangleClassOrInterface(ts::InterfaceType jinterfacetypeRoot) noexcept;