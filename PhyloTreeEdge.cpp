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
    attribute = unique_ptr<EdgeAttribute>(new EdgeAttribute());
    originalEdge = unique_ptr<Bipartition>(new Bipartition());
    originalID = -1;
}

PhyloTreeEdge::PhyloTreeEdge(string s) : super(s) {
    attribute = unique_ptr<EdgeAttribute>(new EdgeAttribute());
    originalEdge = unique_ptr<Bipartition>(new Bipartition());
    originalID = -1;
}

PhyloTreeEdge::PhyloTreeEdge(boost::dynamic_bitset<> &edge) : super(edge) {
    attribute = unique_ptr<EdgeAttribute>(new EdgeAttribute());
    originalEdge = unique_ptr<Bipartition>(new Bipartition());
    originalID = -1;
}

PhyloTreeEdge::PhyloTreeEdge(EdgeAttribute attrib) : super() {
    attribute = unique_ptr<EdgeAttribute>(new EdgeAttribute(attrib));
    originalEdge = unique_ptr<Bipartition>(new Bipartition());
    originalID = -1;
}

PhyloTreeEdge::PhyloTreeEdge(EdgeAttribute attrib, int originalID) : super() {
    attribute = unique_ptr<EdgeAttribute>(new EdgeAttribute(attrib));
    originalEdge = unique_ptr<Bipartition>(new Bipartition());
    this->originalID = originalID;
}

PhyloTreeEdge::PhyloTreeEdge(EdgeAttribute attrib, Bipartition originalEdge, int originalID) : super() {
    attribute = unique_ptr<EdgeAttribute>(new EdgeAttribute(attrib));
    this->originalEdge = unique_ptr<Bipartition>(new Bipartition(originalEdge));
    this->originalID = originalID;
    auto empty_partition = boost::dynamic_bitset<>(originalEdge.size());
    this->setPartition(empty_partition);
}

PhyloTreeEdge::PhyloTreeEdge(Bipartition edge, EdgeAttribute attrib, int originalID) : super(edge) {
    attribute = unique_ptr<EdgeAttribute>(new EdgeAttribute(attrib));
    originalEdge = unique_ptr<Bipartition>(new Bipartition(edge));
    this->originalID = originalID;
}

PhyloTreeEdge::PhyloTreeEdge(boost::dynamic_bitset<> edge, EdgeAttribute attrib,
        boost::dynamic_bitset<> originalEdge, int originalID)
        : super(edge) {
    attribute = unique_ptr<EdgeAttribute>(new EdgeAttribute(attrib));
    this->originalEdge = unique_ptr<Bipartition>(new Bipartition(originalEdge));
    this->originalID = originalID;
}

PhyloTreeEdge::PhyloTreeEdge(const PhyloTreeEdge &other) : super(*(other.partition)) {
    attribute = unique_ptr<EdgeAttribute>(new EdgeAttribute(*(other.attribute)));
    this->originalEdge = unique_ptr<Bipartition>(new Bipartition(*(other.originalEdge)));
    this->originalID = other.originalID;
}

double PhyloTreeEdge::getLength() {
    if (!attribute) throw uninitialised_data("No attribute set");
    return attribute->norm();
}

bool PhyloTreeEdge::isZero() {
    if (!attribute) throw uninitialised_data("No attribute set");
    return fabs(attribute->norm()) < TOLERANCE;
}

string PhyloTreeEdge::toString() {
    if (!attribute || !partition) throw uninitialised_data("Either no attribute or no partition set (or both)");
    string s;
    boost::to_string(*partition, s);
    return attribute->toString() + " " + s;
}

PhyloTreeEdge PhyloTreeEdge::clone() {
    return PhyloTreeEdge(*this);
}

bool PhyloTreeEdge::equals(const PhyloTreeEdge &other) {
    if (!attribute || !partition) throw uninitialised_data("Either no attribute or no partition set (or both)");
    return this->attribute->equals(*(other.attribute)) && *(this->partition) == (*(other.partition));
}

bool PhyloTreeEdge::sameBipartition(const PhyloTreeEdge &other) {
    return *partition == *(other.partition);
}

bool PhyloTreeEdge::sameBipartition(const Bipartition &bip) const {
    return *partition == *(bip.getPartition());
}

Bipartition PhyloTreeEdge::asSplit() {
    if (!partition) throw uninitialised_data("No partition set");
    return Bipartition(*partition);
}

Bipartition PhyloTreeEdge::getOriginalEdge() {
    return *originalEdge;
}

void PhyloTreeEdge::setOriginalEdge(const Bipartition &originalEdge) {
    this->originalEdge = unique_ptr<Bipartition>(new Bipartition(originalEdge));
}

int PhyloTreeEdge::getOriginalID() {
    return originalID;
}

void PhyloTreeEdge::setOriginalID(int originalID) {
    this->originalID = originalID;
}

void PhyloTreeEdge::setAttribute(const EdgeAttribute &attrib) {
    this->attribute = unique_ptr<EdgeAttribute>(new EdgeAttribute(attrib));
}

EdgeAttribute PhyloTreeEdge::getAttribute() {
    return *attribute;
}

void PhyloTreeEdge::printEdgesVerbose(vector<PhyloTreeEdge> edges, vector<string> leaf2NumMap) {
    string output = "";

    cout << "Edge ID\t\tLength\t\tLeaves Below" << endl;
    for (int i = 0; i < edges.size(); i++) {
        if (!edges[i].getOriginalEdge().getPartition()->empty()) {
            cout << edges[i].getOriginalID() << "\t\t" << edges[i].attribute->toString() << "\t\t" << Bipartition::toStringVerbose(*(edges[i].getOriginalEdge().getPartition()), leaf2NumMap) << endl;
        } else {
            cout << edges[i].toStringVerbose(leaf2NumMap);
        }
    }
}

string PhyloTreeEdge::toStringVerbose(vector<string> leaf2NumMap) {
    ostringstream ss;
    ss << originalID << "\t\t" << attribute->toString() << "\t\t" << Bipartition::toStringVerbose(*(this->partition), leaf2NumMap);
    return ss.str();
}
