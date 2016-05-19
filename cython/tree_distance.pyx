#cython: c_string_encoding=ascii  # for cython>=0.19
from  libcpp.string  cimport string as libcpp_string
from  libcpp.set     cimport set as libcpp_set
from  libcpp.vector  cimport vector as libcpp_vector
from  libcpp.pair    cimport pair as libcpp_pair
from  libcpp.map     cimport map  as libcpp_map
from  smart_ptr cimport shared_ptr
from  AutowrapRefHolder cimport AutowrapRefHolder
from  libcpp cimport bool
from  libc.string cimport const_char
from cython.operator cimport dereference as deref, preincrement as inc, address as address
from Distance_h cimport getEuclideanDistance as _getEuclideanDistance_Distance_h
from Distance_h cimport getGeodesicDistance as _getGeodesicDistance_Distance_h
from Distance_h cimport getRobinsonFouldsDistance as _getRobinsonFouldsDistance_Distance_h
from Distance_h cimport getWeightedRobinsonFouldsDistance as _getWeightedRobinsonFouldsDistance_Distance_h
from Distance_h cimport PhyloTree as _PhyloTree
cdef extern from "autowrap_tools.hpp":
    char * _cast_const_away(char *)

def getEuclideanDistance(PhyloTree t1, PhyloTree t2, normalise=False):
    """
    getEuclideanDistance(PhyloTree t1, PhyloTree t2, normalise=False)

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

    cdef double _r = _getEuclideanDistance_Distance_h((deref(t1.inst.get())), (deref(t2.inst.get())), (<bool>normalise))
    py_result = <double>_r
    return py_result

def getGeodesicDistance(PhyloTree t1, PhyloTree t2, normalise=False):
    """
    getGeodesicDistance(PhyloTree t1, PhyloTree t2, normalise)

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

    cdef double _r = _getGeodesicDistance_Distance_h((deref(t1.inst.get())), (deref(t2.inst.get())), (<bool>normalise))
    py_result = <double>_r
    return py_result

def getRobinsonFouldsDistance(PhyloTree t1, PhyloTree t2, normalise=False):
    """
    getRobinsonFouldsDistance(PhyloTree t1, PhyloTree t2, normalise)

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

    cdef double _r = _getRobinsonFouldsDistance_Distance_h((deref(t1.inst.get())), (deref(t2.inst.get())), (<bool>normalise))
    py_result = <double>_r
    return py_result

def getWeightedRobinsonFouldsDistance(PhyloTree t1, PhyloTree t2, normalise=False):
    """
    getWeightedRobinsonFouldsDistance(PhyloTree t1, PhyloTree t2, normalise)

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

    cdef double _r = _getWeightedRobinsonFouldsDistance_Distance_h((deref(t1.inst.get())), (deref(t2.inst.get())), (<bool>normalise))
    py_result = <double>_r
    return py_result 

cdef class PhyloTree:

    cdef shared_ptr[_PhyloTree] inst

    def __dealloc__(self):
         self.inst.reset()

    
    def leaf_difference(self, PhyloTree other ):
        assert isinstance(other, PhyloTree), 'arg other wrong type'
    
        _r = self.inst.get().leaf_difference((deref(other.inst.get())))
        cdef list py_result = [_r.first, _r.second]
        return py_result
    
    def _init_0(self, bytes newick, rooted=False):
        assert isinstance(newick, bytes), 'arg newick wrong type'
        assert isinstance(rooted, (int, long)), 'arg rooted wrong type'
    
    
        self.inst = shared_ptr[_PhyloTree](new _PhyloTree((<libcpp_string>newick), (<bool>rooted)))
    
    def _init_1(self, PhyloTree other , list missing):
        assert isinstance(other, PhyloTree), 'arg other wrong type'
        assert isinstance(missing, list) and all(isinstance(elemt_rec, (int, long)) for elemt_rec in missing), 'arg missing wrong type'
    
        cdef libcpp_vector[int] v1 = missing
        self.inst = shared_ptr[_PhyloTree](new _PhyloTree((deref(other.inst.get())), v1))

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
        PhyloTree, other; list, missing.

        Construct a PhyloTree instance from another PhyloTree, with a list of leaf indices to prune
        """
        if (len(args)==2) and (isinstance(args[0], bytes)) and (isinstance(args[1], (int, long))):
             self._init_0(*args)
        elif (len(args)==2) and (isinstance(args[0], PhyloTree)) and (isinstance(args[1], list) and all(isinstance(elemt_rec, (int, long)) for elemt_rec in args[1])):
             self._init_1(*args)
        else:
               raise Exception('can not handle type of %s' % (args,)) 
