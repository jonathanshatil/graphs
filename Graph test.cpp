#include "Graph.hpp"
#include "IndirectGraph.hpp"

template<class T>
void printGraph(Graph<T>& g)//T should have a << opretor
{
	std::cout << "|V|=" << g.vertNum() << "|E|=" << g.edgeNum() << "\n";
	for (int i = 0; i < g.vertNum(); i++)
	{
		std::cout << "[" << i << "] value: " << g[i];
		if (g.order(i) > 0)
			std::cout << " adjcent to:  ";
		for (auto v : g.allAdjcents(i))
		{
			std::cout << "(" << v.first << ") value: " << g[v.first] << " edge weight:" << v.second << ", ";
		}
		std::cout << "\n";
	}
}


void testGraph()
{
	std::vector < std::string> V = { "s","b","c","d","e","f","g" };
	std::vector <float> V2 = { 12,-10,123.2,123.7,-146.445,123,0 };
	std::vector < std::list<int>> E = { {1},{2},{0},{2,4},{5},{6},{5} };
	std::vector < std::list<std::pair<int, double>>> E2 = { {std::pair < int,double>(1,-12)},
	{std::pair < int,double>(2,0.5)},
	{std::pair < int,double>(0,1.5)},
	{std::pair < int,double>(2,11),std::pair < int,double>(4,4)},
	{std::pair < int,double>(5,3.1425)},
	{std::pair < int,double>(6,5318008)},
	{std::pair < int,double>(5,-1.1)} };
	std::vector <std::pair<int, int>> edgeVec = { std::pair<int,int>(0,1),std::pair<int,int>(1,2),std::pair<int,int>(2,1),std::pair<int,int>(3,2),std::pair<int,int>(3,4),std::pair<int,int>(4,5),std::pair<int,int>(5,6),std::pair<int,int>(6,5)};

	std::cout << "///test 1 - Direct Graph test:///\n ";
	std::cout << "test 1.1.1: empty constractor\n";
	Graph<std::string> empty;
	printGraph(empty);
	std::cout << "test 1.1.2: vertices constractor\n";
	Graph<std::string> onlyVert(V);
	printGraph(onlyVert);
	std::cout << "test 1.1.3: non weighted list constractor\n";
	Graph<std::string> g(V, E);
	printGraph(g);
	std::cout << "test 1.1.4: weighted list constractor\n";
	Graph<std::string> g2(V, E2);
	printGraph(g2);
	std::cout << "test 1.1.5: copy constractor\n";
	Graph<std::string> copy = g;
	copy.setValue(0, "this is the copied graph\n");
	printGraph(copy);
	printGraph(g);
	std::cout << "test 1.1.6: edges vector constractor\n";
	Graph<std::string> g3(V, edgeVec);
	printGraph(g3);

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
	std::cout << "test 1.8.2: transposing the transposed graph to get the original"  << "\n";
	G2 = G2.getTranspose();
	printGraph(G2);
	std::cout << "test 1.9: getting the union of 2 graphs\n";
	std::vector < std::string> V4 = { "h","a","l","l","o"};
	std::vector < std::list<int>> E4 = { {1,2,3,4},{0,2,3,4},{1,4},{2,4},{3,1,0} };
	Graph<std::string> g4(V4, E4);
	Graph<std::string> Gunion = g.getUnion(g4);
	printGraph(Gunion);
	std::cout << "test 1.10.1: adding node with no edges\n";
	g4.addNode("new node");
	printGraph(g4);
	std::cout << "test 1.10.2: adding node with edges\n";
	std::vector<std::pair<int, double>> pt = { std::pair < int, double>(2,2.34),std::pair < int, double>(3,15.76)};
	std::vector<std::pair<int, double>> pf = { std::pair < int, double>(0,10)};
	g4.addNode("n",pt,pf);
	printGraph(g4);
	std::cout << "test 1.11: removing a node\n";
	g4.removeNode(0);
	printGraph(g4);
	std::cout << "test 1.12: removing a value\n";
	g4.removeValue("l");
	printGraph(g4);
	std::cout << "test 1.13.1: removing a edge that exist\n";
	g.removeEdgeByIndexs(0, 1);
	printGraph(g);
	std::cout << "test 1.13.2: removing a edge that doesnt exist\n";
	g.removeEdgeByIndexs(1, 4);
	printGraph(g);
}

void testIndirectGraph()
{
	std::vector < std::list<int>> E = { {1,2},{2,0},{0,1,3},{2,4},{3,5},{4,6},{5} };
	std::vector <float> V = { 12,-10,123.2,123.7,-146.445,123,0 };
	std::vector < std::string> V2 = { "s","b","c","d","e","f","g" };
	std::vector < std::list<int>> E2 = { {1},{2},{0},{2,4},{5},{6},{5} };
	std::cout << "///test 2 - Indirect Graph tests///\n ";
	std::cout << "test 2.1.1: empty constractor \n";
	IndirectGraph<float> empty_ig;
	printGraph(empty_ig);
	std::cout << "test 2.1.2: vector constractor \n";
	IndirectGraph<float> ig(V, E);
	printGraph(ig);
	std::cout << "test 2.1.3: Graph to IndirectGraph copy constractor \n";
	Graph<std::string> G(V2, E2);
	IndirectGraph<std::string> cpoied_ig (G);
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
	std::cout << "test 2.7: adding an edge \n";
	cpoied_ig.addEdgeByIndexs(0, 5);
	printGraph(cpoied_ig);
	std::cout << "test 2.7: removing an edge \n";
	cpoied_ig.removeEdgeByIndexs(0, 1);
	printGraph(cpoied_ig);
}