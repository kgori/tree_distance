# tree_distance

A package to compute distances between phylogenetic trees.

Supported distances:

- Robinson-Foulds distance
- Weighted Robinson-Foulds distance
- Euclidean distance
- Geodesic distance

### Installation
`pip install tree_distance`

### Example
    t1 = PhyloTree(b"(a:3,b:4,(c:5,((d:6,e:7):.2,f:8):.3):.4);", False) # rooted = False
    t2 = PhyloTree(b"(a:3,b:4,(d:5,((c:6,e:7):.2,f:8):.3):.4);", False)
    getRobinsonFouldsDistance(t1, t2, normalise=False) # 4.0
    getRobinsonFouldsDistance(t1, t2, normalise=True) # 0.666...

### Tests
To run the tests:
    mkdir build
    cd build
    cmake ..
    make
    ./tests
