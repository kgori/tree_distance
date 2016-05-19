#ifndef __BIPARTITE_GRAPH_H__
#define __BIPARTITE_GRAPH_H__
#ifndef BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#define BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#endif
#include <deque>
#include <vector>
#include "PhyloTreeEdge.h"
#include "Vertex.h"

class BipartiteGraph {

public :
    BipartiteGraph(std::vector<std::deque<bool>>& IncidenceMatrix, const std::vector<double>& Aweight, const std::vector<double>& Bweight);
    vector<vector<size_t>> vertex_cover(const vector<size_t>& Aindex, const vector<size_t>& Bindex);
    static std::vector<std::deque<bool>> getIncidenceMatrix(std::vector<PhyloTreeEdge>& edges1, std::vector<PhyloTreeEdge>& edges2);

public :
    std::vector<std::deque<bool>> edge;
    size_t nA, nB, n;
    std::vector<Vertex> Avertex, Bvertex;
};

#endif /* __BIPARTITE_GRAPH_H__ */
