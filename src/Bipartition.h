#ifndef __BIPARTITION_H__
#define __BIPARTITION_H__
#ifndef BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#define BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#endif
#include <boost/dynamic_bitset.hpp>
#include <memory>
#include <string>

using namespace std;

class Bipartition {
public:
    Bipartition();

    Bipartition(const Bipartition &e); // copy-constructor

    Bipartition(boost::dynamic_bitset<>& edge);

    Bipartition(const boost::dynamic_bitset<>& edge);

    Bipartition(string s);

    boost::dynamic_bitset<> getPartition() const;

    inline bool operator==(const Bipartition& other) const {
        return (*this).equals(other);
    }

    inline bool operator!=(const Bipartition& other) const {
        return !(*this == other);
    }

    inline bool operator<(const Bipartition& other) const {
        return (this->partition) < (other.partition);
    }

    inline bool operator>(const Bipartition& other) const {
        return other < *this;
    }

    inline bool operator<=(const Bipartition& other) const {
        return !(*this > other);
    }

    inline bool operator>=(const Bipartition& other) const {
        return !(*this < other);
    }

    inline Bipartition &operator&=(const Bipartition& other) {
        partition &= (other.partition);
        return *this;
    }

    inline Bipartition operator~() const {
        return Bipartition(~(this->getPartition()));
    }

    void setPartition(boost::dynamic_bitset<> edge);

    bool isEmpty();

    void addOne(size_t index);

    void removeOne(size_t index);

    bool disjointFrom(const Bipartition &e) const;

    bool contains(const Bipartition &e) const;

    bool contains(size_t i);

    bool properlyContains(const Bipartition &e);

    bool crosses(const Bipartition &e) const;

    void complement(size_t numLeaves);

    virtual string toString();

    void clear();

    bool equals(const Bipartition &e) const;

    bool isCompatibleWith(const vector<Bipartition> &splits);

    Bipartition &andNot(const Bipartition &other) {
        partition &= ~(other.partition);
        return *this;
    }

    size_t size();

    static string toStringVerbose(boost::dynamic_bitset<> edge, vector<string> leaf2NumMap);

protected:
    boost::dynamic_bitset<> partition;
};

#endif /* __BIPARTITION_H__ */
