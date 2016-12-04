#pragma once

#ifndef Dijkstra_disabled

#ifndef Dijkstra_defined

#include <array>
#include <vector>
#include "SparseMatrix.hpp"

std::vector<int> dijkstra(std::vector<std::vector<std::pair<size_t, int>>> const& m, size_t s);

template<size_t N>
std::array<int, N> dijkstra(sparse_matrix2d<int, N, N> const& map, size_t s)
{
	auto m = std::vector<std::vector<std::pair<size_t, int>>>();
	for(auto i = 0; i != N ;++i) {
		m.emplace_back(map.row(i));
	}
	auto x =  dijkstra(m, s);
	auto ret = std::array<int, N>();
	std::copy(x.begin(), x.end(), ret.begin());
	return ret;
}

#define Dijkstra_defined

#endif

#endif