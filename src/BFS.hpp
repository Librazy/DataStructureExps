#pragma once

#ifdef BFS_enabled

#ifndef BFS_defined
// ReSharper disable CppUnusedIncludeDirective
#include <iostream>
#include <map>
#include <tuple>
#include <utility> 
#include <vector>
#include <sstream>

/**
 * \brief 广搜解迷宫
 * \param x1 迷宫起点坐标x
 * \param y1 迷宫起点坐标y
 * \param x2 迷宫终点坐标x
 * \param y2 迷宫终点坐标y
 * \param w 迷宫总宽度
 * \param h 迷宫总高度
 * \param m 迷宫地图
 * \return 
 */
std::vector<std::tuple<int, int, int>> BFS(size_t x1, size_t y1, size_t x2, size_t y2, size_t w, size_t h, std::string m);

/**
 * \brief 
 * \param ans 
 * \return 
 */
std::string BFS_pretty_text(std::vector<std::tuple<int, int, int>> ans);

/**
 * \brief 
 * \param ans 
 * \param w 
 * \param h 
 * \param m 
 * \return 
 */
std::string BFS_pretty_graph(std::vector<std::tuple<int, int, int>> ans, size_t w, size_t h, std::string m);


#define BFS_defined

#endif

#endif