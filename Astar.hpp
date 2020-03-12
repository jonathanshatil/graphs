/*written by yonatan shatil*/
#pragma once
#include "Graph.hpp"
#include "vertNode.hpp"
#include "FibonacciHeap.hpp"
#include <chrono>
#include <set>
#include <stack>
using namespace std::chrono;

std::vector<int> constractPath(int*& came_from, int curr);

template<class T>
std::vector<int> Astar(Graph<T>& graph, int start, int goal, double heuristic(T& curr, T& dst))
{
	if (graph.order(start) == 0)
		return std::vector<int>();
	if (start == goal)
	{
		std::vector<int> path = { start,goal };
		return path;
	}
	T dst = graph[goal];
	T src = graph[start];
	int n = graph.vertNum();
	double* g = new double[n];
	double* h = new double[n];
	double* f = new double[n];
	int* closed = new int[n];
	int* came_from = new int[n];
	for (int i = 0; i < n; i++)
	{
		g[i] = FLT_MAX;
		h[i] = FLT_MAX;
		f[i] = FLT_MAX;
		closed[i] = false;
		came_from[i] = -1;
	}
	g[start] = 0;
	h[start] = heuristic(src, dst);
	f[start] = h[start];
	//std::set<std::pair<double, int>> openSet;
	FibonacciHeap<vertNode> openSet;
	int cnt = 0;
	openSet.insert(vertNode(start, 0));
	while (!openSet.isEmpty())
	{
		cnt++;
		int curr = openSet.removeMinimum().getIndex();
		//openSet.erase(openSet.begin());
		closed[curr] = true;
		double gNew, fNew;
		std::list<std::pair<int, double>>::iterator end = graph.adjcentEnd(curr);
		for (auto it = graph.adjcentBegin(curr); it != end; ++it)
		{

			int i =(*it).first;
			double w = (*it).second;
			if (i == goal)
			{
				came_from[i] = curr;
				return constractPath(came_from, i);
			}
			if (!closed[i])
			{
				gNew = g[curr] + w;
				if (h[i] == FLT_MAX)
				{
					T val = graph[i];
					h[i] = heuristic(val, dst);
				}
				fNew = gNew + h[i];
				if (f[i] == FLT_MAX || fNew < f[i])
				{
					f[i] = fNew;
					g[i] = gNew;
					came_from[i] = curr;
					openSet.insert(vertNode(i, f[i]));
				}
			}
		}
	}
	return std::vector<int>();
}

std::vector<int> constractPath(int*& came_from, int curr)
{
	std::vector<int> path;
	std::stack<int> temp;
	while (curr != -1)
	{
		temp.push(curr);
		curr = came_from[curr];
	}
	while (!temp.empty())
	{
		path.push_back(temp.top());
		temp.pop();
	}
	return path;
}