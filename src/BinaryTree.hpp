#pragma once

#ifndef BinaryTree_disabled

#ifndef BinaryTree_defined

// ReSharper disable CppUnusedIncludeDirective
#include <memory>
#include <functional>
#include <stack>

/**
 * \brief 二叉树的节点类型
 * \tparam T 数据类型
 */
template<typename T>
struct BinaryTree{
	BinaryTree(std::unique_ptr<BinaryTree<T>>&& l, std::unique_ptr<BinaryTree<T>>&& r, T&& d)
		:data(std::forward<T>(d)), left(std::forward<std::unique_ptr<BinaryTree<T>>>(l)), right(std::forward<std::unique_ptr<BinaryTree<T>>>(r)) {}
	explicit BinaryTree(T&& d)
		:data(std::forward<T>(d)){}

	T data;
	std::unique_ptr<BinaryTree<T>> left;
	std::unique_ptr<BinaryTree<T>> right;
	using fun = std::function<void(T&)>;
};

/**
 * \brief 遍历方式
 */
enum class Order{
	PreOrder,
	InOrder,
	PostOrder,
};

/**
 * \brief 二叉树递归遍历
 * \tparam O 遍历方式
 * \tparam T 数据类型
 * \param tree 二叉树
 * \param f 操作函数
 */
template<Order O,typename T>
void TreeTraversalRecursive(std::unique_ptr<BinaryTree<T>> const& tree, typename BinaryTree<T>::fun&& f);

/**
 * \brief 二叉树先序遍历
 * \tparam O == Order::PreOrder
 * \tparam T 数据类型
 * \param root 二叉树节点
 * \param f 操作函数
 * \return void
 */
template<Order O, typename T>
typename std::enable_if<O == Order::PreOrder, void>::type
TreeTraversalIterative(std::unique_ptr<BinaryTree<T>>& root,
	typename BinaryTree<T>::fun&& f) {
	if (!root)return;
	std::stack<BinaryTree<T>*> s;
	s.push(root.get());
	while (!s.empty()) {
		auto cur = s.top();
		std::invoke(std::forward<typename BinaryTree<T>::fun&&>(f), cur->data);
		s.pop();
		if (cur->right)s.emplace(cur->right.get());
		if (cur->left)s.emplace(cur->left.get());
	}
}

/**
 * \brief 二叉树中序遍历
 * \tparam O == Order::InOrder
 * \tparam T 数据类型
 * \param root 二叉树节点
 * \param f 操作函数
 * \return void
 */
template<Order O, typename T>
typename std::enable_if<O == Order::InOrder, void>::type
TreeTraversalIterative(std::unique_ptr<BinaryTree<T>>& root,
	typename BinaryTree<T>::fun&& f) {
	if (!root)return;
	std::stack<BinaryTree<T>*> s;
	auto cur = root.get();
	while (!s.empty() || cur) {
		if (cur) {
			s.push(cur);
			cur = cur->left.get();
		}
		else {
			cur = s.top();
			s.pop();
			std::invoke(std::forward<typename BinaryTree<T>::fun&&>(f), cur->data);
			cur = cur->right.get();
		}
	}
}

/**
 * \brief 二叉树后序遍历
 * \tparam O == Order::PostOrder
 * \tparam T 数据类型
 * \param root 二叉树节点
 * \param f 操作函数
 * \return void
 */
template<Order O, typename T>
typename std::enable_if<O == Order::PostOrder, void>::type
TreeTraversalIterative(std::unique_ptr<BinaryTree<T>>& root,
	typename BinaryTree<T>::fun&& f) {
	if (!root)return;
	std::stack<BinaryTree<T>*> trv;
	std::stack<BinaryTree<T>*> out;
	trv.push(root.get());
	while (!trv.empty()) {
		auto cur = trv.top();
		trv.pop();
		if (cur->left)trv.push(cur->left.get());
		if (cur->right)trv.push(cur->right.get());
		out.push(cur);
	}
	while (!out.empty()) {
		std::invoke(std::forward<typename BinaryTree<T>::fun&&>(f), out.top()->data);
		out.pop();
	}
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
std::unique_ptr<BinaryTree<T>> MakeTree(std::unique_ptr<BinaryTree<T>>&& left, std::unique_ptr<BinaryTree<T>>&& right, T&& t) {
	return std::make_unique<BinaryTree<T>>(std::forward<std::unique_ptr<BinaryTree<T>>>(left), std::forward<std::unique_ptr<BinaryTree<T>>>(right), std::forward<T>(t));
}

/**
 * \brief 构造二叉树叶子节点
 * \tparam T 数据类型
 * \param t 数据
 * \return 构造完成的树
 */
template<typename T>
std::unique_ptr<BinaryTree<T>> MakeTree(T&& t) {
	return std::make_unique<BinaryTree<T>>(std::forward<T>(t));
}

template<Order O, int I, typename T>
typename std::enable_if   <(I == 0 && O == Order::PreOrder)
						|| (I == 1 && O == Order::InOrder)
						|| (I == 2 && O == Order::PostOrder), void>::type
TreeTraversalRecursiveImpl(std::unique_ptr<BinaryTree<T>> const& tree,
typename BinaryTree<T>::fun&& f){
	std::invoke(std::forward<typename BinaryTree<T>::fun&&>(f), tree->data);
}

template<Order O, int I, typename T>
typename std::enable_if   <(I == 1 && O == Order::PreOrder)
						|| (I == 0 && O == Order::InOrder)
						|| (I == 0 && O == Order::PostOrder), void>::type
TreeTraversalRecursiveImpl(std::unique_ptr<BinaryTree<T>> const& tree,
typename BinaryTree<T>::fun&& f){
	TreeTraversalRecursive<O>(tree->left, std::forward<typename BinaryTree<T>::fun&&>(f));
}

template<Order O, int I, typename T>
typename std::enable_if   <(I == 2 && O == Order::PreOrder)
						|| (I == 2 && O == Order::InOrder)
						|| (I == 1 && O == Order::PostOrder), void>::type
TreeTraversalRecursiveImpl(std::unique_ptr<BinaryTree<T>> const& tree,
typename BinaryTree<T>::fun&& f){
	TreeTraversalRecursive<O>(tree->right, std::forward<typename BinaryTree<T>::fun&&>(f));
}

template<Order O,typename T>
void TreeTraversalRecursive(std::unique_ptr<BinaryTree<T>> const& tree,
typename BinaryTree<T>::fun&& f){
	if (!tree)return;
	TreeTraversalRecursiveImpl<O, 0>(tree, std::forward<typename BinaryTree<T>::fun&&>(f));
	TreeTraversalRecursiveImpl<O, 1>(tree, std::forward<typename BinaryTree<T>::fun&&>(f));
	TreeTraversalRecursiveImpl<O, 2>(tree, std::forward<typename BinaryTree<T>::fun&&>(f));
}

#define BinaryTree_defined

#endif

#endif