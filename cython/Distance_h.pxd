from libcpp.string cimport string as libcpp_string
from libcpp.vector cimport vector as libcpp_vector
from libcpp.pair cimport pair as libcpp_pair
from libcpp cimport bool

cdef extern from "../src/PhyloTree.h":
    cdef cppclass PhyloTree:
        PhyloTree(libcpp_string newick, bool rooted) except +
        PhyloTree(PhyloTree other, libcpp_vector[int] missing) except +
        libcpp_pair[libcpp_vector[int], libcpp_vector[int]] leaf_difference(PhyloTree other) except +

cdef extern from "../src/Bipartition.h":
    cdef cppclass Bipartition:
        Bipartition(libcpp_string s) except +
        bool isEmpty() except +
        bool properlyContains(Bipartition other) except +
        bool contains(Bipartition other) except +
        bool disjointFrom(Bipartition other) except +
        bool crosses(Bipartition other) except +
        bool isCompatibleWith(libcpp_vector[Bipartition] splits) except +

cdef extern from "../src/Distance.h" namespace "Distance":
    double getEuclideanDistance(PhyloTree t1, PhyloTree t2, bool normalise) except +
    double getGeodesicDistance(PhyloTree t1, PhyloTree t2, bool normalise) except +
    double getRobinsonFouldsDistance(PhyloTree t1, PhyloTree t2, bool normalise) except +
    double getWeightedRobinsonFouldsDistance(PhyloTree t1, PhyloTree t2, bool normalise) except +

#cdef extern from "../src/PhyloTreeEdge.h":
#    cdef cppclass PhyloTreeEdge:
#        PhyloTreeEdge() except +
#        PhyloTreeEdge(libcpp_string s) except +

#cdef extern from "../src/BipartiteGraph.h":
#    cdef cppclass BipartiteGraph:
#        BipartiteGraph(libcpp_vector[libcpp_vector[bool]] IncidenceMatrix, libcpp_vector[double] Aweight, libcpp_vector[double] Bweight) except +
#        libcpp_vector[libcpp_vector[size_t]] vertex_cover(libcpp_vector[size_t] Aindex, libcpp_vector[size_t] Bindex);
#        libcpp_vector[libcpp_vector[bool]] getIncidenceMatrix(libcpp_vector[PhyloTreeEdge] edges1, libcpp_vector[PhyloTreeEdge] edges2);