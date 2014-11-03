#ifndef __PHYLOTREE_H__
#define __PHYLOTREE_H__

#include "Bipartition.h"
#include "EdgeAttribute.h"
#include "PhyloTreeEdge.h"
#include <string>
#include <vector>

using namespace std;

class edge_not_found_exception : public std::runtime_error {
public:
    edge_not_found_exception(std::string const &msg) :
            std::runtime_error(msg) {
    }
};

class PhyloTree {
public:
    PhyloTree(vector<PhyloTreeEdge> &edges, vector<string> &leaf2NumMap, vector<EdgeAttribute> &leafEdgeLengths);

    PhyloTree(vector<PhyloTreeEdge> &edges, vector<string> &leaf2NumMap);

    PhyloTree(const PhyloTree &t); // copy-constructor

    PhyloTree(string t, bool rooted);

    static void getCommonEdges(PhyloTree &t1, PhyloTree &t2, vector<PhyloTreeEdge>& dest);

    static PhyloTreeEdge getFirstCommonEdge(vector<PhyloTreeEdge> &t1_edges, vector<PhyloTreeEdge> &t2_edges);

    static void getCommonEdges(vector<PhyloTreeEdge> &t1_edges, vector<PhyloTreeEdge> &t2_edges, vector<PhyloTreeEdge> &dest);

    vector<PhyloTreeEdge> getEdges();

    void getEdges(vector<PhyloTreeEdge>& edges_to_add);

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

    void getEdgesNotInCommonWith(PhyloTree &t, vector<PhyloTreeEdge>& dest);

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

    void addEdge(PhyloTreeEdge e) { edges.push_back(e); }

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
