#include "Graph.hpp"
#include "IndirectGraph.hpp"
#include "algorithems.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <chrono> 
#include "Astar.hpp"
#include "dijkstra.h"
#include "A.Star_lib.h"
using namespace std::chrono;

template<class T> void printGraph(Graph<T>& g);
inline double euclidDist(pair<double, double>& curr, pair<double, double>& dest);
void TestAlgo();
void testNodeRank();
void testAsatr();
Graph<std::string>* readGraph(std::string V, std::string E, bool is_weighted, bool is_direct);


template<class T>
void printGraph(Graph<T>  &g)//T should have a << opretor
{
	std::cout << "|V|=" << g.vertNum() << "|E|=" << g.edgeNum() << "\n";
	for (int i = 0; i < g.vertNum(); i++)
	{
		std::cout << "[" << i << "] value: " << g[i];
		if(g.order(i)>0)
			std::cout << " adjcent to:  ";
		for (auto v : g.allAdjcents(i))
		{
			std::cout << "(" << v.first << ") value: " << g[v.first] << " edge weight:" << v.second << ", ";
		}
		std::cout << "\n";
	}
}

void TestAlgo()
{
	std::ifstream infile("test graphs.txt");
	std::string line;
	std::vector<std::string> es;
	std::vector<std::string> vs;
	std::vector<std::pair<bool,bool>> flags;
	int index = 0;
	while (std::getline(infile, line))
	{
		if (index % 5 == 1)
			flags.push_back(std::pair<bool, bool>(line[0] == 'd', line[1] == 'w'));
		if (index % 5 == 2)
			vs.push_back(line);
		if (index % 5 == 3)
			es.push_back(line);
		index++;
	}
	std::vector<Graph<std::string>*> g;
	g.push_back(nullptr);
	for (int i = 0; i < vs.size(); i++)
		g.push_back(readGraph(vs[i], es[i], flags[i].first, flags[i].second));
	std::cout << "///test 4 - Algorithems tests///\n ";
	std::cout << "test 4.1 topological sort(works only on direct graph without cycles)\n";
	printGraph(*g[6]);
	for (auto v : topologicalSort(*g[6]))
		std::cout << (*g[6])[v];
	std::cout << "\n";
	std::cout << "test 4.2.1 get strongly conected components on directed graph\n";
	for (auto componnet : getSCC(*g[7]))
	{
		for (auto i : componnet)
		{
			std::cout << i << ",";
		}
		std::cout << "\n";
	}
	std::cout << "test 4.2.2 get strongly conected components on indirected conected graph (should have one scc of all the nodes in the graph)\n";
	for (auto componnet : getSCC(*g[3]))
	{
		for (auto i : componnet)
		{
			std::cout << i << ",";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
	std::cout << "test 4.2.3 get strongly conected components on indirected non conected graph\n";
	for (auto componnet : getSCC(*g[1]))
	{
		for (auto i : componnet)
		{
			std::cout << i << ",";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
	std::cout << "test 4.3.1 get the super graph (indirect graph) \n";
	Graph<Graph<std::string>> superG = getSuperGraph(*g[7]);
	for (int i = 0; i < superG.vertNum(); i++)
	{
		Graph<std::string> temp = superG[i];
		printGraph(temp);
		if (superG.order(i) > 0)
			std::cout << "adjcent to:";
		for (auto v : superG.allAdjcents(i))
			std::cout << "scc[" << v.first << "],";
		std::cout << "\n";
	}
	std::cout << "\n";
	std::cout << "test 4.3.2 get the super graph (direct non conect graph) \n";
	superG = getSuperGraph(*g[1]);
	for (int i = 0; i < superG.vertNum(); i++)
	{
		Graph<std::string> temp = superG[i];
		printGraph(temp);
		if (superG.order(i) > 0)
			std::cout << "adjcent to:";
		for (auto v : superG.allAdjcents(i))
			std::cout << "scc[" << v.first << "],";
		std::cout << "\n";
	}
	std::cout << "\n";
	IndirectGraph<std::string> ig3 = *g[3];
	std::cout << "test 4.4.1 get the minimal spaning tree graph using kruskal algo\n";
	Graph<std::string>mst = getMST_KRUSKAL(ig3);//need to rewrite
	printGraph(mst);
	std::cout << "test 4.4.2 get the minimal spaning tree graph using prim algo\n";
	mst = getMST_PRIM(ig3);//need to rewrite
	printGraph(mst);
	std::cout << "test 4.5.1 get the shortest pathes on directed graph with dijkstra\n";
	std::vector<std::pair<double, std::vector<int>>> pathes=DijkstraComputePaths(*g[4],0);
	index = 0;
	for (auto p : pathes)
	{
		std::cout << "length of path from [0] to [" << index++ << "] :" << p.first << "\n the path is";
		for (auto v : p.second)
		{
			std::cout << "->[" << v << "]";
		}
		std::cout << "\n";
	}
	std::cout << "test 4.5.2 get the shortest pathes on indirected graph with dijkstra\n";
	pathes = DijkstraComputePaths(*g[3], 7);
	index = 0;
	for (auto p : pathes)
	{
		std::cout << "length of path from [0] to [" << index++ << "] :" << p.first << "\n the path is";
		for (auto v : p.second)
		{
			std::cout << "->[" << v << "]";
		}
		std::cout << "\n";
	}
	std::cout << "test 4.5.3 graph with negative weights that fails the dijkstra algotirhem\n";
	pathes = DijkstraComputePaths(*g[5], 0);
	index = 0;
	for (auto p : pathes)
	{
		std::cout << "length of path from [0] to [" << index++ << "] :" << p.first << "\n the path is";
		for (auto v : p.second)
		{
			std::cout << "->[" << v << "]";
		}
		std::cout << "\n";
	}
	std::cout << "test 4.5.4 the same graph with the topological sort method\n";
	pathes = TLcomputePaths(*g[5], 0);//rewrite
	index = 0;
	for (auto p : pathes)
	{
		std::cout << "length of path from [0] to [" << index++ << "] :" << p.first << "\n the path is";
		for (auto v : p.second)
		{
			std::cout << "->[" << v << "]";
		}
		std::cout << "\n";
	}
	std::cout << "test 4.5.5 get the shortest pathes on directed graph with topological sort\n";
	pathes = TLcomputePaths(*g[8], 1);//rewrite
	index = 0;
	for (auto p : pathes)
	{
		std::cout << "length of path from [1] to [" << index++ << "] :" << p.first << "\n the path is";
		for (auto v : p.second)
		{
			std::cout << "->[" << v << "]";
		}
		std::cout << "\n";
	}
	std::cout << "test 4.6 get sll the shortest dists on directed graph with FW algorithem\n";
	printGraph(*g[2]);
	auto allPathes = FloyedWarshel(*g[2]);
	for (int i = 0; i < (*g[2]).vertNum(); i++)
		for (int j = 0; j < (*g[2]).vertNum(); j++)
			std::cout << "shortest distance [" << i << "]->[" << j << "] is :" << allPathes[i][j].second << "\n";
	printGraph(*g[3]);
	allPathes = FloyedWarshel(*g[3]);
	for (int i = 0; i < (*g[3]).vertNum(); i++)
		for (int j = 0; j < (*g[3]).vertNum(); j++)
			std::cout << "shortest distance [" << i << "]->[" << j << "] is :" << allPathes[i][j].second << "\n";
	printGraph(*g[4]);
	allPathes = FloyedWarshel(*g[4]);
	for (int i = 0; i < (*g[4]).vertNum(); i++)
		for (int j = 0; j < (*g[4]).vertNum(); j++)
			std::cout << "shortest distance [" << i << "]->[" << j << "] is :" << allPathes[i][j].second << "\n";

	std::cout << "test 4.7 bellman ford algorithem\n";

}

Graph<std::string>* readGraph(std::string V, std::string E, bool is_direct,bool is_weighted)
{
 	size_t pos = 0;
	std::vector<std::string> v;
	while ((pos = V.find(",")) != std::string::npos) {
		v.push_back(V.substr(0, pos));
		V.erase(0, pos + 1);
	}
	v.push_back(V);
	E.erase(std::remove(E.begin(), E.end(), '}'), E.end());
	E = E.substr(1, E.size() - 1);
	pos = 0;
	std::list<std::pair<int, double>> temp_vec;
	std::string list_str;
	std::vector<std::list<std::pair<int, double>>> e;
	int i = 0;
	while ((pos = E.find(",{")) != std::string::npos) {
		list_str =E.substr(0, pos);
		size_t temp_pos = 0;
		std::string temp;
		while ((temp_pos = list_str.find(";")) != std::string::npos) {
			temp = list_str.substr(0, temp_pos);
			if (is_weighted)
			{
				size_t s = temp.find(",");
				temp_vec.push_back(std::pair<int, double>(std::stoi(temp.substr(s+1)), std::stod(temp.substr(0,s))));
			}
			else
				temp_vec.push_back(std::pair<int, double>(std::stoi(temp), 1));
			list_str.erase(0, temp_pos + 1);
		}
		if (list_str != "")
		{
			if (is_weighted)
			{
				size_t s = list_str.find(",");
				temp_vec.push_back(std::pair<int, double>(std::stoi(list_str.substr(s+1)), std::stod(list_str.substr(0,s))));
			}
			else
				temp_vec.push_back(std::pair<int, double>(std::stoi(list_str), 1));
		}
		e.push_back(temp_vec);
		temp_vec.clear();
		E.erase(0, pos + 2);
	}
	size_t temp_pos = 0;
	std::string temp;
	while ((temp_pos = E.find(";")) != std::string::npos) {
		temp = E.substr(0, temp_pos);
		if (is_weighted)
		{
			size_t s = temp.find(",");
			temp_vec.push_back(std::pair<int, double>(std::stoi(temp.substr(s+1)), std::stod(temp.substr(0,s))));
		}
		else
			temp_vec.push_back(std::pair<int, double>(std::stoi(temp), 1));
		E.erase(0, temp_pos + 1);
	}
	if (E != "")
	{
		if (is_weighted)
		{
			size_t s = E.find(",");
			temp_vec.push_back(std::pair<int, double>(std::stoi(E.substr(s+1)), std::stod(E.substr(0,s))));
		}
		else
			temp_vec.push_back(std::pair<int, double>(std::stoi(E), 1));
	}
	e.push_back(temp_vec);
	temp_vec.clear();
	Graph<std::string>* g;
	if (is_direct)
		g = new Graph<std::string>(v, e);
	else
		g = new IndirectGraph<std::string>(v, e);
	return g;
}

void testNodeRank()
{
	/*in this example there is an enemy with 7 bases
	each base is represented via a vert in the bases graph
	if vert a has an edge with vert b (a->b) base a provides supplies for base b
	the more supplies a base have the more it can provid 
	the goal is to find out which enemy base has the most supplies and to rank the 7 bases based on amount of supply
	*/
	std::cout << "test 5 - NodeRank(PageRank)\n";
	std::cout << "in this example there is an enemy with 7 bases\neach base is represented via a vert in the bases graph\nif vert a has an edge with vert b(a->b) base a provides supplies for base b\nthe more supplies a base has the more it can provid\nthe goal is to find out which enemy base has the most supplies and to rank the 7 bases based on amount of supply\n";
	//test 1 if a base dosent provide for any specific base then its assumed that he provides all other bases
	std::cout << "test 5.1 if a base dosent provide for any specific base then its assumed that he provides all other bases\n";
	std::vector<std::list<int>> E = { { 1, 2, 3 },{0,2},{0,3,5},{2},{1,3},{3,6},{} };
	std::vector<std::string> V = { "base A","base B","base C","base D","base E","base F","base G" };
	Graph<std::string> g(V, E);
	for (auto v : nodeRank(g))
		std::cout << g[v.first] << " has:" << v.second * 100 << "% of all supply\n";

	std::cout << "test 5.2 if a base dosent provide for any specific base then its assumed that all the supplies it have stays in it\n";
	for (auto v : nodeRank(g, -1, 0.000001, true))
		std::cout << g[v.first] << " has:" << v.second * 100 << "% of all supply\n";

	std::cout << "test 5.3 each providency has a probability it will work(or the frequency of sending supplies). if it dosent work the supplies are evenly distributed in all the bases\n";
	std::vector<std::list<std::pair<int, double>>> E2;
	std::vector<std::vector<double>> W = { {0.9,0.3,0.8},{0.6,0.2},{1,1,1},{0.4},{0.3,0.7},{1,0.5},{} };
	for (int i = 0; i < E.size(); i++)
	{
		E2.push_back(std::list<std::pair<int, double>>());
		int j = 0;
		for (auto it = E[i].begin(); it != E[i].end(); ++it) {
			E2[i].push_back(std::pair<int, double>(*it, W[i][j]));
			j++;
		}
	}
	Graph<std::string> g2(V, E2);
	for (auto v : nodeRank(g2))
		std::cout << g2[v.first] << " has:" << v.second * 100 << "% of all supply\n";
	std::cout << "test 5.4 each providency has a importency that reprsents the part of supplie to be transferd\n";
	E2.clear();
	std::vector<std::vector<double>> W2 = { {10,2,3},{3,1},{6,1,4},{1},{10,1},{1,3},{} };
	for (int i = 0; i < E.size(); i++)
	{
		E2.push_back(std::list<std::pair<int, double>>());
		int j = 0;
		for (auto it = E[i].begin(); it != E[i].end(); ++it) {
			E2[i].push_back(std::pair<int, double>(*it, W2[i][j]));
			j++;
		}
	}
	Graph<std::string> g3(V, E2);
	for (auto v : nodeRank(g3,-1, 0.000001,false,false))
		std::cout << g3[v.first] << " has:" << v.second * 100 << "% of all supply\n";
}

void testMatrixGraph()
{
	std::vector < std::string> V = { "s","b","c","d","e","f","g" };
	std::vector <float> V2 = { 12,-10,123.2,123.7,-146.445,123,0 };
	std::vector < std::vector<int>> E = { {1},{2},{0},{2,4},{5},{6},{5} };



	std::cout << "///test 3 - metrix Graph test:///\n ";
	std::cout << "test 3.1.1: empty constractor\n";
	std::cout << "test 3.1.2: vector constractor\n";
	std::cout << "test 3.1.3: matrix constractor\n";
	std::cout << "test 3.1.4: copy constractor\n";
	std::cout << "test 3.1.5: adjcency list graph cast\n";
	std::cout << "test 3.2.1: verticies number ";
	std::cout << "test 3.2.2: edge number ";
	std::cout << "test 3.3: is adjcent \n";
	std::cout << "test 3.4: conectivity\n";
	std::cout << "test 3.5: sub graphs\n";
	std::cout << "test 3.6.1: transposing the graph\n";
	std::cout << "test 3.6.2: transposing the transposed graph to get the original";
}

void testAsatr()
{
	std::cout << "in this example we have a graph where each vertex represent gemotric place (x,y)\n";
	std::cout << "if vert (x,y) is conected to (z,w) there is a road conecting the two points (the length of the edge is alwayas more than the euclidean distance betwin them)\n";
	std::cout << "the goal is to find the shortest path betwin the src node to the dst node with a star algo(this task can be done with dijkstra alorithem but will be less efficient)\n";
	std::vector<std::list<std::pair<int,double>>> E = {{ std::pair < double,int>(1,6),std::pair < double,int>(2,2.5), std::pair < double,int>(5,4)},
	{std::pair < double,int>(0,5.5),std::pair < double,int>(6,7)},
	{std::pair < double,int>(3,3)},
	{std::pair < double,int>(4,2.5),std::pair < double,int>(5,2)},
	{std::pair < double,int>(6,1.5)},
	{std::pair < double,int>(2,3.5),std::pair < double,int>(1,7)},
	{std::pair < double,int>(0,4.5),std::pair < double,int>(2,4)} };
	std::vector<std::pair<double, double>> V;
	V.push_back(std::pair<double, double>(0, 0));
	V.push_back(std::pair<double, double>(3.13, 4.39));
	V.push_back(std::pair<double, double>(0.29, 2.04));
	V.push_back(std::pair<double, double>(-1.73, 3.68));
	V.push_back(std::pair<double, double>(-2.69, 1.52));
	V.push_back(std::pair<double, double>(-2.8, 2.38));
	V.push_back(std::pair<double, double>(-3.26, 2.47));
	Graph<std::pair<double, double>> g(V, E);
	std::vector<int> path=Astar(g, 0, 6, euclidDist);
	for (auto v : path)
	{
		std::cout << "->(" << g[v].first << "," << g[v].second << ")";
	}
	std::cout << std::endl;
}
double euclidDist(pair<double, double>& curr, pair<double, double> & dest)
{
	return ((double)sqrt((curr.first - dest.first) * (curr.first - dest.first) + (curr.second - dest.second) * (curr.second - dest.second)));
}

