#pragma once
#include <type_traits>
#include <tuple>

namespace unique_tuple
{

template <typename T>
struct leaf {};

template <typename ...R>
struct impl;

template <typename V, typename ...R>
struct impl<V, R...> : leaf<V>, impl<R...> { };

template <typename V>
struct impl<V> : leaf<V> { };

template <>
struct impl<> {};

template <typename A, typename V, typename ...R>
struct unique_tuple_impl;

template <typename ...A, typename V, typename ...R>
struct unique_tuple_impl<impl<A...>, V, R...>
{
	using type = typename std::conditional<
		std::is_base_of<leaf<V>, impl<A...>>() ? true : false
		, unique_tuple_impl<impl<A...>, R...>
		, unique_tuple_impl<impl<V, A...>, R...>
	>::type::type;
};

template <typename ...A, typename V>
struct unique_tuple_impl<impl<A...>, V>
{
	using type = typename std::conditional<
		std::is_base_of<leaf<V>, impl<A...>>() ? true : false
		, std::tuple<A...>
		, std::tuple<V, A...>
	>::type;
};

template <typename V>
struct unique_tuple_impl<impl<>, V>
{
	using type = std::tuple<V>;
};

template <typename ...Values>
using unique_tuple = typename unique_tuple_impl<impl<>, Values...>::type;

} // namespace unique_tuple
