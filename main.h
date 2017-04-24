#pragma once

#ifndef Main_H_defined

// ReSharper disable CppUnusedIncludeDirective
#include<memory>
#include<sstream>

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
	}
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


//http://stackoverflow.com/questions/24278803/how-can-i-write-a-stateful-allocator-in-c11-given-requirements-on-copy-constr
template <typename T>
class FreelistAllocator {
	union node {
		node* next;
		typename std::aligned_storage<sizeof(T), alignof(T)>::type storage;
	};

	node* list = nullptr;

	void clear() noexcept {
		auto p = list;
		while (p) {
			auto tmp = p;
			p = p->next;
			delete tmp;
		}
		list = nullptr;
	}

public:
	using value_type = T;
	using size_type = size_t;
	using propagate_on_container_move_assignment = std::true_type;
	FreelistAllocator() noexcept = default;
	FreelistAllocator(const FreelistAllocator&) noexcept {}
	template <typename U>
	explicit FreelistAllocator(const FreelistAllocator<U>&) noexcept {}
	FreelistAllocator(FreelistAllocator&& other) noexcept :  list(other.list) {
		other.list = nullptr;
	}

	FreelistAllocator& operator = (const FreelistAllocator&) noexcept {
		// noop
		return *this;
	}

	FreelistAllocator& operator = (FreelistAllocator&& other) noexcept {
		clear();
		list = other.list;
		other.list = nullptr;
		return *this;
	}

	~FreelistAllocator() noexcept { clear(); }

	T* allocate(size_type n) {
		// std::cout << "Allocate(" << n << ") from ";
		if (n == 1) {
			auto ptr = list;
			if (ptr) {
				// std::cout << "freelist\n";
				list = list->next;
			}
			else {
				// std::cout << "new node\n";
				ptr = new node;
			}
			return reinterpret_cast<T*>(ptr);
		}

		// std::cout << "::operator new\n";
		return static_cast<T*>(::operator new(n * sizeof(T)));
	}

	void deallocate(T* ptr, size_type n) noexcept {
		// std::cout << "Deallocate(" << static_cast<void*>(ptr) << ", " << n << ") to ";
		if (n == 1) {
			// std::cout << "freelist\n";
			auto node_ptr = reinterpret_cast<node*>(ptr);
			node_ptr->next = list;
			list = node_ptr;
		}
		else {
			// std::cout << "::operator delete\n";
			::operator delete(ptr);
		}
	}
};

template <typename T, typename U>
bool operator == (const FreelistAllocator<T>&, const FreelistAllocator<U>&) {
	return true;
}

#define Main_H_defined

#endif