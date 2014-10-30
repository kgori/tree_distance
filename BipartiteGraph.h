#ifndef __BIPARTITE_GRAPH_H__
#define __BIPARTITE_GRAPH_H__

#include <deque>
#include <vector>
#include "PhyloTreeEdge.h"
#include "Vertex.h"

class BipartiteGraph {

public :
    BipartiteGraph(std::vector<std::deque<bool>> IncidenceMatrix, std::vector<double> Aweight, std::vector<double> Bweight);
    vector<vector<size_t>> vertex_cover(vector<size_t> Aindex, vector<size_t> Bindex);
    static std::vector<std::deque<bool>> getIncidenceMatrix(std::vector<PhyloTreeEdge> edges1, std::vector<PhyloTreeEdge> edges2);

public :
    std::vector<std::deque<bool>> edge;
    size_t nA, nB, n;
    std::vector<Vertex> Avertex, Bvertex;
};

#endif /* __BIPARTITE_GRAPH_H__ */
