#ifndef BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#define BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#endif
#include "Bipartition.h"
#include "Tools.h"

using namespace std;

Bipartition::Bipartition() {
    partition = boost::dynamic_bitset<>(0);
}

// copy-constructor
Bipartition::Bipartition(const Bipartition &e) : partition(e.partition) {
}

Bipartition::Bipartition(boost::dynamic_bitset<> &edge) : partition(edge) {
}

Bipartition::Bipartition(const boost::dynamic_bitset<> &edge) {
    partition = boost::dynamic_bitset<>(edge);
}

Bipartition::Bipartition(string s) {
    partition = boost::dynamic_bitset<>(s);
}

void Bipartition::addOne(size_t index) {
    partition[size() - index - 1] = true;
}

void Bipartition::complement(size_t numLeaves) {
    size_t s = size();
    for (size_t i = 0; i < numLeaves; ++i) {
        partition[s - i - 1].flip();
    }
}

bool Bipartition::contains(const Bipartition& e) const {
    auto foreign = e.partition;
    foreign &= partition;
    return foreign == e.partition;
}

bool Bipartition::contains(size_t i) {
    return partition[size() - i - 1];
}

bool Bipartition::properlyContains(const Bipartition &e) {
    return this->contains(e) && !e.contains(*this);
}

bool Bipartition::crosses(const Bipartition &e) const {
    return !(disjointFrom(e) || this->contains(e) || e.contains(*this));
}

bool Bipartition::disjointFrom(const Bipartition &e) const {
    return !partition.intersects(e.partition);
}

bool Bipartition::equals(const Bipartition &e) const {
    return partition == (e.partition);
}

boost::dynamic_bitset<> Bipartition::getPartition() const {
    return partition;
}

bool Bipartition::isCompatibleWith(const vector<Bipartition>& splits) {
    for (size_t i = 0; i < splits.size(); ++i) {
        if (this->crosses(splits[i])) {
            return false;
        }
    }
    return true;
}

bool Bipartition::isEmpty() {
    return partition.none();
}

size_t Bipartition::size() {
    return partition.size();
}

void Bipartition::removeOne(size_t index) {
    partition[size() - index - 1] = false;
}

void Bipartition::setPartition(boost::dynamic_bitset<> edge) {
    partition = edge;
}

void Bipartition::clear() {
    partition.reset();
}

string Bipartition::toString() {
    string s;
    to_string(partition, s);
    return s;
}

string Bipartition::toStringVerbose(boost::dynamic_bitset<> edge, vector<string> leaf2NumMap) {
    string toDisplay = "";
    size_t edge_size = edge.size();
    for (size_t i = 0; i < edge_size; i++) {
        if ((bool)edge[edge_size - i - 1]) {
            toDisplay = toDisplay + leaf2NumMap[i] + ",";
        }
    }
    // remove the last ,
    return Tools::substring(toDisplay, 0, toDisplay.find_last_not_of(",")+1);
}
