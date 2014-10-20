#include "PhyloTreeEdge.h"
#include <boost/dynamic_bitset.hpp>
#include <cmath>
#include <memory>
#define TOLERANCE 0.000000000000001

using namespace std;

class uninitialised_data: public std::runtime_error
{
    public:
        uninitialised_data(std::string const& msg):
            std::runtime_error(msg)
        {}
};

PhyloTreeEdge::PhyloTreeEdge() : super() {
    originalEdge = unique_ptr<Bipartition>(new Bipartition());
    originalID = -1;
}

PhyloTreeEdge::PhyloTreeEdge(boost::dynamic_bitset<>& edge) : super(edge) {
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

PhyloTreeEdge::PhyloTreeEdge(const PhyloTreeEdge& other) : super (*(other.partition)) {
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
    if (!attribute || !partition)  throw uninitialised_data("Either no attribute or no partition set (or both)");
    string s;
    boost::to_string(*partition, s);
    return attribute->toString() + " " + s;
}

PhyloTreeEdge PhyloTreeEdge::clone() {
    return PhyloTreeEdge(*this);
}

bool PhyloTreeEdge::equals(const PhyloTreeEdge& other) {
    if (!attribute || !partition)  throw uninitialised_data("Either no attribute or no partition set (or both)");
    return this->attribute->equals(*(other.attribute)) && *(this->partition) == (*(other.partition));
}