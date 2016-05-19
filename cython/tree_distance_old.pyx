#cython: c_string_encoding=ascii  # for cython>=0.19
"""
This package provides:
 * PhyloTree:
   - a tree data structure

 * getEuclideanDistance:
 & getGeodesicDistance:
 & getRobinsonFouldsDistance:
 & getWeightedRobinsonFouldsDistance:
   - functions to calculate inter-tree distances
"""
from  libcpp.string  cimport string as libcpp_string
from  libcpp cimport bool
from cython.operator cimport dereference as deref, preincrement as inc, address as address
from Distance_h cimport getEuclideanDistance as _getEuclideanDistance_Distance_h
from Distance_h cimport getGeodesicDistance as _getGeodesicDistance_Distance_h
from Distance_h cimport getRobinsonFouldsDistance as _getRobinsonFouldsDistance_Distance_h
from Distance_h cimport getWeightedRobinsonFouldsDistance as _getWeightedRobinsonFouldsDistance_Distance_h
from Distance_h cimport PhyloTree as _PhyloTree
from numpy cimport ndarray as ar
cimport numpy


def getEuclideanDistance(PhyloTree t1 , PhyloTree t2 ,  normalise=False ):
    """
    Arguments:
    ----------
    PhyloTree object, t1; PhyloTree object, t2; bool, normalise (DEFAULT=False).

    Return the euclidean distance between PhyloTree t1 and PhyloTree t2.
    Optionally, the distance can be normalised by dividing by the square
    root of the sum of the squared branch
    lengths for both trees.
    """
    assert isinstance(t1, PhyloTree), 'arg t1 wrong type'
    assert isinstance(t2, PhyloTree), 'arg t2 wrong type'
    assert isinstance(normalise, (int, long)), 'arg normalise wrong type'
    cdef double _r = _getEuclideanDistance_Distance_h((deref(t1.inst)), (deref(t2.inst)), (<bool>normalise))
    py_result = <double>_r
    return py_result

def getGeodesicDistance(PhyloTree t1 , PhyloTree t2 ,  normalise=False ):
    """
    Arguments:
    ----------
    PhyloTree object, t1; PhyloTree object, t2; bool, normalise (DEFAULT=False).

    Returns the geodesic distance between PhyloTree t1 and PhyloTree t2.
    Optionally, the distance can be normalised by dividing by the square
    root of the sum of the squared branch lengths for both trees.
    """
    assert isinstance(t1, PhyloTree), 'arg t1 wrong type'
    assert isinstance(t2, PhyloTree), 'arg t2 wrong type'
    assert isinstance(normalise, (int, long)), 'arg normalise wrong type'
    cdef double _r = _getGeodesicDistance_Distance_h((deref(t1.inst)), (deref(t2.inst)), (<bool>normalise))
    py_result = <double>_r
    return py_result

def getRobinsonFouldsDistance(PhyloTree t1 , PhyloTree t2 ,  normalise=False ):
    """
    Arguments:
    ----------
    PhyloTree object, t1; PhyloTree object, t2; bool, normalise (DEFAULT=False).

    Returns the Robinson Foulds distance between PhyloTree t1 and PhyloTree t2.
    Optionally, the distance can be normalised by dividing by the total number
    of internal edges in the trees.
    """
    assert isinstance(t1, PhyloTree), 'arg t1 wrong type'
    assert isinstance(t2, PhyloTree), 'arg t2 wrong type'
    assert isinstance(normalise, (int, long)), 'arg normalise wrong type'
    cdef double _r = _getRobinsonFouldsDistance_Distance_h((deref(t1.inst)), (deref(t2.inst)), (<bool>normalise))
    py_result = <double>_r
    return py_result

def getWeightedRobinsonFouldsDistance(PhyloTree t1 , PhyloTree t2 ,  normalise=False ):
    """
    Arguments:
    ----------
    PhyloTree object, t1; PhyloTree object, t2; bool, normalise (DEFAULT=False).

    Returns the Weighted Robinson Foulds distance between PhyloTree t1 and PhyloTree t2.
    Optionally, the distance can be normalised by dividing by the sum of the branch
    lengths for both trees.
    """
    assert isinstance(t1, PhyloTree), 'arg t1 wrong type'
    assert isinstance(t2, PhyloTree), 'arg t2 wrong type'
    assert isinstance(normalise, (int, long)), 'arg normalise wrong type'
    cdef double _r = _getWeightedRobinsonFouldsDistance_Distance_h((deref(t1.inst)), (deref(t2.inst)), (<bool>normalise))
    py_result = <double>_r
    return py_result


cdef class PhyloTree:
    """
    Instances of this class provide a phylogenetic tree data structure that can be input to the
    distance functions.
    """
    cdef _PhyloTree *inst

    def __dealloc__(self):
         del self.inst

    def __init__(self, *args):
        """
        Two constructors are available, one from a newick string, and one from another PhyloTree

        Constructor 1
        Arguments:
        ----------
        string, newick; bool, rooted (DEFAULT=False).

        Construct a PhyloTree instance from a newick string.
        Parameter 'rooted' should match whether the tree is rooted (has a bifurcation at the root)
        DEFAULT: rooted=False

        Constructor 2
        Arguments:
        ----------
        PhyloTree, other; libcpp_vector[int], missing.

        Construct a PhyloTree instance from another PhyloTree, with a list of leaf indices to prune
        """

    def _init_0(self, string newick, bool rooted=False):
        """
        Arguments:
        ----------
        string, newick; bool, rooted (DEFAULT=False).

        Construct a PhyloTree instance from a newick string.
        Parameter 'rooted' should match whether the tree is rooted (has a bifurcation at the root)
        DEFAULT: rooted=False
        """
        if isinstance(newick, str):
            newick = newick.encode()
        assert isinstance(newick, bytes), 'arg newick wrong type'
        assert isinstance(rooted, (int, long)), 'arg rooted wrong type'
        self.inst = new _PhyloTree((<libcpp_string>newick), (<bool>rooted))

    def _init_1(self, PhyloTree other, list missing):
        """
        Arguments:
        ----------
        PhyloTree, other; list, missing.

        Construct a PhyloTree instance from another PhyloTree, with a list of leaf indices to prune.
        Parameter 'rooted' should match whether the tree is rooted (has a bifurcation at the root)
        DEFAULT: rooted=False
        """

        assert isinstance(other, PhyloTree), 'arg other wrong type'
        assert isinstance(rooted, (int, long)), 'arg rooted wrong type'
        cdef libcpp_vector[int] * v0 = new libcpp_vector[int]()
        for index in missing:
            v0.push_back(index)
        self.inst = new _PhyloTree(other.inst, (<bool>rooted))




