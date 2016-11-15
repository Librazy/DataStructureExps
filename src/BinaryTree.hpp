#pragma once

#ifndef BinaryTree_disabled

#ifndef BinaryTree_defined

// ReSharper disable CppUnusedIncludeDirective
#include <memory>
#include <functional>
#include <stack>

namespace lib {
	template <typename T> using unidef_ptr = std::unique_ptr<T, std::default_delete<T>>;
}

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
 */
template<typename T, template<class> class P>
struct BinaryTree{
	BinaryTree(P<BinaryTree<T, P>>&& l, P<BinaryTree<T, P>>&& r, T&& d)
		:data(std::forward<T>(d)), left(std::forward<P<BinaryTree<T, P>>>(l)), right(std::forward<P<BinaryTree<T, P>>>(r)) {}
	explicit BinaryTree(T&& d)
		:data(std::forward<T>(d)){}

	T data;
	P<BinaryTree<T, P>> left;
	P<BinaryTree<T, P>> right;

	/**
	 * \brief 二叉树递归遍历
	 * \tparam O 遍历方式
	 * \tparam F 操作函数类型
	 * \param f 操作函数
	 */
	template<Order O,typename F>
	void TreeTraversalRecursive(F&& f);

	/**
	 * \brief 二叉树先序遍历
	 * \tparam O == Order::PreOrder
	 * \tparam F 操作函数类型
	 * \param f 操作函数
	 * \return void
	 */
	template<Order O, typename F>
	typename std::enable_if<O == Order::PreOrder, void>::type
	TreeTraversalIterative(F&& f){
		std::stack<BinaryTree<T, P>*> s;
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
	TreeTraversalIterative(F&& f){
		std::stack<BinaryTree<T, P>*> s;
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
	TreeTraversalIterative(F&& f){
		std::stack<BinaryTree<T, P>*> trv;
		std::stack<BinaryTree<T, P>*> out;
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

	template<Order O, int I, typename F>
	typename std::enable_if   <(I == 0 && O == Order::PreOrder)
							|| (I == 1 && O == Order::InOrder)
							|| (I == 2 && O == Order::PostOrder), void>::type
	TreeTraversalRecursiveImpl(F&& f){
		std::invoke(std::forward<F&&>(f), data);
	}

	template<Order O, int I, typename F>
	typename std::enable_if   <(I == 1 && O == Order::PreOrder)
							|| (I == 0 && O == Order::InOrder)
							|| (I == 0 && O == Order::PostOrder), void>::type
	TreeTraversalRecursiveImpl(F&& f){
		if(left)left->template TreeTraversalRecursive<O>(std::forward<F&&>(f));
	}

	template<Order O, int I, typename F>
	typename std::enable_if   <(I == 2 && O == Order::PreOrder)
							|| (I == 2 && O == Order::InOrder)
							|| (I == 1 && O == Order::PostOrder), void>::type
	TreeTraversalRecursiveImpl(F&& f){
		if(right)right->template TreeTraversalRecursive<O>(std::forward<F&&>(f));
	}
};

template<typename T, template<class> class P>
template<Order O, typename F>
void BinaryTree<T, P>::TreeTraversalRecursive(F&& f){
	TreeTraversalRecursiveImpl<O, 0>(std::forward<F&&>(f));
	TreeTraversalRecursiveImpl<O, 1>(std::forward<F&&>(f));
	TreeTraversalRecursiveImpl<O, 2>(std::forward<F&&>(f));
}

/**
* \brief 构造二叉树内节点
* \tparam T 数据类型
* \param left 左子树
* \param right 右子树
* \param t 数据
* \return 构造完成的树
*/
template<typename T>
lib::unidef_ptr<BinaryTree<T, lib::unidef_ptr>> MakeUniTree(lib::unidef_ptr<BinaryTree<T, lib::unidef_ptr>>&& left, lib::unidef_ptr<BinaryTree<T, lib::unidef_ptr>>&& right, T&& t) {
	return std::make_unique<BinaryTree<T, lib::unidef_ptr>>(std::forward<lib::unidef_ptr<BinaryTree<T, lib::unidef_ptr>>>(left), std::forward<lib::unidef_ptr<BinaryTree<T, lib::unidef_ptr>>>(right), std::forward<T>(t));
}

/**
* \brief 构造二叉树叶子节点
* \tparam T 数据类型
* \param t 数据
* \return 构造完成的树
*/
template<typename T>
lib::unidef_ptr<BinaryTree<T, lib::unidef_ptr>> MakeUniTree(T&& t) {
	return std::make_unique<BinaryTree<T, lib::unidef_ptr>>(std::forward<T>(t));
}

/**
* \brief 构造二叉树内节点
* \tparam T 数据类型
* \param left 左子树
* \param right 右子树
* \param t 数据
* \return 构造完成的树
*/
template<typename T>
std::shared_ptr<BinaryTree<T, std::shared_ptr>> MakeSrdTree(std::shared_ptr<BinaryTree<T, std::shared_ptr>>&& left, std::shared_ptr<BinaryTree<T, std::shared_ptr>>&& right, T&& t) {
	return std::make_shared<BinaryTree<T, std::shared_ptr>>(std::forward<std::shared_ptr<BinaryTree<T, std::shared_ptr>>>(left), std::forward<std::shared_ptr<BinaryTree<T, std::shared_ptr>>>(right), std::forward<T>(t));
}

/**
* \brief 构造二叉树叶子节点
* \tparam T 数据类型
* \param t 数据
* \return 构造完成的树
*/
template<typename T>
std::shared_ptr<BinaryTree<T, std::shared_ptr>> MakeSrdTree(T&& t) {
	return std::make_shared<BinaryTree<T, std::shared_ptr>>(std::forward<T>(t));
}

#define BinaryTree_defined

#endif

#endif