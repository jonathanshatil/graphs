#pragma once
#include"Graph.hpp"
#include "FibonacciHeap.hpp"
#include <stack>

class vertNode {
private:
	int index;
	double dist;
public:
	vertNode() {}
	vertNode(int i, double d) :index(i), dist(d) {}
	bool operator< (vertNode& other) { return this->dist < other.dist; }
	bool operator> (vertNode& other) { return this->dist > other.dist; }
	int getIndex() { return index; }
};

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

void margeEdges(std::vector<double> weights, int*c,int* a, int* b, int size_a, int size_b);
void margeSortEdges(std::vector<double> weights, int* edges, int edges_size);

template<class T>
std::vector<int> topologicalSort(Graph<T>& g)//returns the topological ssorted order of the nodes
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
	std::vector<std::vector<int>> superAdj(scc.size(), std::vector<int>());
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
	std::vector<bool> coverd(g.vertNum(), false);
	std::vector< std::vector<bool>> used(g.vertNum(), std::vector<bool>(g.vertNum(), false));
	std::vector<bool> tv(g.vertNum(),false);
	tv[0] = true;
	int size = g.edgeNum();
	int* order = new int[size];
	std::vector<std::pair<int, int>> edges;
	std::vector<double> weights;
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
	margeSortEdges(weights, order,size);
	//adding the minimal edge that doesnt creats a cylce
	int mstEdges = 0;
	for (int i = 0; i < edges.size() && mstEdges < g.vertNum() - 1; i++)
	{
		if (tv[edges[order[i]].first] && !tv[edges[order[i]].second] || !tv[edges[order[i]].first] && tv[edges[order[i]].second])
		{
			mst.addEdgeByIndexs(edges[order[i]].first, edges[order[i]].second,weights[order[i]]);
			tv[edges[order[i]].first] = true;
			tv[edges[order[i]].second] = true;
			mstEdges++;
		}
	}
	return mst;
}


void margeEdges(std::vector<double> weights, int* c, int* a, int* b,int size_a, int size_b)
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

void margeSortEdges(std::vector<double> weights, int* edges, int edges_size)
{
	if (edges_size == 1)
		return;
	int size = edges_size / 2;
	margeSortEdges(weights, edges, size);
	margeSortEdges(weights, edges + size, edges_size - size);
	return margeEdges(weights, edges, edges, edges+size, size, edges_size - size);
}

template<class T>
std::vector<double> initEstimate(Graph<T>& g,int s)
{
	std::vector<double> d(g.vertNum(), INFINITY);
	d[s] = 0;
	return d;
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
	std::vector<node<vertNode>*> nodes;
	std::vector<std::pair<double, std::vector<int>>> pathes(g.vertNum(), std::pair<double,std::vector<int>>(INFINITY, std::vector<int>()));
	pathes[s].first = 0;
	FibonacciHeap<vertNode> vertex_queue;
	for (int i = 0; i < g.vertNum(); i++)
		nodes.push_back(vertex_queue.insert(vertNode(i, pathes[i].first)));
	while(!vertex_queue.isEmpty())
	{
		int u = vertex_queue.removeMinimum().getIndex();
		for (auto v : g.allAdjcents(u))
		{
			improv(g,pathes, u, v.first,v.second);
			vertex_queue.decreaseKey(nodes[v.first], vertNode(v.first, pathes[v.first].first));
		}
	}
	return pathes;	
}

template<class T> std::vector<std::pair<double, std::vector<int>>> computePaths(Graph<T> &g, int s)//works only on a graph without cycles
{
	std::vector<std::pair<double, std::vector<int>>> pathes(g.vertNum(), std::pair<double, std::vector<int>>(INFINITY, std::vector<int>()));
	pathes[s].first = 0;
	std::vector<int> order = topologicalSort(g);
	std::vector<double> d = initEstimate(g, s);
	for (auto u : order)
		for (auto v : g.allAdjcents(u))
			improv(g ,pathes, u, v.first, v.second);
	return pathes;
}

//floyd warshel algorithem
template<class T> std::vector<std::vector<double>> computeAlldists(Graph<T> &g)//works only on a graph without negativ cycles
{
	std::vector<std::vector<double>> dists(g.vertNum(),std::vector<double>(g.vertNum(), INFINITY));
	for (int u = 0; u < g.vertNum();u++)
		for (auto v : g.allAdjcents(u))
			dists[u][v.first] = v.second;
	for (int u = 0; u < g.vertNum(); u++)
		dists[u][u] = 0;
	for (int k = 0; k < g.vertNum(); k++)
		for (int i = 0; i < g.vertNum(); i++)
			for (int j = 0; j < g.vertNum(); j++)
				if (dists[i][j] > dists[i][k] + dists[k][j])
					dists[i][j] = dists[i][k] + dists[k][j];
	return dists;
}

