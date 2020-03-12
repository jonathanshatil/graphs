#pragma once
#include "Graph.hpp"
#include <stack>
#include <list>
#include <stdexcept>

template<class T>
class Graph;

template<class T>
class DFSiterator
{
public:
	DFSiterator(Graph<T>& g, bool end=false) :_g(g), _visited(g.vertNum(), false), _curr(0)//if the end flag is true a end iterator is created to the graph
	{
		if (end)
			this->_curr = -1;
		else
		{
			this->_stack.push(0);
			this->operator++();
		}
	}
	DFSiterator(const DFSiterator& other) :_g(other._g), _visited(other._visited),_stack(other._stack),_curr(other._curr){}
	~DFSiterator() = default;
	DFSiterator& operator++() {

		if (_stack.empty())
		{
			for(int i=0;i<_g.vertNum();i++)
				if (!_visited[i])
				{
					_stack.push(i);
					return this->operator++();
				}
			this->_curr = -1;
		}
		else
		{
			_curr = _stack.top();
			_stack.pop();
			if (_visited[_curr])
				return this->operator++();
			for (auto i = _g.adjcentBegin(_curr); i != _g.adjcentEnd(_curr); ++i)
				if (!_visited[(*i).first])
					_stack.push((*i).first);
			_visited[_curr] = true;
		}
		return *this;
	}
	int& operator*() { return _curr; }
	bool operator==(const DFSiterator& other) { return &other._g == &this->_g && other._curr == this->_curr; }
	bool operator!=(const DFSiterator& other) { return &other._g != &this->_g || other._curr != this->_curr; }

private:
	Graph<T>& _g;
	std::vector<bool> _visited;
	int _curr;
	std::stack<int> _stack;
};

template<class T>
class BFSiterator
{
public:
	BFSiterator(Graph<T>& g, bool end = false) :_g(g), _visited(g.vertNum(), false), _curr(0)//if the end flag is true a end iterator is created to the graph
	{
		if (end)
			this->_curr = -1;
		else
		{
			this->_queue.push(0);
			this->operator++();
		}
	}
	BFSiterator(const BFSiterator& other) :_g(other._g), _visited(other._visited), _queue(other._queue), _curr(other._curr) {}
	~BFSiterator() = default;
	BFSiterator& operator++() {

		if (_queue.empty())
		{
			for (int i = 0; i < _g.vertNum(); i++)
				if (!_visited[i])
				{
					_queue.push(i);
					return this->operator++();
				}
			this->_curr = -1;
		}
		else
		{
			_curr = _queue.front();
			_queue.pop();
			for (auto i = _g.adjcentBegin(_curr); i != _g.adjcentEnd(_curr); ++i)
				if (!_visited[(*i).first])
				{
					_visited[(*i).first] = true;
					_queue.push((*i).first);
				}
		}
		return *this;
	}
	int& operator*() { return _curr; }
	bool operator==(const BFSiterator& other) { return &other._g == &this->_g && other._curr == this->_curr; }
	bool operator!=(const BFSiterator& other) { return &other._g != &this->_g || other._curr != this->_curr; }

private:
	Graph<T>& _g;
	std::vector<bool> _visited;
	int _curr;
	std::queue<int> _queue;
};