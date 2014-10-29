#ifndef __POLY_MAIN_H__
#define __POLY_MAIN_H__

#include "Geodesic.h"
#include "PhyloTree.h"
#include <string>
#include <vector>

class PolyMain {
public:
    PolyMain();

    static double getRobinsonFouldsDistance(PhyloTree t1, PhyloTree t2, bool normalise);

    static double getWeightedRobinsonFouldsDistance(PhyloTree t1, PhyloTree t2, bool normalise);

    static double getEuclideanDistance(PhyloTree t1, PhyloTree t2, bool normalise);

    static double getGeodesicDistance(PhyloTree t1, PhyloTree t2, bool normalise);
};

#endif /* __POLY_MAIN_H__ */
