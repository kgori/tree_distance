#ifndef __PHYLOTREE_H__
#define __PHYLOTREE_H__

#include "Bipartition.h"
#include "PhyloTreeEdge.h"
#include <string>
#include <utility>
#include <vector>

using namespace std;

class edge_not_found_exception : public std::runtime_error {
public:
    edge_not_found_exception(std::string const &msg) :
            std::runtime_error(msg) {
    }
};

struct EdgeInfo {
    double length=0.0;
    string name="";
    int id=0;
    bool leaf=false;
};

class PhyloTree {
public:
    PhyloTree(vector<PhyloTreeEdge> &edges, vector<string> &leaf2NumMap, vector<double> &leafEdgeLengths);

    PhyloTree(vector<PhyloTreeEdge> &edges, vector<string> &leaf2NumMap);

    PhyloTree(const PhyloTree &t); // copy-constructor

    PhyloTree(const PhyloTree&, const vector<int>& missing); // pruning constructor

    PhyloTree(string t, bool rooted);

    static void getCommonEdges(PhyloTree &t1, PhyloTree &t2, vector<PhyloTreeEdge> &dest);

    static PhyloTreeEdge getFirstCommonEdge(vector<PhyloTreeEdge> &t1_edges, vector<PhyloTreeEdge> &t2_edges);

    static void getCommonEdges(vector<PhyloTreeEdge> &t1_edges, vector<PhyloTreeEdge> &t2_edges, vector<PhyloTreeEdge> &dest);

    vector<PhyloTreeEdge> getEdges();

    const vector<PhyloTreeEdge> &getEdgesByRef() const;

    void getEdges(vector<PhyloTreeEdge> &edges_to_add);

    void setEdges(vector<PhyloTreeEdge> edges);

    PhyloTreeEdge getEdge(size_t i);

    vector<string> getLeaf2NumMap();

    vector<string> getLeaf2NumMap() const;

    void setLeaf2NumMap(vector<string> leaf2NumMap);

    double getAttribOfSplit(Bipartition &edge);

    vector<Bipartition> getSplits();

    void normalize();

    void normalize(PhyloTree &other);

    size_t numLeaves();

    double getDistanceFromOrigin();

//    double getDistanceFromOrigin() const;

//    double getDistanceFromOriginNoLeaves();

    double getBranchLengthSum();

    void getEdgesNotInCommonWith(PhyloTree &t, vector<PhyloTreeEdge> &dest);

    PhyloTree clone();

    bool equals(PhyloTree t);

    bool removeSplit(Bipartition const &e);

//    void removeSplits(const vector<Bipartition>& splits);

    vector<double> getLeafEdgeLengths();

    const vector<double>& getLeafEdgeLengthsByRef() const;

//    vector<EdgeAttribute> getLeafEdgeAttribs() const;

    void setLeafEdgeLengths(vector<double> &leafEdgeLengths);

//    vector<EdgeAttribute> getCopyLeafEdgeAttribs();

    vector<double> getIntEdgeAttribNorms();

    string getNewick(bool branchLengths);

    void setNewick(string newick);

    size_t numEdges();

//    void addEdge(PhyloTreeEdge e) { edges.push_back(e); }

    string toString();

    string newick;

    std::pair<std::vector<int>, std::vector<int>> leaf_difference(const PhyloTree& other);

private:
    vector<PhyloTreeEdge> edges;
    vector<string> leaf2NumMap;
    vector<double> leafEdgeLengths;

    void setLeaf2NumMapFromNewick(string &s);

    void normalize(double constant);
};

#endif /* __PHYLOTREE_H__ */
