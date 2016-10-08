#pragma once
#include <map>
#include <tuple>
#include <functional>
#include <exception>
#include <utility> 
#include <cassert>
#include <vector>
#include <iostream>

#ifndef SparseMatrix_defined



template <size_t ...Dims>
constexpr bool dim_bound_check_static(decltype(Dims)... args) {
	return ((args < Dims) && ... && true);
}

template <typename T, size_t DimA, size_t DimB>
class SparseMatrix2;

template <typename T, size_t DimA, size_t DimB>
std::ostream& operator<< (std::ostream& out, const SparseMatrix2<T, DimA, DimB>& d) noexcept;

/// @brief ��άϡ�����
/// @details
/// ��άϡ�����ʵ���˾���Ļ�������
/// @tparam T ����Ԫ������
/// @tparam DimA ��������
/// @tparam DimB ��������
template <typename T, size_t DimA, size_t DimB>
class SparseMatrix2
{
public:
	/// Ĭ�Ϲ��캯��
	constexpr SparseMatrix2();

	/// @brief �Զ�ά����Ϊ�����Ĺ��캯��
	/// @param Args ����Ķ�ά����
	/// @tparam A ��������
	/// @tparam B ��������
	template<size_t A, size_t B>
	SparseMatrix2(const T (&Args)[A][B]);

	//��������ģ���ػ�Ϊ��Ԫ��
	template<typename, size_t, size_t> friend class SparseMatrix2;

	/// ������������
	using dim_t = std::tuple<size_t, size_t>;

	/// ������Ԫ������
	using item_t = std::tuple<T, const dim_t>;

private:

	/// �����ά����Ϣ
	static const dim_t dim_tuple;

	/// �����ڲ��洢
	std::map<const dim_t, T> container;

	/// ��̬�߽���
	template<size_t I = 0>
	typename std::enable_if<I == std::tuple_size<dim_t>::value, void>::type
		static constexpr dim_bound_check(dim_t const&, dim_t const&) noexcept;
	template<size_t I = 0>
	typename std::enable_if < I < std::tuple_size<dim_t>::value, void>::type
		static constexpr dim_bound_check(dim_t const& t1, dim_t const& t2);

protected:

	/// @brief �����߽���Ļ�ȡ
	/// @return ֵ
	/// @param DimAg ������
	/// @param DimBg ������
	T get_no_check(size_t DimAg, size_t DimBg) const;

public:

	/// @brief ��̬�߽��������
	/// @param ele ֵ
	/// @param DimAs ������
	/// @param DimBs ������
	void set(T ele, size_t DimAs, size_t DimBs);

	/// @brief ��̬�߽��������
	/// @param ele ֵ
	/// @tparam DimA ������
	/// @tparam DimB ������
	template<size_t DimAs, size_t DimBs>
	void set(T ele) noexcept;

	/// @brief ��̬�߽���Ļ�ȡ
	/// @return ֵ
	/// @param DimAg ������
	/// @param DimBg ������
	T get(size_t DimAg, size_t DimBg) const;

	/// @brief ��̬�߽���Ļ�ȡ
	/// @return ֵ
	/// @tparam DimAg ������
	/// @tparam DimBg ������
	template<size_t DimAg, size_t DimBg>
	T get() const noexcept;

	/// @brief AxB��BxC�ľ���˻�
	/// @return �˻�
	/// @param m2 Ŀ�����
	/// @tparam DimA ����1������
	/// @tparam DimB ����1������������2������
	/// @tparam DimC ����2������
	template <size_t DimC>
	SparseMatrix2<T, DimA, DimC> Mul(SparseMatrix2<T, DimB, DimC> m2) const noexcept;

	/// @brief AxB�ľ���ӷ�
	/// @return ��
	/// @param m2 Ŀ�����
	/// @tparam DimA ���������
	/// @tparam DimB ���������
	SparseMatrix2<T, DimA, DimB> Add(SparseMatrix2<T, DimA, DimB> m2) const noexcept;

	/// @brief AxB�ľ������
	/// @return ��
	/// @param m2 Ŀ�����
	/// @tparam DimA ���������
	/// @tparam DimB ���������
	SparseMatrix2<T, DimA, DimB> Sub(SparseMatrix2<T, DimA, DimB> m2) const noexcept;

	/// @brief AxB�ľ���ת��
	/// @return ת��
	/// @tparam DimA ���������
	/// @tparam DimB ���������
	SparseMatrix2<T, DimB, DimA> Rev() const noexcept;

	/// @brief AxB�ľ������
	/// @return ԭ�����
	/// @param out �����
	/// @param d ����ľ���
	/// @tparam DimA ���������
	/// @tparam DimB ���������
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
constexpr SparseMatrix2<T, DimA, DimB>::dim_bound_check(dim_t const&, dim_t const&) noexcept// Unused arguments are given no names.
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