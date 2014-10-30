#ifndef __PHYLOTREE_H__
#define __PHYLOTREE_H__

#include "Bipartition.h"
#include "EdgeAttribute.h"
#include "PhyloTreeEdge.h"
#include <string>
#include <vector>

using namespace std;

class PhyloTree {
public:
    PhyloTree(vector<PhyloTreeEdge> edges, vector<string> leaf2NumMap, vector<EdgeAttribute> leafEdgeLengths);

    PhyloTree(vector<PhyloTreeEdge> edges, vector<string> leaf2NumMap);

    PhyloTree(const PhyloTree &t); // copy-constructor

    PhyloTree(string t, bool rooted);

    static vector<PhyloTreeEdge> getCommonEdges(PhyloTree &t1, PhyloTree &t2);

    vector<PhyloTreeEdge> getEdges();

    void setEdges(vector<PhyloTreeEdge> edges);

    PhyloTreeEdge getEdge(size_t i);

    vector<string> getLeaf2NumMap();

    void setLeaf2NumMap(vector<string> leaf2NumMap);

    EdgeAttribute getAttribOfSplit(Bipartition edge);

    vector<Bipartition> getSplits();

    void normalize();

    void normalize(PhyloTree &other);

    size_t numLeaves();

    double getDistanceFromOrigin();

    double getDistanceFromOrigin() const;

    double getDistanceFromOriginNoLeaves();

    double getBranchLengthSum();

    vector<PhyloTreeEdge> getEdgesNotInCommonWith(PhyloTree t);

    PhyloTree clone();

    bool equals(PhyloTree t);

    bool removeSplit(Bipartition e);

    void removeSplits(vector<Bipartition> splits);

    vector<EdgeAttribute> getLeafEdgeAttribs();

    void setLeafEdgeAttribs(vector<EdgeAttribute> leafEdgeAttribs);

    vector<EdgeAttribute> getCopyLeafEdgeAttribs();

    vector<double> getIntEdgeAttribNorms();

    string getNewick(bool branchLengths);

    void setNewick(string newick);

    int numEdges();

    void addEdge(PhyloTreeEdge e) { edges.push_back(e); }

    string toString();

private:
    vector<PhyloTreeEdge> edges;
    vector<string> leaf2NumMap;
    vector<EdgeAttribute> leafEdgeAttribs;
    string newick;

    void setLeaf2NumMapFromNewick();

    void normalize(double constant);
};

#endif /* __PHYLOTREE_H__ */
