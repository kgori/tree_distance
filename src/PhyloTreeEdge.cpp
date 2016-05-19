#ifndef BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#define BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#endif

#include "PhyloTreeEdge.h"
#include <cmath>
#include <sstream>

#define TOLERANCE 0.000000000000001

using namespace std;

class uninitialised_data : public std::runtime_error {
public:
    uninitialised_data(std::string const &msg) :
            std::runtime_error(msg) {
    }
};

PhyloTreeEdge::PhyloTreeEdge() : super() {
    originalID = -1;
}

PhyloTreeEdge::PhyloTreeEdge(string s) : super(s) {
}

PhyloTreeEdge::PhyloTreeEdge(boost::dynamic_bitset<> edge) : super(edge) {
}

PhyloTreeEdge::PhyloTreeEdge(boost::dynamic_bitset<> edge, double length, int id) : super(edge), length(length), originalID(id) {
}

PhyloTreeEdge::PhyloTreeEdge(double attrib) : super(), length(attrib) {
}

PhyloTreeEdge::PhyloTreeEdge(double attrib, int originalID) : super(), length(attrib), originalID(originalID) {
    length = attrib;
}

PhyloTreeEdge::PhyloTreeEdge(double attrib, shared_ptr<Bipartition> originalEdge, int originalID) : originalEdge(originalEdge), originalID(originalID), length(attrib) {
    this->partition = boost::dynamic_bitset<>(originalEdge->size());
}

PhyloTreeEdge::PhyloTreeEdge(Bipartition edge, double attrib, int originalID) : super(edge), originalEdge(make_shared<Bipartition>(edge)), length(attrib) {
    this->originalID = originalID;
}

PhyloTreeEdge::PhyloTreeEdge(boost::dynamic_bitset<> edge, double attrib,
        boost::dynamic_bitset<> originalEdge, int originalID)
        : super(edge), originalEdge(make_shared<Bipartition>(originalEdge)), length(attrib), originalID(originalID) {
}

PhyloTreeEdge::PhyloTreeEdge(const PhyloTreeEdge &other) : super(other.partition),
                                                           length(other.length),
                                                           originalEdge(other.originalEdge),
                                                           originalID(other.originalID) {
}

double PhyloTreeEdge::getLength() {
    return length;
}

bool PhyloTreeEdge::isZero() {
    return fabs(length) < TOLERANCE;
}

string PhyloTreeEdge::toString() {
    string s;
    boost::to_string(partition, s);
    ostringstream ss;
    ss << "[" << length << "] " << s;
    return ss.str();
}

PhyloTreeEdge PhyloTreeEdge::clone() {
    return PhyloTreeEdge(*this);
}

//bool PhyloTreeEdge::equals(const PhyloTreeEdge &other) {
//    return this->length.equals(other.length) && this->partition == other.partition;
//}

bool PhyloTreeEdge::sameBipartition(const PhyloTreeEdge &other) {
    return partition == other.partition;
}

bool PhyloTreeEdge::sameBipartition(const Bipartition &bip) const {
    return partition == bip.getPartition();
}

Bipartition PhyloTreeEdge::asSplit() {
    return Bipartition(partition);
}

const shared_ptr<Bipartition> PhyloTreeEdge::getOriginalEdge() {
    return originalEdge;
}

void PhyloTreeEdge::setOriginalEdge(shared_ptr<Bipartition> originalEdge) {
    this->originalEdge = originalEdge;
}

int PhyloTreeEdge::getOriginalID() {
    return originalID;
}

void PhyloTreeEdge::setOriginalID(int originalID) {
    this->originalID = originalID;
}

void PhyloTreeEdge::setAttribute(double attrib) {
    length = attrib;
}

double PhyloTreeEdge::getAttribute() {
    return length;
}

void PhyloTreeEdge::scaleBy(double factor) {
    length *= factor;
}

//void PhyloTreeEdge::printEdgesVerbose(vector<PhyloTreeEdge> edges, vector<string> leaf2NumMap) {
//    string output = "";
//
//    cout << "Edge ID\t\tLength\t\tLeaves Below" << endl;
//    for (int i = 0; i < edges.size(); i++) {
//        if (!edges[i].getOriginalEdge().getPartition().empty()) {
//            cout << edges[i].getOriginalID() << "\t\t" << edges[i].length.toString() << "\t\t" << Bipartition::toStringVerbose(edges[i].getOriginalEdge().getPartition(), leaf2NumMap) << endl;
//        } else {
//            cout << edges[i].toStringVerbose(leaf2NumMap);
//        }
//    }
//}

string PhyloTreeEdge::toStringVerbose(vector<string> leaf2NumMap) {
    ostringstream ss;
    ss << originalID << "\t\t" << "[" << length << "]" << "\t\t" << Bipartition::toStringVerbose(this->partition, leaf2NumMap);
    return ss.str();
}

double PhyloTreeEdge::getLength() const {
    return length;
}

bool PhyloTreeEdge::isCompatibleWith(const vector<Bipartition> &splits) {
    for (size_t i = 0; i < splits.size(); ++i) {
        if (this->crosses(splits[i])) {
            return false;
        }
    }
    return true;
}

bool PhyloTreeEdge::isCompatibleWith(const vector<PhyloTreeEdge> &splits) {
    for (size_t i = 0; i < splits.size(); ++i) {
        if (this->crosses(splits[i])) {
            return false;
        }
    }
    return true;
}
