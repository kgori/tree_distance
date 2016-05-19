#ifndef BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#define BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#endif
#include "BipartiteGraph.h"
#include "Geodesic.h"

#include <cmath>
#include <sstream>

void deleteEmptyEdges(vector<PhyloTreeEdge> &v) {
    v.erase(std::remove_if(v.begin(), v.end(), [](PhyloTreeEdge &element) {
        return element.isEmpty();
    }), v.end());
}

Geodesic::Geodesic(RatioSequence rs) {
    this->rs = rs;
}

Geodesic::Geodesic(const RatioSequence& rs, const vector<PhyloTreeEdge>& cEdges) {
    this->rs = rs;
    commonEdges = cEdges;
}

Geodesic::Geodesic(const RatioSequence& rs, const vector<PhyloTreeEdge>& cEdges, double leafContributionSquared) {
    this->rs = rs;
    commonEdges = cEdges;
    this->leafContributionSquared = leafContributionSquared;
}

Geodesic::Geodesic(const Geodesic &other) : rs(other.rs), commonEdges(other.commonEdges),
                                            leafContributionSquared(other.leafContributionSquared) {
};

//PhyloTree Geodesic::getTreeAt(PhyloTree t1, PhyloTree t2, double position) {
//    /*
//    In the java code the higher and lower ratio indices were used as loop counters,
//    and as booleans (if index == -2: do / don't do something). This doesn't work so well
//    in C++, where loop counters are usually size_t signed integers, so necessarily > 0).
//    As a work around, to avoid having to rewrite a lot of the java logic, I've added some
//    boolean flags to take over the boolean work of the ratio indices.
//
//    // LOWER RATIO INDEX: the index of the ratio containing all f edges in the tree we want
//    // i.e. the index of the ratio with time < position, but such that the next ratio has time >= position
//    // if position is in the starting orthant, we don't want any f edges.
//
//    // HIGHER RATIO INDEX: the index of the ratio containing all e edges in the tree we want
//    // i.e. the index of the ratio with time >= position, but such that the next ratio has time >= position
//    // if position is in the target orthant, we don't want any e edges
//    */
//
//    bool lowerRatioIndexMINUS2 = true;
//    bool lowerRatioIndexMINUS1 = false;
//    size_t lowerRatioIndex = 0;
//    bool higherRatioIndexMINUS2 = true;
//    size_t higherRatioIndex = 0;
//    vector<PhyloTreeEdge> eEdges;
//    vector<PhyloTreeEdge> fEdges;
//
//    // set the commonEdges
//    vector<PhyloTreeEdge> commonEdges = getCommonEdges(t1, t2, position);
//
//    for (int i = 0; i < commonEdges.size(); i++) {
//        t1.removeSplit(commonEdges[i]);
//        t2.removeSplit(commonEdges[i]);
//    }
//
//    //	System.out.println("t1NoCommonEdges is " + t1NoCommonEdges);
//    //	System.out.println("t2NoCommonEdges is " + t2NoCommonEdges);
//
//    Geodesic geo = getGeodesic(t1, t2);
//
//    auto l2nm = t1.getLeaf2NumMap();
//    PhyloTree tree(commonEdges, l2nm);
//
//    // set the leaf lengths
//    vector<EdgeAttribute> newLeafEdgeAttribs;
//    size_t len = t1.getLeafEdgeAttribs().size();
//    newLeafEdgeAttribs.resize(len);
//    for (int i = 0; i < len; ++i) {
//        newLeafEdgeAttribs[i] = EdgeAttribute::weightedPairAverage(t1.getLeafEdgeAttribs()[i], t2.getLeafEdgeAttribs()[i], position);
//    }
//    tree.setLeafEdgeLengths(newLeafEdgeAttribs);
//
//    if (geo.getRS().size() == 0) {
//        // then we are done, because the two trees are in the same orthant
//        return tree;
//    }
//    // figure out what orthant the new tree is in
//    // first check if the new tree is in the starting orthant
//    if (geo.getRS().getRatio(0).getTime() > position) {
//        // new tree is in the interior of the starting orthant
//        lowerRatioIndexMINUS1 = true;
//        lowerRatioIndexMINUS2 = false;
//
//        higherRatioIndexMINUS2 = false;
//        higherRatioIndex = 0;
//
//    }
//    // if the new tree is in the last orthant
//    else if (geo.getRS().getRatio(geo.getRS().size() - 1).getTime() < position) {
//        if (geo.getRS().size() == 0) {
//            lowerRatioIndexMINUS1 = true;
//            lowerRatioIndexMINUS2 = false;
//        }
//        else {
//            lowerRatioIndex = geo.getRS().size() - 1;
//            lowerRatioIndexMINUS1 = false;
//            lowerRatioIndexMINUS2 = false;
//        }
//        higherRatioIndex = geo.getRS().size();
//        higherRatioIndexMINUS2 = false;
//    }
//    // the new tree is in an intermediate orthant
//    else {
//        for (size_t i = 0; i < geo.getRS().size(); i++) {
//            // note:  want < instead of <= so we are in an orthant and not still on the boundary,
//            // if we have a string of equalities
//            double ratioTime = geo.getRS().getRatio(i).getTime();
//
//            if (ratioTime >= position) {
//                if (i == 0) {
//                    lowerRatioIndexMINUS1 = true;
//                    lowerRatioIndexMINUS2 = false;
//                }
//                else {
//                    lowerRatioIndex = i - 1;
//                    lowerRatioIndexMINUS1 = false;
//                    lowerRatioIndexMINUS2 = false;
//                }
//            }
//            if (!lowerRatioIndexMINUS2 && (ratioTime > position)) {
//                higherRatioIndex = i;
//                higherRatioIndexMINUS2 = false;
//            }
//        }
//    }
//    // if we didn't set the higherRatioIndex, then we are on the boundary with the target orthant.
//    // we want all no e edges, so set higherRatioIndex to
//    if (higherRatioIndexMINUS2) {
//        higherRatioIndex = geo.getRS().size();
//    }
//
//    // add the edges for all f edges in ratios indexed <= lowerRatioIndex
//    if (!lowerRatioIndexMINUS2 && !lowerRatioIndexMINUS1) {
//        for (size_t i = 0; i <= lowerRatioIndex; i++) {
//            auto curr_ratio = geo.getRS().getRatio(i);
//            fEdges = curr_ratio.getFEdges();
//
//            for (PhyloTreeEdge &f : fEdges) {
//                EdgeAttribute newAttrib = f.getAttribute();
//                newAttrib.scaleBy((position * curr_ratio.getFLength() - (1 - position) * curr_ratio.getELength()) / curr_ratio.getFLength());
//                tree.addEdge(PhyloTreeEdge(f.asSplit(), newAttrib, f.getOriginalID()));
//            }
//        }
//    }
//
//    // to the new tree, add the e edges in the ratios indexed >= higherRatioIndex
//    for (size_t i = higherRatioIndex; i < geo.getRS().size(); i++) {
//        auto curr_ratio = geo.getRS().getRatio(i);
//        eEdges = curr_ratio.getEEdges();
//
//        for (PhyloTreeEdge &e : eEdges) {
//            EdgeAttribute newAttrib = e.getAttribute();
//            newAttrib.scaleBy(((1 - position) * curr_ratio.getELength() - position * curr_ratio.getFLength()) / curr_ratio.getELength());
//            tree.addEdge(PhyloTreeEdge(e.asSplit(), newAttrib, e.getOriginalID()));
//        }
//    }
//    return tree;
//}

RatioSequence &Geodesic::getRS() {
    return this->rs;
}

void Geodesic::setRS(const RatioSequence& rs) {
    this->rs = rs;
}

double Geodesic::getDist() {
    double commonEdgeDistSquared = 0;
    for (size_t i = 0; i < commonEdges.size(); ++i) {
        commonEdgeDistSquared += pow(commonEdges[i].getLength(), 2);
    }
    return sqrt(pow(rs.getNonDesRSWithMinDist().getDistance(), 2) + commonEdgeDistSquared + leafContributionSquared);
}

//void Geodesic::addCommonEdge(PhyloTreeEdge e) {
//    commonEdges.push_back(e);
//}

Geodesic Geodesic::clone() {
    return Geodesic(*this);
}

string Geodesic::toString() {
    ostringstream ss;
    ss << getDist() << "; " << rs.getNonDesRSWithMinDist().toString();
    return ss.str();
}

//vector<PhyloTreeEdge> Geodesic::getCommonEdges(PhyloTree t1, PhyloTree t2, double position) {
//    EdgeAttribute commonEdgeAttribute;
//    Bipartition commonSplit;
//
//    vector<PhyloTreeEdge> commonEdges;
//
//    // if the two trees do not have the same leaf2NumMap
//    if (t1.getLeaf2NumMap() != t2.getLeaf2NumMap()) {
//        throw invalid_argument("Trees have mismatched leaves");
//    }
//
//    if (position < 0 || position > 1) {
//        ostringstream err_msg;
//        err_msg << "Error:  position " << position << " must be between 0 and 1";
//        throw invalid_argument(err_msg.str());
//    }
//    // end error checking
//
//    for (PhyloTreeEdge &e : t1.getEdges()) {
//        auto t2_splits = t2.getSplits();
//        bool split_in_t2 = std::find(t2_splits.begin(), t2_splits.end(), e.asSplit()) != t2_splits.end();
//        if (split_in_t2) {
//            // then we have found the same split in each tree
//            commonSplit = Bipartition(e.asSplit());
//            commonEdgeAttribute = EdgeAttribute::weightedPairAverage(e.getAttribute(), t2.getAttribOfSplit(commonSplit), position);
//            commonEdges.push_back(PhyloTreeEdge(Bipartition(commonSplit), commonEdgeAttribute.clone(), -1));
//        }
//            // otherwise check if the split is compatible with all splits in t2
//        else if (e.isCompatibleWith(t2.getSplits())) {
//            commonEdgeAttribute = EdgeAttribute::weightedPairAverage(e.getAttribute(), EdgeAttribute::zeroAttribute(), position);
//            commonEdges.push_back(PhyloTreeEdge(e.asSplit(), commonEdgeAttribute.clone(), -1));
//        }
//    }
//    // check for splits in t2 that are compatible with all splits in t1
//    for (PhyloTreeEdge &e : t2.getEdges()) {
//        auto t1_splits = t1.getSplits();
//        bool split_not_in_t1 = std::find(t1_splits.begin(), t1_splits.end(), e.asSplit()) == t1_splits.end();
//        if (e.isCompatibleWith(t1.getSplits()) && split_not_in_t1) {
//            commonEdgeAttribute = EdgeAttribute::weightedPairAverage(EdgeAttribute::zeroAttribute(), e.getAttribute(), position);
//            commonEdges.push_back(PhyloTreeEdge(e.asSplit(), commonEdgeAttribute.clone(), -1));
//        }
//    }
//    return commonEdges;
//}

void Geodesic::setCommonEdges(vector<PhyloTreeEdge> commonEdges) {
    this->commonEdges = commonEdges;
}

//size_t Geodesic::numCommonEdges() {
//    return commonEdges.size();
//}

//size_t Geodesic::numTopologies() {
//    return rs.getAscRSWithMinDist().size() + 1;
//}

//Geodesic Geodesic::reverse() {
//    return Geodesic(rs.reverse(), commonEdges, leafContributionSquared);
//}

//double Geodesic::getLeafContributionSquared() {
//    return leafContributionSquared;
//}

void Geodesic::setLeafContributionSquared(double leafContributionSquared) {
    this->leafContributionSquared = leafContributionSquared;
}

Geodesic Geodesic::getGeodesic(PhyloTree &t1, PhyloTree &t2) {
    double leafContributionSquared = 0;
    vector<double> t1_leaf_lengths = t1.getLeafEdgeLengths();
    vector<double> t2_leaf_lengths = t2.getLeafEdgeLengths();
    Geodesic geo = Geodesic(RatioSequence());

    // get the leaf contributions
    auto ref_leaf_num_map = t1.getLeaf2NumMap();
    auto chk_leaf_num_map = t2.getLeaf2NumMap();
    for (size_t i = 0; i < ref_leaf_num_map.size(); i++) {
        if (ref_leaf_num_map[i] != chk_leaf_num_map[i]) {
            throw invalid_argument("Error getting geodesic: trees do not have the same sets of leaves");
        }
        leafContributionSquared += pow(t1_leaf_lengths[i] - t2_leaf_lengths[i], 2);
    }
    geo.setLeafContributionSquared(leafContributionSquared);

    vector<PhyloTree> aTreesNoCommonEdges;
    vector<PhyloTree> bTreesNoCommonEdges;

    // get the pairs of trees with no common edges put into aTreesNoCommonEdges and bTreesNoCommonEdges
    //  aTreesNoCommonEdges[i] goes with bTreesNoCommonEdges[i]
    auto t1_edges = t1.getEdges();
    auto t2_edges = t2.getEdges();
    auto l2nm = t1.getLeaf2NumMap();
    splitOnCommonEdge(t1_edges, t2_edges, l2nm, aTreesNoCommonEdges, bTreesNoCommonEdges);
//    splitOnCommonEdge(t1, t2, aTreesNoCommonEdges, bTreesNoCommonEdges);
    //set the common edges
    vector<PhyloTreeEdge> eic;
    PhyloTree::getCommonEdges(t1_edges, t2_edges, eic);
    geo.setCommonEdges(eic);

    // find the geodesic between each pair of subtrees found by removing the common edges
    for (size_t i = 0; i < aTreesNoCommonEdges.size(); i++) {
        Geodesic newGeo = getGeodesicNoCommonEdges(aTreesNoCommonEdges[i], bTreesNoCommonEdges[i]);
        geo.setRS(RatioSequence::interleave(geo.getRS(), newGeo.getRS()));
    }
    return geo;
}

Geodesic Geodesic::getGeodesicNoCommonEdges(PhyloTree &t1, PhyloTree &t2) {
    auto t1_edges = t1.getEdges();
    auto t2_edges = t2.getEdges();
    size_t numEdges1 = t1_edges.size(); // number of edges in tree 1
    size_t numEdges2 = t2_edges.size(); // number of edges in tree 2

    if (numEdges1 == 0 && numEdges2 == 0)
        return Geodesic(RatioSequence());

    // double-check that both trees have splits.  Otherwise didn't remove a common edge.
    if (numEdges1 == 0 || numEdges2 == 0) {
        throw ("Error: tried to compute geodesic between subtrees that should not have common/compatible edges, but do!  t1 = " + t1.getNewick(true) + " and t2 = " + t2.getNewick(true));
    }

    std::sort(t1_edges.begin(), t1_edges.end());
    std::sort(t2_edges.begin(), t2_edges.end());

    // double-check no common edges
//    vector<PhyloTreeEdge> commonEdges;
//    PhyloTree::getCommonEdges(t1_edges, t2_edges, commonEdges);
//    if (commonEdges.size() != 0) {
//        throw invalid_argument("Error: tried to compute geodesic between subtrees that should not have common edges, but do!  t1 = " + t1.getNewick(true) + " and t2 = " + t2.getNewick(true));
//    }

    auto rs = RatioSequence();
    // if we can't split the ratio because it has too few edges in either the numerator or denominator
    if ((numEdges1 == 1) || (numEdges2 == 1)) {
        rs.push_back_value(Ratio(t1_edges, t2_edges));
        return Geodesic(rs);
    }

    vector<size_t> aVertices, bVertices;
    deque<Ratio> queue;
    Ratio ratio;

    // initialize BipartiteGraph
    auto incidenceMatrix = BipartiteGraph::getIncidenceMatrix(t1_edges, t2_edges);
    BipartiteGraph bg(incidenceMatrix, t1.getIntEdgeAttribNorms(), t2.getIntEdgeAttribNorms());
    queue.emplace_back(t1_edges, t2_edges);
    aVertices.reserve(numEdges1);
    bVertices.reserve(numEdges2);

    while (queue.size() > 0) {
        ratio = queue.front();
        queue.pop_front();
        aVertices.clear();
        bVertices.clear();

        // convert the ratio to what we pass to vertex cover
        auto ratio_e_edges = ratio.getEEdges();
        auto ratio_f_edges = ratio.getFEdges();
        for (int i = 0; i < ratio_e_edges.size(); i++) {
            auto index_iter = std::lower_bound(t1_edges.begin(), t1_edges.end(), ratio_e_edges[i]);
            aVertices.push_back(std::distance(t1_edges.begin(), index_iter));
        }

        for (int i = 0; i < ratio.getFEdges().size(); i++) {
            auto index_iter = std::lower_bound(t2_edges.begin(), t2_edges.end(), ratio_f_edges[i]);
            bVertices.push_back(std::distance(t2_edges.begin(), index_iter));
        }

        // get the cover
        auto cover = bg.vertex_cover(aVertices, bVertices);
        // check if cover is trivial
        if ((cover[0][0] == 0) || (cover[0][0] == aVertices.size())) {
            // add ratio to geodesic
            rs.push_back(ratio);


        } else {  // cover not trivial
            // make two new ratios
            auto r1 = Ratio();
            auto r2 = Ratio();

            int j = 0;  // for index in cover array

            // split the ratio based on the cover
            for (size_t i = 0; i < aVertices.size(); i++) {
                if ((j < cover[2].size()) && (aVertices[i] == cover[2][j])) {
                    r1.addEEdge(t1_edges[aVertices[i]]);
                    j++;
                } else { // the split is not in the cover, and hence dropped first
                    r2.addEEdge(t1_edges[aVertices[i]]);
                }
            }

            j = 0;   // reset index
            // split the ratio based on the cover
            for (size_t i = 0; i < bVertices.size(); i++) {
                if ((j < cover[3].size()) && (bVertices[i] == cover[3][j])) {
                    r2.addFEdge(t2_edges[bVertices[i]]);
                    j++;
                } else { // the split is not in the cover, and hence dropped first
                    r1.addFEdge(t2_edges[bVertices[i]]);
                }
            }

            // add ratios to the queue
            queue.push_front(r2);
            queue.push_front(r1);
        }
    }
    return Geodesic(rs);
}

void Geodesic::splitOnCommonEdge(vector<PhyloTreeEdge> &t1_edges, vector<PhyloTreeEdge> &t2_edges,
        vector<string> &reference_leaf_num_map, vector<PhyloTree> &destination_a, vector<PhyloTree> &destination_b) {
    size_t numEdges1 = t1_edges.size(); // number of edges in tree 1
    size_t numEdges2 = t2_edges.size(); /// number of edges in tree 2
    if (numEdges1 == 0 || numEdges2 == 0) {
        return;
    }
    string leaf_label;
    // look for common edges
    PhyloTreeEdge commonEdge;
    try {
        commonEdge = PhyloTree::getFirstCommonEdge(t1_edges, t2_edges);
    }
    catch (edge_not_found_exception &err) {
        destination_a.reserve(destination_a.size() + numEdges1);
        destination_b.reserve(destination_b.size() + numEdges2);
        destination_a.emplace_back(t1_edges, reference_leaf_num_map);
        destination_b.emplace_back(t2_edges, reference_leaf_num_map);
        return;
    }

    // A will be the tree with leaves corresponding to 1's in commonEdge
    vector<string> leaf2NumMapA;
    vector<string> leaf2NumMapB;

    vector<PhyloTreeEdge> edgesA1;
    vector<PhyloTreeEdge> edgesA2;
    vector<PhyloTreeEdge> edgesB1;
    vector<PhyloTreeEdge> edgesB2;

    edgesA1.reserve(numEdges1);
    edgesA2.reserve(numEdges2);
    edgesB1.reserve(numEdges1);
    edgesB2.reserve(numEdges2);

    for (PhyloTreeEdge e : t1_edges) { // have to copy e
        e.clear();
        edgesA1.push_back(e);
        edgesB1.push_back(e);
    }

    for (PhyloTreeEdge e : t2_edges) {
        e.clear();
        edgesA2.push_back(e);
        edgesB2.push_back(e);
    }

    bool aLeavesAdded = false;  // if we have added a leaf in B representing the A tree
    size_t indexAleaves = 0;  // the index we are at in  the vectors holding the leaves in the A and B subtrees
    size_t indexBleaves = 0;

    // step through the leaves represented in commonEdge
    // (there should be two more leaves than edges)
    for (size_t i = 0; i < reference_leaf_num_map.size(); i++) {
        leaf_label = reference_leaf_num_map[i];
        if (commonEdge.contains(i)) {
            // commonEdge contains leaf i
            leaf2NumMapA.push_back(leaf_label);

            // these leaves must be added as a group to the B trees
            if (!aLeavesAdded) {
                leaf2NumMapB.push_back(leaf_label + "*");    // add a one of the leaves of the A tree to represent all the A trees leaves
                for (size_t j = 0; j < numEdges1; j++) {
                    if (t1_edges[j].properlyContains(commonEdge)) {
                        edgesB1[j].addOne(indexBleaves);
                    }
                }
                for (size_t j = 0; j < numEdges2; j++) {
                    if (t2_edges[j].properlyContains(commonEdge)) {
                        edgesB2[j].addOne(indexBleaves);
                    }
                }
                indexBleaves++;
                aLeavesAdded = true;
            }
            // add the column corresponding to this leaf to the A edges vector (for the corresponding trees)
            // XXX: problem: might be adding edges which contain leaves in A but also
            for (size_t j = 0; j < numEdges1; j++) {
                if (commonEdge.properlyContains(t1_edges[j]) && t1_edges[j].contains(i)) {
                    edgesA1[j].addOne(indexAleaves);
                }
            }
            for (size_t j = 0; j < numEdges2; j++) {
                if (commonEdge.properlyContains(t2_edges[j]) && t2_edges[j].contains(i)) {
                    edgesA2[j].addOne(indexAleaves);
                }
            }
            indexAleaves++;
        } else {
            // commonEdge does not contain leaf i
            leaf2NumMapB.push_back(leaf_label);
            for (int j = 0; j < numEdges1; j++) {
                if (t1_edges[j].contains(i)) {
                    edgesB1[j].addOne(indexBleaves);
                }
            }
            for (int j = 0; j < numEdges2; j++) {
                if (t2_edges[j].contains(i)) {
                    edgesB2[j].addOne(indexBleaves);
                }
            }
            indexBleaves++;
        }
    }

    deleteEmptyEdges(edgesA1);
    deleteEmptyEdges(edgesA2);
    splitOnCommonEdge(edgesA1, edgesA2, leaf2NumMapA, destination_a, destination_b);

    deleteEmptyEdges(edgesB1);
    deleteEmptyEdges(edgesB2);
    splitOnCommonEdge(edgesB1, edgesB2, leaf2NumMapB, destination_a, destination_b);
}

//vector<PhyloTreeEdge> Geodesic::getCommonEdges() {
//    return commonEdges;
//}
