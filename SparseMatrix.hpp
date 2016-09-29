#pragma once
#include <cstddef>
#include <map>
#include <tuple>
#include <functional>
#include <exception>
#include <utility> 
#include <cassert>

#ifndef SparseMatrix_defined

template <std::size_t ...Dims>
constexpr bool dim_bound_check_static(decltype(Dims)... args)
{
	return ((args < Dims) && ... && true);
}

template <typename T, std::size_t ...Dims>
class SparseMatrix
{
public:
	SparseMatrix()
	{
		
	}

	using dim_t = std::tuple<decltype(Dims)...>;
	using item_t = std::tuple<T,const dim_t>;

private:
	std::map<const dim_t, T> container;

	static const dim_t dim_tuple;

	template<std::size_t I = 0>
	typename std::enable_if<I == std::tuple_size<dim_t>::value, void>::type
		static constexpr dim_bound_check(dim_t const&, dim_t const&) noexcept// Unused arguments are given no names.
	{ }

	template<std::size_t I = 0>
	typename std::enable_if < I < std::tuple_size<dim_t>::value, void>::type
		static constexpr dim_bound_check(dim_t const& t1, dim_t const& t2)
	{
		auto x = std::get<I>(t1);
		auto y = std::get<I>(t2);
		if(x < y) {
			throw std::out_of_range("Matrix bound check failed");
		}
		dim_bound_check<I + 1>(t1, t2);
	}

public:
	void set(T ele, decltype(Dims)... args)
	{
		dim_t i = std::make_tuple(args...);
		dim_bound_check(dim_tuple, i);
		container.insert_or_assign(i, ele);
	}

	template<decltype(Dims)... Args>
	void set(T ele)
	{
		static_assert(dim_bound_check_static<Dims...>(Args...),"Matrix bound check failed");
		dim_t i = std::make_tuple(Args...);
		container.insert_or_assign(i, ele);
	}
};

template <typename T, std::size_t ...Dims>
constexpr typename SparseMatrix<T, Dims... >::dim_t SparseMatrix<T, Dims... >::dim_tuple = std::make_tuple(Dims...);
#define SparseMatrix_defined

#endif
