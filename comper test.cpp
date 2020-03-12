#include "Graph.hpp"
#include "A.Star_lib.h"
#include "dijkstra.h"
#include "Astar.hpp"
#include "IndirectGraph.hpp"
#include "algorithems.hpp"
double euclidDist(pair<double, double>& curr, pair<double, double>& dest)
{
	return ((double)sqrt((curr.first - dest.first) * (curr.first - dest.first) + (curr.second - dest.second) * (curr.second - dest.second)));
}
void comper()
{
	int grid[9][10] =
	{
		{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
		{ 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 },
		{ 1, 1, 1, 0, 1, 1, 0, 1, 0, 1 },
		{ 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
		{ 1, 1, 1, 0, 1, 1, 1, 0, 1, 0 },
		{ 1, 0, 1, 1, 1, 1, 0, 1, 0, 0 },
		{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
		{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
		{ 1, 1, 1, 0, 0, 0, 1, 0, 0, 1 }
	};
	std::vector<std::list<std::pair<int, double>>> E;
	std::vector<std::pair<double, double>> V;
	auto start = high_resolution_clock::now();
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			V.push_back(std::pair<int, int>(i, j));
			E.push_back(std::list<std::pair<int, double>>());
			if (grid[i][j])
			{
				if (i > 0 && grid[i - 1][j])
					E[i * 10 + j].push_back(std::pair<int, double>((i - 1) * 10 + j, 1));
				if (i + 1 < 9 && grid[i + 1][j])
					E[i * 10 + j].push_back(std::pair<int, double>((i + 1) * 10 + j, 1));
				if (j + 1 < 10 && grid[i][j + 1])
					E[i * 10 + j].push_back(std::pair<int, double>(i * 10 + (j + 1), 1));
				if (j > 0 && grid[i][j - 1])
					E[i * 10 + j].push_back(std::pair<int, double>(i * 10 + (j - 1), 1));
				if (j + 1 < 10 && i > 0 && grid[i - 1][j + 1])
					E[i * 10 + j].push_back(std::pair<int, double>((i - 1) * 10 + (j + 1), sqrt(2)));
				if (j > 0 && i > 0 && grid[i - 1][j - 1])
					E[i * 10 + j].push_back(std::pair<int, double>((i - 1) * 10 + (j - 1), sqrt(2)));
				if (j + 1 < 10 && i + 1 < 9 && grid[i + 1][j + 1])
					E[i * 10 + j].push_back(std::pair<int, double>((i + 1) * 10 + (j + 1), sqrt(2)));
				if (j > 0 && i + 1 < 9 && grid[i + 1][j - 1])
					E[i * 10 + j].push_back(std::pair<int, double>((i + 1) * 10 + (j - 1), sqrt(2)));
			}
		}
	}
	Graph<std::pair<double, double>> g(V, E);
	int n = g.vertNum();
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	std::cout << "constracting graph time:" << duration.count() << std::endl;


	std::cout << "generic A* 8,0->0,0" << std::endl;
	start = high_resolution_clock::now();
	Astar(g, 80, 0, euclidDist);
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	std::cout << "time:" << duration.count() << "ms" << std::endl;
	std::cout << "old A* 8,0->0,0" << std::endl;
	start = high_resolution_clock::now();
	aStarSearch(grid, Pair(8, 0), Pair(0, 0));
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	std::cout << "time:" << duration.count() << "ms" << std::endl;

	std::cout << "generic A* 8,0->3,5" << std::endl;
	start = high_resolution_clock::now();
	Astar(g, 80, 35, euclidDist);
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	std::cout << "time:" << duration.count() << "ms" << std::endl;
	std::cout << "old A* 8,0->3,5" << std::endl;
	start = high_resolution_clock::now();
	aStarSearch(grid, Pair(8, 0), Pair(3, 5));
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	std::cout << "time:" << duration.count() << "ms" << std::endl;

	std::cout << "generic A* 8,0->0,9" << std::endl;
	start = high_resolution_clock::now();
	Astar(g, 80, 9, euclidDist);
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	std::cout << "time:" << duration.count() << "ms" << std::endl;
	std::cout << "thier A* 8,0->0,9" << std::endl;
	start = high_resolution_clock::now();
	aStarSearch(grid, Pair(8, 0), Pair(0, 9));
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	std::cout << "time:" << duration.count() << "ms" << std::endl;

	std::cout << "my A* 1,6->4,8" << std::endl;
	start = high_resolution_clock::now();
	Astar(g, 16, 48, euclidDist);
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	std::cout << "time:" << duration.count() << "ms" << std::endl;
	std::cout << "their A* 1,6->4,8" << std::endl;
	start = high_resolution_clock::now();
	aStarSearch(grid, Pair(1, 6), Pair(4, 8));
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	std::cout << "time:" << duration.count() << "ms" << std::endl;

	std::cout << "compering old dijkstra with new dijkstra\n";
	start = high_resolution_clock::now();
	adjacency_list_t adjacency_list(6);
	adjacency_list[0].push_back(neighbor(1, 7));//b
	adjacency_list[0].push_back(neighbor(2, 9));//c
	adjacency_list[0].push_back(neighbor(5, 14));//f
	adjacency_list[1].push_back(neighbor(0, 7));//a
	adjacency_list[1].push_back(neighbor(2, 10));//c
	adjacency_list[1].push_back(neighbor(3, 15));//d
	adjacency_list[2].push_back(neighbor(0, 9));//a
	adjacency_list[2].push_back(neighbor(1, 10));//b
	adjacency_list[2].push_back(neighbor(3, 11));//d
	adjacency_list[2].push_back(neighbor(5, 2));//f
	adjacency_list[3].push_back(neighbor(1, 15));//b
	adjacency_list[3].push_back(neighbor(2, 11));//c
	adjacency_list[3].push_back(neighbor(4, 6));//e
	adjacency_list[4].push_back(neighbor(3, 6));//d
	adjacency_list[4].push_back(neighbor(5, 9));//f
	adjacency_list[5].push_back(neighbor(0, 14));//a
	adjacency_list[5].push_back(neighbor(2, 2));//c
	adjacency_list[5].push_back(neighbor(4, 9));//e
	stop = high_resolution_clock::now();
	CDijkstra d = CDijkstra(adjacency_list, 0);
	duration = duration_cast<microseconds>(stop - start);
	std::cout << "adjacency list constraction time:" << duration.count() << "ms" << std::endl;
	start = high_resolution_clock::now();
	d.DijkstraComputePaths();
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	std::cout << "old dijkstra algo time:" << duration.count() << "ms" << std::endl;
	start = high_resolution_clock::now();
	std::vector<int> V2 = { 0,1,2,3,4,5 };
	IndirectGraph<int> ig(V2);
	ig.addEdgeByIndexs(0, 1, 7);
	ig.addEdgeByIndexs(0, 2, 9);
	ig.addEdgeByIndexs(0, 5, 14);
	ig.addEdgeByIndexs(1, 2, 10);
	ig.addEdgeByIndexs(1, 3, 15);
	ig.addEdgeByIndexs(2, 3, 11);
	ig.addEdgeByIndexs(2, 5, 2);
	ig.addEdgeByIndexs(3, 4, 6);
	ig.addEdgeByIndexs(4, 5, 9);
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	std::cout << "graph constraction time:" << duration.count() << "ms" << std::endl;
	start = high_resolution_clock::now();
	DijkstraComputePaths(ig, 0);
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	std::cout << "new dijkstra algo time:" << duration.count() << "ms" << std::endl;
	IndirectGraph<std::pair<double, double>>bigG = g;
	adjacency_list_t big_adjacency_list(bigG.vertNum());
	for (int i = 0; i < bigG.vertNum(); i++)
		for (auto v : bigG.allAdjcents(i))
			big_adjacency_list[i].push_back(neighbor(v.first, v.second));
	d = CDijkstra(big_adjacency_list, 0);
	start = high_resolution_clock::now();
	d.DijkstraComputePaths();
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	std::cout << "old dijkstra algo time:" << duration.count() << "ms" << std::endl;
	start = high_resolution_clock::now();
	DijkstraComputePaths(bigG, 0);
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	std::cout << "new dijkstra algo time:" << duration.count() << "ms" << std::endl;

	std::cout << "compering prim and kruskel algorithems for minimal spannig trees\n";
	std::vector<char> V3 = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i' };
	std::vector<std::list<std::pair<int, double>>>E3 = { {std::pair < int, double>(4, 1), std::pair < int, double>(8, 8)},
	{ std::pair < int, double>(0,4),std::pair < int, double>(2,8),std::pair < int, double>(8,11) },
	{ std::pair < int, double>(1,8),std::pair < int, double>(3,7),std::pair < int, double>(5,4),std::pair < int, double>(7,2) },
	{ std::pair < int, double>(2,7),std::pair < int, double>(4,9),std::pair < int, double>(5,14) },
	{std::pair < int, double>(3,9),std::pair < int, double>(5,10)},
	{ std::pair < int, double>(4,4),std::pair < int, double>(3,14), std::pair < int, double>(4,10),std::pair < int, double>(6,2) },
	{ std::pair < int, double>(5,2),std::pair < int, double>(7,6),std::pair < int, double>(8,1) },
	{ std::pair < int, double>(2,2),std::pair < int, double>(6,6),std::pair < int, double>(8,7) },
	{ std::pair < int, double>(0,8),std::pair < int, double>(1,11),std::pair < int, double>(6,1),std::pair < int, double>(7,7) } };

	IndirectGraph<char> ig3(V3, E3);
	std::cout << "minimal spaning tree graph using kruskal algo\n";
	start = high_resolution_clock::now();
	auto mst = getMST_KRUSKAL(ig3);
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	std::cout << "algo time:" << duration.count() << "ms" << std::endl;
	std::cout << " minimal spaning tree graph using prim algo\n";
	start = high_resolution_clock::now();
	mst = getMST_PRIM(ig3);
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	std::cout << "algo time:" << duration.count() << "ms" << std::endl;
}
