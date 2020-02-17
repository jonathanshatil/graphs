#include "Graph.hpp"
#include "IndirectGraph.hpp"
#include "algorithems.hpp"
#include <iostream>
#include <string>
#include <fstream>

template<class T> void printGraph(Graph<T>& g);
void TestDirected();
void TestInDirected();
void TestAlgo();
Graph<std::string>* readGraph(std::string V, std::string E, bool is_weighted, bool is_direct);


int main(void)
{
	TestDirected();
	TestInDirected();
	std::cout << "///test 3 - Weighted graph///\n ";
	TestAlgo();
	return 0;
}

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

void TestDirected()
{
	std::vector < std::string> V = { "s","b","c","d","e","f","g" };
	std::vector <float> V2 = { 12,-10,123.2,123.7,-146.445,123,0 };
	std::vector < std::vector<int>> E = { {1},{2},{0},{2,4},{5},{6},{5} };



	std::cout << "///test 1 - Direct Graph test:///\n ";
	std::cout << "test 1.1.1: empty constractor\n";
	Graph<std::string> empty;
	printGraph(empty);
	std::cout << "test 1.1.2: vector constractor\n";
	Graph<std::string> g(V, E);
	printGraph(g);
	std::cout << "test 1.1.3: copy constractor\n";
	Graph<std::string> copy = g;
	copy.setValue(0, "this is the copied graph\n");
	printGraph(copy);
	printGraph(g);
	std::cout << "test 1.2.1: verticies number " << g.vertNum() << "\n";
	std::cout << "test 1.2.2: edge number " << g.edgeNum() << "\n";
	std::cout << "test 1.3: is adjcent\n";
	for (int i = 0; i < g.vertNum(); i++)
		for (int j = 0; j < g.vertNum(); j++)
			if (g.is_adjacent(i, j))
				std::cout << "[" << i << "] is adjcent to [" << j << "]\n";
	std::cout << "test 1.4: compering nodes\n";
	g.setValue(1, "c");
	g.setValue(3, "b");
	g.setValue(0, "c");
	for (int i = 0; i < g.vertNum(); i++)
		for (int j = 0; j < g.vertNum(); j++)
			if (g.are_equal(i, j))
				std::cout << "[" << i << "] is equal to [" << j << "]\n";
	std::cout << "test 1.5: geting nodes index by value (-1 if non existing)\n";
	Graph<std::string> G(V, E);
	std::string s[10] = { "a","b","c","d","e","f","g","h","i","s" };
	for (int i = 0; i < 10; i++)
	{
		int j = G.getIndex(s[i]);
		if (j != -1)
			std::cout << "[" << j << "] is equal to " << s[i] << "\n";
		else
			std::cout << "the value: " << s[i] << " does not exist in the graph\n";
	}
	std::cout << "test 1.6: conectivity\n";
	for (int i = 0; i < G.vertNum(); i++)
		for (int j = 0; j < G.vertNum(); j++)
			if (G.is_reachable(i, j))
				std::cout << "[" << j << "] is reachable from [" << i << "]\n";
	std::cout << "test 1.7: sub graphs\n";
	Graph<std::string> sub = G.getSubGraph({ 0,1,2 });
	printGraph(sub);
	sub = G.getSubGraph({ 0,1,2,6 });
	printGraph(sub);
	sub = G.getSubGraph({ 0,1,2,4,5,6 });
	printGraph(sub);
	Graph<float> G2(V2, E);
	printGraph(G2);
	std::cout << "test 1.7.1: minimun node in the graph " << G2.getMin().first << "\n";
	std::cout << "test 1.7.2: maximun node in the graph " << G2.getMax().first << "\n";
	std::cout << "test 1.8.1: transposing the previos graph\n";
	printGraph(G2);
	G2 = G2.getTranspose();
	printGraph(G2);
	std::cout << "test 1.8.2: transposing the transposed graph to get the original" << G2.getMax().first << "\n";
	G2 = G2.getTranspose();
	printGraph(G2);
}
void TestInDirected()
{
	std::vector < std::vector<int>> E = { {1,2},{2,0},{0,1,3},{2,4},{3,5},{4,6},{5} };
	std::vector <float> V = { 12,-10,123.2,123.7,-146.445,123,0 };
	std::vector < std::string> V2 = { "s","b","c","d","e","f","g" };
	std::vector < std::vector<int>> E2 = { {1},{2},{0},{2,4},{5},{6},{5} };
	std::cout << "///test 2 - Indirect Graph tests///\n ";
	std::cout << "test 2.1.1: empty constractor \n";
	IndirectGraph<float> empty_ig;
	printGraph(empty_ig);
	std::cout << "test 2.1.2: vector constractor \n";
	IndirectGraph<float> ig(V, E);
	printGraph(ig);
	std::cout << "test 2.1.3: directGraph copy constractor \n";
	Graph<std::string> G(V2, E2);
	IndirectGraph<std::string> cpoied_ig = G;
	printGraph(cpoied_ig);
	std::cout << "test 2.2: edge number " << ig.edgeNum() << "\n";
	std::cout << "test 2.3: is adjcent\n";
	for (int i = 0; i < ig.vertNum(); i++)
		for (int j = 0; j < ig.vertNum(); j++)
			if (ig.is_adjacent(i, j))
				std::cout << "[" << i << "] is adjcent to [" << j << "]\n";
	std::cout << "test 2.4: conectivity\n";
	for (int i = 0; i < ig.vertNum(); i++)
		for (int j = 0; j < ig.vertNum(); j++)
			if (ig.is_reachable(i, j))
				std::cout << "[" << j << "] is reachable from [" << i << "]\n";
	std::cout << "test 2.5: transposing the indirect graph\n";
	printGraph(ig);
	ig = ig.getTranspose();
	printGraph(ig);
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
	pathes = computePaths(*g[5], 0);//rewrite
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
	pathes = computePaths(*g[8], 1);//rewrite
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
	std::vector<std::vector<double>> allPathes = computeAlldists(*g[2]);
	for (int i = 0; i < (*g[2]).vertNum(); i++)
		for (int j = 0; j < (*g[2]).vertNum(); j++)
			std::cout << "shortest distance [" << i << "]->[" << j << "] is :" << allPathes[i][j] << "\n";
	printGraph(*g[3]);
	allPathes = computeAlldists(*g[3]);
	for (int i = 0; i < (*g[3]).vertNum(); i++)
		for (int j = 0; j < (*g[3]).vertNum(); j++)
			std::cout << "shortest distance [" << i << "]->[" << j << "] is :" << allPathes[i][j] << "\n";
	printGraph(*g[4]);
	allPathes = computeAlldists(*g[4]);
	for (int i = 0; i < (*g[4]).vertNum(); i++)
		for (int j = 0; j < (*g[4]).vertNum(); j++)
			std::cout << "shortest distance [" << i << "]->[" << j << "] is :" << allPathes[i][j] << "\n";
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
	std::vector<std::pair<int, double>> temp_vec;
	std::string list_str;
	std::vector<std::vector<std::pair<int, double>>> e;
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