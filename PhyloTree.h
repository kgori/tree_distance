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
    PhyloTree(vector<PhyloTreeEdge> &edges, vector<string> &leaf2NumMap, vector<EdgeAttribute> &leafEdgeLengths);

    PhyloTree(vector<PhyloTreeEdge> &edges, vector<string> &leaf2NumMap);

    PhyloTree(const PhyloTree &t); // copy-constructor

    PhyloTree(string t, bool rooted);

    static vector<PhyloTreeEdge> getCommonEdges(PhyloTree &t1, PhyloTree &t2);

    vector<PhyloTreeEdge> getEdges();

    void setEdges(vector<PhyloTreeEdge> edges);

    PhyloTreeEdge getEdge(size_t i);

    vector<string> getLeaf2NumMap();

    vector<string> getLeaf2NumMap() const;

    void setLeaf2NumMap(vector<string> leaf2NumMap);

    EdgeAttribute getAttribOfSplit(Bipartition& edge);

    vector<Bipartition> getSplits();

    void normalize();

    void normalize(PhyloTree &other);

    size_t numLeaves();

    double getDistanceFromOrigin();

    double getDistanceFromOrigin() const;

    double getDistanceFromOriginNoLeaves();

    double getBranchLengthSum();

    vector<PhyloTreeEdge> getEdgesNotInCommonWith(PhyloTree &t);

    PhyloTree clone();

    bool equals(PhyloTree t);

    bool removeSplit(Bipartition const &e);

    void removeSplits(const vector<Bipartition>& splits);

    vector<EdgeAttribute> getLeafEdgeAttribs();

    vector<EdgeAttribute> getLeafEdgeAttribs() const;

    void setLeafEdgeAttribs(vector<EdgeAttribute>& leafEdgeAttribs);

    vector<EdgeAttribute> getCopyLeafEdgeAttribs();

    vector<double> getIntEdgeAttribNorms();

    string getNewick(bool branchLengths);

    void setNewick(string newick);

    size_t numEdges();

    void addEdge(PhyloTreeEdge e) { std::cout << "addEdge(PhyloTreeEdge e)" << std::endl; edges.push_back(e); }

    string toString();

    string newick;

private:
    vector<PhyloTreeEdge> edges;
    vector<string> leaf2NumMap;
    vector<EdgeAttribute> leafEdgeAttribs;


    void setLeaf2NumMapFromNewick(string& s);

    void normalize(double constant);
};

#endif /* __PHYLOTREE_H__ */
