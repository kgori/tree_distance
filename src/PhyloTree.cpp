#include "PhyloTree.h"
#include "Tools.h"
#include "bitset_hash.h"
#include <unordered_map>
#include <cmath>

#define LENGTH_DEFAULT 0.0
//#define DEBUGPRINT

using namespace std;

PhyloTree::PhyloTree(vector<PhyloTreeEdge> &edges, vector<string> &leaf2NumMap, vector<double> &leafEdgeLengths) {
    this->edges = edges;
    this->leaf2NumMap = leaf2NumMap;
    this->leafEdgeLengths = leafEdgeLengths;
}

PhyloTree::PhyloTree(vector<PhyloTreeEdge> &edges, vector<string> &leaf2NumMap) : leaf2NumMap(leaf2NumMap) {
//    this->leaf2NumMap = leaf2NumMap;
    size_t len = leaf2NumMap.size();
    this->edges.reserve(edges.size());
    for (auto &edge : edges) {
        boost::dynamic_bitset<> new_bitset(len);
        auto partition = edge.getPartition();
        auto plen = partition.size();
        for (size_t i=0; i < len; ++i) {
            new_bitset[len - i - 1] = partition[plen - i - 1];
        }
        edge.setOriginalEdge(make_shared<Bipartition>(new_bitset));
        this->edges.push_back(edge);
    }
}

PhyloTree::PhyloTree(const PhyloTree &t) : edges(t.edges), leaf2NumMap(t.leaf2NumMap), leafEdgeLengths(t.leafEdgeLengths), newick(t.newick) {
}

PhyloTree::PhyloTree(string t, bool rooted) {
    // do bracket counting sanity check
    if (count(t.begin(), t.end(), '(') != count(t.begin(), t.end(), ')')) {
        throw invalid_argument("Bracket mismatch error in tree: " + t);
    }

    int leafNum = 0;
    deque<PhyloTreeEdge> q;
    string label, length;
    size_t i = 0;

    // remove anything before the first (
    t = t.substr(t.find_first_of("("));
    newick = t;

    // remove whitespace
    Tools::despace(t);
    setLeaf2NumMapFromNewick(t);
    //pull off ';' if at end
//    t = t.erase(t.find_last_of(";"));

    // pull off the first and last brackets (and a root length, between the last bracket and ;, if there is one.
    t = t.substr(t.find_first_of("(") + 1);
    t = t.erase(t.find_last_of(")"));

    try {
        size_t end_of_label, end_of_length, alt_end_of_label;
        leafEdgeLengths = vector<double>(leaf2NumMap.size());
        while (i < t.size()) {
            switch (t.at(i)) {
                case '(': {
                    q.push_front(PhyloTreeEdge(string(leaf2NumMap.size(), '0')));
                    i++;
                    break;
                }

                case ')': {
                    end_of_length = Tools::nextIndex(t, i + 2, ",)");
                    if (t[i + 1] == ':') {
                        length = Tools::substring(t, i + 2, end_of_length);
                    }
                    else {
                        length = LENGTH_DEFAULT;
                    }
                    q.front().setAttribute(stod(length));
                    edges.push_back(q.front());
                    q.pop_front();
                    i = end_of_length;
                    break;
                }

                case ',': {
                    i++;
                    break;
                }

                    // this char is the beginning of a leaf name
                default: {
                    end_of_label = Tools::nextIndex(t, i, ":");

                    // check if 'correct' :-delimited length is missing
                    alt_end_of_label = Tools::nextIndex(t, i, " ,()");
                    if (alt_end_of_label < end_of_label) {
                        end_of_label = end_of_length = alt_end_of_label;
                        length = LENGTH_DEFAULT;
                    }
                    else {
                        end_of_length = Tools::nextIndex(t, end_of_label, " ,)");
                        length = Tools::substring(t, end_of_label + 1, end_of_length);
                    }
                    label = Tools::substring(t, i, end_of_label);
                    leafNum = lower_bound(leaf2NumMap.begin(), leaf2NumMap.end(), label) - leaf2NumMap.begin();
                    if (leafNum == leaf2NumMap.size()) throw out_of_range("Could not find label (" + label + ") in leaf2NumMap");
                    leafEdgeLengths[leafNum] = stod(length);

                    for (auto &e: q) {
                        e.addOne((size_t) leafNum);
                    }
                    i = end_of_length;
                }
            }
        }
    }
    catch (out_of_range &range_err) {
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
        edges[k].setOriginalEdge(make_shared<Bipartition>(edges[k].asSplit()));
        edges[k].setOriginalID((int) k);
    }
}

vector<PhyloTreeEdge> PhyloTree::getEdges() {
    return edges;
}

const vector<PhyloTreeEdge>& PhyloTree::getEdgesByRef() const {
    return edges;
}

void PhyloTree::getEdges(vector<PhyloTreeEdge>& edges_to_add) {
    edges_to_add.reserve(edges.size());
    for (auto &edge : edges) {
        edges_to_add.push_back(edge);
    }
}

void PhyloTree::setEdges(vector<PhyloTreeEdge> edges) {
    this->edges = edges;
}

PhyloTreeEdge PhyloTree::getEdge(size_t i) {
    return edges[i];
}

vector<string> PhyloTree::getLeaf2NumMap() {
    return leaf2NumMap;
}

vector<string> PhyloTree::getLeaf2NumMap() const {
    return leaf2NumMap;
}

void PhyloTree::setLeaf2NumMap(vector<string> leaf2NumMap) {
    this->leaf2NumMap = leaf2NumMap;
}

double PhyloTree::getAttribOfSplit(Bipartition& edge) {
    for (auto &e : edges) {
        if (e.sameBipartition(edge)) {
            return e.getLength();
        }
    }
    return 0;
}

//EdgeAttribute PhyloTree::getAttribOfSplit(PhyloTreeEdge& edge) {
//    for (auto &e : edges) {
//        if (e.sameBipartition(edge)) {
//            return e.getAttribute();
//        }
//    }
//    return EdgeAttribute();
//}

vector<Bipartition> PhyloTree::getSplits() {
    vector<Bipartition> splits;
    splits.reserve(edges.size());
    for (auto &edge : edges) {
        splits.push_back(edge.asSplit());
    }

    return splits;
}

void PhyloTree::normalize() {
    double vecLength = getDistanceFromOrigin();
    normalize(vecLength);
}

void PhyloTree::normalize(PhyloTree &other) {
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

    for (auto attrib : leafEdgeLengths) {
        dist += std::pow(attrib, 2);
    }

    return std::sqrt(dist);
}

//double PhyloTree::getDistanceFromOriginNoLeaves() {
//    double dist = 0;
//    for (auto &edge : edges) {
//        dist += std::pow(edge.getLength(), 2);
//    }
//    return std::sqrt(dist);
//}

double PhyloTree::getBranchLengthSum() {
    double sum = 0;
    for (auto &edge : edges) {
        sum += edge.getLength();
    }

    for (auto attrib : leafEdgeLengths) {
        sum += attrib;
    }

    return sum;
}

void PhyloTree::getEdgesNotInCommonWith(PhyloTree &t, vector<PhyloTreeEdge>& dest) {
    bool not_common;
    if (leaf2NumMap != t.leaf2NumMap) {
        throw runtime_error("leaf2NumMaps are not equal");
    }
    for (auto &this_edge : edges) {
        not_common = true;
        //if (this_edge.isZero()) continue;

        for (auto &that_edge : t.edges) {
            //if (that_edge.isZero()) continue;
            if (this_edge.sameBipartition(that_edge)) {
                not_common = false;
            }
        }
        if (not_common)
            dest.push_back(this_edge);
    }
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
    return (leaf2NumMap == t.leaf2NumMap) && Tools::vector_equal(leafEdgeLengths, t.leafEdgeLengths) && Tools::vector_equal(edges, t.edges);
}

//bool PhyloTree::approxEquals(PhyloTree t, double epsilon) {
//    return false;
//}

vector<double> PhyloTree::getLeafEdgeLengths() {
    return leafEdgeLengths;
}

const vector<double>& PhyloTree::getLeafEdgeLengthsByRef() const {
    return leafEdgeLengths;
}

void PhyloTree::setLeafEdgeLengths(vector<double>& otherEdgeAttribs) {
    leafEdgeLengths = otherEdgeAttribs;
}

//vector<EdgeAttribute> PhyloTree::getCopyLeafEdgeAttribs() {
//    return std::vector<EdgeAttribute, allocator<EdgeAttribute>>();
//}

vector<double> PhyloTree::getIntEdgeAttribNorms() {
    vector<double> norms;
    norms.reserve(edges.size());
    for (auto &e : edges) {
        norms.push_back(e.getLength());
    }
    return norms;
}

string PhyloTree::getNewick(bool branchLengths) {
    deque<string> strPieces;
    deque<PhyloTreeEdge> corrEdges;

    if (edges.size() == 0) // star-tree
    {
        strPieces.push_back("(");
        for (size_t i = 0; i < leaf2NumMap.size() - 1; ++i) {
            strPieces.push_back(leaf2NumMap[i]);
            if (branchLengths) {
                strPieces.push_back(":");
                strPieces.push_back(Tools::double_to_string(leafEdgeLengths[i]));
            }
            strPieces.push_back(",");
        }
        strPieces.push_back(leaf2NumMap.back());
        strPieces.push_back(":");
        strPieces.push_back(Tools::double_to_string(leafEdgeLengths.back()));
        strPieces.push_back(");");
        return Tools::string_join(strPieces, "");
    }

    vector<PhyloTreeEdge> edgesLeft(edges);

    while (edgesLeft.size() > 0) {
        auto minEdge = edgesLeft.front();
        size_t pos = 0;
        for (size_t i = 1; i < edgesLeft.size(); ++i) {
            if (minEdge.contains(edgesLeft[i])) {
                minEdge = edgesLeft[i];
                pos = i;
            }
        }
        Tools::vector_remove_element_at_index(edgesLeft, pos);

        corrEdges.push_front(minEdge.clone());
        strPieces.push_front("");

        // now we have a min element.
        // Start its Newick string.
        string str1 = "(";

        // Since we are allowing degenerate trees, there could be an arbitrary number of such edges.
        size_t k = 1;
        while (k < corrEdges.size()) {
            if (minEdge.contains(corrEdges[k])) {
                // add it to the string
                str1 += strPieces[k] + ",";

                // remove each leaf in this split from minEdge
                minEdge.andNot(corrEdges[k]);

                // remove this split and its corresponding string from the vectors
                Tools::deque_remove_element_at_index(strPieces, k);
                Tools::deque_remove_element_at_index(corrEdges, k);
            } else {
                k++;
            }
        }

        // add all the elements still in minEdge (These are leaves that weren't already added as part of
        // a min split contained by minEdge.)
        if (!minEdge.getPartition().empty()) {
            for (size_t i = 0; i < minEdge.getPartition().size(); i++) {
                if (minEdge.getPartition()[i]) {
                    str1 += leaf2NumMap[i];
                    if (branchLengths) {
                        str1 += ":" + Tools::double_to_string(leafEdgeLengths[i]);
                    }
                    str1 += ",";
                }
            }
        }
        // remove the last , and add the bracket and minEdge length
        str1 = Tools::substring(str1, 0, str1.length() - 1) + ")";
        if (branchLengths) {
            str1 += ":" + Tools::double_to_string(minEdge.getLength());
        }
        // store str1
        strPieces.push_front(str1);
    }

    // now we need to combine all edges in corrEdges and all remaining leaves
    Bipartition allLeaves = Bipartition(string(leaf2NumMap.size(), '1'));

    string newickString = "(";
    // add all the string pieces we've accumulated
    for (int i = 0; i < corrEdges.size(); i++) {
        newickString += strPieces[i] + ",";
        allLeaves.andNot(corrEdges[i]);
    }
    // add all remaining leaves
    if (!allLeaves.isEmpty()) {
        for (size_t i = 0; i < allLeaves.size(); i++) {
            if ((allLeaves.getPartition())[i]) {
                newickString = newickString + leaf2NumMap[i];
                if (branchLengths) {
                    newickString += ":" + Tools::double_to_string(leafEdgeLengths[i]);
                }
                newickString += ",";
            }
        }
    }

    // remove the last ,
    newickString = Tools::substring(newickString, 0, newickString.length() - 1) + ");";
    return newickString;
}

void PhyloTree::setNewick(string newick) {
    this->newick = newick;
}

size_t PhyloTree::numEdges() {
    return edges.size();
}

void PhyloTree::setLeaf2NumMapFromNewick(string& s) {
    // go through the string and pull out all the leaf labels:  any string between '(' and ':' or ',' and ':'
    size_t i = 0;
    while (i < s.length()) {
        // however, the first character might be the beginning of a leaf label
        if ((s[i] == '(' || s[i] == ',') && (s[i + 1] != '(')) {
            size_t end_of_label = Tools::nextIndex(s, i, ":");
            string label = Tools::substring(s, i + 1, end_of_label);
            leaf2NumMap.push_back(label);
            i = Tools::nextIndex(s, i, ",)");
        } else {
            i++;
        }
    }
    // sort the elements of leaf2NumMap
    std::sort(leaf2NumMap.begin(), leaf2NumMap.end());
}

void PhyloTree::normalize(double constant) {
    // divide by the length of the split length vector to normalize
    for (int i = 0; i < leafEdgeLengths.size(); i++) {
        leafEdgeLengths[i] += (1.0 / constant);
    }
    for (int i = 0; i < edges.size(); i++) {
        edges[i].scaleBy(1.0 / constant);
    }
}

bool PhyloTree::removeSplit(const Bipartition &e) {
    bool removed = false;
    size_t i = 0;
    while (i < edges.size() && !removed) {
        if (edges[i].sameBipartition(e)) {
            Tools::vector_remove_element_at_index(edges, i);
            removed = true;
        }
        i++;
    }
    return removed;
}

//void PhyloTree::removeSplits(const vector<Bipartition>& splits) {
//    for (auto &split : splits) {
//        removeSplit(split);
//    }
//}

string PhyloTree::toString() {
    size_t nlabels = this->leaf2NumMap.size();
    size_t nedges = this->edges.size();
    size_t nleaves = this->leafEdgeLengths.size();
    if (nlabels == 0 || nedges == 0 || nleaves == 0) return "Empty";
    stringstream ss;
    ss << "Leaves: ";
    for (size_t i = 0; i < nlabels - 1; ++i) {
        ss << leaf2NumMap[i] << " ";
    }
    ss << leaf2NumMap.back();
    if (nedges > 0) {
        ss << "; edges: ";
        for (size_t i = 0; i < edges.size() - 1; ++i) {
            ss << edges[i].toString() << " ";
        }
        ss << edges.back().toString();
    }
    if (nleaves > 0) {
        ss << "; leaf edges: [";
        for (size_t i = 0; i < nleaves - 1; ++i) {
            ss << leafEdgeLengths[i] << " ";
        }
        ss << leafEdgeLengths.back() << "]";
    }
    return ss.str();
}

//double PhyloTree::getDistanceFromOrigin() const {
//    double dist = 0;
//    for (auto &edge : edges) {
//        dist += std::pow(edge.getLength(), 2);
//    }
//
//    for (auto &attrib : leafEdgeLengths) {
//        dist += std::pow(attrib.norm(), 2);
//    }
//
//    return std::sqrt(dist);
//}

//vector<EdgeAttribute> PhyloTree::getLeafEdgeAttribs() const {
//    return leafEdgeLengths;
//}

void PhyloTree::getCommonEdges(PhyloTree &t1, PhyloTree &t2, vector<PhyloTreeEdge> &dest) {
    vector<PhyloTreeEdge> t1_edges;
    vector<PhyloTreeEdge> t2_edges;
    t1.getEdges(t1_edges);
    t2.getEdges(t2_edges);
    std::sort(t1_edges.begin(), t1_edges.end());
    std::sort(t2_edges.begin(), t2_edges.end());

    auto first1 = t1_edges.begin();
    auto first2 = t2_edges.begin();
    auto last1 = t1_edges.end();
    auto last2 = t2_edges.end();
    auto common_bkinstr = std::back_inserter(dest);

    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            if (first1->isCompatibleWith(t2_edges)) {
                dest.emplace_back(first1->asSplit(), first1->getAttribute(), first1->getOriginalID());
            }
            ++first1; // first1 not in list2
        } else {
            if (!(*first2 < *first1)) { // first1 == first2
                *common_bkinstr++ = std::move(PhyloTreeEdge(first1->asSplit(), first1->getLength() - first2->getLength(), first1->getOriginalID()));
                ++first1;
            }
            else { // first2 not in list1
                if (first2->isCompatibleWith(t1_edges)) {
                    dest.emplace_back(first2->asSplit(), first2->getAttribute(), first2->getOriginalID());
                }
            }
            ++first2;
        }
    }
}

void PhyloTree::getCommonEdges(vector<PhyloTreeEdge> &t1_edges, vector<PhyloTreeEdge> &t2_edges, vector<PhyloTreeEdge> &dest) {
    std::sort(t1_edges.begin(), t1_edges.end());
    std::sort(t2_edges.begin(), t2_edges.end());

    auto first1 = t1_edges.begin();
    auto first2 = t2_edges.begin();
    auto last1 = t1_edges.end();
    auto last2 = t2_edges.end();
    auto common_bkinstr = std::back_inserter(dest);

    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            if (first1->isCompatibleWith(t2_edges)) {
                dest.emplace_back(first1->asSplit(), first1->getAttribute(), first1->getOriginalID());
            }
            ++first1; // first1 not in list2
        } else {
            if (!(*first2 < *first1)) { // first1 == first2
                *common_bkinstr++ = std::move(PhyloTreeEdge(first1->asSplit(), first1->getLength() - first2->getLength(), first1->getOriginalID()));
                ++first1;
            }
            else { // first2 not in list1
                if (first2->isCompatibleWith(t1_edges)) {
                    dest.emplace_back(first2->asSplit(), first2->getAttribute(), first2->getOriginalID());
                }
            }
            ++first2;
        }
    }
}

PhyloTreeEdge PhyloTree::getFirstCommonEdge(vector<PhyloTreeEdge> &t1_edges, vector<PhyloTreeEdge> &t2_edges) {
    std::sort(t1_edges.begin(), t1_edges.end());
    std::sort(t2_edges.begin(), t2_edges.end());

    auto first1 = t1_edges.begin();
    auto first2 = t2_edges.begin();
    auto last1 = t1_edges.end();
    auto last2 = t2_edges.end();

    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            if (first1->isCompatibleWith(t2_edges)) {
                return PhyloTreeEdge(first1->asSplit(), first1->getAttribute(), first1->getOriginalID());
            }
            ++first1; // first1 not in list2
        } else {
            if (!(*first2 < *first1)) { // first1 == first2
                return PhyloTreeEdge(first1->asSplit(), first1->getLength() - first2->getLength(), first1->getOriginalID());
            }
            else { // first2 not in list1
                if (first2->isCompatibleWith(t1_edges)) {
                    return PhyloTreeEdge(first2->asSplit(), first2->getAttribute(), first2->getOriginalID());
                }
            }
            ++first2;
        }
    }
    throw edge_not_found_exception("No common edges");
}

/*
 * Remove taxa at indices in `missing` from PhyloTree `tree`,
 * returning a new pruned tree
 */
PhyloTree::PhyloTree(const PhyloTree& other, const vector<int>& missing)
{
    vector<double> pruned_lengths;  // temp store relevant edge lengths from other tree

    // TODO: combine these into a single loop
    Tools::prune_container(other.leaf2NumMap.begin(), other.leaf2NumMap.end(),
            missing.begin(), missing.end(), std::back_inserter(this->leaf2NumMap)); // fill pruned leaf2NumMap
    Tools::prune_container(other.leafEdgeLengths.begin(), other.leafEdgeLengths.end(),
            missing.begin(), missing.end(), std::back_inserter(pruned_lengths)); // fill temp lengths store

    this->leafEdgeLengths.resize(this->leaf2NumMap.size(), 0); // initialise leaf lengths vector

#ifdef DEBUGPRINT
    cout << "DEBUG - pruned_names" << endl;
    Tools::vector_print(this->leaf2NumMap.begin(), this->leaf2NumMap.end());

    cout << "DEBUG - pruned_lengths" << endl;
    Tools::vector_print(pruned_lengths.begin(), pruned_lengths.end());
    cout << "DEBUG end" << endl;
#endif

    std::unordered_map<string, size_t> name_indices;
    for (size_t i = 0; i<this->leaf2NumMap.size(); i++) {
        name_indices[this->leaf2NumMap[i]] = i;
    }
#ifdef DEBUGPRINT
    cout << "DEBUG - name_indices" << endl;
    for (auto& item: name_indices) {
        cout << item.first << " " << item.second << endl;
    }
    cout << "DEBUG end" << endl;
#endif

    std::unordered_map<bitset_t, EdgeInfo, BitsetHash> hashmap;
    int inner_edge_counter = 0;
    for (auto& edge: other.edges) {
        auto pruned = Tools::prune_bitset(edge.getPartition(), missing);

#ifdef DEBUGPRINT
        auto edge_copy = &edge;
        cout << "DEBUG - edge = " << const_cast<PhyloTreeEdge*>(edge_copy)->toString() << " pruned = " << pruned
                << endl;
#endif

        EdgeInfo info;
        if (hashmap.find(pruned)==hashmap.end()) {
            if (hashmap.find(~pruned)==hashmap.end()) {
                info.length = edge.getLength();
                if (Tools::is_leaf(pruned)) {
                    size_t ix = Tools::leaf_index_nothrow(pruned);
                    info.name = this->leaf2NumMap[ix];
                    info.length += pruned_lengths[ix];
                    info.leaf = true;
                }
                else {
                    info.id = inner_edge_counter++;
                }
                hashmap[pruned] = info;
            }
            else {
                info = hashmap[~pruned];
            }

#ifdef DEBUGPRINT
            cout << "DEBUG - adding hashmap[" << pruned << "] = {" << info.length << ", " << info.name
                    << ", " << info.id << ", " << info.leaf << "}" << endl;
            cout << "DEBUG end" << endl;
#endif
        }
        else {
            info = hashmap[pruned];
            info.length += edge.getLength();
#ifdef DEBUGPRINT
            cout << "DEBUG - altering hashmap[" << pruned << "] = {" << info.length << ", " << info.name
                    << ", " << info.id << ", " << info.leaf << "}" << endl;
            cout << "DEBUG end" << endl;
#endif
        }
    }
    // Add remaining leaves
    size_t size = this->leaf2NumMap.size();
    for (size_t i=0; i < size; ++i) {
        bitset_t leaf_split(size);
        leaf_split[size - i - 1] = true;
        size_t index = Tools::leaf_index_nothrow(leaf_split);
        if (hashmap.find(leaf_split)==hashmap.end() && hashmap.find(~leaf_split)==hashmap.end()) {
            EdgeInfo info;
            info.name = this->leaf2NumMap[index];
            info.length = pruned_lengths[index];
            info.leaf = true;
            hashmap[leaf_split] = info;
        }
    }

    // All info to build this PhyloTree is in the hashmap
    for (auto& item : hashmap) {
#ifdef DEBUGPRINT
        cout << "DEBUG - hashmap key: " << item.first << ", value: {" << item.second.length << ", " << item.second.name
                << ", " << item.second.id << ", " << item.second.leaf << "}" << endl;
#endif
        if (item.second.leaf) {
            this->leafEdgeLengths[name_indices[item.second.name]] = item.second.length;
        }
        else {
            if (!item.first.all() && !item.first.none()) {
                this->edges.emplace_back(item.first, item.second.length, item.second.id);
            }
#ifdef DEBUGPRINT
            cout << "DEBUG - PhyloTreeEdge check" << endl;
            PhyloTreeEdge check(item.first, item.second.length, item.second.id);
            cout << check.toString() << " " << check.getLength() << endl;
            cout << "DEBUG end" << endl;
#endif
        }
    }
}

std::pair<std::vector<int>, std::vector<int>> PhyloTree::leaf_difference(const PhyloTree& other) {
    std::pair<std::vector<int>, std::vector<int>> out;
    if (this->leaf2NumMap == other.leaf2NumMap) {
        return std::move(out);
    }

    auto beg1 = this->leaf2NumMap.begin();
    auto beg2 = other.leaf2NumMap.begin();
    auto end1 = this->leaf2NumMap.end();
    auto end2 = other.leaf2NumMap.end();
    auto o1 = std::back_inserter(out.first);
    auto o2 = std::back_inserter(out.second);
    auto front1 = beg1;
    auto front2 = beg2;

    while (beg1 != end1 && beg2 != end2) {
        if ((*beg1) < (*beg2)) {
            (*o1++) = std::distance(front1, beg1++);
        }
        else if ((*beg1) == (*beg2)) {
            beg1++;
            beg2++;
        }
        else if ((*beg2) < (*beg1)) {
            (*o2++) = std::distance(front2, beg2++);
        }
    }

    while (beg1 != end1) {
        (*o1++) = std::distance(front1, beg1++);
    }

    while (beg2 != end2) {
        (*o2++) = std::distance(front2, beg2++);
    }
    return std::move(out);
}