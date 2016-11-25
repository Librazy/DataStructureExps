#include <numeric>
#include <functional>
#include "Kruskal.h"


#ifdef Kruskal_defined

std::vector<std::tuple<int, size_t, size_t>> Kruskal(std::vector<std::vector<std::pair<size_t, int>>> m)
{
	auto s = m.size();
	auto d = std::vector<std::tuple<int, size_t, size_t>>();
	auto ans = std::vector<std::tuple<int, size_t, size_t>>();
	for(size_t i = 0; i != s; ++i) {
		for (auto p : m[i]) {
			d.emplace_back(std::make_tuple(std::get<1>(p), i, std::get<0>(p)));
		}
	}
	auto f = std::vector<size_t>(s);
	std::function<size_t(size_t)> getb = [&f, &getb](size_t x) {return f[x] == x ? x : f[x] = getb(f[x]); };
	iota(f.begin(), f.end(), 0);
	sort(d.begin(), d.end(), std::greater<>());
	while(ans.size() != s- 1) {
		auto cur = d.back();
		d.pop_back();
		if(getb(std::get<1>(cur)) != getb(std::get<2>(cur))) {
			f[getb(std::get<1>(cur))] = f[(std::get<2>(cur))];
			ans.push_back(cur);
		}
	}
	return ans;
}

#endif