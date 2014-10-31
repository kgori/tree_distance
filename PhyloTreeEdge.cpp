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
    std::cout << "PhyloTreeEdge::PhyloTreeEdge() : super()" << std::endl;
    attribute = unique_ptr<EdgeAttribute>(new EdgeAttribute());
    originalEdge = unique_ptr<Bipartition>(new Bipartition());
    originalID = -1;
}

PhyloTreeEdge::PhyloTreeEdge(string s) : super(s) {
    std::cout << "PhyloTreeEdge::PhyloTreeEdge(string s) : super(s)" << std::endl;
    attribute = unique_ptr<EdgeAttribute>(new EdgeAttribute());
    originalEdge = unique_ptr<Bipartition>(new Bipartition());
    originalID = -1;
}

PhyloTreeEdge::PhyloTreeEdge(boost::dynamic_bitset<> &edge) : super(edge) {
    std::cout << "PhyloTreeEdge::PhyloTreeEdge(boost::dynamic_bitset<> &edge) : super(edge)" << std::endl;
    attribute = unique_ptr<EdgeAttribute>(new EdgeAttribute());
    originalEdge = unique_ptr<Bipartition>(new Bipartition());
    originalID = -1;
}

PhyloTreeEdge::PhyloTreeEdge(EdgeAttribute attrib) : super() {
    std::cout << "PhyloTreeEdge::PhyloTreeEdge(EdgeAttribute attrib) : super()" << std::endl;
    attribute = unique_ptr<EdgeAttribute>(new EdgeAttribute(attrib));
    originalEdge = unique_ptr<Bipartition>(new Bipartition());
    originalID = -1;
}

PhyloTreeEdge::PhyloTreeEdge(EdgeAttribute attrib, int originalID) : super() {
    std::cout << "PhyloTreeEdge::PhyloTreeEdge(EdgeAttribute attrib, int originalID) : super()" << std::endl;
    attribute = unique_ptr<EdgeAttribute>(new EdgeAttribute(attrib));
    originalEdge = unique_ptr<Bipartition>(new Bipartition());
    this->originalID = originalID;
}

PhyloTreeEdge::PhyloTreeEdge(EdgeAttribute attrib, Bipartition originalEdge, int originalID) : super() {
    std::cout << "PhyloTreeEdge::PhyloTreeEdge(EdgeAttribute attrib, Bipartition originalEdge, int originalID) : super()" << std::endl;
    attribute = unique_ptr<EdgeAttribute>(new EdgeAttribute(attrib));
    this->originalEdge = unique_ptr<Bipartition>(new Bipartition(originalEdge));
    this->originalID = originalID;
    auto empty_partition = boost::dynamic_bitset<>(originalEdge.size());
    this->setPartition(empty_partition);
}

PhyloTreeEdge::PhyloTreeEdge(Bipartition edge, EdgeAttribute attrib, int originalID) : super(edge) {
    std::cout << "PhyloTreeEdge::PhyloTreeEdge(Bipartition edge, EdgeAttribute attrib, int originalID) : super(edge)" << std::endl;
    attribute = unique_ptr<EdgeAttribute>(new EdgeAttribute(attrib));
    originalEdge = unique_ptr<Bipartition>(new Bipartition(edge));
    this->originalID = originalID;
}

PhyloTreeEdge::PhyloTreeEdge(boost::dynamic_bitset<> edge, EdgeAttribute attrib,
        boost::dynamic_bitset<> originalEdge, int originalID)
        : super(edge) {
    std::cout << "PhyloTreeEdge::PhyloTreeEdge(boost::dynamic_bitset<> edge, EdgeAttribute attrib, boost::dynamic_bitset<> originalEdge, int originalID)" << std::endl;
    attribute = unique_ptr<EdgeAttribute>(new EdgeAttribute(attrib));
    this->originalEdge = unique_ptr<Bipartition>(new Bipartition(originalEdge));
    this->originalID = originalID;
}

PhyloTreeEdge::PhyloTreeEdge(const PhyloTreeEdge &other) : super(*(other.partition)) {
    std::cout << "PhyloTreeEdge::PhyloTreeEdge(const PhyloTreeEdge &other) : super(*(other.partition))" << std::endl;
    attribute = unique_ptr<EdgeAttribute>(new EdgeAttribute(*(other.attribute)));
    this->originalEdge = unique_ptr<Bipartition>(new Bipartition(*(other.originalEdge)));
    this->originalID = other.originalID;
}

double PhyloTreeEdge::getLength() {
    std::cout << "PhyloTreeEdge::getLength()" << std::endl;
    if (!attribute) throw uninitialised_data("No attribute set");
    return attribute->norm();
}

bool PhyloTreeEdge::isZero() {
    std::cout << "PhyloTreeEdge::isZero()" << std::endl;
    if (!attribute) throw uninitialised_data("No attribute set");
    return fabs(attribute->norm()) < TOLERANCE;
}

string PhyloTreeEdge::toString() {
    std::cout << "PhyloTreeEdge::toString()" << std::endl;
    if (!attribute || !partition) throw uninitialised_data("Either no attribute or no partition set (or both)");
    string s;
    boost::to_string(*partition, s);
    return attribute->toString() + " " + s;
}

PhyloTreeEdge PhyloTreeEdge::clone() {
    std::cout << "PhyloTreeEdge::clone()" << std::endl;
    return PhyloTreeEdge(*this);
}

bool PhyloTreeEdge::equals(const PhyloTreeEdge &other) {
    std::cout << "PhyloTreeEdge::equals(const PhyloTreeEdge &other)" << std::endl;
    if (!attribute || !partition) throw uninitialised_data("Either no attribute or no partition set (or both)");
    return this->attribute->equals(*(other.attribute)) && *(this->partition) == (*(other.partition));
}

bool PhyloTreeEdge::sameBipartition(const PhyloTreeEdge &other) {
    std::cout << "PhyloTreeEdge::sameBipartition(const PhyloTreeEdge &other)" << std::endl;
    return *partition == *(other.partition);
}

bool PhyloTreeEdge::sameBipartition(const Bipartition &bip) const {
    std::cout << "PhyloTreeEdge::sameBipartition(const Bipartition &bip) const" << std::endl;
    return *partition == *(bip.getPartition());
}

Bipartition PhyloTreeEdge::asSplit() {
    std::cout << "PhyloTreeEdge::asSplit()" << std::endl;
    if (!partition) throw uninitialised_data("No partition set");
    return Bipartition(*partition);
}

Bipartition PhyloTreeEdge::getOriginalEdge() {
    std::cout << "PhyloTreeEdge::getOriginalEdge()" << std::endl;
    return *originalEdge;
}

void PhyloTreeEdge::setOriginalEdge(const Bipartition &originalEdge) {
    std::cout << "PhyloTreeEdge::setOriginalEdge(const Bipartition &originalEdge)" << std::endl;
    this->originalEdge = unique_ptr<Bipartition>(new Bipartition(originalEdge));
}

int PhyloTreeEdge::getOriginalID() {
    std::cout << "PhyloTreeEdge::getOriginalID()" << std::endl;
    return originalID;
}

void PhyloTreeEdge::setOriginalID(int originalID) {
    std::cout << "PhyloTreeEdge::setOriginalID(int originalID)" << std::endl;
    this->originalID = originalID;
}

void PhyloTreeEdge::setAttribute(const EdgeAttribute &attrib) {
    std::cout << "PhyloTreeEdge::setAttribute(const EdgeAttribute &attrib)" << std::endl;
    this->attribute = unique_ptr<EdgeAttribute>(new EdgeAttribute(attrib));
}

EdgeAttribute PhyloTreeEdge::getAttribute() {
    std::cout << "PhyloTreeEdge::getAttribute()" << std::endl;
    return *attribute;
}

void PhyloTreeEdge::printEdgesVerbose(vector<PhyloTreeEdge> edges, vector<string> leaf2NumMap) {
    std::cout << "PhyloTreeEdge::printEdgesVerbose(vector<PhyloTreeEdge> edges, vector<string> leaf2NumMap)" << std::endl;
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
    std::cout << "PhyloTreeEdge::toStringVerbose(vector<string> leaf2NumMap)" << std::endl;
    ostringstream ss;
    ss << originalID << "\t\t" << attribute->toString() << "\t\t" << Bipartition::toStringVerbose(*(this->partition), leaf2NumMap);
    return ss.str();
}

double PhyloTreeEdge::getLength() const {
    std::cout << "PhyloTreeEdge::getLength() const" << std::endl;
    if (!attribute) throw uninitialised_data("No attribute set");
    return attribute->norm();
}
