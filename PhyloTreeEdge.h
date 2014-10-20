#ifndef __PHYLOTREE_EDGE_H__
#define __PHYLOTREE_EDGE_H__

#include <boost/dynamic_bitset.hpp>
#include <memory>
#include <string>
#include <vector>
#include "Bipartition.h"
#include "EdgeAttribute.h"

using namespace std;

class PhyloTreeEdge : public Bipartition {
private:
    typedef Bipartition super;

public:
    PhyloTreeEdge();

    PhyloTreeEdge(boost::dynamic_bitset<> &edge);

    PhyloTreeEdge(EdgeAttribute attrib);

    PhyloTreeEdge(EdgeAttribute attrib, int originalID);

    PhyloTreeEdge(EdgeAttribute attrib, Bipartition originalEdge, int originalID);

    PhyloTreeEdge(Bipartition edge, EdgeAttribute attrib, int originalID);

    PhyloTreeEdge(boost::dynamic_bitset<> edge, EdgeAttribute attrib, boost::dynamic_bitset<> originalEdge, int originalID);

    PhyloTreeEdge(const PhyloTreeEdge &other); // copy-constructor

    PhyloTreeEdge& operator=(const PhyloTreeEdge &other) {
        if (this != &other) {
            super(*(other.partition));
            attribute = unique_ptr<EdgeAttribute>(new EdgeAttribute(*(other.attribute)));
            this->originalEdge = unique_ptr<Bipartition>(new Bipartition(*(other.originalEdge)));
            this->originalID = other.originalID;
        }
        return *this;
    }

    double getLength();

    bool isZero();

    string toString();

    PhyloTreeEdge clone();

    bool equals(const PhyloTreeEdge &other);

    bool sameBipartition(const PhyloTreeEdge &other);

    bool sameBipartition(const Bipartition &bip) const;

    Bipartition asSplit();

    Bipartition getOriginalEdge();

    void setOriginalEdge(const Bipartition &originalEdge);

    int getOriginalID();

    void setOriginalID(int originalID);

    EdgeAttribute getAttribute();

    void setAttribute(const EdgeAttribute &attrib);

private:
    unique_ptr<EdgeAttribute> attribute = nullptr;
    unique_ptr<Bipartition> originalEdge = nullptr;
    int originalID;
};

#endif /* __PHYLOTREE_EDGE_H__ */
