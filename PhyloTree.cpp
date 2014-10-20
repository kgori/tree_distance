#include "PhyloTreeEdge"
#include "Bipartition.h"
#include "EdgeAttribute.h"

PhyloTree::PhyloTree(vector<PhyloTreeEdge> edges, vector<string> leaf2NumMap, vector<EdgeAttribute> leafEdgeLengths) {

}

PhyloTree::PhyloTree(vector<PhyloTreeEdge> edges, vector<string> leaf2NumMap) {

}

PhyloTree::PhyloTree(const PhyloTree &t) {

}

PhyloTree::PhyloTree(string t, bool rooted) {

}

PhyloTree::~PhyloTree() {

}

vector<PhyloTreeEdge> PhyloTree::getCommonEdges(PhyloTree t1, PhyloTree t2) {
    return std::vector<PhyloTreeEdge, allocator<PhyloTreeEdge>>();
}

int PhyloTree::nextIndex(string t, int i, string s) {
    return 0;
}

vector<PhyloTreeEdge> PhyloTree::getEdges() {
    return std::vector<PhyloTreeEdge, allocator<PhyloTreeEdge>>();
}

void PhyloTree::setEdges(vector<PhyloTreeEdge> edges) {

}

PhyloTreeEdge PhyloTree::getEdge(int i) {
    return PhyloTreeEdge();
}

vector<string> PhyloTree::getLeaf2NumMap() {
    return std::vector<string, allocator<string>>();
}

void PhyloTree::setLeaf2NumMap(vector<string> leaf2NumMap) {

}

EdgeAttribute PhyloTree::getAttribOfSplit(Bipartition edge) {
    return EdgeAttribute();
}

vector<Bipartition> PhyloTree::getSplits() {
    return std::vector<Bipartition, allocator<Bipartition>>();
}

void PhyloTree::normalize() {

}

void PhyloTree::normalize(PhyloTree other) {

}

int PhyloTree::numLeaves() {
    return 0;
}

double PhyloTree::getDistanceFromOrigin() {
    return 0;
}

double PhyloTree::getDistanceFromOriginNoLeaves() {
    return 0;
}

double PhyloTree::getBranchLengthSum() {
    return 0;
}

vector<PhyloTreeEdge> PhyloTree::getEdgesNotInCommonWith(PhyloTree t) {
    return std::vector<PhyloTreeEdge, allocator<PhyloTreeEdge>>();
}

vector<Bipartition> PhyloTree::getCrossingsWith(PhyloTree t) {
    return std::vector<Bipartition, allocator<Bipartition>>();
}

string PhyloTree::toString() {
    return std::basic_string<char, char_traits<_CharT>, allocator<_CharT>>();
}

PhyloTree PhyloTree::clone() {
    return PhyloTree(std::vector<PhyloTreeEdge, allocator<PhyloTreeEdge>>(), std::vector<string, allocator<string>>(), std::vector<EdgeAttribute, allocator<EdgeAttribute>>());
}

bool PhyloTree::equals(PhyloTree t) {
    return false;
}

bool PhyloTree::approxEquals(PhyloTree t, double epsilon) {
    return false;
}

vector<EdgeAttribute> PhyloTree::getLeafEdgeAttribs() {
    return std::vector<EdgeAttribute, allocator<EdgeAttribute>>();
}

void PhyloTree::setLeafEdgeAttribs(vector<EdgeAttribute> leafEdgeAttribs) {

}

vector<EdgeAttribute> PhyloTree::getCopyLeafEdgeAttribs() {
    return std::vector<EdgeAttribute, allocator<EdgeAttribute>>();
}

vector<double> PhyloTree::getIntEdgeAttribNorms() {
    return std::vector<double, allocator<double>>();
}

string PhyloTree::getNewick(bool branchLengths) {
    return std::basic_string<char, char_traits<_CharT>, allocator<_CharT>>();
}

void PhyloTree::setNewick(string newick) {

}

int PhyloTree::numEdges() {
    return 0;
}

void PhyloTree::setLeaf2NumMapFromNewick() {

}

void PhyloTree::normalize(double constant) {

}