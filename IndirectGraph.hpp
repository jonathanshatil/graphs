#pragma once
#include "Graph.hpp"
template<class T>
class IndirectGraph :public Graph<T>
{
public:
	IndirectGraph() :Graph<T>() {}
	IndirectGraph(std::vector<T> values) :Graph<T>{ values } {}
	IndirectGraph(IndirectGraph const& other) :Graph<T>{ other._values,other._edges } {}
	IndirectGraph(Graph<T> const& other);//takes a direct graph and convert it to indirect
	IndirectGraph(std::vector<T> values, std::vector<std::vector<std::pair<int,double>>> adjVector) :Graph<T>{ values, adjVector } {}
	IndirectGraph(std::vector<T> values, std::vector<std::vector<int>> adjVector) :Graph<T>{ values,adjVector } {}
	IndirectGraph(std::vector<T> values, std::vector<std::pair<std::pair<int, int>, double>> edgesVector);
	IndirectGraph(std::vector<T> values, std::vector<std::pair<int, int>> edgesVector);

	int edgeNum()const;
	inline Graph<T> getTranspose() const { IndirectGraph<T> transpose(this->_values, this->_edges); return transpose; }

	//dynamic indirect graph methods 
	void addNode(T value, std::vector<int, double>adj);
	void addNode(T value) { this->_values.push_back(value); this->_edges.push_back(std::vector<std::pair<int, double>>()); }
	void addEdgeByIndexs(int a, int b,double weight=1) { this->_edges[a].push_back(std::pair<int, double>(b,weight)); this->_edges[b].push_back(std::pair<int,double>(a, weight)); }
	void removeEdgeByIndexs(int a, int b)
	{
		std::vector<std::pair<int, double>> temp;
		for (auto v : this->_edges[a])
			if (v.first != b)
				temp.push_back(v);
		this->_edges[a] = temp;
		
		for (auto v : this->_edges[b])
			if (v.first != a)
				temp.push_back(v);
		this->_edges[b] = temp;
	}
};

template<class T> int IndirectGraph<T>::edgeNum()const
{
	int sum = 0;
	for (int i = 0; i < this->vertNum(); i++)
		sum += this->order(i);
	return sum / 2;
}
template<class T> IndirectGraph<T>::IndirectGraph(Graph<T> const& other)
{
	std::vector<std::vector<bool>> e(other.vertNum(), std::vector<bool>(other.vertNum(), false));
	this->_edges = std::vector<std::vector<std::pair<int,double>>>(other.vertNum(), std::vector<std::pair<int,double>>());
	for (int i = 0; i < other.vertNum(); i++)
	{
		this->_values.push_back(other[i]);
		for (auto v : other.allAdjcents(i))
		{
			if (!e[i][v.first])
			{
				this->_edges[i].push_back(v);
				e[i][v.first] = true;
				this->_edges[v.first].push_back(std::pair<int,double>(i,v.second));
				e[v.first][i] = true;
			}
		}
	}
}

template<class T> IndirectGraph<T>::IndirectGraph(std::vector<T> values, std::vector<std::pair<int, int>> edgesVector):Graph<T>{values}
{
	std::vector<std::vector<bool>> e(values.size(), std::vector<bool>(values.size(), false));
	for (auto v : edgesVector)
	{
		if (!e[v.first][v.second])
		{
			this->_edges[v.first].push_back(v.second,1);
			e[v.first][v.second] = true;
			this->_edges[v.second].push_back(v.first,1);
			e[v.second][v.first] = true;
		}
	}
}
template<class T> IndirectGraph<T>::IndirectGraph(std::vector<T> values, std::vector<std::pair<std::pair<int, int>, double>> edgesVector) :Graph<T>{ values }
{
	std::vector<std::vector<bool>> e(values.size(), std::vector<bool>(values.size(), false));
	for (auto v : edgesVector)
	{
		if (!e[v.first.first][v.first.second])
		{
			this->_edges[v.first.first].push_back(v.first.second, v.second);
			e[v.first.first][v.first.second] = true;
			this->_edges[v.first.second].push_back(v.first.first, v.second);
			e[v.first.second][v.first.first] = true;
		}
	}
}