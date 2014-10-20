#include "Bipartition.h"
#include <boost/dynamic_bitset.hpp>
#include <memory>

using namespace std;

Bipartition::Bipartition() {
    partition = unique_ptr<boost::dynamic_bitset<>>(new boost::dynamic_bitset<>());
}

// copy-constructor
Bipartition::Bipartition(const Bipartition &e) {
    partition = unique_ptr<boost::dynamic_bitset<>>(new boost::dynamic_bitset<>(*(e.partition)));
}

Bipartition::Bipartition(boost::dynamic_bitset<> &edge) {
    partition = unique_ptr<boost::dynamic_bitset<>>(new boost::dynamic_bitset<>(edge));
}

Bipartition::Bipartition(string s) {
    partition = unique_ptr<boost::dynamic_bitset<>>(new boost::dynamic_bitset<>(s));
}

void Bipartition::addOne(size_t index) {
    (*partition)[size() - index - 1] = true;
}

void Bipartition::complement(int numLeaves) {
    size_t s = size();
    for (size_t i = 0; i < numLeaves; ++i) {
        (*partition)[s - i - 1].flip();
    }
}

bool Bipartition::contains(const Bipartition &e) const {
    auto foreign_edge = unique_ptr<boost::dynamic_bitset<>>(new boost::dynamic_bitset<>(*e.partition));
    (*foreign_edge) &= (*partition);
    return (*foreign_edge) == (*(e.partition));
}

bool Bipartition::contains(size_t i) {
    if (!partition) throw exception();
    return (*partition)[i];
}

bool Bipartition::crosses(const Bipartition &e) const {
    return !(disjointFrom(e) || this->contains(e) || e.contains(*this));
}

bool Bipartition::disjointFrom(const Bipartition &e) const {
    return !(*partition).intersects(*(e.partition));
}

bool Bipartition::equals(const Bipartition &e) const {
    return (*partition) == (*(e.partition));
}

unique_ptr<boost::dynamic_bitset<>> Bipartition::getPartition() const {
    if (!partition) throw exception();
    return unique_ptr<boost::dynamic_bitset<>>(new boost::dynamic_bitset<>(*partition));
}

bool Bipartition::isCompatibleWith(vector<Bipartition> splits) {
    for (size_t i = 0; i < splits.size(); ++i) {
        if (this->crosses(splits[i])) {
            return false;
        }
    }
    return true;
}

bool Bipartition::isEmpty() {
    return partition->empty();
}

size_t Bipartition::size() {
    return partition->size();
}

bool Bipartition::properlyContains(const Bipartition &e) {
    return this->contains(e) && !e.contains(*this);
}

void Bipartition::removeOne(size_t index) {
    (*partition)[size() - index - 1] = false;
}

void Bipartition::setPartition(boost::dynamic_bitset<> &edge) {
    partition = unique_ptr<boost::dynamic_bitset<>>(new boost::dynamic_bitset<>(edge));
}

string Bipartition::toString() {
    string s;
    to_string(*partition, s);
    return s;
}
