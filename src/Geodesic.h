#ifndef __GEODESIC_H__
#define __GEODESIC_H__
#ifndef BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#define BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#endif
#include "PhyloTree.h"
#include "PhyloTreeEdge.h"
#include "RatioSequence.h"
#include <string>
#include <vector>

using namespace std;

class Geodesic {
public:
    Geodesic(RatioSequence rs);

    Geodesic(const RatioSequence& rs, const vector<PhyloTreeEdge>& cEdges);

    Geodesic(const RatioSequence& rs, const vector<PhyloTreeEdge>& cEdges, double leafContributionSquared);

    Geodesic(const Geodesic &other);

//    PhyloTree getTreeAt(PhyloTree t1, PhyloTree t2, double position);

    RatioSequence &getRS();

    void setRS(const RatioSequence& rs);

    double getDist();

//    void addCommonEdge(PhyloTreeEdge e);

    Geodesic clone();

    string toString();

//    static vector<PhyloTreeEdge> getCommonEdges(PhyloTree t1, PhyloTree t2, double position);

//    vector<PhyloTreeEdge> getCommonEdges();

    void setCommonEdges(vector<PhyloTreeEdge> commonEdges);

//    size_t numCommonEdges();

//    size_t numTopologies();

//    Geodesic reverse();

//    double getLeafContributionSquared();

    void setLeafContributionSquared(double leafContributionSquared);

    static Geodesic getGeodesic(PhyloTree &t1, PhyloTree &t2);

    static Geodesic getGeodesicNoCommonEdges(PhyloTree &t1, PhyloTree &t2);

private:
    RatioSequence rs;
    vector<PhyloTreeEdge> commonEdges;
    double leafContributionSquared = 0;
public:
    static void splitOnCommonEdge(vector<PhyloTreeEdge> &t1_edges, vector<PhyloTreeEdge> &t2_edges,
            vector<string> &reference_leaf_num_map, vector<PhyloTree> &destination_a, vector<PhyloTree> &destination_b);
};

#endif /* __GEODESIC_H__ */
