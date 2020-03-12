/*written by yonatan shatil*/
#pragma once
#include"Graph.hpp"
#include "FibonacciHeap.hpp"
#include "vertNode.hpp"
#include <stack>
#include <set>
#include <chrono>
using namespace std::chrono;



template<class T>
std::stack<int> dfsRank(Graph<T>& g);

template<class T>
std::vector<int> topologicalSort(Graph<T>& g);

template<class T>
void dfsRankUtil(Graph<T>& g, int u, std::vector<int>& colors, std::stack<int>& nodes);

template<class T>
std::vector<std::vector<int>> getSCC(Graph<T>& g);

template<class T> 
Graph<Graph<T>> getSuperGraph(Graph<T>& g);

template<class T>
IndirectGraph<T> getMST_KRUSKAL(IndirectGraph<T>& g);

template<class T>
IndirectGraph<T> getMST_PRIM(IndirectGraph<T>& g);

void margeEdges(std::vector<double>& weights, int*c,int* a, int* b, int size_a, int size_b);
void margeSortEdges(std::vector<double>& weights, int* edges, int edges_size);
double getError(std::vector<double>& ranks, std::vector<double>& oldranks);

template<class T>
std::vector<int> topologicalSort(Graph<T>& g)//returns the topological sorted order of the nodes
{
	std::stack<int> nodes = dfsRank(g);
	std::vector<int> order;
	for (int i = 0; i < g.vertNum(); i++)
	{
		order.push_back(nodes.top());
		nodes.pop();
	}
	return order;
}

template<class T>
std::stack<int> dfsRank(Graph<T>& g)
{
	std::vector<int> colors(g.vertNum(), 0);
	std::stack<int> nodes;
	for (int i = 0; i < g.vertNum(); i++)
		if (colors[i] == 0)
			dfsRankUtil(g, i, colors, nodes);
	return nodes;
}

template<class T>
void dfsRankUtil(Graph<T>& g, int u, std::vector<int> &colors, std::stack<int> &nodes)
{
	colors[u] = 1;
	for (auto v : g.allAdjcents(u))
		if (colors[v.first] == 0)
			dfsRankUtil(g, v.first, colors, nodes);
	colors[u] = 2;
	nodes.push(u);
}

template<class T>
std::vector<std::vector<int>> getSCC(Graph<T> &g)//SCC tands for strongly connected componnets
{
	std::vector<std::vector<int>> scc;//vector conatins the scc indexes
	std::stack<int> scTemp;
	std::vector<int> colors(g.vertNum(), 0);
	int size = 0;
	//getting the dfs order
	std::stack<int> order = dfsRank(g);
	//transposing the graph
	Graph<T> transpose = g.getTranspose();
	// dfs on the transpose graph using the order of the first dfs
	while (!order.empty())
	{
		int i = order.top();
		if (colors[i] == 0)
		{
			scc.push_back(std::vector<int>());
			dfsRankUtil(transpose, i, colors, scTemp);
			while (!scTemp.empty())
			{
				scc[size].push_back(scTemp.top());
				scTemp.pop();
			}
			size++;
		}
		order.pop();
	}
	return scc;
}

template<class T>
Graph<Graph<T>> getSuperGraph(Graph<T> &g)//this function is not very eficcent 
{
	std::vector<std::vector<int>> scc = getSCC(g);//getting the trongly connected componnets
	std::vector<Graph<T>> superValues;
	std::vector<std::list<int>> superAdj(scc.size(), std::list<int>());
	for (auto v : scc)
		superValues.push_back(g.getSubGraph(v));//making sub graphs for each
	for (int i = 0; i < scc.size(); i++)
	{
		for (int j = 0; j < scc.size(); j++)
		{
			bool flag = false;
			if (i != j)
			{
				for (auto v : scc[i])
				{
					for (auto u : scc[j])
					{
						if (g.is_adjacent(v, u))
						{
							superAdj[i].push_back(j);
							flag = true;
							break;
						}
					}
					if (flag)
						break;
				}
			}
		}
	}
	Graph<Graph<T>> superGraph(superValues, superAdj);
	return superGraph;
}

template<class T>
IndirectGraph<T> getMST_KRUSKAL(IndirectGraph<T> &g)
{
	std::vector<bool> coverd(g.vertNum(),false);
	std::vector< std::vector<bool>> used(g.vertNum(), std::vector<bool>(g.vertNum(), false));
	IndirectGraph<T> mst;
	std::vector<std::pair<int, int>> edges;
	std::vector<double> weights;
	int size = g.edgeNum();
	int* order = new int[size];
	int edge = 0;
	for (int i = 0; i < g.vertNum(); i++)
	{
		mst.addNode(g[i]);
		for (auto v : g.allAdjcents(i))
		{
			if (!used[i][v.first])
			{
				edges.push_back(std::pair<int, int>(i, v.first));
				weights.push_back(v.second);
				used[i][v.first] = true;
				used[v.first][i] = true;
				order[edge] = edge++;
			}
		}
	}
	//sorting the edges
	margeSortEdges(weights, order, size);
	//adding the minimal edge that doesnt creats a cylce
	int mstEdges = 0;
	for (int i = 0; i < size && mstEdges < g.vertNum() - 1; i++)
	{
		if (!mst.is_reachable(edges[order[i]].first, edges[order[i]].second))
		{
			mst.addEdgeByIndexs(edges[order[i]].first, edges[order[i]].second, weights[order[i]]);
			mstEdges++;
			coverd[edges[order[i]].first] = true;
			coverd[edges[order[i]].second] = true;
		}
	}
	return mst;
}

template<class T>
IndirectGraph<T> getMST_PRIM(IndirectGraph<T>& g)
{
	IndirectGraph<T> mst;
	std::vector<bool> in(g.vertNum(), false);
	std::vector<node<vertNode>*> nodes(g.vertNum(), nullptr);
	std::vector<int> parent(g.vertNum(), 0);
	std::vector<double> dists(g.vertNum(), FLT_MAX);
	FibonacciHeap<vertNode> heap;
	nodes.push_back(0);
	for (int i = 0; i < g.vertNum();i++)
		mst.addNode(g[i]);
	in[0] = true;
	heap.insert(vertNode(0, 0));
	for (int i = 0; i < g.vertNum(); i++)
	{
		vertNode curr = heap.removeMinimum();
		in[curr.getIndex()] = true;
		if (i != 0)
			mst.addEdgeByIndexs(curr.getIndex(), parent[curr.getIndex()], curr.getDist());
		for (auto v : g.allAdjcents(curr.getIndex()))
		{
			if (in[v.first])
				continue;
			if (nodes[v.first] != nullptr)
			{
				if (dists[v.first] > v.second)
				{
					heap.decreaseKey(nodes[v.first], vertNode(v.first,v.second));
					parent[v.first] = curr.getIndex();
					dists[v.first] = v.second;
				}
			}
			else
			{
				nodes[v.first] = heap.insert(vertNode(v.first, v.second));
				parent[v.first] = curr.getIndex();
				dists[v.first] = v.second;
			}
				
		}

	}
	return mst;
}


void margeEdges(std::vector<double>& weights, int* c, int* a, int* b,int size_a, int size_b)
{
	int* tempA=new int[size_a];
	int* tempB = new int[size_b];
	int ic = 0;
	for (int i=0; i < size_a; i++)
	{
		tempA[i] = a[i];
	}
	for (int i = 0; i < size_b; i++)
	{
		tempB[i] = b[i];
	}
	int ia = 0, ib = 0;
	while (ia<size_a&&ib<size_b)
	{
		if (weights[tempA[ia]] <= weights[tempB[ib]])
			c[ic++]=tempA[ia++];
		else
			c[ic++] = tempB[ib++];
	}
	for (; ia < size_a; ia++)
	{
		c[ic++] = tempA[ia];
	}
	for (; ib < size_b; ib++)
	{
		c[ic++] = tempB[ib];
	}
	delete tempA;
	delete tempB;
}

void margeSortEdges(std::vector<double>& weights, int* edges, int edges_size)
{
	if (edges_size == 1)
		return;
	int size = edges_size / 2;
	margeSortEdges(weights, edges, size);
	margeSortEdges(weights, edges + size, edges_size - size);
	return margeEdges(weights, edges, edges, edges+size, size, edges_size - size);
}

template<class T>
void improv(Graph<T>& g, std::vector<std::pair<double, std::vector<int>>>& pathes,int u,int v,double w)
{
	if (pathes[v].first > pathes[u].first + w)
	{
		pathes[v].second = pathes[u].second;
		pathes[v].first = pathes[u].first + w;
		pathes[v].second.push_back(v);
	}
}

template<class T> std::vector<std::pair<double, std::vector<int>>> DijkstraComputePaths(Graph<T>& g, int s)//works only on a graph without negative pathes
{
	std::vector<node<vertNode>*> nodes(g.vertNum(),nullptr);
	std::vector<std::pair<double, std::vector<int>>> pathes(g.vertNum(), std::pair<double,std::vector<int>>(INFINITY, std::vector<int>()));
	pathes[s].first = 0;
	FibonacciHeap<vertNode> vertex_queue;
	vertex_queue.insert(vertNode(s,0));
	while(!vertex_queue.isEmpty())
	{
		int u = vertex_queue.removeMinimum().getIndex();
		nodes[u] = nullptr;
		std::list<std::pair<int, double>>::iterator end = g.adjcentEnd(u);
		for (auto it = g.adjcentBegin(u); it != end; ++it) {
			std::pair<int, double> v = *it;
			double temp = pathes[v.first].first;
			improv(g, pathes, u, v.first, v.second);
			if (pathes[v.first].first < temp)
			{
				if (nodes[v.first] != nullptr)
					vertex_queue.decreaseKey(nodes[v.first], vertNode(v.first, pathes[v.first].first));
				nodes[v.first] = vertex_queue.insert(vertNode(v.first, pathes[v.first].first));
			}
		}
	}
	return pathes;	
}

template<class T> std::vector<std::pair<double, std::vector<int>>> TLcomputePaths(Graph<T> &g, int s)//works only on a graph without cycles
{
	std::vector<std::pair<double, std::vector<int>>> pathes(g.vertNum(), std::pair<double, std::vector<int>>(INFINITY, std::vector<int>()));
	pathes[s].first = 0;
	std::vector<int> order = topologicalSort(g);
	for (auto u : order)
		for (auto v : g.allAdjcents(u))
			improv(g ,pathes, u, v.first, v.second);
	return pathes;
}


template<class T> std::vector<std::vector<std::pair<std::vector<int>,double>>> FloyedWarshel(Graph<T> &g)//works only on a graph without negativ cycles
{
	//std::vector<std::vector<double>> dists(g.vertNum(),std::vector<double>(g.vertNum(), INFINITY));
	std::vector<std::vector<std::pair<std::vector<int>, double>>> pathes(g.vertNum(), std::vector<std::pair<std::vector<int>, double>>(g.vertNum(), std::pair<std::vector<int>, double>(std::vector<int>(), INFINITY)));
	for (int u = 0; u < g.vertNum();u++)
		for (auto v : g.allAdjcents(u))
		{
			pathes[u][v.first].second = v.second;
			pathes[u][v.first].first.push_back(v.first);
		}
	for (int u = 0; u < g.vertNum(); u++)
		pathes[u][u].second = 0;
	for (int k = 0; k < g.vertNum(); k++)
		for (int i = 0; i < g.vertNum(); i++)
			for (int j = 0; j < g.vertNum(); j++)
				if (pathes[i][j].second > pathes[i][k].second + pathes[k][j].second)
				{
					pathes[i][j].second = pathes[i][k].second + pathes[k][j].second;
					pathes[i][j].first.reserve(pathes[i][k].first.size() + pathes[k][j].first.size()); // preallocate memory
					pathes[i][j].first.insert(pathes[i][j].first.end(), pathes[i][k].first.begin(), pathes[i][k].first.end());
					pathes[i][j].first.insert(pathes[i][j].first.end(), pathes[k][j].first.begin(), pathes[k][j].first.end());
				}
	return pathes;
}

#pragma once
#include "Graph.hpp"
#include <chrono>
#include <set>
#include <stack>
using namespace std::chrono;



template<class T> std::vector<std::pair<double, std::vector<int>>> BelmanFord(Graph<T>& g, int s)
{
	std::vector<std::pair<double, std::vector<int>>> pathes(g.vertNum(), std::pair<double, std::vector<int>>(INFINITY, std::vector<int>()));
	for (int i = 0; i < g.vertNum(); i++)
		for (int u = 0; u < g.vertNum(); u++)
			for (auto v : g.allAdjcents(u))
				improv(g, pathes, u, v.first, v.second);
	for (int u = 0; u < g.vertNum(); u++)
		for (auto v : g.allAdjcents(u))
			if (pathes[v.first].first > pathes[u].first + v.second)
				return nullptr;
	return pathes;
}

/*ranks the "importncy" of the nodes on the graph the node is more importans if it is pointed by important nodes
 this function assumes that the graph is weighted with weights betwin 0-1 without self pointing nodes*/
template<class T>
std::vector<std::pair<int, double>> nodeRank(Graph<T> &g,int iteretions=-1,double conv_flag=0.000001,bool self_stay=false,bool prob_weights=true)
{
	std::vector<std::pair<int, double>> ret;
	//init all nodes ranks to be 1/|v|
	std::vector<double>ranks(g.vertNum(), 1.0/g.vertNum());
	int* indexes=new int[g.vertNum()];
	for (int i = 0; i < g.vertNum(); i++)
		indexes[i] = i;
	bool is_conv = true;
	if (iteretions == -1)
		is_conv = false;
	for (int i = 0; i < iteretions || !is_conv; i++)
	{
		std::vector<double>oldranks = ranks;
		ranks = std::vector<double>(g.vertNum(), 0);
		for (int u = 0; u < g.vertNum(); u++)
		{
			double d = oldranks[u];
			int allWeights = 0;
			if(!prob_weights)
				for (auto v : g.allAdjcents(u))
				{
					allWeights += v.second;
				}
			for (auto v : g.allAdjcents(u))
			{
				if (prob_weights)
				{
					ranks[v.first] += oldranks[u] * v.second / g.order(u);
					d -= oldranks[u] * v.second / g.order(u);
				}
				else
				{
					ranks[v.first] += oldranks[u] * v.second / allWeights;
					d -= oldranks[u] * v.second / allWeights;
				}
			}
			if (self_stay)
			{
				ranks[u] += d;
			}
			else
			{
				for (int j = 0; j < g.vertNum(); j++)
				{
					ranks[j] += d / g.vertNum();
				}
			}
		}
		if (getError(ranks, oldranks) <= conv_flag)
		{
			std::cout << "the node rankes converaged in " << i << " iterations\n";
			break;
		}

	}
	margeSortEdges(ranks, indexes, g.vertNum());
	for (int i = 0; i < g.vertNum(); i++)
		ret.push_back(std::pair<int, double>(indexes[i], ranks[indexes[i]]));
	delete indexes;
	return ret;
}

double getError(std::vector<double>&ranks, std::vector<double>& oldranks)
{
	double err = 0;
	for (int i = 0; i < ranks.size(); i++)
		err += abs(ranks[i] - oldranks[i]);
	return err;
}