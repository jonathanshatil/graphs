#include <iostream>
#include <vector>
#include <string>
#include <list>

#include <limits> // for numeric_limits

#include <set>
#include <utility> // for pair
#include <algorithm>
#include <iterator>

using namespace std;

typedef int vertex_t;
typedef double weight_t;


const weight_t max_weight = std::numeric_limits<double>::infinity();

struct neighbor {
	vertex_t target;

	weight_t weight;

	neighbor(vertex_t arg_target, weight_t arg_weight)
		: target(arg_target), weight(arg_weight) { }

	void operator =(neighbor n) {
		target = n.target;
		weight = n.weight; 
	}
};

typedef std::vector<std::vector<neighbor> > adjacency_list_t;

//void DijkstraComputePaths(vertex_t source,
//	const adjacency_list_t &adjacency_list,
//	std::vector<weight_t> &min_distance,
//	std::vector<vertex_t> &previous);

class CDijkstra {
public:
	CDijkstra(const adjacency_list_t &adjacency_list, vertex_t source) {
		m_source = source;
		m_adjacency_list = adjacency_list;
		int n = adjacency_list.size();
		m_min_distance.clear();
		m_min_distance.resize(n, max_weight);
		m_min_distance[source] = 0;
		m_previous.clear();
		m_previous.resize(n, -1);
	}
	void DijkstraComputePaths();


	std::list<vertex_t> DijkstraGetShortestPathTo(vertex_t vertex);

	weight_t getMinDist(vertex_t vertex) {
		return m_min_distance[vertex];
	}

private:
	adjacency_list_t m_adjacency_list;
	std::vector<weight_t> m_min_distance;
	std::vector<vertex_t> m_previous;
	vertex_t m_source;

};