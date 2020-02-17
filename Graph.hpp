#pragma once
#include <iostream>
#include <queue>
#include <vector>

template<class T>
class Graph
{
protected:
	std::vector<T> _values;
	std::vector<std::vector<std::pair<int, double>>> _edges;
	bool is_reachableUtil(int a, int b, bool* visited)const;
public:
	//constractors
	Graph() = default;
	Graph(std::vector<T> values) :_values(values) {}
	Graph(Graph const& other) :_values(other._values), _edges(other._edges) {}
	Graph(std::vector<T> values, std::vector<std::vector<std::pair<int, double>>> edges) :_values(values), _edges(edges) {}
	Graph(std::vector<T> values, std::vector<std::vector<int>> adjVector);
	Graph(std::vector<T> values, std::vector<std::pair<std::pair<int, int>, double>> edgesVector);
	Graph(std::vector<T> values, std::vector<std::pair<int, int>> edgesVector);
	~Graph() = default;
	//basic graph methods
	bool is_adjacent(int a, int b) const;
	T operator[] (int i) const { return _values[i]; }
	inline void setValue(int index, T val) { _values[index] = val; }
	inline std::vector<std::pair<int, double>> allAdjcents(int a)const { return _edges[a]; }
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
		std::vector<std::vector<std::pair<int, double>>> union_edges=_edges;
		for (int i = 0; i < other.vertNum(); i++)
		{
			union_vals.push_back(other._values[i]);
			union_edges.push_back(std::vector<std::pair<int, double>>());
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
	virtual void addNode(T value) { this->_values.push_back(value); this->_edges.push_back(std::vector<std::pair<int, double>>()); }
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
	virtual inline void addEdgeByIndexs(int a, int b,double weight=1) { this->_edges[a].push_back(std::pair<int,double>(b,weight)); }
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
		std::vector<std::pair<int, double>> temp;
		for (auto v : _edges[a])
			if (v.first != b)
				temp.push_back(v);
		_edges[a] = temp;
	}
};

template<class T>
bool Graph<T>::is_adjacent(int a, int b) const
{
	for (int i = 0; i < _edges[a].size(); i++)
		if (_edges[a][i].first == b)
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
	std::vector<std::vector<int>> adjVector;
	std::vector<T> values;
	for (int i = 0; i < verticis.size(); i++)
	{
		values.push_back(this->_values[verticis[i]]);
		adjVector.push_back(std::vector<int>());
		for (int j = 0; j < verticis.size(); j++)
			if (this->is_adjacent(verticis[i], verticis[j]))
				adjVector[i].push_back(j);
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
	std::vector<std::vector<std::pair<int, double>>> adj(vertNum(), std::vector<std::pair<int, double>>());
	for (int i = 0; i < vertNum(); i++)
		for (auto v : _edges[i])
			adj[v.first].push_back(std::pair<int,double>(i, v.second));
	Graph<T> transpose(this->_values, adj);
	return transpose;
}
template<class T> Graph<T>::Graph(std::vector<T> values, std::vector<std::vector<int>> adjVector):_values(values)
{
	this->_edges = std::vector<std::vector<std::pair<int, double>>>(values.size(), std::vector<std::pair<int, double>>());
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