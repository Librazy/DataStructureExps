#pragma once

#ifndef AVL_disabled

#ifndef AVL_defined

// ReSharper disable CppUnusedIncludeDirective
#include <memory>
#include <cstddef>
#include "BinaryTree.hpp"

/**
 * \brief 节点数据类型
 * \tparam T 存储类型
 */
template<typename T>
struct avl_data
{
	/**
	 * \brief 默认构造
	 */
	avl_data()
		: height(1), size(1), val(T()) {}

	/**
	 * \brief 使用给定数据构造
	 * \tparam K 传入数据类型
	 * \param d 传入数据
	 */
	template<typename K>
	explicit avl_data(K&& d)
		: height(1), size(1), val(std::forward<K>(d)) {}

	/**
	 * \brief 子树高
	 */
	size_t height;

	/**
	 * \brief 子树大小
	 */
	size_t size;

	/**
	 * \brief 数据
	 */
	T val;
};

/**
 * \brief 节点构造器
 * \tparam P 包装类型
 */
template<template<class...> class P>
struct ptr_maker
{
	/**
	 * \brief 构造给定类型节点
	 * \tparam N 节点类型
	 * \tparam K 传入数据类型
	 * \param arg 传入数据
	 * \return 节点
	 */
	template<typename N, typename K>
	P<N> make(K&& arg){
		return P<N>(new N(std::forward<K>(arg)));
	};
};

/**
 * \brief std::shared_ptr节点构造器
 */
template<>
struct ptr_maker<std::shared_ptr>
{
	/**
	 * \brief 构造给定std::shared_ptr类型节点
	 * \tparam N 节点类型
	 * \tparam K 传入数据类型
	 * \param arg 传入数据
	 * \return 节点
	 */
	template<typename N, typename K>
	std::shared_ptr<N> make(K&& arg) {
		return std::make_shared<N>(std::forward<K>(arg));
	};
};

/**
 * \brief std::unique_ptr节点构造器
 */
template<>
struct ptr_maker<std::unique_ptr>
{
	/**
	 * \brief 构造给定std::unique_ptr类型节点
	 * \tparam N 节点类型
	 * \tparam K 传入数据类型
	 * \param arg 传入数据
	 * \return 节点
	 */
	template<typename N, typename K>
	std::unique_ptr<N> make(K&& arg) {
		return std::make_unique<N>(std::forward<K>(arg));
	};
};


/**
 * \brief AVL树
 * \tparam T 存储类型
 * \tparam Compare 比较器类型
 * \tparam P 包装类型
 * \tparam Make 节点构造器类型
 */
template<typename T, typename Compare = std::less<>,template<class...> class P = std::unique_ptr, typename Make = ptr_maker<P>>
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
	using difference_type = ptrdiff_t;
	using key_compare = Compare;
	using value_compare = Compare;
	using node_make = Make;
	using reference = T&;
	using const_reference = T const&;
	using iterator = avl_it<const T>;
	using const_iterator = avl_it<const T>;

private:


	/**
	 * \brief 节点类型
	 */
	struct avl_node : bt_t
	{
		/**
		 * \brief 虚析构
		 */
		virtual ~avl_node() {}

		/**
		 * \brief 默认构造
		 */
		avl_node() : bt_t(avl_data<T>())
		{ };

		/**
		 * \brief 使用给定数据构造
		 * \tparam K 传入数据类型
		 * \param val 传入数据
		 */
		template<typename K>
		explicit avl_node(K&& val) : bt_t(avl_data<T>(std::forward<K>(val)))
		{ };
	};

	/**
	 * \brief 迭代器类型
	 * \tparam V 解引用类型
	 */
	template<typename V>
	class avl_it
	{
		/**
		 * \brief 当前数据索引
		 */
		size_type current;


		/**
		 * \brief 所属容器指针
		 */
		avl_tree* pt;

		/**
		 * \brief 私有构造
		 * \param current 数据索引
		 * \param pt 所属容器指针
		 */
		avl_it(size_type current, avl_tree* pt) :current(current), pt(pt)
		{ }

	public:
		friend class avl_tree;

		using iterator_category = std::random_access_iterator_tag;
		using value_type = V;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using reference = V&;
		using const_reference = V const&;
		using pointer = V*;

		/**
		 * \brief 默认构造
		 */
		avl_it() :current(0), pt(nullptr)
		{ }

		/**
		 * \brief 偏移量+迭代器
		 * \param i 偏移量
		 * \param a 迭代器
		 * \return 目标迭代器
		 */
		friend iterator operator+(difference_type i, iterator const& a) {
			auto res = avl_it(a);
			res.current += i;
			return res;
		}

		/**
		 * \brief 迭代器前自增
		 * \return 自增后迭代器
		 */
		avl_it operator++() {
			++current;
			return *this;
		}

		/**
		 * \brief 迭代器前自增
		 * \return 自增前迭代器
		 */
		avl_it operator++(int) {
			auto res = avl_it(*this);
			++current;
			return res;
		}

		/**
		 * \brief 迭代器前自增
		 * \return 自增后迭代器
		 */
		avl_it operator--() {
			--current;
			return *this;
		}

		/**
		 * \brief 迭代器后自减
		 * \return 自减前迭代器
		 */
		avl_it operator--(int) {
			auto res = avl_it(*this);
			--current;
			return res;
		}

		/**
		 * \brief 迭代器+=偏移量
		 * \param i 偏移量
		 * \return 目标迭代器
		 */
		avl_it& operator+=(difference_type i) {
			current += i;
			return this;
		}

		/**
		 * \brief 迭代器-=偏移量
		 * \param i 偏移量
		 * \return 目标迭代器
		 */
		avl_it& operator-=(difference_type i) {
			current -= i;
			return this;
		}

		/**
		 * \brief 迭代器-偏移量
		 * \param i 偏移量
		 * \return 目标迭代器
		 */
		avl_it operator-(difference_type i) {
			auto res = avl_it(*this);
			res.current -= i;
			return res;
		}

		/**
		 * \brief 迭代器-目标迭代器
		 * \param i 目标迭代器
		 * \return 偏移量
		 */
		difference_type operator-(avl_it const& i) {
			return current - i.current;
		}

		/**
		 * \brief 迭代器+偏移量
		 * \param i 偏移量
		 * \return 目标迭代器
		 */
		avl_it operator+(difference_type i) {
			return i + *this;
		}

		/**
		 * \brief 迭代器相等
		 * \param a 目标迭代器
		 * \return ==
		 */
		bool operator==(avl_it const& a) const {
			return current == a.current;
		}

		/**
		 * \brief 迭代器小于
		 * \param a 目标迭代器
		 * \return <
		 */
		bool operator<(avl_it const& a) const {
			return current < a.current;
		}

		/**
		 * \brief 迭代器大于
		 * \param a 目标迭代器
		 * \return >
		 */
		bool operator>(avl_it const& a) const {
			return current > a.current;
		}
		
		/**
		 * \brief 迭代器小于等于
		 * \param a 目标迭代器
		 * \return <=
		 */
		bool operator<=(avl_it const& a) const {
			return current <= a.current;
		}

		/**
		 * \brief 迭代器大于等于
		 * \param a 目标迭代器
		 * \return >=
		 */
		bool operator>=(avl_it const& a) const {
			return current >= a.current;
		}

		/**
		 * \brief 迭代器不等于
		 * \param a 目标迭代器
		 * \return !=
		 */
		bool operator!=(avl_it const& a) const {
			return current != a.current;
		}

		/**
		 * \brief **O(log n) **解引用
		 * \return 指向的元素的引用
		 */
		reference operator*() {
			return (*pt).nth(current);
		}

		/**
		 * \brief **O(log n) **解指针
		 * \return 指向的元素的指针
		 */
		pointer operator->() {
			return &((*pt).nth(current));
		}
		/**
		 * \brief **O(log n) **解引用偏移量
		 * \return 指向的给定偏移量元素的引用
		 */
		reference operator[](size_type s)
		{
			return &((*pt).nth(current + s));
		}
	};

	/**
	 * \brief 比较器
	 */
	key_compare comp;

	/**
	 * \brief 节点构造器
	 */
	node_make maker;

	/**
	 * \brief 根节点
	 */
	node_t root;

	/**
	 * \brief 获取子树高
	 * \param cur 给定节点
	 * \return 子树高
	 */
	static size_type get_height(node_t const& cur)
	{
		if(cur) {
			return cur->data.height;
		}
		return 0;
	}

	/**
	 * \brief 获取子树大小
	 * \param cur 给定节点
	 * \return 子树大小
	 */
	static size_type get_size(node_t const& cur)
	{
		if (cur) {
			return cur->data.size;
		}
		return 0;
	}

	/**
	 * \brief 判断子树是否需要旋转
	 * \param cur 子树
	 */
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

	/**
	 * \brief LL旋转
	 * \param cur 子树
	 */
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

	/**
	 * \brief LR旋转
	 * \param cur 子树
	 */
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

	/**
	 * \brief RR旋转
	 * \param cur 子树
	 */
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

	/**
	 * \brief RL旋转
	 * \param cur 子树
	 */
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

	/**
	 * \brief 维护节点数据
	 * \param cur 节点
	 */
	static void maintain_node(node_t& cur)
	{
		maintain_node(cur.get());
	}

	/**
	 * \brief 维护节点数据
	 * \param cur 节点裸指针
	 */
	static void maintain_node(bt_t* cur)
	{
		auto ls = cur->left ? cur->left->data.size: 0;
		auto rs = cur->right ? cur->right->data.size : 0;
		auto lh = cur->left ? cur->left->data.height : 0;
		auto rh = cur->right ? cur->right->data.height : 0;
		cur->data.height = std::max(lh, rh) + 1;
		cur->data.size = ls + rs + 1;
	}

	/**
	 * \brief 搜索实现
	 * \tparam K 传入查询类型
	 * \param t 查询数据
	 * \param cur 当前节点
	 * \param s 输出当前节点rank
	 * \return 目标存储引用
	 */
	template <typename K>
	T& search_impl(K&& t, node_t const& cur, size_type& s) const
	{
		if (!cur) {
			throw std::out_of_range("not found");
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

	/**
	 * \brief 查询指定rank元素实现
	 * \param s 查询rank
	 * \param cur 当前节点
	 * \return 目标存储引用
	 */
	static T& nth_impl(size_type s, node_t const& cur)
	{
		if (s > get_size(cur)) {
			throw std::out_of_range("too large");
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

	/**
	 * \brief 插入实现
	 * \tparam K 传入存储类型
	 * \param t 待插入存储
	 * \param cur 当前节点
	 * \return 是否插入
	 */
	template <typename K>
	bool insert_impl(K&& t, node_t& cur) noexcept
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

	/**
	 * \brief 弹出目标节点中序前驱（左子树存在）
	 * \param cur 目标节点
	 * \return 弹出的节点
	 */
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

	/**
	 * \brief 弹出目标节点中序后继（右子树存在）
	 * \param cur 目标节点
	 * \return 弹出的节点
	 */
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

	/**
	 * \brief 删除实现
	 * \tparam K 传入查询类型
	 * \param t 查询数据
	 * \param cur 当前节点
	 * \return 是否删除
	 */
	template <typename K>
	bool remove_impl(K&& t, node_t& cur)
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

	/**
	 * \brief 默认构造
	 */
	avl_tree() noexcept : comp(key_compare()), maker(node_make())
	{ }

	/**
	 * \brief 使用给定存储初始化根节点
	 * \param t 根节点存储
	 */
	explicit avl_tree(T&& t) noexcept : comp(key_compare()), maker(node_make())
	{ }

	/**
	 * \brief 使用给定比较器
	 * \param t 比较器
	 */
	explicit avl_tree(key_compare const& t) noexcept : comp(t), maker(node_make())
	{ }

	/**
	 * \brief 使用给定节点构造器
	 * \param t 构造器
	 */
	explicit avl_tree(node_make const& t) noexcept : comp(key_compare()), maker(t)
	{ }

	/**
	 * \brief 使用给定比较器与节点构造器
	 * \param c 比较器
	 * \param m 构造器
	 */
	avl_tree(key_compare const& c, node_make const& m) noexcept : comp(c), maker(m)
	{ }

	/**
	 * \brief 使用初始化列表
	 * \param il 初始化列表
	 */
	avl_tree(std::initializer_list<value_type> il) : comp(key_compare()), maker(node_make())
	{
		std::for_each(il.begin(), il.end(), [&](auto& ele)
		{
			insert(ele);
		});
	}

	/**
	 * \brief 使用迭代器范围
	 * \tparam It 输入迭代器类型
	 * \param b 头迭代器
	 * \param e 尾迭代器
	 */
	template<typename It>
	avl_tree(It b, It e) : comp(key_compare()), maker(node_make())
	{
		std::for_each(b, e, [&](auto& ele)
		{
			insert(ele);
		});
	}

	/**
	 * \brief 使用给定比较器与迭代器范围
	 * \tparam It 输入迭代器类型
	 * \param b 头迭代器
	 * \param e 尾迭代器
	 * \param t 比较器
	 */
	template<typename It>
	avl_tree(It b, It e, key_compare const& t) : comp(t), maker(node_make())
	{
		std::for_each(b, e, [&](auto& ele)
		{
			insert(ele);
		});
	}

	/**
	 * \brief 使用给定节点构造器与迭代器范围
	 * \tparam It 输入迭代器类型
	 * \param b 头迭代器
	 * \param e 尾迭代器
	 * \param t 构造器
	 */
	template<typename It>
	avl_tree(It b, It e, node_make const& t) : comp(key_compare()), maker(t)
	{
		std::for_each(b, e, [&](auto& ele)
		{
			insert(ele);
		});
	}

	/**
	 * \brief 使用给定比较器与节点构造器与迭代器范围
	 * \tparam It 输入迭代器类型
	 * \param b 头迭代器
	 * \param e 尾迭代器
	 * \param c 比较器
	 * \param m 构造器
	 */
	template<typename It>
	avl_tree(It b, It e, key_compare const& c, node_make const& m) : comp(c), maker(m)
	{
		std::for_each(b, e, [&](auto& ele)
		{
			insert(ele);
		});
	}

	/**
	 * \brief 移动构造
	 * \param a 目标AVL
	 */
	avl_tree(avl_tree&& a) noexcept
	{
		swap(a);
	}

	/**
	 * \brief 移动赋值
	 * \param a 目标AVL
	 */
	avl_tree& operator=(avl_tree&& a) noexcept
	{
		swap(a);
		return *this;
	}

	/**
	 * \brief 初始化列表赋值
	 * \param il 初始化列表
	 */
	avl_tree& operator=(std::initializer_list<value_type> il) noexcept
	{
		avl_tree tmp(il);
		swap(tmp);
		return *this;
	}

	/**
	 * \brief 获取key_compare实例
	 * \return key_compare实例
	 */
	key_compare key_comp() const noexcept
	{
		return comp;
	}

	/**
	 * \brief 获取value_compare实例
	 * \return value_compare实例
	 */
	value_compare value_comp() const noexcept
	{
		return comp;
	}

	/**
	 * \brief 插入
	 * \tparam K 传入存储类型
	 * \param t 待插入存储
	 * \return 是否插入
	 */
	template <typename K>
	bool insert(K&& t) noexcept {
		return insert_impl(std::forward<K>(t), root);
	}

	/**
	 * \brief 搜索
	 * \tparam K 传入查询类型
	 * \param t 查询数据
	 * \return 目标存储只读引用
	 */
	template <typename K>
	const_reference search(K&& t) const
	{
		size_type ig;
		return search_impl(std::forward<K>(t), root, ig);
	}

	/**
	 * \brief 根据key搜索pair
	 * \tparam K 传入查询类型
	 * \tparam ST = T
	 * \param t 查询数据
	 * \return 目标pair的第二个元素的引用
	 */
	template <typename K, typename ST = T>
	typename ST::second_type& search_pair_key(K&& t) const
	{
		size_type ig;
		auto& ret = search_impl(std::forward<K>(t), root, ig);
		return std::get<1>(ret);
	}

	/**
	 * \brief 查询rank
	 * \tparam K 传入查询类型
	 * \param t 查询数据
	 * \return 目标存储rank
	 */
	template <typename K>
	size_type rank(K&& t) const
	{
		size_type res = 0;
		search_impl(std::forward<K>(t), root, res);
		return res;
	}

	/**
	 * \brief 查询迭代器
	 * \tparam K 传入查询类型
	 * \param t 查询数据
	 * \return 目标迭代器
	 */
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

	/**
	 * \brief 删除
	 * \tparam K 传入查询类型
	 * \param t 查询数据
	 * \return 是否删除
	 */
	template <typename K>
	bool remove(K&& t) noexcept
	{
		return remove_impl(std::forward<K>(t), root);
	}

	/**
	 * \brief 指定rank元素
	 * \param s rank
	 * \return 目标存储只读引用
	 */
	const_reference nth(size_type s) const
	{
		return nth_impl(s + 1, root);
	}

	/**
	 * \brief AVL大小
	 * \return 当前AVL大小
	 */
	size_type size() const noexcept
	{
		return get_size(root);
	}

	/**
	 * \brief AVL树高
	 * \return 当前AVL树高
	 */
	size_type height() const noexcept
	{
		return get_height(root);
	}

	/**
	 * \brief 是否为空
	 * \return 是否为空
	 */
	bool empty() const noexcept
	{
		return root;
	}

	/**
	 * \brief 交换AVL
	 * \param another 目标AVL
	 */
	void swap(avl_tree& another) noexcept
	{
		std::swap(root, another.root);
		std::swap(another.maker, maker);
		std::swap(another.comp, comp);
	}

	/**
	 * \brief 头迭代器
	 * \return 头迭代器
	 */
	iterator begin() & noexcept
	{
		return iterator(0, this);
	}

	/**
	 * \brief 头迭代器
	 * \return 头迭代器
	 */
	iterator end() & noexcept
	{
		return iterator(size(), this);
	}
	
	/**
	 * \brief 只读头迭代器
	 * \return 只读头迭代器
	 */
	iterator begin() const& noexcept
	{
		return cbegin();
	}
	
	/**
	 * \brief 只读尾迭代器
	 * \return 只读尾迭代器
	 */
	iterator end() const& noexcept
	{
		return cend();
	}
	
	/**
	 * \brief 只读头迭代器
	 * \return 只读头迭代器
	 */
	const_iterator cbegin() const& noexcept
	{
		return const_iterator(0, this);
	}
	
	/**
	 * \brief 只读尾迭代器
	 * \return 只读尾迭代器
	 */
	const_iterator cend() const& noexcept
	{
		return const_iterator(size(), this);
	}

	/**
	 * \brief AVL递归遍历
	 * \tparam O 遍历方式
	 * \tparam F 操作函数类型
	 * \param f 操作函数
	 */
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

#define AVL_defined

#endif

#endif