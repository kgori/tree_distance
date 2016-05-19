#ifndef __RATIO_H__
#define __RATIO_H__
#ifndef BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#define BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#endif
#include "Bipartition.h"
#include "PhyloTreeEdge.h"
#include <string>
#include <vector>

using namespace std;

class Ratio {
public:
    Ratio();

    Ratio(vector<PhyloTreeEdge>& eEdges, vector<PhyloTreeEdge>& fEdges);

    Ratio(double e, double f);

    static Ratio combine(Ratio& r1, Ratio& r2);

    Ratio(const Ratio &other); // copy-constructor

    inline bool operator<(const Ratio &other) const {
        return this->getRatio() < other.getRatio();
    }

    inline bool operator>(const Ratio &other) const {
        return other < *this;
    }

    inline bool operator<=(const Ratio &other) const {
        return !(*this > other);
    }

    inline bool operator>=(const Ratio &other) const {
        return !(*this < other);
    }

    static double geoAvg(double d1, double d2);

    static double geoAvg(vector<PhyloTreeEdge>& edges);

    const vector<PhyloTreeEdge>& getEEdges();

    void addEEdge(PhyloTreeEdge& edge);

    void addAllEEdges(vector<PhyloTreeEdge>& edges);

    void setAllEEdges(vector<PhyloTreeEdge>& edges);

    void setAllFEdges(vector<PhyloTreeEdge>& edges);

    double getELength();

    void setELength(double eLen);

    const vector<PhyloTreeEdge>& getFEdges();

    void addFEdge(PhyloTreeEdge& edge);

    void addAllFEdges(vector<PhyloTreeEdge>& edges);

    double getFLength();

    void setFLength(double fLen);

    double getRatio() const;

    double getTime();

    Ratio reverse();

    bool containsOriginalEEdge(const Bipartition &edge);

    string toString();

    string toStringCombType();

    string toStringVerbose(vector<string> leaf2NumMap);

    Ratio clone();

private:
    double eLength;
    double fLength;
    vector<PhyloTreeEdge> eEdges;
    vector<PhyloTreeEdge> fEdges;
};

#endif /* __RATIO_H__ */
