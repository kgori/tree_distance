#ifndef __VERTEX_H__
#define __VERTEX_H__

class Vertex {

public :
    double label=0, weight=0, residual=0;
    int pred=0;
    Vertex(double weight) { this->weight = weight * weight; }
};

#endif /* __VERTEX_H__ */
