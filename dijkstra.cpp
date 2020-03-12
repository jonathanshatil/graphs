#include "dijkstra.h"
 
void CDijkstra::DijkstraComputePaths()
{
    std::set<std::pair<weight_t, vertex_t> > vertex_queue;
    vertex_queue.insert(std::make_pair(m_min_distance[m_source], m_source));
 
    while (!vertex_queue.empty()) 
    {
        weight_t dist = vertex_queue.begin()->first;
        vertex_t u = vertex_queue.begin()->second;
        vertex_queue.erase(vertex_queue.begin());
 
        // Visit each edge exiting u
	    const std::vector<neighbor> &neighbors = m_adjacency_list[u];
        for (std::vector<neighbor>::const_iterator neighbor_iter = neighbors.begin();
             neighbor_iter != neighbors.end();
             neighbor_iter++)
        {
            vertex_t v = neighbor_iter->target;
            weight_t weight = neighbor_iter->weight;
            weight_t distance_through_u = dist + weight;
	    if (distance_through_u < m_min_distance[v]) {
	        vertex_queue.erase(std::make_pair(m_min_distance[v], v));
 
	        m_min_distance[v] = distance_through_u;
	        m_previous[v] = u;
	        vertex_queue.insert(std::make_pair(m_min_distance[v], v));
 
	    }
 
        }
    }
}
 
 
std::list<vertex_t> CDijkstra::DijkstraGetShortestPathTo(vertex_t vertex)
{
    std::list<vertex_t> path;
    for ( ; vertex != -1; vertex = m_previous[vertex])
        path.push_front(vertex);
    return path;
}
 