#pragma once

#ifndef AVL_disabled

#ifndef AVL_defined

#include <cstdint>
#include <memory>
#include "BinaryTree.hpp"

template<typename T>
struct avl_data
{
	avl_data()
		: height(1), size(1), val(T()) {}

	template<typename K>
	explicit avl_data(K&& d)
		: height(1), size(1), val(std::forward<K>(d)) {}
	size_t height;

	size_t size;

	T val;

	T* operator->() {
		return &val;
	}
};

template<template<class...> class P, typename T>
struct ptr_maker
{
	template<typename N, typename K>
	P<N> make(K&& arg){
		return P<N>(new N(std::forward<K>(arg)));
	};
};

template<typename T>
struct ptr_maker<std::shared_ptr, T>
{
	template<typename N, typename K>
	std::shared_ptr<N> make(K&& arg) {
		return std::make_shared<N>(std::forward<K>(arg));
	};
};

template<typename T>
struct ptr_maker<std::unique_ptr, T>
{
	template<typename N, typename K>
	std::unique_ptr<N> make(K&& arg) {
		return std::make_unique<N>(std::forward<K>(arg));
	};
};


template<typename T, typename Compare = std::less<>,template<class...> class P = std::unique_ptr, typename Make = ptr_maker<P, T>>
class avl_tree{
	template<typename V>
	class avl_it;
	struct avl_node;
public:
	using bt_t = binary_tree<avl_data<T>, P>;
	using node_t = P<bt_t>;
	using key_type = T;
	using value_type = T;
	using size_type = size_t;
	using difference_type = int64_t;
	using key_compare = Compare;
	using value_compare = Compare;
	using node_make = Make;
	using reference = T&;
	using const_reference = T const&;
	using iterator = avl_it<const T>;
	using const_iterator = avl_it<const T>;

private:


	struct avl_node : bt_t
	{
		virtual ~avl_node() {}

		avl_node() : bt_t(avl_data<T>())
		{ };

		template<typename K>
		explicit avl_node(K&& val) : bt_t(avl_data<T>(std::forward<K>(val)))
		{ };

		avl_data<T>* operator->() override {
			return &(this->data);
		}
	};

	template<typename V>
	class avl_it
	{
		size_type current;
		avl_tree* pt;

		avl_it(size_type current, avl_tree* pt) :current(current), pt(pt)
		{ }

	public:
		friend class avl_tree;

		avl_it() :current(0), pt(nullptr)
		{ }

		avl_it operator++() {
			++current;
			return *this;
		}

		avl_it operator++(int) {
			auto res = avl_it(*this);
			++current;
			return res;
		}

		avl_it operator--() {
			--current;
			return *this;
		}

		avl_it operator--(int) {
			auto res = avl_it(*this);
			--current;
			return res;
		}

		avl_it& operator+=(difference_type i) {
			current += i;
			return this;
		}

		avl_it& operator-=(difference_type i) {
			current -= i;
			return this;
		}

		avl_it operator+(difference_type i) {
			auto res = avl_it(*this);
			res.current += i;
			return res;
		}

		avl_it operator-(difference_type i) {
			auto res = avl_it(*this);
			res.current -= i;
			return res;
		}

		difference_type operator-(avl_it const& i) {
			return current - i.current;
		}

		friend avl_it operator+(difference_type i, avl_it const& a);

		bool operator==(avl_it const& a) const {
			return a.current == current && a.pt == pt;
		}

		bool operator<(avl_it const& a) const {
			return a.current < current;
		}

		bool operator>(avl_it const& a) const {
			return a.current > current;
		}

		bool operator<=(avl_it const& a) const {
			return a.current <= current;
		}

		bool operator>=(avl_it const& a) const {
			return a.current >= current;
		}

		bool operator!=(avl_it const& a) const {
			return !(a == *this);
		}

		V& operator*() {
			return (*pt).nth(current);
		}

		V* operator->() {
			return &((*pt).nth(current));
		}

		V& operator[](size_type s)
		{
			return &((*pt).nth(current + s));
		}
	};

	static key_compare comp;

	static node_make maker;

	node_t root;

	static size_t get_height(node_t const& cur)
	{
		if(cur) {
			return cur->data.height;
		}
		return 0;
	}

	static size_t get_size(node_t const& cur)
	{
		if (cur) {
			return cur->data.size;
		}
		return 0;
	}

	static void check_rotate(node_t& cur)
	{
		difference_type det = get_height(cur->left) - get_height(cur->right);
		if (det >= 2) {
			difference_type detl = get_height(cur->left->left) - get_height(cur->left->right);

			if (detl > 0) {
				rotate_ll(cur);
			}
			else {
				rotate_lr(cur);
			}
		}
		else if (det <= -2) {
			difference_type detr = get_height(cur->right->right) - get_height(cur->right->left);

			if (detr > 0) {
				rotate_rr(cur);
			}
			else {
				rotate_rl(cur);
			}
		}
	}

	static void rotate_ll(node_t& cur)
	{
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

		maintain_node(cur->right);
	}

	static void rotate_lr(node_t& cur)
	{
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

		maintain_node(cur->right);
		maintain_node(cur->left);
	}

	static void rotate_rr(node_t& cur)
	{
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

		maintain_node(cur->left);
	}

	static void rotate_rl(node_t& cur)
	{
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

		maintain_node(cur->right);
		maintain_node(cur->left);
	}

	static void maintain_node(node_t& cur)
	{
		maintain_node(cur.get());
	}

	static void maintain_node(bt_t* cur)
	{
		auto ls = cur->left ? cur->left->data.size: 0;
		auto rs = cur->right ? cur->right->data.size : 0;
		auto lh = cur->left ? cur->left->data.height : 0;
		auto rh = cur->right ? cur->right->data.height : 0;
		cur->data.height = std::max(lh, rh) + 1;
		cur->data.size = ls + rs + 1;
	}

	template <typename K>
	static T& search_impl(K&& t, node_t const& cur, size_type& s)
	{
		if (!cur) {
			throw std::out_of_range("");
		}

		auto cnt = comp(cur->data.val, t);
		auto ctn = comp(t, cur->data.val);

		if (!cnt && !ctn) {
			s += get_size(cur->left);
			return cur->data.val;
		}
		if (ctn) {
			return search_impl(t, cur->left, s);
		}
		s += get_size(cur->left) + 1;
		return search_impl(t, cur->right, s);
	}

	static T& nth_impl(size_type s, node_t const& cur)
	{
		if (s > get_size(cur)) {
			throw std::out_of_range("");
		}
		difference_type det = s - get_size(cur->left) - 1;
		if (!det) {
			return cur->data.val;
		}
		if (det < 0) {
			return nth_impl(s, cur->left);
		}
		return nth_impl(det, cur->right);
	}

	template <typename K>
	static bool insert_impl(K&& t, node_t& cur) noexcept
	{
		if(!cur) {
			cur = maker.template make<avl_node>(std::forward<K>(t));
			return true;
		}

		auto cnt = comp(cur->data.val, t);
		auto ctn = comp(t, cur->data.val);

		if(!cnt && !ctn) {
			return false;
		}

		if(ctn) {
			if(insert_impl(std::forward<K>(t), cur->left)) {
				++cur->data.size;
				cur->data.height = std::max(get_height(cur->left), get_height(cur->right)) + 1;
			}
			else return false;
		} else {
			if (insert_impl(std::forward<K>(t), cur->right)) {
				++cur->data.size;
				cur->data.height = std::max(get_height(cur->left), get_height(cur->right)) + 1;
			}
			else return false;
		}
		check_rotate(cur);
		maintain_node(cur);
		return true;
	}

	static node_t extract_max(node_t& cur)
	{
		auto res = node_t();
		auto& lt = cur->left;
		auto lmax = lt.get();
		auto st = std::stack<decltype(cur.get())>();
		st.push(cur.get());
		while (lmax->right) { st.push(lmax); lmax = lmax->right.get();  }
		auto ml = node_t();
		std::swap(lmax->left, ml);
		if (st.size() == 1) {
			std::swap(st.top()->left, res);
			std::swap(ml, st.top()->left);
		}
		else {
			std::swap(st.top()->right, res);
			std::swap(ml, st.top()->right);
		}
		while (!st.empty()) {
			maintain_node(st.top());
			st.pop();
		}
		return res;
	}

	static node_t extract_min(node_t& cur)
	{
		auto res = node_t();
		auto& rt = cur->right;
		auto lmin = rt.get();
		auto st = std::stack<decltype(cur.get())>();
		st.push(cur.get());
		while (lmin->left) { st.push(lmin); lmin = lmin->left.get(); }
		auto mr = node_t();
		std::swap(lmin->right, mr);
		if(st.size()==1) {
			std::swap(st.top()->right, res);
			std::swap(mr, st.top()->right);
		} else{
			std::swap(st.top()->left, res);
			std::swap(mr, st.top()->left);
		}
		while (!st.empty()) {
			maintain_node(st.top());
			st.pop();
		}
		return res;
	}

	static bool remove_impl(T&& t, node_t& cur)
	{
		if (!cur) {
			return false;
		}

		auto cnt = comp(cur->data.val, t);
		auto ctn = comp(t, cur->data.val);

		if (!cnt && !ctn) {
			if(!cur->left && !cur->right) {
				cur = node_t();
			}
			else if(get_height(cur->left) > get_height(cur->right)) {
				auto max = extract_max(cur);
				std::swap(cur->data, max->data);
				maintain_node(cur);
			} else {
				auto min = extract_min(cur);
				std::swap(cur->data, min->data);
				maintain_node(cur);
			}
			return true;
		}

		if (ctn) {
			if (!remove_impl(std::forward<T>(t), cur->left)) return false;
		}
		else {
			if (!remove_impl(std::forward<T>(t), cur->right)) return false;
		}
		check_rotate(cur);
		maintain_node(cur);
		return true;
	}

public:

	avl_tree()
	{ }

	explicit avl_tree(T&& t) : root(maker.template make<avl_node>(std::forward<T>(t)))
	{ }

	template <typename K>
	bool insert(K&& t) noexcept {
		return insert_impl(std::forward<K>(t), root);
	}

	template <typename K>
	const_reference search(K&& t) const
	{
		size_type ig;
		return search_impl(std::forward<K>(t), root, ig);
	}

	template <typename K, typename ST = T>
	typename ST::second_type& search_pair_key(K&& t) const
	{
		size_type ig;
		auto& ret = search_impl(std::forward<K>(t), root, ig);
		return ret.second;
	}

	template <typename K>
	size_type rank(K&& t) const
	{
		size_type res = 0;
		search_impl(std::forward<K>(t), root, res);
		return res;
	}

	template <typename K>
	iterator find(K&& t) const noexcept
	{
		size_type ig;
		try {
			search_impl(std::forward<K>(t), root, ig);
			return iterator(ig, this);
		}catch(std::out_of_range) {
			return iterator(size(), this);
		}
	}

	template <typename K>
	bool remove(K&& t) noexcept
	{
		return remove_impl(std::forward<K>(t), root);
	}

	const_reference nth(size_type s) const
	{
		return nth_impl(s + 1, root);
	}

	size_type size() const noexcept
	{
		return get_size(root);
	}

	size_type height() const noexcept
	{
		return get_height(root);
	}

	bool empty() const noexcept
	{
		return root;
	}

	void swap(avl_tree another) noexcept
	{
		std::swap(root, another.root);
	}

	iterator begin() & noexcept
	{
		return iterator(0, this);
	}

	iterator end() & noexcept
	{
		return iterator(size(), this);
	}

	iterator begin() const& noexcept
	{
		return cbegin();
	}

	iterator end() const& noexcept
	{
		return cend();
	}

	const_iterator cbegin() const& noexcept
	{
		return const_iterator(0, this);
	}

	const_iterator cend() const& noexcept
	{
		return const_iterator(size(), this);
	}

	template<Order O, typename F>
	void traversal_recursive(F&& f) const
	{
		if(root) {
			root->template traversal_recursive<O>([&f](avl_data<T> const& data)
			{
				std::invoke(std::forward<F>(f), data.val);
			});
		}
	}
};

template<typename T, typename Compare,template<class...> class P, typename Make>
Compare avl_tree<T, Compare, P, Make>::comp = key_compare();

template<typename T, typename Compare,template<class...> class P, typename Make>
Make avl_tree<T, Compare, P, Make>::maker = node_make();

template<typename T, typename Compare,template<class...> class P, typename Make, typename V>
typename avl_tree<T, Compare, P, Make>::avl_it<V> operator+(typename avl_tree<T, Compare, P, Make>::difference_type i, typename avl_tree<T, Compare, P, Make>::avl_it<V> const& a) {
	auto res = avl_it(a);
	res.current += i;
	return res;
}

#define AVL_defined

#endif

#endif