#pragma once

#ifndef Kruskal_disabled

#ifndef Kruskal_defined

#include <array>
#include <vector>
#include <SparseMatrix.hpp>

std::vector<std::tuple<int, size_t, size_t>> Kruskal(std::vector<std::vector<std::pair<size_t, int>>> const& m);

template<size_t N>
std::array<std::tuple<int, size_t, size_t>, N - 1> Kruskal(SparseMatrix2<int, N, N> const& map)
{
	auto m = std::vector<std::vector<std::pair<size_t, int>>>();
	for(auto i = 0; i != N ;++i) {
		m.emplace_back(map.row(i));
	}
	auto x =  Kruskal(m);
	auto ret = std::array<std::tuple<int, size_t, size_t>, N - 1>();
	std::copy(x.begin(), x.end(), ret.begin());
	return ret;
}

#define Kruskal_defined

#endif

#endif