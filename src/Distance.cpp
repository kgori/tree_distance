#ifndef BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#define BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#endif
#include "Distance.h"
#include <cmath>

double Distance::getRobinsonFouldsDistance(PhyloTree &t1, PhyloTree &t2, bool normalise) {
    double rf_value = 0;
    vector<PhyloTreeEdge> enic;
    t1.getEdgesNotInCommonWith(t2, enic);
    t2.getEdgesNotInCommonWith(t1, enic);
    rf_value = enic.size();
    if (normalise)
        rf_value /= t1.numEdges() + t2.numEdges();
    return rf_value;
}

double Distance::getWeightedRobinsonFouldsDistance(PhyloTree &t1, PhyloTree &t2, bool normalise) {
    double wrf_value = 0;

    // Collect edges-in-common and edges-not-in-common...
    vector<PhyloTreeEdge> eic;
    PhyloTree::getCommonEdges(t1, t2, eic);
    vector<PhyloTreeEdge> enic;
    t1.getEdgesNotInCommonWith(t2, enic);
    t2.getEdgesNotInCommonWith(t1, enic);
    // ... and leaves
    vector<double> leaves1 = t1.getLeafEdgeLengths();
    vector<double> leaves2 = t2.getLeafEdgeLengths(); // Assuming these are the same leaves

    // Collect length differences for internal edges...
    for (PhyloTreeEdge &pte : eic) {
        wrf_value += abs(pte.getLength());
    }

    for (PhyloTreeEdge &pte : enic) {
        wrf_value += abs(pte.getLength());
    }

    // ... and leaves
    for (int i = 0; i < leaves1.size(); i++) {
        wrf_value += abs(leaves1[i] - leaves2[i]);
    }

    if (normalise) return wrf_value / (t1.getBranchLengthSum() + t2.getBranchLengthSum());
    return wrf_value;
}

double Distance::getEuclideanDistance(PhyloTree &t1, PhyloTree &t2, bool normalise) {
    double euc_value = 0;

    // Collect edges-in-common and edges-not-in-common...
    vector<PhyloTreeEdge> eic;
    PhyloTree::getCommonEdges(t1, t2, eic);
    vector<PhyloTreeEdge> enic;
    t1.getEdgesNotInCommonWith(t2, enic);
    t2.getEdgesNotInCommonWith(t1, enic);
    // ... and leaves
    vector<double> leaves1 = t1.getLeafEdgeLengths();
    vector<double> leaves2 = t2.getLeafEdgeLengths(); // Assuming these are the same

    // Collect length differences for internal edges...
    for (PhyloTreeEdge &pte : eic) {
        euc_value += pow(pte.getLength(), 2);
    }

    for (PhyloTreeEdge &pte : enic) {
        euc_value += pow(pte.getLength(), 2);
    }

    // ... and leaves
    for (int i = 0; i < leaves1.size(); i++) {
        euc_value += pow(leaves1[i] - leaves2[i], 2);
    }

    if (normalise) return sqrt(euc_value) / (t1.getDistanceFromOrigin() + t2.getDistanceFromOrigin());
    return sqrt(euc_value);
}

double Distance::getGeodesicDistance(PhyloTree &t1, PhyloTree &t2, bool normalise) {
    double distance = Geodesic::getGeodesic(t1, t2).getDist();
    if (normalise) return distance / (t1.getDistanceFromOrigin() + t2.getDistanceFromOrigin());
    return distance;
}

double Distance::getEuclideanDistance(const string& t1, const string& t2, bool normalise, bool rooted1, bool rooted2) {
    PhyloTree a(t1, rooted1);
    PhyloTree b(t2, rooted2);
    return getEuclideanDistance(a, b, normalise);
}

double Distance::getGeodesicDistance(const string &t1, const string &t2, bool normalise, bool rooted1, bool rooted2) {
    PhyloTree a(t1, rooted1);
    PhyloTree b(t2, rooted2);
    return getGeodesicDistance(a, b, normalise);
}

double Distance::getRobinsonFouldsDistance(const string& t1, const string& t2, bool normalise, bool rooted1, bool rooted2) {
    PhyloTree a(t1, rooted1);
    PhyloTree b(t2, rooted2);
    return getRobinsonFouldsDistance(a, b, normalise);
}

double Distance::getWeightedRobinsonFouldsDistance(const string& t1, const string& t2, bool normalise, bool rooted1, bool rooted2) {
    PhyloTree a(t1, rooted1);
    PhyloTree b(t2, rooted2);
    return getWeightedRobinsonFouldsDistance(a, b, normalise);
}
