#ifndef __PHYLOTREE_EDGE_H__
#define __PHYLOTREE_EDGE_H__

#ifndef BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#define BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#endif

#include <memory>
#include <string>
#include <tuple>
#include <vector>
#include "Bipartition.h"
#include "boost/dynamic_bitset.hpp"

using namespace std;

class PhyloTreeEdge : public Bipartition {
private:
    typedef Bipartition super;

public:
    PhyloTreeEdge();

    PhyloTreeEdge(string s);

    PhyloTreeEdge(boost::dynamic_bitset<> edge);

    PhyloTreeEdge(boost::dynamic_bitset<> edge, double length, int id);

    PhyloTreeEdge(double attrib);

    PhyloTreeEdge(double attrib, int originalID);

    PhyloTreeEdge(double attrib, shared_ptr<Bipartition> originalEdge, int originalID);

    PhyloTreeEdge(Bipartition edge, double attrib, int originalID);

    PhyloTreeEdge(boost::dynamic_bitset<> edge, double attrib, boost::dynamic_bitset<> originalEdge, int originalID);

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

//    bool equals(const PhyloTreeEdge &other);

    bool sameBipartition(const PhyloTreeEdge &other);

    bool sameBipartition(const Bipartition &bip) const;

    Bipartition asSplit();

    const shared_ptr<Bipartition> getOriginalEdge();

    void setOriginalEdge(const shared_ptr<Bipartition> originalEdge);

    int getOriginalID();

    void setOriginalID(int originalID);

    double getAttribute();

    void scaleBy(double factor);

    void setAttribute(double attrib);

//    static void printEdgesVerbose(vector<PhyloTreeEdge> edges, vector<string> leaf2NumMap);

    string toStringVerbose(vector<string> leaf2NumMap);

    bool isCompatibleWith(const vector<Bipartition>& splits);

    bool isCompatibleWith(const vector<PhyloTreeEdge>& splits);

private:
    double length = 0;
    shared_ptr<Bipartition> originalEdge;
    int originalID = -1;
};

#endif /* __PHYLOTREE_EDGE_H__ */
