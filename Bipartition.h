#ifndef __BIPARTITION_H__
#define __BIPARTITION_H__

#include <memory>
#include <string>
#include <boost/dynamic_bitset.hpp>

using namespace std;

class Bipartition {
public:
    Bipartition();

    Bipartition(const Bipartition &e); // copy-constructor

    Bipartition(boost::dynamic_bitset<> &edge);

    Bipartition(string s);

    unique_ptr<boost::dynamic_bitset<>> getPartition() const;

    void setPartition(boost::dynamic_bitset<> &edge);

    bool isEmpty();

    void addOne(int index);

    void removeOne(int index);

    bool disjointFrom(const Bipartition &e) const;

    bool contains(const Bipartition &e) const;

    bool contains(int i);

    bool properlyContains(const Bipartition &e);

    bool crosses(const Bipartition &e) const;

    void complement(int numLeaves);

    virtual string toString();

    bool equals(const Bipartition &e);

    bool isCompatibleWith(vector<Bipartition> splits);
    size_t size();

protected:
    unique_ptr<boost::dynamic_bitset<>> partition = nullptr;
};

#endif /* __BIPARTITION_H__ */
