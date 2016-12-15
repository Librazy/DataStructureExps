#pragma once

#ifndef Main_H_defined

// ReSharper disable CppUnusedIncludeDirective
#include<memory>

struct nop
{
	template <typename T>
	void operator() (T const &) const noexcept { }
};

template <typename T>
using nop_unique_ptr = std::unique_ptr<T, nop>;

struct nop_ptr_maker
{

	size_t count;

	template<typename N, typename K>
	nop_unique_ptr<N> make(K&& arg) {
		static N storage[100];
		storage[count].data.val = arg;
		return nop_unique_ptr<N>(storage + count++);
	};
};

template <typename P>
struct ptr_int_cmp
{
	bool operator()(P const& a, int const& b) const
	{
		return *a < b;
	}

	bool operator()(int const& a, P const& b) const
	{
		return a < *b;
	}

	bool operator()(P const& a, P const& b) const
	{
		return *a < *b;
	}
};
#define Main_H_defined

#endif