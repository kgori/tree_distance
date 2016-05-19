#ifndef __VERTEX_H__
#define __VERTEX_H__
#ifndef BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#define BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#endif
class Vertex {

public :
    double label = 0, weight = 0, residual = 0;
    int pred = 0;

    Vertex(double weight) {
        this->weight = weight * weight;
    }
};

#endif /* __VERTEX_H__ */
