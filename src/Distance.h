#ifndef __DISTANCE_H__
#define __DISTANCE_H__
#ifndef BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#define BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#endif
#include "Geodesic.h"
#include "PhyloTree.h"
#include <string>
#include <vector>

class Distance {
public:
//    Distance();

    static double getRobinsonFouldsDistance(PhyloTree &t1, PhyloTree &t2, bool normalise);

    static double getWeightedRobinsonFouldsDistance(PhyloTree &t1, PhyloTree &t2, bool normalise);

    static double getEuclideanDistance(PhyloTree &t1, PhyloTree &t2, bool normalise);

    static double getGeodesicDistance(PhyloTree &t1, PhyloTree &t2, bool normalise);

    static double getRobinsonFouldsDistance(const string& t1, const string& t2, bool normalise, bool rooted1, bool rooted2);

    static double getWeightedRobinsonFouldsDistance(const string& t1, const string& t2, bool normalise, bool rooted1, bool rooted2);

    static double getEuclideanDistance(const string& t1, const string& t2, bool normalise, bool rooted1, bool rooted2);

    static double getGeodesicDistance(const string &t1, const string &t2, bool normalise, bool rooted1, bool rooted2);
};

#endif /* __DISTANCE_H__ */
