from libcpp.string cimport string as libcpp_string
from libcpp.vector cimport vector as libcpp_vector
from libcpp.pair cimport pair as libcpp_pair
from libcpp cimport bool

cdef extern from "../src/PhyloTree.h":
    cdef cppclass PhyloTree:
        PhyloTree(libcpp_string newick, bool rooted) except +
        PhyloTree(PhyloTree other, libcpp_vector[int] missing) except +
        libcpp_pair[libcpp_vector[int], libcpp_vector[int]] leaf_difference(PhyloTree other) except +

cdef extern from "../src/Distance.h" namespace "Distance":
    double getEuclideanDistance(PhyloTree t1, PhyloTree t2, bool normalise) except +
    double getGeodesicDistance(PhyloTree t1, PhyloTree t2, bool normalise) except +
    double getRobinsonFouldsDistance(PhyloTree t1, PhyloTree t2, bool normalise) except +
    double getWeightedRobinsonFouldsDistance(PhyloTree t1, PhyloTree t2, bool normalise) except +
