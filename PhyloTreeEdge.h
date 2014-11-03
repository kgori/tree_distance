#ifndef __PHYLOTREE_EDGE_H__
#define __PHYLOTREE_EDGE_H__

#include <boost/dynamic_bitset.hpp>
#include <memory>
#include <string>
#include <tuple>
#include <vector>
#include "Bipartition.h"
#include "EdgeAttribute.h"

using namespace std;

class PhyloTreeEdge : public Bipartition {
private:
    typedef Bipartition super;

public:
    PhyloTreeEdge();

    PhyloTreeEdge(string s);

    PhyloTreeEdge(boost::dynamic_bitset<> edge);

    PhyloTreeEdge(EdgeAttribute attrib);

    PhyloTreeEdge(EdgeAttribute attrib, int originalID);

    PhyloTreeEdge(EdgeAttribute attrib, Bipartition originalEdge, int originalID);

    PhyloTreeEdge(Bipartition edge, EdgeAttribute attrib, int originalID);

    PhyloTreeEdge(boost::dynamic_bitset<> edge, EdgeAttribute attrib, boost::dynamic_bitset<> originalEdge, int originalID);

    PhyloTreeEdge(const PhyloTreeEdge &other); // copy-constructor

//    inline bool operator< (const EdgeAttribute& other) const {
//        return Tools::vector_equal(this->vect, other.vect);
//    }
//
//    inline bool operator>(const PhyloTreeEdge& other) const { return other < *this; }
//
//    inline bool operator<=(const PhyloTreeEdge& other) const { return !(*this > other); }
//
//    inline bool operator>=(const PhyloTreeEdge& other) const { return !(*this < other); }
//
//    inline bool operator==(const PhyloTreeEdge& other) const { return this->norm() < other.norm(); }
//
//    inline bool operator!=(const PhyloTreeEdge& other) const { return !(*this == other); }

    double getLength();

    double getLength() const;

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

    EdgeAttribute& getAttribute();

    void setAttribute(const EdgeAttribute &attrib);

    static void printEdgesVerbose(vector<PhyloTreeEdge> edges, vector<string> leaf2NumMap);

    string toStringVerbose(vector<string> leaf2NumMap);

    bool isCompatibleWith(const vector<Bipartition>& splits);

    bool isCompatibleWith(const vector<PhyloTreeEdge>& splits);

private:
    EdgeAttribute attribute;
    Bipartition originalEdge;
    int originalID;
};

#endif /* __PHYLOTREE_EDGE_H__ */
