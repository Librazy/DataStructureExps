#pragma once

#ifndef AVL_disabled

#ifndef AVL_defined

#include <cstdint>
#include <memory>
#include "BinaryTree.hpp"

template<template<class...> class P, typename T>
struct ptr_maker
{
	template<typename N>
	P<N> make(T&& arg){
		return P<N>(new N(std::forward<T>(arg)));
	};
};

template<typename T>
struct ptr_maker<std::shared_ptr, T>
{
	template<typename N>
	std::shared_ptr<N> make(T&& arg) {
		return std::make_shared<N>(std::forward<T>(arg));
	};
};

template<typename T>
struct ptr_maker<std::unique_ptr, T>
{
	template<typename N>
	std::unique_ptr<N> make(T&& arg) {
		return std::make_unique<N>(std::forward<T>(arg));
	};
};

template<typename T, typename Compare = std::less<>,template<class...> class P = std::shared_ptr, typename Make = ptr_maker<P, T>>
class avl_tree{
public:
	using bt_t = binary_tree<T, P>;

	class avl_node : public bt_t
	{

	public:
		virtual ~avl_node() {}

		avl_node() : bt_t(T()), height(1), size(1)
		{ };

		explicit avl_node(T&& val) : bt_t(std::move(val)), height(1), size(1)
		{ };

		P<avl_node> left;

		P<avl_node> right;

		size_t height;

		size_t size;

		void maintain()
		{
			binary_tree<T, P>::left = left;
			binary_tree<T, P>::right = right;
			height = std::max(this->left ? this->left->height : 0, this->right ? this->right->height : 0) + 1;
			size = (this->left ? this->left->size : 0) + (this->right ? this->right->size : 0) + 1;
		}
	};

	using node_t = P<avl_node>;

private:

	static Compare comp;

	static Make maker;

	node_t root;

	static size_t get_height(node_t& cur)
	{
		if(cur) {
			return cur->height;
		}
		return 0;
	}

	static bool insert_impl(T&& t, node_t& cur)
	{
		if(!cur) {
			cur = maker.template make<avl_node>(std::forward<T>(t));
			return true;
		}

		auto cnt = comp(cur->data, t);
		auto ctn = comp(t, cur->data);

		if(!cnt && !ctn) {
			return false;
		}

		if(ctn) {
			if(insert_impl(std::forward<T>(t), cur->left)) {
				++cur->size;
				cur->height = std::max(get_height(cur->left), get_height(cur->right)) + 1;
			}
			else return false;
		} else {
			if (insert_impl(std::forward<T>(t), cur->right)) {
				++cur->size;
				cur->height = std::max(get_height(cur->left), get_height(cur->right)) + 1;
			}
			else return false;
		}
		
		int64_t det = get_height(cur->left) - get_height(cur->right);
		if(det >= 2) {
			auto detl = get_height(cur->left->left) - get_height(cur->left->right);

			if(detl > 0) {
				//LL
				/*          A
				 *         / \
				 *        /   \
				 *       B    .Ar
				 *      / \
				 *     /   \
				 *   .Bl    Br
				 */

				/*          B
				 *         / \
				 *        /   \
				 *      .Bl    A
				 *            / \
				 *           /   \
				 *          Br   .Ar
				 */
				node_t A, B, Br;

				std::swap(Br, cur->left->right);
				std::swap(B, cur->left);
				std::swap(A, cur);

				std::swap(cur, B);
				std::swap(cur->right, A);
				std::swap(cur->right->left, Br);

				cur->right->maintain();
			} else {
				//LR
				/*          A
				 *         / \
				 *        /   \
				 *       B    .Ar
				 *      / \
				 *     /   \
				 *   .Bl    C
				 *         / \
				 *        /   \
				 *       Cl    Cr
				 */

				/*          C
				 *         / \
				 *        /   \
				 *       /     \
				 *      /       \
				 *     B         A
				 *    / \       / \
				 *   /   \     /   \
				 * .Bl    Cl  Cr   .Ar
				 */
				node_t A, B, C, Cl, Cr;

				std::swap(Cl, cur->left->right->left);
				std::swap(Cr, cur->left->right->right);
				std::swap(C, cur->left->right);
				std::swap(B, cur->left);
				std::swap(A, cur);

				std::swap(cur, C);
				std::swap(cur->right, A);
				std::swap(cur->right->left, Cr);
				std::swap(cur->left, B);
				std::swap(cur->left->right, Cl);

				cur->right->maintain();
				cur->left->maintain();
			}
		}else if(det <= -2) {
			auto detr = get_height(cur->right->right) - get_height(cur->right->left);

			if (detr > 0) {
				//RR
				/*          A
				 *         / \
				 *        /   \
				 *      .Al    B
				 *            / \
				 *           /   \
				 *          Bl   .Br
				 */

				/*          B
				 *         / \
				 *        /   \
				 *       A    .Br
				 *      / \
				 *     /   \
				 *   .Al    Bl
				 */
				node_t A, B, Bl;

				std::swap(Bl, cur->right->left);
				std::swap(B, cur->right);
				std::swap(A, cur);

				std::swap(cur, B);
				std::swap(cur->left, A);
				std::swap(cur->left->right, Bl);

				cur->left->maintain();
			} else {
				//RL
				/*          A
				 *         / \
				 *        /   \
				 *      .Al    B
				 *            / \
				 *           /   \
				 *          C    .Br
				 *         / \
				 *        /   \
				 *       Cl    Cr
				 */

				/*          C
				 *         / \
				 *        /   \
				 *       /     \
				 *      /       \
				 *     A         B
				 *    / \       / \
				 *   /   \     /   \
				 * .Al    Cl  Cr   .Br
				 */
				node_t A, B, C, Cl, Cr;

				std::swap(Cl, cur->right->left->left);
				std::swap(Cr, cur->right->left->right);
				std::swap(C, cur->right->left);
				std::swap(B, cur->right);
				std::swap(A, cur);

				std::swap(cur, C);
				std::swap(cur->left, A);
				std::swap(cur->left->right, Cl);
				std::swap(cur->right, B);
				std::swap(cur->right->left, Cr);

				cur->right->maintain();
				cur->left->maintain();
			}
		}
		cur->maintain();
		static_cast<binary_tree<T, P>>(*cur).right = cur->right;
		static_cast<binary_tree<T, P>>(*cur).left = cur->left;
		return true;
	}

public:

	avl_tree()
	{ }

	explicit avl_tree(T&& t) : root(maker.template make<avl_node>(std::forward<T>(t)))
	{ }

	bool insert(T&& t){
		return insert_impl(std::forward<T>(t), root);
	}

	template<Order O, typename F>
	void traversal_recursive(F&& f)
	{
		if(root) {
			root->template traversal_recursive<O>(std::forward<F>(f));
		}
	}
};

template<typename T, typename Compare,template<class...> class P, typename Make>
Compare avl_tree<T, Compare, P, Make>::comp = Compare();

template<typename T, typename Compare,template<class...> class P, typename Make>
Make avl_tree<T, Compare, P, Make>::maker = Make();

#define AVL_defined

#endif

#endif