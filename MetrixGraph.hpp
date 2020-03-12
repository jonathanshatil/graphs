#pragma once
#include <vector>
template<class T>
class MetrixGraph
{
public:
	//constractors
	MetrixGraph() = default;
	MetrixGraph(std::vector<T> values) :_values(values) {}
	MetrixGraph(MetrixGraph const& other) :_values(other._values), _adjMat(other._adjMat) {}
	MetrixGraph(std::vector<T> values, std::vector<std::vector<double>> mat) :_values(values), _adjMat(mat) {}
	MetrixGraph(std::vector<T> values, std::vector<std::vector<int>> mat) :_values(values), _adjMat(mat) {}
	MetrixGraph(std::vector<T> values, std::vector<std::pair<std::pair<int, int>, double>> edgesVector);
	MetrixGraph(std::vector<T> values, std::vector<std::pair<int, int>> edgesVector);
	~MetrixGraph() = default;
	//basic graph methods
	inline bool is_adjacent(int a, int b) const { return _adjMat[a][b] != 0; }
	T operator[] (int i) const { return _values[i]; }
	inline void setValue(int index, T val) { _values[index] = val; }
	std::vector<std::pair<int, double>> allAdjcents(int a)const;
	bool is_reachable(int a, int b)const;
	inline bool are_equal(int a, int b)const { return this->_values[a] == this->_values[b]; }
	int getIndex(T val)const;
	virtual int edgeNum()const;
	inline int order(int a)const;
	inline int vertNum()const { return this->_values.size(); }
	double getWeight(int a, int b) { return _adjMat[a][b]; }
	MetrixGraph getSubGraph(std::vector<int> verticis)const;
	std::pair<T, int> getMax()const;//there must be a function that can comper 2 nodes (operator <= for T)
	std::pair<T, int> getMin()const;//there must be a function that can comper 2 nodes (operator <= for T)

	virtual MetrixGraph getTranspose() const;
	MetrixGraph getUnion(MetrixGraph  const& other) const;

	//difrent multiplaying methods
	MetrixGraph cartesianMul(Graph const& other) const;
	MetrixGraph tensorMul(Graph  const& other) const;
	MetrixGraph lexicoraphMul(Graph  const& other) const;
	MetrixGraph strongMul(Graph  const& other) const;


	//dinamic graph
	void addNode(T value, std::vector<int> points_to, std::vector<int> pointed_from);
	void addNode(T value);
	void removeNode(int a);
	void removeValue(T val);
	virtual inline void addEdgeByIndexs(int a, int b, double weight = 1) { this->_edges[a].push_back(std::pair<int, double>(b, weight)); }
	void addEdgeByValues(T a, T b);
	inline void removeEdgeByValues(T a, T b);
	virtual void removeEdgeByIndexs(int a, int b) { this->_adjMat[a][b] = 0; }
	void uniteVerticisByIndexs(int a, int b);
	void uniteVerticisByValues(T a, T b);//must be uniqe graph(each value exist only once)
private:
	std::vector<T> _values;
	std::vector<std::vector<double>> _adjMat;
	bool MetrixGraph<T>::is_reachableUtil(int a, int b, bool* visited)const;
};

template<class T>
int MetrixGraph<T>::getIndex(T val)const
{
	for (int i = 0; i < this->vertNum(); i++)
		if (val == this->_values[i])
			return i;
	return -1;
}

template<class T>
int MetrixGraph<T>::edgeNum()const
{
	int sum = 0;
	for (int i = 0; i < this->vertNum(); i++)
		sum += this->order(i);
	return sum;
}

template<class T>
MetrixGraph<T> MetrixGraph<T>::getSubGraph(std::vector<int> verticis)const
{
	std::vector<std::vector<double>> adjMat(verticis.size(),std::vector<double>(verticis.size(),0));
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
std::pair<T, int> MetrixGraph<T>::getMax()const
{
	int maxIndex = 0;
	for (int i = 0; i < this->vertNum(); i++)
		if (this->_values[i] > this->_values[maxIndex])
			maxIndex = i;
	return std::pair<T, int>(this->_values[maxIndex], maxIndex);
}

template<class T>
std::pair<T, int> MetrixGraph<T>::getMin()const
{
	int minIndex = 0;
	for (int i = 0; i < this->vertNum(); i++)
		if (this->_values[i] < this->_values[minIndex])
			minIndex = i;
	return std::pair<T, int>(this->_values[minIndex], minIndex);
}

template<class T>
bool MetrixGraph<T>::is_reachable(int a, int b)const
{
	bool* visited = new bool[vertNum()];
	for (int i = 0; i < vertNum(); i++)
		visited[i] = false;
	bool ret = is_reachableUtil(a, b, visited);
	delete visited;
	return ret;
}

template<class T>
bool MetrixGraph<T>::is_reachableUtil(int a, int b, bool* visited)const
{
	if (a == b)return true;
	visited[a] = true;
	for (auto v : allAdjcents(a))
		if (!visited[v.first])
			if (is_reachableUtil(v.first, b, visited))
				return true;
	return false;
}

template<class T> MetrixGraph<T> MetrixGraph<T>::getTranspose()const
{
	std::vector<std::vector<double>> adj(vertNum(), std::vector<double>(vertNum(),0));
	for (int i = 0; i < vertNum(); i++)
		for (int j = 0; j < vertNum(); j++)
			adj[j][i]=adj[i][j];
	MetrixGraph<T> transpose(this->_values, adj);
	return transpose;
}

template<class T> MetrixGraph<T>::MetrixGraph(std::vector<T> values, std::vector<std::pair<int, int>> edgesVector) :_values(values)
{
	this->_adjMat = std::vector<std::vector<double>>(values.size(), std::vector<double>(values.size(), 0));
	for (auto v : edgesVector)
		this->_adjMat[v.first][v.second] =1;
}

template<class T> MetrixGraph<T>::MetrixGraph(std::vector<T> values, std::vector<std::pair<std::pair<int, int>, double>> edgesVector) :_values(values)
{
	this->_adjMat = std::vector<std::vector<double>>(values.size(), std::vector<double>(values.size(), 0));
	for (auto v : edgesVector)
		this->_adjMat[v.first.first][v.first.second] = v.second;
}
