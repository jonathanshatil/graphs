/*written by yonatan shatil*/
#pragma once
#include <iostream>
#include <queue>
#include <vector>
#include <list>
#include "GraphIterator.hpp"
//#include "MetrixGraph.hpp"
template<class T>
class MatrixGraph;
	
template<class T>
class Graph
{
protected:
	std::vector<T> _values;
	std::vector<std::list<std::pair<int, double>>> _edges;
	bool is_reachableUtil(int a, int b, bool* visited)const;
public:
	//constractors
	Graph() = default;
	Graph(std::vector<T> values) :_values(values) { _edges.resize(values.size()); }
	Graph(Graph const& other) :_values(other._values), _edges(other._edges) {}
	Graph(std::vector<T> values, std::vector<std::list<std::pair<int, double>>> edges) :_values(values), _edges(edges) {}
	Graph(std::vector<T> values, std::vector<std::list<int>> adjVector);
	Graph(std::vector<T> values, std::vector<std::pair<std::pair<int, int>, double>> edgesVector);
	Graph(std::vector<T> values, std::vector<std::pair<int, int>> edgesVector);
	Graph(MatrixGraph<T>& g)
	{
		for (int i = 0; i < g.vertNum(); i++)
		{
			this->_values.push_back(g[i]);
			this->_edges.push_back(g.allAdjcents(i));
		}
	}
	~Graph() = default;
	//basic graph methods
	bool is_adjacent(int a, int b) const;
	T operator[] (int i) const { return _values[i]; }
	inline void setValue(int index, T val) { _values[index] = val; }
	inline std::list<std::pair<int, double>> allAdjcents(int a)const { return _edges[a]; }
	inline std::list<std::pair<int, double>>::iterator adjcentBegin(int a) { return _edges[a].begin(); }
	inline std::list<std::pair<int, double>>::iterator adjcentEnd(int a) { return _edges[a].end(); }
	bool is_reachable(int a, int b)const;
	inline bool are_equal(int a, int b)const { return this->_values[a] == this->_values[b]; }
	int getIndex(T val)const;
	virtual int edgeNum()const;
	inline int order(int a)const { return this->_edges[a].size(); }
	inline int vertNum()const { return this->_values.size(); }
	double getWeight(int a, int b);
	Graph getSubGraph(std::vector<int> verticis)const;
	std::pair<T, int> getMax()const;//there must be a function that can comper 2 nodes (operator <= for T)
	std::pair<T, int> getMin()const;//there must be a function that can comper 2 nodes (operator <= for T)

	virtual Graph getTranspose() const;
	Graph getUnion(Graph  const& other) const
	{
		int offset = this->vertNum();
		std::vector<T> union_vals = this->_values;
		std::vector<std::list<std::pair<int, double>>> union_edges=_edges;
		for (int i = 0; i < other.vertNum(); i++)
		{
			union_vals.push_back(other._values[i]);
			union_edges.push_back(std::list<std::pair<int, double>>());
		}
		for (int i = 0; i < other.vertNum(); i++)
			for (auto v : other._edges[i])
				union_edges[i].push_back(std::pair<int, double>(v.first + offset, v.second));
		Graph<T> unionG(union_vals, union_edges);
		return unionG;
	}


	//dinamic graph
	void addNode(T value, std::vector<std::pair<int,double>> points_to, std::vector<std::pair<int, double>> pointed_from)
	{
		_values.push_back(value);
		_edges.push_back(points_to);
		for (auto v : pointed_from)
		{
			_edges[v.first].push_back(std::pair<int, double>(vertNum() - 1, v.second));
		}
	}
	virtual void addNode(T value) { this->_values.push_back(value); this->_edges.push_back(std::list<std::pair<int, double>>()); }
	void removeNode(int a)
	{
		std::vector<T> temp;
		std::vector<std::vector<std::pair<int, double>>> tempEdges;
		int i = 0;
		while (temp.size() < vertNum() - 1)
		{
			if (i != a)
				temp.push_back(_values[i]);
			i++;
		}
		i = 0;
		while(tempEdges.size() < vertNum() - 1)
		{
			if (i != a)
			{
				std::vector<std::pair<int, double>> vec;
				for (auto v : _edges[i])
				{
					if(v!=a)
					{
						v = (v < a) ? v : v - 1;
						vec.push_back(v);
					}
				}
				tempEdges.push_back(vec);
			}
			i++;
		}
		_values = temp;
		_edges = tempEdges;
	}
	void removeValue(T val)
	{
		int i = 0;
		while (i=getIndex(val)!=-1)
			removeNode(i);
	}
	virtual inline void addEdgeByIndexs(int a, int b, double weight = 1) { if (!is_adjacent(a, b)) { this->_edges[a].push_back(std::pair<int, double>(b, weight)); } }
	void addEdgeByValues(T a, T b)
	{
		int ia = getIndex(a);
		int ib = getIndex(b);
		addEdgeByIndexs(ia, ib);
	}
	void removeEdgeByValues(T a, T b)
	{
		int ia = getIndex(a);
		int ib = getIndex(b);
		removeEdgeByIndexs(ia, ib);
	}
	virtual void removeEdgeByIndexs(int a, int b)
	{
		std::list<std::pair<int, double>> temp;
		for (auto v : _edges[a])
			if (v.first != b)
				temp.push_back(v);
		_edges[a] = temp;
	}
	inline DFSiterator<T> dfsBegin(){return DFSiterator<T>(*this);}
	inline DFSiterator<T> dfsEnd(){return DFSiterator<T>(*this,true);}
	inline BFSiterator<T> bfsBegin() { return BFSiterator<T>(*this); }
	inline BFSiterator<T> bfsEnd() { return BFSiterator<T>(*this, true); }
};

template<class T>
bool Graph<T>::is_adjacent(int a, int b) const
{
	for (auto v:_edges[a])
		if (v.first == b)
			return true;
	return false;
}

template<class T>
int Graph<T>::getIndex(T val)const
{
	for(int i = 0; i < this->vertNum(); i++)
		if (val == this->_values[i])
			return i;
	return -1;
}

template<class T>
int Graph<T>::edgeNum()const
{
	int sum = 0;
	for(int i = 0; i < this->vertNum(); i++)
		sum += this->order(i);
	return sum;
}

template<class T>
Graph<T> Graph<T>::getSubGraph(std::vector<int> verticis)const
{
	std::vector<std::list<std::pair<int,double>>> adjVector;
	std::vector<T> values;
	int i = 0;
	for (int i = 0; i < verticis.size(); i++)
	{
		values.push_back(this->_values[verticis[i]]);
		adjVector.push_back(std::list<std::pair<int, double>>());
		for(int j=0;j<verticis.size();j++)
			for(auto e : _edges[verticis[i]])
				if(e.first==verticis[j])
					adjVector[i].push_back(std::pair<int,double>(j,e.second));
	}
	return Graph(values, adjVector);
}

template<class T>
std::pair<T, int> Graph<T>::getMax()const
{
	int maxIndex = 0;
	for (int i = 0; i < this->vertNum(); i++)
		if (this->_values[i] > this->_values[maxIndex])
			maxIndex = i;
	return std::pair<T, int>(this->_values[maxIndex], maxIndex);
}

template<class T>
std::pair<T, int> Graph<T>::getMin()const
{
	int minIndex = 0;
	for (int i = 0; i < this->vertNum(); i++)
		if (this->_values[i] < this->_values[minIndex])
			minIndex = i;
	return std::pair<T, int>(this->_values[minIndex], minIndex);
}

template<class T>
bool Graph<T>::is_reachable(int a, int b)const
{
	bool* visited = new bool[vertNum()];
	for (int i = 0;i < vertNum(); i++)
		visited[i] = false;
	bool ret = is_reachableUtil(a, b, visited);
	delete visited;
	return ret;
}

template<class T>
bool Graph<T>::is_reachableUtil(int a, int b,bool* visited)const
{
	if (a == b)return true;
	visited[a] = true;
	for (auto v : allAdjcents(a))
		if (!visited[v.first])
			if (is_reachableUtil(v.first, b, visited))
				return true;
	return false;
}

template<class T> Graph<T> Graph<T>::getTranspose()const
{
	std::vector<std::list<std::pair<int, double>>> adj(vertNum(), std::list<std::pair<int, double>>());
	for (int i = 0; i < vertNum(); i++)
		for (auto v : _edges[i])
			adj[v.first].push_back(std::pair<int,double>(i, v.second));
	Graph<T> transpose(this->_values, adj);
	return transpose;
}
template<class T> Graph<T>::Graph(std::vector<T> values, std::vector<std::list<int>> adjVector):_values(values)
{
	this->_edges = std::vector<std::list<std::pair<int, double>>>(values.size(), std::list<std::pair<int, double>>());
	for (int i=0;i<adjVector.size();i++)
		for (auto v : adjVector[i])
			this->_edges[i].push_back(std::pair<int, double>(v, 1));//all weights are 1
}

template<class T> Graph<T>::Graph(std::vector<T> values, std::vector<std::pair<int, int>> edgesVector) :_values(values)
{
	this->_edges = std::vector<std::vector<std::pair<int, double>>>(values.size(), std::vector<std::pair<int, double>>());
	for (auto v : edgesVector)
		this->_edges[v.first].push_back(std::pair<int, double>(v.second, 1));
}

template<class T> Graph<T>::Graph(std::vector<T> values, std::vector<std::pair<std::pair<int, int>, double>> edgesVector) :_values(values)
{
	this->_edges = std::vector<std::vector<std::pair<int, double>>>(values.size(), std::vector<std::pair<int, double>>());
	for (auto v : edgesVector)
		this->_edges[v.first.first].push_back(std::pair<int, double>(v.first.second, v.second));
}

template<class T>
double Graph<T>::getWeight(int a, int b)
{
	for (auto e : this->_edges[a])
		if (e.first == b)
			return e.second;
}


template<class T>
class MatrixGraph
{
public:
	//constractors
	MatrixGraph() = default;
	MatrixGraph(std::vector<T> values) :_values(values) {}
	MatrixGraph(MatrixGraph const& other) :_values(other._values), _adjMat(other._adjMat) {}
	MatrixGraph(std::vector<T> values, std::vector<std::vector<double>> mat) :_values(values), _adjMat(mat) {}
	MatrixGraph(std::vector<T> values, std::vector<std::vector<int>> mat) :_values(values), _adjMat(mat) {}
	MatrixGraph(std::vector<T> values, std::vector<std::pair<std::pair<int, int>, double>> edgesVector);
	MatrixGraph(std::vector<T> values, std::vector<std::pair<int, int>> edgesVector);
	MatrixGraph(Graph<T>& g)
	{
		for (int i = 0; i < g.vertNum(); i++)
		{
			_values.push_back(g[i]);
			_adjMat.push_back(std::vector<double>(g.vertNum(), 0));
			for (auto v : g.allAdjcents(i))
			{
				addEdgeByIndexs(i, v.first, v.second);
			}
		}
	}
	~MatrixGraph() = default;
	//basic graph methods
	inline bool is_adjacent(int a, int b) const { return _adjMat[a][b] != 0; }
	T operator[] (int i) const { return _values[i]; }
	inline void setValue(int index, T val) { _values[index] = val; }
	std::vector<std::pair<int, double>> allAdjcents(int a)const
	{
		std::vector<std::pair<int, double>> ret;
		for (int i = 0; i < vertNum(); i++)
			if (_adjMat[a][i] != 0)
				ret.push_back(std::pair<int, double>(i, _adjMat[a][i]));
		return ret;
	}
	bool is_reachable(int a, int b)const;
	inline bool are_equal(int a, int b)const { return this->_values[a] == this->_values[b]; }
	int getIndex(T val)const;
	virtual int edgeNum()const;
	inline int order(int a)const
	{
		int ord = 0;
		for (int i = 0; i < vertNum(); i++)
			if (_adjMat[a][i] != 0)
				ord++;
		return ord;
	}
	inline int vertNum()const { return this->_values.size(); }
	double getWeight(int a, int b) { return _adjMat[a][b]; }
	MatrixGraph getSubGraph(std::vector<int> verticis)const;
	std::pair<T, int> getMax()const;//there must be a function that can comper 2 nodes (operator <= for T)
	std::pair<T, int> getMin()const;//there must be a function that can comper 2 nodes (operator <= for T)

	virtual MatrixGraph getTranspose() const;
	MatrixGraph getUnion(MatrixGraph  const& other) const 
	{
		MatrixGraph unionG = *this;
		for (auto v : other._values)
			unionG._values.push_back(v);
		for (int i = 0; i < other.vertNum(); i++)
		{
			std::vector<double> temp(this->vertNum(), 0);
			for (int j = 0; j < other.vertNum(); j++)
				temp.push_back(other._adjMat[i][j]);
			for (int j = 0; j < this->vertNum(); j++)
				unionG._adjMat[j].push_back(0);
			unionG._adjMat.push_back(temp);
		}
		return unionG;
	}

	//dinamic graph
	void addNode(T value, std::vector<std::pair<int, double>> points_to, std::vector<std::pair<int, double>> pointed_from)
	{
		addNode(value);
		for (auto v : points_to)
		{
			addEdgeByIndexs(vertNum() - 1, v.first, v.second);
		}
		for (auto v : pointed_from)
		{
			addEdgeByIndexs(v.first, vertNum() - 1, v.second);
		}
	}
	void addNode(T value)
	{
		_adjMat.push_back(std::vector<double>(vertNum(), 0));
		_values.push_back(value);
		for (int i = 0; i < vertNum(); i++)
			_adjMat[i].push_back(0);
	}
	void removeNode(int a)
	{
		if (a < 0 || a >= this->vertNum())
			return;
		this->_values.erase(this->_values.begin()+a);
		this->_adjMat.erase(this->_adjMat.begin() + a);
		for(auto v:this->_adjMat)
			v.erase(v.begin() + a);
	}
	void removeValue(T val)
	{
		int i = 0;
		while (i = getIndex(val) != -1)
			removeNode(i);
	}
	virtual inline void addEdgeByIndexs(int a, int b, double weight = 1) { this->_adjMat[a][b] = weight; }
	void addEdgeByValues(T a, T b, double weight = 1)
	{
		int ia = getIndex(a);
		int ib = getIndex(b);
		addEdgeByIndexs(ia, ib, weight);
	}
	void removeEdgeByValues(T a, T b)
	{
		int ia = getIndex(a);
		int ib = getIndex(b);
		removeEdgeByIndexs(ia, ib);
	}
	inline virtual void removeEdgeByIndexs(int a, int b) { this->_adjMat[a][b] = 0; }
private:
	std::vector<T> _values;
	std::vector<std::vector<double>> _adjMat;
	bool MatrixGraph<T>::is_reachableUtil(int a, int b, bool* visited)const;
};

template<class T>
int MatrixGraph<T>::getIndex(T val)const
{
	for (int i = 0; i < this->vertNum(); i++)
		if (val == this->_values[i])
			return i;
	return -1;
}

template<class T>
int MatrixGraph<T>::edgeNum()const
{
	int sum = 0;
	for (int i = 0; i < this->vertNum(); i++)
		sum += this->order(i);
	return sum;
}

template<class T>
MatrixGraph<T> MatrixGraph<T>::getSubGraph(std::vector<int> verticis)const
{
	std::vector<std::vector<double>> adjMat(verticis.size(), std::vector<double>(verticis.size(), 0));
	std::vector<T> values;
	for (int i = 0; i < verticis.size(); i++)
	{
		values.push_back(this->_values[verticis[i]]);
		for (int j = 0; j < verticis.size(); j++)
			if (this->is_adjacent(verticis[i], verticis[j]))
				adjMat[i][j] = getWeight(i, j);
	}
	return MetrixGraph(values, adjMat);
}

template<class T>
std::pair<T, int> MatrixGraph<T>::getMax()const
{
	int maxIndex = 0;
	for (int i = 0; i < this->vertNum(); i++)
		if (this->_values[i] > this->_values[maxIndex])
			maxIndex = i;
	return std::pair<T, int>(this->_values[maxIndex], maxIndex);
}

template<class T>
std::pair<T, int> MatrixGraph<T>::getMin()const
{
	int minIndex = 0;
	for (int i = 0; i < this->vertNum(); i++)
		if (this->_values[i] < this->_values[minIndex])
			minIndex = i;
	return std::pair<T, int>(this->_values[minIndex], minIndex);
}

template<class T>
bool MatrixGraph<T>::is_reachable(int a, int b)const
{
	bool* visited = new bool[vertNum()];
	for (int i = 0; i < vertNum(); i++)
		visited[i] = false;
	bool ret = is_reachableUtil(a, b, visited);
	delete visited;
	return ret;
}

template<class T>
bool MatrixGraph<T>::is_reachableUtil(int a, int b, bool* visited)const
{
	if (a == b)return true;
	visited[a] = true;
	for (auto v : allAdjcents(a))
		if (!visited[v.first])
			if (is_reachableUtil(v.first, b, visited))
				return true;
	return false;
}

template<class T> MatrixGraph<T> MatrixGraph<T>::getTranspose()const
{
	std::vector<std::vector<double>> adj(vertNum(), std::vector<double>(vertNum(), 0));
	for (int i = 0; i < vertNum(); i++)
		for (int j = 0; j < vertNum(); j++)
			adj[j][i] = adj[i][j];
	MatrixGraph<T> transpose(this->_values, adj);
	return transpose;
}

template<class T> MatrixGraph<T>::MatrixGraph(std::vector<T> values, std::vector<std::pair<int, int>> edgesVector) :_values(values)
{
	this->_adjMat = std::vector<std::vector<double>>(values.size(), std::vector<double>(values.size(), 0));
	for (auto v : edgesVector)
		this->_adjMat[v.first][v.second] = 1;
}

template<class T> MatrixGraph<T>::MatrixGraph(std::vector<T> values, std::vector<std::pair<std::pair<int, int>, double>> edgesVector) :_values(values)
{
	this->_adjMat = std::vector<std::vector<double>>(values.size(), std::vector<double>(values.size(), 0));
	for (auto v : edgesVector)
		this->_adjMat[v.first.first][v.first.second] = v.second;
}
