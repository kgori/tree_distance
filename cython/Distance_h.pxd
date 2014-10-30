from libcpp.string cimport string as libcpp_string
from libcpp cimport bool

cdef extern from "../PhyloTree.h":
    cdef cppclass PhyloTree:
        PhyloTree(libcpp_string newick, bool rooted) except +

cdef extern from "../Distance.h" namespace "Distance":
    double getEuclideanDistance(PhyloTree t1, PhyloTree t2, bool normalise) except +
    double getGeodesicDistance(PhyloTree t1, PhyloTree t2, bool normalise) except +
    double getRobinsonFouldsDistance(PhyloTree t1, PhyloTree t2, bool normalise) except +
    double getWeightedRobinsonFouldsDistance(PhyloTree t1, PhyloTree t2, bool normalise) except +
