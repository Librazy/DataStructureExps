#include <climits>
#include <queue>
#include <functional>
#include "Dijkstra.h"

#ifdef Dijkstra_defined

std::vector<int> Dijkstra(std::vector<std::vector<std::pair<size_t, int>>> m, size_t s)
{
	auto q = std::priority_queue<std::pair<int, size_t>, std::vector<std::pair<int, size_t>>, std::greater<>>();
	auto d = std::vector<int>(m.size(), INT_MAX);
	q.push(std::make_pair(0, s));
	d[s] = 0;
	while (!q.empty()) {
		auto c = q.top(); q.pop();
		auto ps = c.second;
		if (c.first != d[ps]) { continue; }
		for (auto i = 0; i < m[ps].size(); i++)
		{
			auto pt = m[ps][i].first;
			auto pv = m[ps][i].second;
			if (d[pt] > d[ps] + pv)
			{
				d[pt] = d[ps] + pv;
				q.push(std::make_pair(d[pt], pt));
			}
		}
	}
	return d;
}

#endif