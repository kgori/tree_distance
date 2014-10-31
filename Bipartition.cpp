#include "Bipartition.h"
#include "Tools.h"
//#include <boost/dynamic_bitset.hpp>
//#include <memory>

using namespace std;

Bipartition::Bipartition() {
    std::cout << "Bipartition::Bipartition()" << std::endl;
    partition = unique_ptr<boost::dynamic_bitset<>>(new boost::dynamic_bitset<>());
}

// copy-constructor
Bipartition::Bipartition(const Bipartition &e) {
    std::cout << "Bipartition::Bipartition(const Bipartition &e)" << std::endl;
    partition = unique_ptr<boost::dynamic_bitset<>>(new boost::dynamic_bitset<>(*(e.partition)));
}

Bipartition::Bipartition(boost::dynamic_bitset<> &edge) {
    std::cout << "Bipartition::Bipartition(boost::dynamic_bitset<> &edge)" << std::endl;
    partition = unique_ptr<boost::dynamic_bitset<>>(new boost::dynamic_bitset<>(edge));
}

Bipartition::Bipartition(string s) {
    std::cout << "Bipartition::Bipartition(string s)" << std::endl;
    partition = unique_ptr<boost::dynamic_bitset<>>(new boost::dynamic_bitset<>(s));
}

void Bipartition::addOne(size_t index) {
    std::cout << "Bipartition::addOne(size_t index)" << std::endl;
    (*partition)[size() - index - 1] = true;
}

void Bipartition::complement(int numLeaves) {
    std::cout << "Bipartition::complement(int numLeaves)" << std::endl;
    size_t s = size();
    for (size_t i = 0; i < numLeaves; ++i) {
        (*partition)[s - i - 1].flip();
    }
}

bool Bipartition::contains(const Bipartition &e) const {
    std::cout << "Bipartition::contains(const Bipartition &e) const" << std::endl;
    auto foreign_edge = *e.partition;
    foreign_edge &= (*partition);
    return foreign_edge == *(e.partition);
}

bool Bipartition::contains(size_t i) {
    std::cout << "Bipartition::contains(size_t i)" << std::endl;
    if (!partition) throw exception();
    return (*partition)[size() - i - 1];
}

bool Bipartition::crosses(const Bipartition &e) const {
    std::cout << "Bipartition::crosses(const Bipartition &e) const" << std::endl;
    return !(disjointFrom(e) || this->contains(e) || e.contains(*this));
}

bool Bipartition::disjointFrom(const Bipartition &e) const {
    std::cout << "Bipartition::disjointFrom(const Bipartition &e) const" << std::endl;
    return !(*partition).intersects(*(e.partition));
}

bool Bipartition::equals(const Bipartition &e) const {
    std::cout << "Bipartition::equals(const Bipartition &e) const" << std::endl;
    return (*partition) == (*(e.partition));
}

unique_ptr<boost::dynamic_bitset<>> Bipartition::getPartition() const {
    std::cout << "Bipartition::getPartition() const" << std::endl;
    if (!partition) throw exception();
    return unique_ptr<boost::dynamic_bitset<>>(new boost::dynamic_bitset<>(*partition));
}

bool Bipartition::isCompatibleWith(const vector<Bipartition>& splits) {
    std::cout << "Bipartition::isCompatibleWith(const vector<Bipartition>& splits)" << std::endl;
    for (size_t i = 0; i < splits.size(); ++i) {
        if (this->crosses(splits[i])) {
            return false;
        }
    }
    return true;
}

bool Bipartition::isEmpty() {
    std::cout << "Bipartition::isEmpty()" << std::endl;
    return partition->none();
}

size_t Bipartition::size() {
    std::cout << "Bipartition::size()" << std::endl;
    return partition->size();
}

bool Bipartition::properlyContains(const Bipartition &e) {
    std::cout << "Bipartition::properlyContains(const Bipartition &e)" << std::endl;
    return this->contains(e) && !e.contains(*this);
}

void Bipartition::removeOne(size_t index) {
    std::cout << "Bipartition::removeOne(size_t index)" << std::endl;
    (*partition)[size() - index - 1] = false;
}

void Bipartition::setPartition(boost::dynamic_bitset<>& edge) {
    std::cout << "Bipartition::setPartition(boost::dynamic_bitset<>& edge)" << std::endl;
    partition = unique_ptr<boost::dynamic_bitset<>>(new boost::dynamic_bitset<>(edge));
}

string Bipartition::toString() {
    std::cout << "Bipartition::toString()" << std::endl;
    string s;
    to_string(*partition, s);
    return s;
}

string Bipartition::toStringVerbose(boost::dynamic_bitset<> edge, vector<string> leaf2NumMap) {
    std::cout << "Bipartition::toStringVerbose(boost::dynamic_bitset<> edge, vector<string> leaf2NumMap)" << std::endl;
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
