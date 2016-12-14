#pragma once

#ifndef BinaryTree_disabled

#ifndef BinaryTree_defined

// ReSharper disable CppUnusedIncludeDirective
#include <memory>
#include <functional>
#include <stack>

/**
 * \brief 遍历方式
 */
enum class Order{
	PreOrder,
	InOrder,
	PostOrder,
};

/**
 * \brief 二叉树的节点类型
 * \tparam T 数据类型
 * \tparam P 节点指针类型
 */
template<typename T, template<class...> class P>
class binary_tree{
public:
	virtual ~binary_tree() {}

	binary_tree(P<binary_tree<T, P>>&& l, P<binary_tree<T, P>>&& r, T&& d)
		:data(std::forward<T>(d)), left(std::forward<P<binary_tree<T, P>>>(l)), right(std::forward<P<binary_tree<T, P>>>(r)) {}
	explicit binary_tree(T&& d)
		:data(std::forward<T>(d)){}

	T data;
	P<binary_tree<T, P>> left;
	P<binary_tree<T, P>> right;

	virtual std::remove_reference_t<T>* operator->(){
		return &data;
	}

	/**
	 * \brief 二叉树递归遍历
	 * \tparam O 遍历方式
	 * \tparam F 操作函数类型
	 * \param f 操作函数
	 */
	template<Order O, typename F>
	void traversal_recursive(F&& f);

	/**
	 * \brief 二叉树先序遍历
	 * \tparam O == Order::PreOrder
	 * \tparam F 操作函数类型
	 * \param f 操作函数
	 * \return void
	 */
	template<Order O, typename F>
	typename std::enable_if<O == Order::PreOrder, void>::type
	traversal_iterative(F&& f){
		std::stack<binary_tree<T, P>*> s;
		s.push(this);
		while (!s.empty()) {
			auto cur = s.top();
			std::invoke(std::forward<F&&>(f), cur->data);
			s.pop();
			if (cur->right)s.emplace(cur->right.get());
			if (cur->left)s.emplace(cur->left.get());
		}
	}

	/**
	 * \brief 二叉树中序遍历
	 * \tparam O == Order::InOrder
	 * \tparam F 操作函数类型
	 * \param f 操作函数
	 * \return void
	 */
	template<Order O, typename F>
	typename std::enable_if<O == Order::InOrder, void>::type
	traversal_iterative(F&& f){
		std::stack<binary_tree<T, P>*> s;
		auto cur = this;
		while (!s.empty() || cur) {
			if (cur) {
				s.push(cur);
				cur = cur->left.get();
			}
			else {
				cur = s.top();
				s.pop();
				std::invoke(std::forward<F&&>(f), cur->data);
				cur = cur->right.get();
			}
		}
	}

	/**
	 * \brief 二叉树后序遍历
	 * \tparam O == Order::PostOrder
	 * \tparam F 操作函数类型
	 * \param f 操作函数
	 * \return void
	 */
	template<Order O, typename F>
	typename std::enable_if<O == Order::PostOrder, void>::type
	traversal_iterative(F&& f){
		std::stack<binary_tree<T, P>*> trv;
		std::stack<binary_tree<T, P>*> out;
		trv.push(this);
		while (!trv.empty()) {
			auto cur = trv.top();
			trv.pop();
			if (cur->left)trv.push(cur->left.get());
			if (cur->right)trv.push(cur->right.get());
			out.push(cur);
		}
		while (!out.empty()) {
			std::invoke(std::forward<F&&>(f), out.top()->data);
			out.pop();
		}
	}

private:
	template<Order O, int I, typename F>
	typename std::enable_if   <(I == 0 && O == Order::PreOrder)
							|| (I == 1 && O == Order::InOrder)
							|| (I == 2 && O == Order::PostOrder), void>::type
	traversal_recursive_impl(F&& f){
		std::invoke(std::forward<F&&>(f), data);
	}

	template<Order O, int I, typename F>
	typename std::enable_if   <(I == 1 && O == Order::PreOrder)
							|| (I == 0 && O == Order::InOrder)
							|| (I == 0 && O == Order::PostOrder), void>::type
	traversal_recursive_impl(F&& f){
		if(left)left->template traversal_recursive<O>(std::forward<F&&>(f));
	}

	template<Order O, int I, typename F>
	typename std::enable_if   <(I == 2 && O == Order::PreOrder)
							|| (I == 2 && O == Order::InOrder)
							|| (I == 1 && O == Order::PostOrder), void>::type
	traversal_recursive_impl(F&& f){
		if(right)right->template traversal_recursive<O>(std::forward<F&&>(f));
	}
};

template<typename T, template<class...> class P>
template<Order O, typename F>
void binary_tree<T, P>::traversal_recursive(F&& f){
	traversal_recursive_impl<O, 0>(std::forward<F&&>(f));
	traversal_recursive_impl<O, 1>(std::forward<F&&>(f));
	traversal_recursive_impl<O, 2>(std::forward<F&&>(f));
}

/**
 * \brief 构造二叉树内节点（以shared_ptr包装）
 * \tparam T 数据类型
 * \param left 左子树
 * \param right 右子树
 * \param t 数据
 * \return 构造完成的树
 */
template<typename T>
std::shared_ptr<binary_tree<T, std::shared_ptr>> make_tree(std::shared_ptr<binary_tree<T, std::shared_ptr>>&& left, std::shared_ptr<binary_tree<T, std::shared_ptr>>&& right, T&& t) {
	return std::make_shared<binary_tree<T, std::shared_ptr>>(std::forward<std::shared_ptr<binary_tree<T, std::shared_ptr>>>(left), std::forward<std::shared_ptr<binary_tree<T, std::shared_ptr>>>(right), std::forward<T>(t));
}

/**
 * \brief 构造二叉树内节点（以unique_ptr包装）
 * \tparam T 数据类型
 * \param left 左子树
 * \param right 右子树
 * \param t 数据
 * \return 构造完成的树
 */
template<typename T>
std::unique_ptr<binary_tree<T, std::unique_ptr>> make_tree(std::unique_ptr<binary_tree<T, std::unique_ptr>>&& left, std::unique_ptr<binary_tree<T, std::unique_ptr>>&& right, T&& t) {
	return std::make_unique<binary_tree<T, std::unique_ptr>>(std::forward<std::unique_ptr<binary_tree<T, std::unique_ptr>>>(left), std::forward<std::unique_ptr<binary_tree<T, std::unique_ptr>>>(right), std::forward<T>(t));
}

/**
 * \brief 构造二叉树内节点
 * \tparam P 包装指针类型
 * \tparam T 数据类型
 * \param left 左子树
 * \param right 右子树
 * \param t 数据
 * \return 构造完成的树
 */
template<template<class...> class P, typename T>
P<binary_tree<T, P>> make_tree(P<binary_tree<T, P>>&& left, P<binary_tree<T, P>>&& right, T&& t)
{
	return P<binary_tree<T, P>>(new binary_tree<T, P>(std::forward<P<binary_tree<T, P>>>(left), std::forward<P<binary_tree<T, P>>>(right), std::forward<T>(t)));
}

/**
 * \brief 构造二叉树叶子节点
 * \tparam P 包装指针类型
 * \tparam T 数据类型
 * \param t 数据
 * \return 构造完成的树
 */
template<template<class...> class P, typename T>
auto make_tree(T&& t) ->
typename std::enable_if<
	!std::is_same<P<int>, std::unique_ptr<int>>::value && !std::is_same<P<int>, std::shared_ptr<int>>::value
	, P<binary_tree<T, P>>
>::type
{
	return P<binary_tree<T, P>>(new binary_tree<T, P>(std::forward<T>(t)));
}

/**
 * \brief 构造二叉树叶子节点（以unique_ptr包装）
 * \tparam P = unique_ptr 包装指针类型
 * \tparam T 数据类型
 * \param t 数据
 * \return 构造完成的树
 */
template<template<class...> class P, typename T>
auto make_tree(T&& t) ->
typename std::enable_if<
	std::is_same<P<T>, std::unique_ptr<T>>::value
	, P<binary_tree<T, P>>
>::type 
{
	return std::make_unique<binary_tree<T, P>>(std::forward<T>(t));
}

/**
 * \brief 构造二叉树叶子节点（以shared_ptr包装）
 * \tparam P = shared_ptr 包装指针类型
 * \tparam T 数据类型
 * \param t 数据
 * \return 构造完成的树
 */
template<template<class...> class P = std::shared_ptr, typename T>
auto make_tree(T&& t) ->
typename std::enable_if<
	std::is_same<P<T>, std::shared_ptr<T>>::value
	, P<binary_tree<T, P>>
>::type
{
	return std::make_shared<binary_tree<T, P>>(std::forward<T>(t));
}

#define BinaryTree_defined

#endif

#endif