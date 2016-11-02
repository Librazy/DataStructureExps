#include "BFS.hpp"

#ifdef BFS_defined

std::vector<std::tuple<int, int, int>> BFS(size_t x1, size_t y1, size_t x2, size_t y2, size_t w, size_t h, std::string m)
{
	std::vector<std::vector<int>> map(h, std::vector<int>(w));
	std::vector<std::vector<int>> vis(h, std::vector<int>(w));
	std::vector<std::tuple<int, int, int, int>> q;
	std::vector<std::tuple<int, int, int>> ans;
	int d[4][2] = { {-1,0},{0,-1},{1,0},{0,1} };
	std::stringstream ss(m);

	q.reserve(w*h);
	for (size_t i = 0; i != h; ++i) {
		for (size_t j = 0; j != w; ++j) {
			ss >> map[i][j];
		}
	}

	size_t front = 0;
	q.emplace_back(std::make_tuple(y1, x1, -1, -1));
	while (front < q.size()) {
		for (size_t u = 0; u != 4; ++u) {
			auto nh = std::get<0>(q[front]) + d[u][0];
			auto nw = std::get<1>(q[front]) + d[u][1];
			if ( !((nh < 0) || (static_cast<size_t>(nh) >= h)
			    || (nw < 0) || (static_cast<size_t>(nw) >= w)
			    || (map[nh][nw] == 1) || (vis[nh][nw] == 1))) {
				if (static_cast<size_t>(nh) == y2 && static_cast<size_t>(nw) == x2) {
					ans.emplace_back(std::make_tuple(y2, x2, u + 2));
					int cur = front;
					while (cur != -1) {
						ans.emplace_back(std::make_tuple(std::get<0>(q[cur]), std::get<1>(q[cur]), std::get<3>(q[cur])));
						cur = std::get<2>(q[cur]);
					}
					return ans;
				}
				vis[nh][nw] = 1;
				q.emplace_back(std::make_tuple(nh, nw, front, u + 2));
			}
		}
		++front;
	}
	return ans;
}

std::string BFS_pretty_text(std::vector<std::tuple<int, int, int>> ans)
{
	std::stringstream res;
	for (auto it = ans.rbegin(); it != ans.rend(); ++it) {
		res << "(" << std::get<0>(*it) << "," << std::get<1>(*it) << ")" << std::endl;
	}
	return res.str();
}

std::string BFS_pretty_graph(std::vector<std::tuple<int, int, int>> ans, size_t w, size_t h, std::string m)
{
	std::stringstream res;
	std::vector<std::vector<int>> map(h, std::vector<int>(w));
	std::stringstream ss(m);

	for (size_t i = 0; i != h; ++i) {
		for (size_t j = 0; j != w; ++j) {
			ss >> map[i][j];
		}
	}
	char arr[] = {' ', 'W', 'U', 'L', 'D', 'R', 'F'};

	for (auto it = ans.begin(); it != ans.end(); ++it) {
		if (it == ans.end() - 1 || it == ans.begin()) {
			map[std::get<0>(*it)][std::get<1>(*it)] = 6;
		}
		if (std::get<2>(*it) != -1) {
			map[std::get<0>(*(it + 1))][std::get<1>(*(it + 1))] = std::get<2>(*it);
		}
	}

	for (size_t i = 0; i != h; ++i) {
		for (size_t j = 0; j != w; ++j) {
			res << arr[map[i][j]];
		}
		res << std::endl;
	}
	return res.str();
}

#endif