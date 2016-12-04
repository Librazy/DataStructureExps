#include <limits>
#include <queue>
#include <functional>
#include "dijkstra.h"

#ifdef Dijkstra_defined

std::vector<int> dijkstra(std::vector<std::vector<std::pair<size_t, int>>> const& m, size_t s)
{
	auto q = std::priority_queue<std::pair<int, size_t>, std::vector<std::pair<int, size_t>>, std::greater<>>();
	auto d = std::vector<int>(m.size(), std::numeric_limits<int>::max());
	q.push(std::make_pair(0, s));
	d[s] = 0;
	while (!q.empty()) {
		auto c = q.top(); q.pop();
		auto current_step = c.second;
		if (c.first != d[current_step]) { continue; }
		for (size_t i = 0; i < m[current_step].size(); i++)
		{
			auto next_step = m[current_step][i].first;
			auto next_length = m[current_step][i].second;
			if (d[next_step] > d[current_step] + next_length)
			{
				d[next_step] = d[current_step] + next_length;
				q.push(std::make_pair(d[next_step], next_step));
			}
		}
	}
	return d;
}

#endif