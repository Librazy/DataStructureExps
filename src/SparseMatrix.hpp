#pragma once

#ifndef SparseMatrix_disabled

#ifndef SparseMatrix_defined
// ReSharper disable CppUnusedIncludeDirective
#include <iostream>
#include <map>
#include <tuple>
#include <utility>
#include <vector>
#include <optional.hpp>

#ifdef Use_FoldExp
template <size_t ...Dims>
constexpr bool dim_bound_check_static(decltype(Dims)... args) {
	return (true && ... && (args < Dims));
}
#else
template <size_t DimA, size_t DimB>
constexpr bool dim_bound_check_static(size_t arga, size_t argb) {
	return DimA > arga && DimB > argb;
}
#endif

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
	explicit SparseMatrix2(const T (&Args)[A][B]);

	//声明所有模版特化为友元类
	template<typename, size_t, size_t> friend class SparseMatrix2;

	/// 矩阵坐标类型
	using dim_t = std::tuple<size_t, size_t>;

	/// 矩阵三元组类型
	using item_t = std::tuple<T, const dim_t>;

	/// 矩阵内部存储类型
	using container_t = std::map<const dim_t, T>;

private:

	/// 矩阵的维度信息
	static const dim_t dim_tuple;

	/// 矩阵内部存储
	container_t container;

	/// 动态边界检查
	void static constexpr dim_bound_check(dim_t const& t1, dim_t const& t2);

	void check_row_min_max(size_t row, size_t cur);

protected:

	/// @brief 不带边界检查的获取
	/// @return 值
	/// @param DimAg 行坐标
	/// @param DimBg 列坐标
	stx::optional<T> get_unchecked(size_t DimAg, size_t DimBg) const;

	/// @brief 不带边界检查的设置
	/// @param ele 值
	/// @param DimAs 行坐标
	/// @param DimBs 列坐标
	void set_unchecked(T ele, size_t DimAs, size_t DimBs);

	///行最小列下标
	std::map<size_t, size_t> row_min;

	///行最大列下标
	std::map<size_t, size_t> row_max;

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
	stx::optional<T> get(size_t DimAg, size_t DimBg) const;

	/// @brief 静态边界检查的获取
	/// @return 值
	/// @tparam DimAg 行坐标
	/// @tparam DimBg 列坐标
	template<size_t DimAg, size_t DimBg>
	stx::optional<T> get() const noexcept;

	/// @brief 动态边界检查的查找
	/// @return 是否存在
	/// @param DimAg 行坐标
	/// @param DimBg 列坐标
	/// @param out 返回值
	bool have(size_t DimAg, size_t DimBg, T& out) const;

	/// @brief 静态边界检查的查找
	/// @return 是否存在
	/// @tparam DimAg 行坐标
	/// @tparam DimBg 列坐标
	/// @param out 返回值
	template<size_t DimAg, size_t DimBg>
	bool have(T& out) const noexcept;

	std::vector<std::pair<size_t, T>> row(size_t r) const;

	template<size_t R>
	std::vector<std::pair<size_t, T>> row() const noexcept;

	/// @brief AxB与BxC的矩阵乘积
	/// @return 乘积
	/// @param m2 目标矩阵
	/// @tparam DimA 矩阵1的行数
	/// @tparam DimB 矩阵1的列数即矩阵2的行数
	/// @tparam DimC 矩阵2的列数
	template <size_t DimC>
	SparseMatrix2<T, DimA, DimC> Mul(SparseMatrix2<T, DimB, DimC> const& m2) const noexcept;

	/// @brief AxB的矩阵加法
	/// @return 和
	/// @param m2 目标矩阵
	/// @tparam DimA 矩阵的行数
	/// @tparam DimB 矩阵的列数
	SparseMatrix2<T, DimA, DimB> Add(SparseMatrix2<T, DimA, DimB> const& m2) const noexcept;

	/// @brief AxB的矩阵减法
	/// @return 差
	/// @param m2 目标矩阵
	/// @tparam DimA 矩阵的行数
	/// @tparam DimB 矩阵的列数
	SparseMatrix2<T, DimA, DimB> Sub(SparseMatrix2<T, DimA, DimB> const& m2) const noexcept;

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
const typename SparseMatrix2<T, DimA, DimB>::dim_t SparseMatrix2<T, DimA, DimB>::dim_tuple = std::make_tuple(DimA, DimB);

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
			if(Args[i][j])set_unchecked(Args[i][j], i, j);
		}
	}
}

template <typename T, size_t DimA, size_t DimB>
void SparseMatrix2<T, DimA, DimB>::check_row_min_max(size_t row, size_t cur)
{
	auto it = row_max.find(row);
	if(!(it!=row_max.end() && it->second >= cur)) {
		row_max.insert_or_assign(row, cur);
	}
	it = row_min.find(row);
	if (!(it != row_min.end() && it->second <= cur)) {
		row_min.insert_or_assign(row, cur);
	}
}

template <typename T, size_t DimA, size_t DimB>
void constexpr SparseMatrix2<T, DimA, DimB>::dim_bound_check(dim_t const& t1, dim_t const& t2)
{
	if (std::get<0>(t1) < std::get<0>(t2) || std::get<1>(t1) < std::get<1>(t2)) {
		throw std::out_of_range("Matrix bound check failed");
	}
}

template <typename T, size_t DimA, size_t DimB>
void SparseMatrix2<T, DimA, DimB>::set_unchecked(T ele, size_t DimAs, size_t DimBs)
{
	container.insert_or_assign(std::make_tuple(DimAs, DimBs), ele);
	check_row_min_max(DimAs, DimBs);
}

template <typename T, size_t DimA, size_t DimB>
stx::optional<T> SparseMatrix2<T, DimA, DimB>::get_unchecked(size_t DimAg, size_t DimBg) const
{
	auto i = std::make_tuple(DimAg, DimBg);
	auto x = container.find(i);
	if (x != container.end()) {
		return x->second;
	}
	return {};
}

template <typename T, size_t DimA, size_t DimB>
void SparseMatrix2<T, DimA, DimB>::set(T ele, size_t DimAs, size_t DimBs)
{
	auto i = std::make_tuple(DimAs, DimBs);
	dim_bound_check(dim_tuple, i);
	set_unchecked(ele, DimAs, DimBs);
}

template <typename T, size_t DimA, size_t DimB>
template<size_t DimAs, size_t DimBs>
void SparseMatrix2<T, DimA, DimB>::set(T ele) noexcept
{
	static_assert(dim_bound_check_static<DimA, DimB>(DimAs, DimBs), "Matrix bound check failed");
	set_unchecked(ele, DimAs, DimBs);
}

template <typename T, size_t DimA, size_t DimB>
stx::optional<T> SparseMatrix2<T, DimA, DimB>::get(size_t DimAg, size_t DimBg) const
{
	auto i = std::make_tuple(DimAg, DimBg);
	dim_bound_check(dim_tuple, i);
	return get_unchecked(DimAg, DimBg);
}

template <typename T, size_t DimA, size_t DimB>
template<size_t DimAg, size_t DimBg>
stx::optional<T> SparseMatrix2<T, DimA, DimB>::get() const noexcept
{
	static_assert(dim_bound_check_static<DimA, DimB>(DimAg, DimBg), "Matrix bound check failed");
	return get_unchecked(DimAg, DimBg);
}

template <typename T, size_t DimA, size_t DimB>
bool SparseMatrix2<T, DimA, DimB>::have(size_t DimAg, size_t DimBg, T& out) const
{
	auto i = std::make_tuple(DimAg, DimBg);
	dim_bound_check(dim_tuple, i);
	auto x = container.find(i);
	if (x != container.end()) {
		out = x->second;
		return true;
	}
	out = T();
	return false;
}

template <typename T, size_t DimA, size_t DimB>
template<size_t DimAg, size_t DimBg>
bool SparseMatrix2<T, DimA, DimB>::have(T& out) const noexcept
{
	static_assert(dim_bound_check_static<DimA, DimB>(DimAg, DimBg), "Matrix bound check failed");
	auto i = std::make_tuple(DimAg, DimBg);
	auto x = container.find(i);
	if (x != container.end()) {
		out = x->second;
		return true;
	}
	out = T();
	return false;
}

template <typename T, size_t DimA, size_t DimB>
template<size_t R>
std::vector<std::pair<size_t, T>> SparseMatrix2<T, DimA, DimB>::row() const noexcept
{
	static_assert(R < DimA, "Matrix bound check failed");
	auto ret = std::vector<std::pair<size_t, T>>();
	auto it = row_min.find(R);
	if(it == row_min.end()) {
		return ret;
	}
	auto min = container.find(std::make_tuple(R, it->second));
	auto max = ++container.find(std::make_tuple(R, row_max.at(R)));

	for(; min!=max; ++min) {
		ret.emplace_back(std::get<1>(min->first), min->second);
	}
	return ret;
}

template <typename T, size_t DimA, size_t DimB>
std::vector<std::pair<size_t, T>> SparseMatrix2<T, DimA, DimB>::row(size_t r) const
{
	if (DimA <= r) {
		throw std::out_of_range("Matrix bound check failed");
	}
	auto ret = std::vector<std::pair<size_t, T>>();
	auto it = row_min.find(r);
	if (it == row_min.end()) {
		return ret;
	}
	auto min = container.find(std::make_tuple(r, it->second));
	auto max = ++container.find(std::make_tuple(r, row_max.at(r)));

	for (; min != max; ++min) {
		ret.emplace_back(std::get<1>(min->first), min->second);
	}
	return ret;
}

template <typename T, size_t DimA, size_t DimB>
template <size_t DimC>
SparseMatrix2<T, DimA, DimC> SparseMatrix2<T, DimA, DimB>::Mul(SparseMatrix2<T, DimB, DimC> const& m2) const noexcept
{
	SparseMatrix2<T, DimA, DimC> res;
	for (auto ele : container) {
		size_t Da = std::get<0>(ele.first);
		for (size_t i = 0; i != DimC; ++i) {
			T a = T();
			for (size_t j = 0; j != DimB; ++j) {
				T val;
				if ((val = m2.get_unchecked(j, i).value_or(0)) != 0) {
					a += ele.second*val;
				}
			}
			if ((a = res.get_unchecked(Da, i).value_or(0) + a) != 0) {
				res.set_unchecked(a, Da, i);
			}
		}
	}
	return res;
}

template <typename T, size_t DimA, size_t DimB>
SparseMatrix2<T, DimA, DimB> SparseMatrix2<T, DimA, DimB>::Add(SparseMatrix2<T, DimA, DimB> const& m2) const noexcept {
	SparseMatrix2<T, DimA, DimB> res;
	for (auto ele : container) {
		res.set_unchecked(ele.second, std::get<0>(ele.first), std::get<1>(ele.first));
	}
	for (auto ele : m2.container) {
		res.set_unchecked(res.get_unchecked(std::get<0>(ele.first), std::get<1>(ele.first)).value_or(0) + ele.second, std::get<0>(ele.first), std::get<1>(ele.first));
	}
	return res;
}

template <typename T, size_t DimA, size_t DimB>
SparseMatrix2<T, DimA, DimB> SparseMatrix2<T, DimA, DimB>::Sub(SparseMatrix2<T, DimA, DimB> const& m2) const noexcept {
	SparseMatrix2<T, DimA, DimB> res;
	for (auto ele : container) {
		res.set_unchecked(ele.second, std::get<0>(ele.first), std::get<1>(ele.first));
	}
	for (auto ele : m2.container) {
		res.set_unchecked(res.get_unchecked(std::get<0>(ele.first), std::get<1>(ele.first)).value_or(0) - ele.second, std::get<0>(ele.first), std::get<1>(ele.first));
	}
	return res;
}

template <typename T, size_t DimA, size_t DimB>
SparseMatrix2<T, DimB, DimA> SparseMatrix2<T, DimA, DimB>::Rev() const noexcept
{
	SparseMatrix2<T, DimB, DimA> res;
	for (auto ele : container) {
		res.set_unchecked(ele.second, std::get<1>(ele.first), std::get<0>(ele.first));
	}
	return res;
}

template <typename T, size_t DimA, size_t DimB>
SparseMatrix2<T, DimA, DimB> operator+(SparseMatrix2<T, DimA, DimB> const& a, SparseMatrix2<T, DimA, DimB> const& b) noexcept
{
	return a.Add(b);
}

template <typename T, size_t DimA, size_t DimB>
SparseMatrix2<T, DimA, DimB> operator-(SparseMatrix2<T, DimA, DimB> const& a, SparseMatrix2<T, DimA, DimB> const& b) noexcept
{
	return a.Sub(b);
}

template <typename T, size_t DimA, size_t DimB, size_t DimC>
SparseMatrix2<T, DimA, DimC> operator*(SparseMatrix2<T, DimA, DimB> const& a, SparseMatrix2<T, DimB, DimC> const& b) noexcept
{
	return a.Mul(b);
}

template <typename T, size_t DimA, size_t DimB>
std::ostream& operator<< (std::ostream& out, SparseMatrix2<T, DimA, DimB> const& d) noexcept
{
	for (size_t i = 0; i != DimA; ++i) {
		for (size_t j = 0; j != DimB; ++j) {
			out << (j ? " " : "") << d.get(i, j).value_or(T());
		}
		out << std::endl;
	}
	return out;
}

#ifdef Use_FoldExp
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
#endif

#define SparseMatrix_defined

#endif

#endif