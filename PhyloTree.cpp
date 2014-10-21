#include "PhyloTree.h"
#include <deque>

using namespace std;

PhyloTree::PhyloTree(vector<PhyloTreeEdge> edges, vector<string> leaf2NumMap, vector<EdgeAttribute> leafEdgeLengths) {
    this->edges = edges;
    this->leaf2NumMap = leaf2NumMap;
    this->leafEdgeAttribs = leafEdgeLengths;
}

PhyloTree::PhyloTree(vector<PhyloTreeEdge> edges, vector<string> leaf2NumMap) {
    this->edges = edges;
    this->leaf2NumMap = leaf2NumMap;
}

PhyloTree::PhyloTree(const PhyloTree &t) {
    edges = vector<PhyloTreeEdge>(t.edges);
    leaf2NumMap = vector<string>(t.leaf2NumMap);
    leafEdgeAttribs = vector<EdgeAttribute>(t.leafEdgeAttribs);
    newick = string(t.newick);
}

PhyloTree::PhyloTree(string t, bool rooted) {
    int leafNum = 0;
    deque<PhyloTreeEdge> q;
    vector<PhyloTreeEdge> edges;
    string label, length;
    size_t i = 0;

    // remove anything before the first (
    t = t.substr(t.find_first_of("("));
    newick = t;

    //pull off ';' if at end
    t = t.erase(t.find_last_of(";"));

    // pull off the first and last brackets (and a root length, between the last bracket and ;, if there is one.
    t = t.substr(t.find_first_of("("));
    t = t.erase(t.find_last_of(")"));

    // do bracket counting sanity check
    if (count(t.begin(), t.end(), '(') != count(t.begin(), t.end(), ')')) {
        throw invalid_argument("Bracket mismatch error in tree: " + newick);
    }

    try {
        setLeaf2NumMapFromNewick();
        leafEdgeAttribs = vector<EdgeAttribute>(leaf2NumMap.size());
        while (i < t.size()) {
            switch (t.at(i)) {
                case '(': {
                    q.push_front(PhyloTreeEdge());
                    i++;
                    break;
                }

                case ')': {
                    size_t j = nextIndex(t, i+2, ",)");
                    q.front().setAttribute(EdgeAttribute(t.substr(i+2, j)));
                    edges.push_back(q.front());
                    q.pop_front();
                    i = j;
                    break;
                }

                case ',': {
                    i++;
                    break;
                }

                // this char is the beginning of a leaf name
                default: {
                    size_t end_of_label = nextIndex(t, i, ":");
                    size_t end_of_length = nextIndex(t, end_of_label, " ,)");
                    label = t.substr(i, end_of_label - i);
                    length = t.substr(end_of_label + 1, end_of_length - end_of_label - 1);
                    leafNum = lower_bound(leaf2NumMap.begin(), leaf2NumMap.end(), label) - leaf2NumMap.begin();
                    if (leafNum == leaf2NumMap.size()) throw out_of_range("Could not find label (" + label + ") in leaf2NumMap");
                    leafEdgeAttribs[leafNum] = EdgeAttribute(length);

                    for (auto &e: q) {
                        e.addOne((size_t) leafNum);
                    }
                    i = end_of_length;
                }
            }
        }
    }
    catch (out_of_range& range_err) {
        throw range_err;
    }

    if (!rooted) {
        for (auto &e : edges) {
            size_t lastLeaf = leaf2NumMap.size() - 1;
            if (e.contains(lastLeaf)) {
                e.complement(lastLeaf + 1);
            }
        }
    }

    for (size_t k = 0; k < edges.size(); ++k) {
        edges[k].setOriginalEdge(edges[k].asSplit());
        edges[k].setOriginalID(k);
    } 
}

vector<PhyloTreeEdge> PhyloTree::getCommonEdges(PhyloTree t1, PhyloTree t2) {
    vector<PhyloTreeEdge> commonEdges;
    if (t1.leaf2NumMap != t2.leaf2NumMap) {
        throw runtime_error("leaf2NumMaps are not equal");
    }
    for (auto e1 : t1.edges) {
        if (!e1.isZero()) {
            auto splits = t2.getSplits();
            if (std::find(splits.begin(), splits.end(), e1.asSplit()) != splits.end()) {
                auto commonAttrib = EdgeAttribute::difference(e1.getAttribute(), t2.getAttribOfSplit(e1.asSplit()));
                commonEdges.push_back(PhyloTreeEdge(e1.asSplit(), commonAttrib, e1.getOriginalID()));
            }
            else if (e1.isCompatibleWith(t2.getSplits())) {
                auto commonAttrib = e1.getAttribute();
                commonEdges.push_back(PhyloTreeEdge(e1.asSplit(), commonAttrib, e1.getOriginalID()));
            }
        }
    }
    for (auto e2 : t2.getEdges()) {
        if (!e2.isZero()) {
            auto splits = t1.getSplits();
            if (e2.isCompatibleWith(t1.getSplits()) && std::find(splits.begin(), splits.end(), e2.asSplit()) == splits.end()) {
                auto commonAttrib = e2.getAttribute();
                commonEdges.push_back(PhyloTreeEdge(e2.asSplit(), commonAttrib, e2.getOriginalID()));
            }
        }
    }
    return commonEdges;
}

size_t PhyloTree::nextIndex(string t, size_t i, string s) {
    return t.find_first_of(s, i);
}

vector<PhyloTreeEdge> PhyloTree::getEdges() {
    return edges;
}

void PhyloTree::setEdges(vector<PhyloTreeEdge> edges) {
    this->edges = edges;
}

PhyloTreeEdge PhyloTree::getEdge(int i) {
    return edges[i];
}

vector<string> PhyloTree::getLeaf2NumMap() {
    return leaf2NumMap;
}

void PhyloTree::setLeaf2NumMap(vector<string> leaf2NumMap) {
    this->leaf2NumMap = leaf2NumMap;
}

EdgeAttribute PhyloTree::getAttribOfSplit(Bipartition edge) {
    return EdgeAttribute();
}

vector<Bipartition> PhyloTree::getSplits() {
    vector<Bipartition> splits;

    for (auto edge : edges) {
        splits.push_back(edge.asSplit());
    }

    return splits;
}

void PhyloTree::normalize() {
    double vecLength = getDistanceFromOrigin();
    normalize(vecLength);
}

void PhyloTree::normalize(PhyloTree& other) {
    double combinedVecLength = getDistanceFromOrigin() + other.getDistanceFromOrigin();
    normalize(combinedVecLength);
    other.normalize(combinedVecLength);
}

size_t PhyloTree::numLeaves() {
    return leaf2NumMap.size();
}

double PhyloTree::getDistanceFromOrigin() {
    double dist = 0;
    for (auto &edge : edges) {
        dist += std::pow(edge.getLength(), 2);
    }

    for (auto &attrib : leafEdgeAttribs) {
        dist += std::pow(attrib.norm(), 2);
    }

    return std::sqrt(dist);
}

double PhyloTree::getDistanceFromOriginNoLeaves() {
    double dist = 0;
    for (auto &edge : edges) {
        dist += std::pow(edge.getLength(), 2);
    }
    return std::sqrt(dist);
}

double PhyloTree::getBranchLengthSum() {
    double sum = 0;
    for (auto &edge : edges) {
        sum += edge.getLength();
    }

    for (auto &attrib : leafEdgeAttribs) {
        sum += attrib.norm();
    }

    return sum;
}

vector<PhyloTreeEdge> PhyloTree::getEdgesNotInCommonWith(PhyloTree t) {
    vector<PhyloTreeEdge> notCommonEdges;
    if (leaf2NumMap != t.leaf2NumMap) {
        throw runtime_error("leaf2NumMaps are not equal");
    }
    for (auto &this_edge : edges) {
        if (this_edge.isZero()) continue;

        for (auto &that_edge : t.edges) {
            if (that_edge.isZero()) continue;
            if (this_edge.sameBipartition(that_edge)) {
                break;
            }
            notCommonEdges.push_back(this_edge.clone());
        }
    }
    return notCommonEdges;
}

//vector<Bipartition> PhyloTree::getCrossingsWith(PhyloTree t) {
//    return std::vector<Bipartition, allocator<Bipartition>>();
//}

//string PhyloTree::toString() {
//    return string();
//}

PhyloTree PhyloTree::clone() {
    return PhyloTree(*this);
}

bool PhyloTree::equals(PhyloTree t) {
    return (leaf2NumMap == t.leaf2NumMap) && Tools::vector_equal(leafEdgeAttribs, t.leafEdgeAttribs) && Tools::vector_equal(edges, t.edges);
}

//bool PhyloTree::approxEquals(PhyloTree t, double epsilon) {
//    return false;
//}

vector<EdgeAttribute> PhyloTree::getLeafEdgeAttribs() {
    return leafEdgeAttribs;
}

void PhyloTree::setLeafEdgeAttribs(vector<EdgeAttribute> otherEdgeAttribs) {
    leafEdgeAttribs.clear();
    for (auto e : otherEdgeAttribs) {
        leafEdgeAttribs.push_back(e);
    }
}

vector<EdgeAttribute> PhyloTree::getCopyLeafEdgeAttribs() {
    return std::vector<EdgeAttribute, allocator<EdgeAttribute>>();
}

vector<double> PhyloTree::getIntEdgeAttribNorms() {
    vector<double> norms;
    for (auto &e : edges) {
        norms.push_back(e.getAttribute().norm());
    }
}

string PhyloTree::getNewick(bool branchLengths) {
    return std::string();
}

void PhyloTree::setNewick(string newick) {

}

int PhyloTree::numEdges() {
    return 0;
}

void PhyloTree::setLeaf2NumMapFromNewick() {

}

void PhyloTree::normalize(double constant) {
    // divide by the length of the split length vector to normalize
    for (int i = 0; i < leafEdgeAttribs.size(); i++) {
        leafEdgeAttribs[i].scaleBy(1.0 / constant);
    }
    for (int i = 0; i < edges.size(); i++) {
        edges[i].getAttribute().scaleBy(1.0 / constant);
    }
}