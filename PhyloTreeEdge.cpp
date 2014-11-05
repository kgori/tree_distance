#include "PhyloTreeEdge.h"
#include <cmath>

#define TOLERANCE 0.000000000000001

using namespace std;

class uninitialised_data : public std::runtime_error {
public:
    uninitialised_data(std::string const &msg) :
            std::runtime_error(msg) {
    }
};

PhyloTreeEdge::PhyloTreeEdge() : super() {
    attribute = EdgeAttribute();
//    originalEdge = Bipartition();
    originalID = -1;
}

PhyloTreeEdge::PhyloTreeEdge(string s) : super(s) {
    attribute = EdgeAttribute();
//    originalEdge = Bipartition();
    originalID = -1;
}

PhyloTreeEdge::PhyloTreeEdge(boost::dynamic_bitset<> edge) : super(edge) {
    attribute = EdgeAttribute();
//    originalEdge = Bipartition();
    originalID = -1;
}

PhyloTreeEdge::PhyloTreeEdge(EdgeAttribute attrib) : super() {
    attribute = attrib;
//    originalEdge = Bipartition();
    originalID = -1;
}

PhyloTreeEdge::PhyloTreeEdge(EdgeAttribute attrib, int originalID) : super() {
    attribute = attrib;
//    originalEdge = Bipartition();
    this->originalID = originalID;
}

PhyloTreeEdge::PhyloTreeEdge(EdgeAttribute attrib, shared_ptr<Bipartition> originalEdge, int originalID) : super(), originalEdge(originalEdge), originalID(originalID), attribute(attrib) {
//    attribute = attrib;
//    this->originalEdge = originalEdge;
//    this->originalID = originalID;
    auto empty_partition = boost::dynamic_bitset<>(originalEdge->size());
    this->setPartition(empty_partition);
}

PhyloTreeEdge::PhyloTreeEdge(Bipartition edge, EdgeAttribute attrib, int originalID) : super(edge), originalEdge(make_shared<Bipartition>(edge)) {
    attribute = attrib;

    this->originalID = originalID;
}

PhyloTreeEdge::PhyloTreeEdge(boost::dynamic_bitset<> edge, EdgeAttribute attrib,
        boost::dynamic_bitset<> originalEdge, int originalID)
        : super(edge), originalEdge(make_shared<Bipartition>(originalEdge)) {
    attribute = attrib;
//    this->originalEdge = make_shared<Bipartition>(originalEdge);
    this->originalID = originalID;
}

PhyloTreeEdge::PhyloTreeEdge(const PhyloTreeEdge &other) : super(other.partition),
                                                           attribute(other.attribute),
                                                           originalEdge(other.originalEdge),
                                                           originalID(other.originalID) {
}

double PhyloTreeEdge::getLength() {
    return attribute.norm();
}

bool PhyloTreeEdge::isZero() {
    return fabs(attribute.norm()) < TOLERANCE;
}

string PhyloTreeEdge::toString() {
    string s;
    boost::to_string(partition, s);
    return attribute.toString() + " " + s;
}

PhyloTreeEdge PhyloTreeEdge::clone() {
    return PhyloTreeEdge(*this);
}

//bool PhyloTreeEdge::equals(const PhyloTreeEdge &other) {
//    return this->attribute.equals(other.attribute) && this->partition == other.partition;
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

void PhyloTreeEdge::setAttribute(const EdgeAttribute &attrib) {
    this->attribute = EdgeAttribute(attrib);
}

EdgeAttribute& PhyloTreeEdge::getAttribute() {
    return attribute;
}

//void PhyloTreeEdge::printEdgesVerbose(vector<PhyloTreeEdge> edges, vector<string> leaf2NumMap) {
//    string output = "";
//
//    cout << "Edge ID\t\tLength\t\tLeaves Below" << endl;
//    for (int i = 0; i < edges.size(); i++) {
//        if (!edges[i].getOriginalEdge().getPartition().empty()) {
//            cout << edges[i].getOriginalID() << "\t\t" << edges[i].attribute.toString() << "\t\t" << Bipartition::toStringVerbose(edges[i].getOriginalEdge().getPartition(), leaf2NumMap) << endl;
//        } else {
//            cout << edges[i].toStringVerbose(leaf2NumMap);
//        }
//    }
//}

string PhyloTreeEdge::toStringVerbose(vector<string> leaf2NumMap) {
    ostringstream ss;
    ss << originalID << "\t\t" << attribute.toString() << "\t\t" << Bipartition::toStringVerbose(this->partition, leaf2NumMap);
    return ss.str();
}

double PhyloTreeEdge::getLength() const {
    return attribute.norm();
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
