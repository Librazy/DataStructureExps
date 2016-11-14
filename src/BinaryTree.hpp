#pragma once

#ifndef BinaryTree_disabled

#ifndef BinaryTree_defined

// ReSharper disable CppUnusedIncludeDirective
#include <memory>
#include <functional>
#include <stack>

template <typename T> struct identity
{
  using type = T;
};

template<typename T>
struct BinaryTree{
	BinaryTree(std::shared_ptr<BinaryTree<T>> l, std::shared_ptr<BinaryTree<T>> r, T&& d)
		:data(std::forward<T>(d)), left(l), right(r) {}
	BinaryTree(T&& d)
		:data(std::forward<T>(d)){}
	T data;
	std::shared_ptr<BinaryTree<T>> left;
	std::shared_ptr<BinaryTree<T>> right;
	using fun = typename identity<std::function<void(T&)>>::type;
};

enum class Order{
	PreOrder,
	InOrder,
	PostOrder,
};

template<Order O,typename T>
void TreeTraversalRecursive(std::shared_ptr<BinaryTree<T>> const& tree,
typename BinaryTree<T>::fun&& f);

template<Order O,typename T, int I>
typename std::enable_if   <(I == 0 && O == Order::PreOrder)
						|| (I == 1 && O == Order::InOrder)
						|| (I == 2 && O == Order::PostOrder), void>::type
TreeTraversalRecursiveImpl(std::shared_ptr<BinaryTree<T>> const& tree,
typename BinaryTree<T>::fun&& f){
	if (tree)std::invoke(std::forward<typename BinaryTree<T>::fun&&>(f), tree->data);
}

template<Order O,typename T, int I>
typename std::enable_if   <(I == 1 && O == Order::PreOrder)
						|| (I == 0 && O == Order::InOrder)
						|| (I == 0 && O == Order::PostOrder), void>::type
TreeTraversalRecursiveImpl(std::shared_ptr<BinaryTree<T>> const& tree,
typename BinaryTree<T>::fun&& f){
	if(tree)TreeTraversalRecursive<O>(tree->left, std::forward<typename BinaryTree<T>::fun&&>(f));
}

template<Order O,typename T, int I>
typename std::enable_if   <(I == 2 && O == Order::PreOrder)
						|| (I == 2 && O == Order::InOrder)
						|| (I == 1 && O == Order::PostOrder), void>::type
TreeTraversalRecursiveImpl(std::shared_ptr<BinaryTree<T>> const& tree,
typename BinaryTree<T>::fun&& f){
	if(tree)TreeTraversalRecursive<O>(tree->right, std::forward<typename BinaryTree<T>::fun&&>(f));
}

template<Order O,typename T>
void TreeTraversalRecursive(std::shared_ptr<BinaryTree<T>> const& tree,
typename BinaryTree<T>::fun&& f){
	TreeTraversalRecursiveImpl<O, T, 0>(tree, std::forward<typename BinaryTree<T>::fun&&>(f));
	TreeTraversalRecursiveImpl<O, T, 1>(tree, std::forward<typename BinaryTree<T>::fun&&>(f));
	TreeTraversalRecursiveImpl<O, T, 2>(tree, std::forward<typename BinaryTree<T>::fun&&>(f));
}

template<Order O,typename T>
typename std::enable_if<O == Order::PreOrder, void>::type
TreeTraversalIterative(std::shared_ptr<BinaryTree<T>> const& root,
typename BinaryTree<T>::fun&& f){
	if(!root)return;
	std::stack<std::shared_ptr<BinaryTree<T>>> s;
	s.emplace(root);
	while(!s.empty()){
		auto cur = s.top();
		std::invoke(std::forward<typename BinaryTree<T>::fun&&>(f), cur->data);
		s.pop();
		if(cur->right)s.emplace(cur->right);
		if(cur->left)s.emplace(cur->left);
	}
}

template<Order O,typename T>
typename std::enable_if<O == Order::InOrder, void>::type
TreeTraversalIterative(std::shared_ptr<BinaryTree<T>> const& root,
typename BinaryTree<T>::fun&& f){
	if(!root)return;
	std::stack<std::shared_ptr<BinaryTree<T>>> s;
	auto cur = root;
	while (!s.empty() || cur) {
		if (cur) {
			s.push(cur);
			cur = cur->left;
		} else {
			cur = s.top();
			s.pop();
			std::invoke(std::forward<typename BinaryTree<T>::fun&&>(f), cur->data);
			cur = cur->right;
		}
	}
}

template<Order O,typename T>
typename std::enable_if<O == Order::PostOrder, void>::type
TreeTraversalIterative(std::shared_ptr<BinaryTree<T>> const& root,
typename BinaryTree<T>::fun&& f){
	if(!root)return;
	std::stack<std::shared_ptr<BinaryTree<T>>> trv;
	std::stack<std::shared_ptr<BinaryTree<T>>> out;
	trv.push(root);
	while (!trv.empty()) {
		auto cur = trv.top();
		trv.pop();
		if (cur->left)trv.push(cur->left);
		if (cur->right)trv.push(cur->right);
		out.emplace(std::move(cur));
	}
	while (!out.empty()) {
		std::invoke(std::forward<typename BinaryTree<T>::fun&&>(f), out.top()->data);
		out.pop();
	}
}

template<typename T>
std::shared_ptr<BinaryTree<T>> MakeTree(std::shared_ptr<BinaryTree<T>> a, std::shared_ptr<BinaryTree<T>> b, T&& t){
  return std::make_shared<BinaryTree<T>>(a, b, std::forward<T>(t));
}

template<typename T>
std::shared_ptr<BinaryTree<T>> MakeTree(T&& t){
  return std::make_shared<BinaryTree<T>>(std::forward<T>(t));
}

#define BinaryTree_defined

#endif

#endif