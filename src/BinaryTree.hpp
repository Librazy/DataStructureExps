#pragma once

#ifndef BinaryTree_disabled

#ifndef BinaryTree_defined

// ReSharper disable CppUnusedIncludeDirective
#include <memory>
#include <functional>

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
void VisitTreeRecurse(std::shared_ptr<BinaryTree<T>> tree,
typename BinaryTree<T>::fun&& f);

template<Order O,typename T, int I>
typename std::enable_if   <(I == 0 && O == Order::PreOrder)
						|| (I == 1 && O == Order::InOrder)
						|| (I == 2 && O == Order::PostOrder), void>::type
VisitTreeRecurseImpl(std::shared_ptr<BinaryTree<T>> tree,
typename BinaryTree<T>::fun&& f){
	if (tree)std::invoke(std::forward<typename BinaryTree<T>::fun&&>(f), tree->data);
}

template<Order O,typename T, int I>
typename std::enable_if   <(I == 1 && O == Order::PreOrder)
						|| (I == 0 && O == Order::InOrder)
						|| (I == 0 && O == Order::PostOrder), void>::type
VisitTreeRecurseImpl(std::shared_ptr<BinaryTree<T>> tree,
typename BinaryTree<T>::fun&& f){
	if(tree)VisitTreeRecurse<O>(tree->left, std::forward<typename BinaryTree<T>::fun&&>(f));
}

template<Order O,typename T, int I>
typename std::enable_if   <(I == 2 && O == Order::PreOrder)
						|| (I == 2 && O == Order::InOrder)
						|| (I == 1 && O == Order::PostOrder), void>::type
VisitTreeRecurseImpl(std::shared_ptr<BinaryTree<T>> tree,
typename BinaryTree<T>::fun&& f){
	if(tree)VisitTreeRecurse<O>(tree->right, std::forward<typename BinaryTree<T>::fun&&>(f));
}

template<Order O,typename T>
void VisitTreeRecurse(std::shared_ptr<BinaryTree<T>> tree,
typename BinaryTree<T>::fun&& f){
	VisitTreeRecurseImpl<O, T, 0>(tree, std::forward<typename BinaryTree<T>::fun&&>(f));
	VisitTreeRecurseImpl<O, T, 1>(tree, std::forward<typename BinaryTree<T>::fun&&>(f));
	VisitTreeRecurseImpl<O, T, 2>(tree, std::forward<typename BinaryTree<T>::fun&&>(f));
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