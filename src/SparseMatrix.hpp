#pragma once

#ifdef SparseMatrix_enabled

#ifndef SparseMatrix_defined
// ReSharper disable CppUnusedIncludeDirective
#include <iostream>
#include <map>
#include <tuple>
#include <utility>
#include <vector>

template <size_t ...Dims>
constexpr bool dim_bound_check_static(decltype(Dims)... args) {
	return (true && ... && (args < Dims));
}

template <typename T, size_t DimA, size_t DimB>
class SparseMatrix2;

template <typename T, size_t DimA, size_t DimB>
std::ostream& operator<< (std::ostream& out, const SparseMatrix2<T, DimA, DimB>& d) noexcept;

/// @brief 二维稀疏矩阵
/// @details
/// 二维稀疏矩阵，实现了矩阵的基本操作
/// @tparam T 矩阵元素类型
/// @tparam DimA 矩阵行数
/// @tparam DimB 矩阵列数
template <typename T, size_t DimA, size_t DimB>
class SparseMatrix2
{
public:
	/// 默认构造函数
	constexpr SparseMatrix2();

	/// @brief 以二维数组为参数的构造函数
	/// @param Args 输入的二维矩阵
	/// @tparam A 矩阵行数
	/// @tparam B 矩阵列数
	template<size_t A, size_t B>
	SparseMatrix2(const T (&Args)[A][B]);

	//声明所有模版特化为友元类
	template<typename, size_t, size_t> friend class SparseMatrix2;

	/// 矩阵坐标类型
	using dim_t = std::tuple<size_t, size_t>;

	/// 矩阵三元组类型
	using item_t = std::tuple<T, const dim_t>;

private:

	/// 矩阵的维度信息
	static const dim_t dim_tuple;

	/// 矩阵内部存储
	std::map<const dim_t, T> container;

	/// 动态边界检查
	template<size_t I = 0>
	typename std::enable_if<I == std::tuple_size<dim_t>::value, void>::type
		static constexpr dim_bound_check(dim_t const&, dim_t const&) noexcept;
	template<size_t I = 0>
	typename std::enable_if < I < std::tuple_size<dim_t>::value, void>::type
		static constexpr dim_bound_check(dim_t const& t1, dim_t const& t2);

protected:

	/// @brief 不带边界检查的获取
	/// @return 值
	/// @param DimAg 行坐标
	/// @param DimBg 列坐标
	T get_no_check(size_t DimAg, size_t DimBg) const;

public:

	/// @brief 动态边界检查的设置
	/// @param ele 值
	/// @param DimAs 行坐标
	/// @param DimBs 列坐标
	void set(T ele, size_t DimAs, size_t DimBs);

	/// @brief 静态边界检查的设置
	/// @param ele 值
	/// @tparam DimA 行坐标
	/// @tparam DimB 列坐标
	template<size_t DimAs, size_t DimBs>
	void set(T ele) noexcept;

	/// @brief 动态边界检查的获取
	/// @return 值
	/// @param DimAg 行坐标
	/// @param DimBg 列坐标
	T get(size_t DimAg, size_t DimBg) const;

	/// @brief 静态边界检查的获取
	/// @return 值
	/// @tparam DimAg 行坐标
	/// @tparam DimBg 列坐标
	template<size_t DimAg, size_t DimBg>
	T get() const noexcept;

	/// @brief AxB与BxC的矩阵乘积
	/// @return 乘积
	/// @param m2 目标矩阵
	/// @tparam DimA 矩阵1的行数
	/// @tparam DimB 矩阵1的列数即矩阵2的行数
	/// @tparam DimC 矩阵2的列数
	template <size_t DimC>
	SparseMatrix2<T, DimA, DimC> Mul(SparseMatrix2<T, DimB, DimC> m2) const noexcept;

	/// @brief AxB的矩阵加法
	/// @return 和
	/// @param m2 目标矩阵
	/// @tparam DimA 矩阵的行数
	/// @tparam DimB 矩阵的列数
	SparseMatrix2<T, DimA, DimB> Add(SparseMatrix2<T, DimA, DimB> m2) const noexcept;

	/// @brief AxB的矩阵减法
	/// @return 差
	/// @param m2 目标矩阵
	/// @tparam DimA 矩阵的行数
	/// @tparam DimB 矩阵的列数
	SparseMatrix2<T, DimA, DimB> Sub(SparseMatrix2<T, DimA, DimB> m2) const noexcept;

	/// @brief AxB的矩阵转置
	/// @return 转置
	/// @tparam DimA 矩阵的行数
	/// @tparam DimB 矩阵的列数
	SparseMatrix2<T, DimB, DimA> Rev() const noexcept;

	/// @brief AxB的矩阵输出
	/// @return 原输出流
	/// @param out 输出流
	/// @param d 输出的矩阵
	/// @tparam DimA 矩阵的行数
	/// @tparam DimB 矩阵的列数
	friend std::ostream& operator<< <>(std::ostream& out, const SparseMatrix2<T, DimA, DimB>& d) noexcept;
};

template <typename T, size_t DimA, size_t DimB>
constexpr typename SparseMatrix2<T, DimA, DimB>::dim_t SparseMatrix2<T, DimA, DimB>::dim_tuple = std::make_tuple(DimA, DimB);

template <typename T, size_t DimA, size_t DimB>
constexpr SparseMatrix2<T, DimA, DimB>::SparseMatrix2(){}

template <typename T, size_t DimA, size_t DimB>
template<size_t A, size_t B>
SparseMatrix2<T, DimA, DimB>::SparseMatrix2(const T (&Args)[A][B])
{
	static_assert(A == DimA, "Row size doesn't match");
	static_assert(B == DimB, "Col size doesn't match");
	for (size_t i = 0; i != DimA; ++i) {
		for (size_t j = 0; j != DimB; ++j) {
			dim_t t = std::make_tuple(i, j);
			container.insert_or_assign(t, Args[i][j]);
		}
	}
}

template <typename T, size_t DimA, size_t DimB>
template<size_t I>
typename std::enable_if<I == std::tuple_size<typename SparseMatrix2<T, DimA, DimB>::dim_t>::value, void>::type
constexpr SparseMatrix2<T, DimA, DimB>::dim_bound_check(dim_t const&, dim_t const&) noexcept
{ }

template <typename T, size_t DimA, size_t DimB>
template<size_t I>
typename std::enable_if < I < std::tuple_size<typename SparseMatrix2<T, DimA, DimB>::dim_t>::value, void>::type
constexpr SparseMatrix2<T, DimA, DimB>::dim_bound_check(dim_t const& t1, dim_t const& t2)
{
	auto x = std::get<I>(t1);
	auto y = std::get<I>(t2);
	if (x < y) {
		throw std::out_of_range("Matrix bound check failed");
	}
	dim_bound_check<I + 1>(t1, t2);
}

template <typename T, size_t DimA, size_t DimB>
T SparseMatrix2<T, DimA, DimB>::get_no_check(size_t DimAg, size_t DimBg) const
{
	dim_t i = std::make_tuple(DimAg, DimBg);
	auto x = container.find(i);
	if (x != container.end()) {
		return x->second;
	}
	return T();
}

template <typename T, size_t DimA, size_t DimB>
void SparseMatrix2<T, DimA, DimB>::set(T ele, size_t DimAs, size_t DimBs)
{
	dim_t i = std::make_tuple(DimAs, DimBs);
	dim_bound_check(dim_tuple, i);
	container.insert_or_assign(i, ele);
}

template <typename T, size_t DimA, size_t DimB>
template<size_t DimAs, size_t DimBs>
void SparseMatrix2<T, DimA, DimB>::set(T ele) noexcept
{
	static_assert(dim_bound_check_static<DimA, DimB>(DimAs, DimBs), "Matrix bound check failed");
	dim_t i = std::make_tuple(DimAs, DimBs);
	container.insert_or_assign(i, ele);
}

template <typename T, size_t DimA, size_t DimB>
T SparseMatrix2<T, DimA, DimB>::get(size_t DimAg, size_t DimBg) const
{
	dim_t i = std::make_tuple(DimAg, DimBg);
	dim_bound_check(dim_tuple, i);
	auto x = container.find(i);
	if (x != container.end()) {
		return x->second;
	}
	return T();
}

template <typename T, size_t DimA, size_t DimB>
template<size_t DimAg, size_t DimBg>
T SparseMatrix2<T, DimA, DimB>::get() const noexcept
{
	static_assert(dim_bound_check_static<DimA, DimB>(DimAg, DimBg), "Matrix bound check failed");
	dim_t i = std::make_tuple(DimAg, DimBg);
	auto x = container.find(i);
	if (x != container.end()) {
		return x->second;
	}
	return T();
}

template <typename T, size_t DimA, size_t DimB>
template <size_t DimC>
SparseMatrix2<T, DimA, DimC> SparseMatrix2<T, DimA, DimB>::Mul(SparseMatrix2<T, DimB, DimC> m2) const noexcept
{
	SparseMatrix2<T, DimA, DimC> res;
	for (auto ele : container) {
		size_t Da = std::get<0>(ele.first);
		for (size_t i = 0; i != DimC; ++i) {
			T a = T();
			for (size_t j = 0; j != DimB; ++j) {
				T val;
				if ((val = m2.get_no_check(j, i)) != 0) {
					a += ele.second*val;
				}
			}
			if ((a = res.get_no_check(Da, i) + a) != 0) {
				res.set(a, Da, i);
			}
		}
	}
	return res;
}

template <typename T, size_t DimA, size_t DimB>
SparseMatrix2<T, DimA, DimB> SparseMatrix2<T, DimA, DimB>::Add(SparseMatrix2<T, DimA, DimB> m2) const noexcept {
	SparseMatrix2<T, DimA, DimB> res;
	for (auto ele : container) {
		res.set(ele.second, std::get<0>(ele.first), std::get<1>(ele.first));
	}
	for (auto ele : m2.container) {
		res.set(res.get_no_check(std::get<0>(ele.first), std::get<1>(ele.first)) + ele.second, std::get<0>(ele.first), std::get<1>(ele.first));
	}
	return res;
}

template <typename T, size_t DimA, size_t DimB>
SparseMatrix2<T, DimA, DimB> SparseMatrix2<T, DimA, DimB>::Sub(SparseMatrix2<T, DimA, DimB> m2) const noexcept {
	SparseMatrix2<T, DimA, DimB> res;
	for (auto ele : container) {
		res.set(ele.second, std::get<0>(ele.first), std::get<1>(ele.first));
	}
	for (auto ele : m2.container) {
		res.set(res.get_no_check(std::get<0>(ele.first), std::get<1>(ele.first)) - ele.second, std::get<0>(ele.first), std::get<1>(ele.first));
	}
	return res;
}

template <typename T, size_t DimA, size_t DimB>
SparseMatrix2<T, DimB, DimA> SparseMatrix2<T, DimA, DimB>::Rev() const noexcept
{
	SparseMatrix2<T, DimB, DimA> res;
	for (auto ele : container) {
		res.set(ele.second, std::get<1>(ele.first), std::get<0>(ele.first));
	}
	return res;
}

template <typename T, size_t DimA, size_t DimB>
SparseMatrix2<T, DimA, DimB> operator+(SparseMatrix2<T, DimA, DimB> a, SparseMatrix2<T, DimA, DimB> b) noexcept
{
	return a.Add(b);
}

template <typename T, size_t DimA, size_t DimB>
SparseMatrix2<T, DimA, DimB> operator-(SparseMatrix2<T, DimA, DimB> a, SparseMatrix2<T, DimA, DimB> b) noexcept
{
	return a.Sub(b);
}

template <typename T, size_t DimA, size_t DimB, size_t DimC>
SparseMatrix2<T, DimA, DimC> operator*(SparseMatrix2<T, DimA, DimB> a, SparseMatrix2<T, DimB, DimC> b) noexcept
{
	return a.Mul(b);
}

template <typename T, size_t DimA, size_t DimB>
std::ostream& operator<< (std::ostream& out, const SparseMatrix2<T, DimA, DimB>& d) noexcept
{
	for (size_t i = 0; i != DimA; ++i) {
		for (size_t j = 0; j != DimB; ++j) {
			out << (j ? " " : "") << d.get(i, j);
		}
		out << std::endl;
	}
	return out;
}

template <typename T, size_t ...Dims>
class SparseMatrix
{
public:
	template<typename, size_t ...> friend class SparseMatrix;
	using dim_t = std::tuple<decltype(Dims)...>;
	using item_t = std::tuple<T, const dim_t>;

private:
	std::map<const dim_t, T> container;

	static const dim_t dim_tuple;

	template<size_t I = 0>
	typename std::enable_if<I == std::tuple_size<dim_t>::value, void>::type
		static constexpr dim_bound_check(dim_t const&, dim_t const&) noexcept;
	template<size_t I = 0>
	typename std::enable_if < I < std::tuple_size<dim_t>::value, void>::type
		static constexpr dim_bound_check(dim_t const& t1, dim_t const& t2);

public:
	void set(T ele, decltype(Dims)... args);

	template<decltype(Dims)... Args>
	void set(T ele);

	T get(decltype(Dims)... args) const;

	template<decltype(Dims)... Args>
	T get() const noexcept;
};

template <typename T, size_t ...Dims>
constexpr typename SparseMatrix<T, Dims... >::dim_t SparseMatrix<T, Dims... >::dim_tuple = std::make_tuple(Dims...);

template <typename T, size_t ...Dims>
template<size_t I>
typename std::enable_if<I == std::tuple_size<typename SparseMatrix< T, Dims...>::dim_t>::value, void>::type
constexpr SparseMatrix< T, Dims...>::dim_bound_check(dim_t const&, dim_t const&) noexcept
{ }

template <typename T, size_t ...Dims>
template<size_t I>
typename std::enable_if < I < std::tuple_size<typename  SparseMatrix< T, Dims...>::dim_t>::value, void>::type
	constexpr SparseMatrix< T, Dims...>::dim_bound_check(dim_t const& t1, dim_t const& t2)
{
	auto x = std::get<I>(t1);
	auto y = std::get<I>(t2);
	if (x < y) {
		throw std::out_of_range("Matrix bound check failed");
	}
	dim_bound_check<I + 1>(t1, t2);
}

template <typename T, size_t ...Dims>
void SparseMatrix< T, Dims...>::set(T ele, decltype(Dims)... args)
{
	dim_t i = std::make_tuple(args...);
	dim_bound_check(dim_tuple, i);
	container.insert_or_assign(i, ele);
}

template <typename T, size_t ...Dims>
template<decltype(Dims)... Args>
void SparseMatrix< T, Dims...>::set(T ele)
{
	static_assert(dim_bound_check_static<Dims...>(Args...), "Matrix bound check failed");
	dim_t i = std::make_tuple(Args...);
	container.insert_or_assign(i, ele);
}

template <typename T, size_t ...Dims>
T SparseMatrix< T, Dims...>::get(decltype(Dims)... args) const
{
	dim_t i = std::make_tuple(args...);
	dim_bound_check(dim_tuple, i);
	auto x = container.find(i);
	if (x != container.end()) {
		return x->second;
	}
	return T();
}

template <typename T, size_t ...Dims>
template<decltype(Dims)... Args>
T SparseMatrix< T, Dims...>::get() const noexcept
{
	static_assert(dim_bound_check_static<Dims...>(Args...), "Matrix bound check failed");
	dim_t i = std::make_tuple(Args...);
	auto x = container.find(i);
	if (x != container.end()) {
		return x->second;
	}
	return T();
}

#define SparseMatrix_defined

#endif

#endif