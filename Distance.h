#ifndef __DISTANCE_H__
#define __DISTANCE_H__

#include "Geodesic.h"
#include "PhyloTree.h"
#include <string>
#include <vector>

class Distance {
public:
//    Distance();

    static double getRobinsonFouldsDistance(PhyloTree t1, PhyloTree t2, bool normalise);

    static double getWeightedRobinsonFouldsDistance(PhyloTree t1, PhyloTree t2, bool normalise);

    static double getEuclideanDistance(PhyloTree t1, PhyloTree t2, bool normalise);

    static double getGeodesicDistance(PhyloTree t1, PhyloTree t2, bool normalise);
};

#endif /* __DISTANCE_H__ */
