//
// Created by Kevin Gori on 16/05/2016.
    //
#ifndef BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#define BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#endif
#include "Distance.h"
#include "Tools.h"
#include "bitset_hash.h"
#include <unordered_map>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using bitset_t = boost::dynamic_bitset<>;

template <typename InputIt, typename OutputIt>
void leaf_intersection(InputIt beg1, InputIt end1, InputIt beg2, InputIt end2, OutputIt o1, OutputIt o2) {

    InputIt front1 = beg1;
    auto front2 = beg2;

    while(beg1 != end1 && beg2 != end2) {
        if ((*beg1) < (*beg2)) {
            ++beg1;
        }
        else if ((*beg1) == (*beg2)) {
            (*o1++) = std::distance(front1, beg1++);
            (*o2++) = std::distance(front2, beg2++);
        }
        else if ((*beg2) < (*beg1)) {
            ++beg2;
        }
    }
}

template <typename InputIt, typename OutputIt>
void leaf_difference(InputIt beg1, InputIt end1, InputIt beg2, InputIt end2, OutputIt o1, OutputIt o2) {

    InputIt front1 = beg1;
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

    while (beg2 != end2) {
        (*o2++) = std::distance(front2, beg2++);
    }
}


/*
 * Fills an OutputIterator to a container with items drawn from an Iterator to an input container,
 * at indices specified by an IndexIterator to an index container
 */
template <typename Iterator, typename IndexIterator, typename OutputIterator>
void slice_container(Iterator begin, Iterator end, IndexIterator index_begin, IndexIterator index_end,
        OutputIterator inserter) {
    auto front = begin;
    for (; begin!=end && index_begin != index_end; begin++) {
        if (std::distance(front, begin)==(*index_begin)) {
            (*inserter++) = (*begin);
            index_begin++;
        }
    }
}


/*
 * Remove taxa at indices in `missing` from PhyloTree `tree`,
 * returning a new pruned tree
 */
template <typename Container>
PhyloTree prune_tree(const PhyloTree& tree, const Container& missing) {
    auto edges = tree.getEdgesByRef();
    auto leaf_lengths = tree.getLeafEdgeLengthsByRef();
    auto leaf_names = tree.getLeaf2NumMap();

    vector<string> pruned_names;
    vector<double> pruned_lengths;

    // TODO: combine these into a single loop
    Tools::prune_container(leaf_names.begin(), leaf_names.end(),
                    missing.begin(), missing.end(), std::back_inserter(pruned_names));
    Tools::prune_container(leaf_lengths.begin(), leaf_lengths.end(),
                    missing.begin(), missing.end(), std::back_inserter(pruned_lengths));

    cout << "DEBUG - pruned_names" << endl;
    Tools::vector_print(pruned_names.begin(), pruned_names.end());

    cout << "DEBUG - pruned_lengths" << endl;
    Tools::vector_print(pruned_lengths.begin(), pruned_lengths.end());


    std::unordered_map<bitset_t, EdgeInfo, BitsetHash> hashmap;

    int inner_edge_counter = 0;
    for (auto &edge: edges) {
        auto pruned = Tools::prune_bitset(edge.getPartition(), missing);
        if (hashmap.find(pruned) == hashmap.end()) {
            EdgeInfo info;
            info.length = edge.getLength();
            if(Tools::is_leaf(pruned)) {
                size_t ix = Tools::leaf_index_nothrow(pruned);
                info.name = pruned_names[ix];
                info.length += pruned_lengths[ix];
                info.leaf = true;
            }
            else {
                info.id = inner_edge_counter++;
            }
            hashmap[pruned] = info;
        }
        else {
            hashmap[pruned].length += edge.getLength();
        }
    }

    // The new PhyloTree needs a vector of edges, a vector of leaf names and a vector of leaf edge lengths
    vector<EdgeInfo> leaf_edges;
    vector<PhyloTreeEdge> new_edges;
    vector<double> new_leaf_lengths;
    for (auto &item : hashmap) {
        if (item.second.leaf) {
            leaf_edges.push_back(item.second);
        }
        else {
            new_edges.emplace_back(item.first, item.second.length, item.second.id);
        }
    }
    std::sort(leaf_edges.begin(), leaf_edges.end(), [](const EdgeInfo& edge1, const EdgeInfo& edge2) { return edge1.name < edge2.name; });
    new_leaf_lengths.reserve(leaf_edges.size());

    new_leaf_lengths.reserve(leaf_edges.size());
    for (auto &item : leaf_edges) {
        new_leaf_lengths.push_back(item.length);
    }

    return PhyloTree(new_edges, pruned_names, new_leaf_lengths);
}


int main(int argc, char const *argv[]) {
//    string bignewick1("((a:1,b:2):3,(c:4,d:5):6,(e:7,f:8):9);");
//    string bignewick2("((a:1,(b:2,c:3):4):5,(d:6,e:7):8,f:9);");
//    string smallnewick1("((a:1,b:2):3,c:10,f:17);");
//    string smallnewick2("(a:1,(b:2,c:3):4,f:14);");
    string t1_rooted_newick("((((a:0.0589988989102,(b:0.0418326893396,c:0.859140507521):2.61261891006):0.990549866809,(d:4.14397236141,f:0.17088326593):0.0323075746352):2.02262974527,g:0.693675923359):0.428536225039,(i:1.69919541269,j:0.362508537093):0.195899634738):0.0;");
    string t2_rooted_newick("((a:2.03481158672,((b:0.374332029372,c:3.8377563921):0.258935532423,((e:2.18939751337,f:2.17982498905):0.623171789534,g:0.12443873179):0.845298009923):0.0780727161207):0.271887473804,(h:0.125928604756,j:0.64774830898):0.240154552836):0.0;");
    string t1_unrooted_newick("((((a:0.0589988989102,(b:0.0418326893396,c:0.859140507521):2.61261891006):0.990549866809,(d:4.14397236141,f:0.17088326593):0.0323075746352):2.02262974527,g:0.693675923359):0.624435859777,i:1.69919541269,j:0.362508537093):0.0;");
    string t2_unrooted_newick("((a:2.03481158672,((b:0.374332029372,c:3.8377563921):0.258935532423,((e:2.18939751337,f:2.17982498905):0.623171789534,g:0.12443873179):0.845298009923):0.0780727161207):0.51204202664,h:0.125928604756,j:0.64774830898):0.0;");
//    PhyloTree bigtree1(bignewick1, false);
//    PhyloTree bigtree2(bignewick2, false);
//    PhyloTree smalltree1(smallnewick1, false);
//    PhyloTree smalltree2(smallnewick2, false);
//    PhyloTree prunedtree1(bigtree1, std::vector<int>{3,4});
//    PhyloTree prunedtree2(bigtree2, std::vector<int>{3,4});
    PhyloTree t1_rooted(t1_rooted_newick, true);
    PhyloTree t1_unrooted(t1_unrooted_newick, false);
    PhyloTree t2_rooted(t2_rooted_newick, true);
    PhyloTree t2_unrooted(t2_unrooted_newick, false);
    auto p_rooted = t1_rooted.leaf_difference(t2_rooted);
    PhyloTree t1_rooted_pruned(t1_rooted, p_rooted.first);
    PhyloTree t2_rooted_pruned(t2_rooted, p_rooted.first);
    auto p_unrooted = t1_unrooted.leaf_difference(t2_unrooted);
    PhyloTree t1_unrooted_pruned(t1_unrooted, p_unrooted.first);
    PhyloTree t2_unrooted_pruned(t2_unrooted, p_unrooted.first);
    cout << "T1 rooted: " << endl << t1_rooted.toString() << endl;
    cout << "T1 unrooted: " << endl << t1_unrooted.toString() << endl;
    cout << "T2 rooted: " << endl << t2_rooted.toString() << endl;
    cout << "T2 unrooted: " << endl << t2_unrooted.toString() << endl;
    cout << "T1 rooted,pruned: " << endl << t1_rooted_pruned.toString() << endl;
    cout << "T1 unrooted,pruned: " << endl << t1_unrooted_pruned.toString() << endl;
    cout << "T2 rooted,pruned: " << endl << t2_rooted_pruned.toString() << endl;
    cout << "T2 unrooted,pruned: " << endl << t2_unrooted_pruned.toString() << endl;

    PhyloTree t1_r_as_u(t1_rooted_newick, false);
    PhyloTree t1_u_as_r(t1_unrooted_newick, true);

    cout << "T1 rooted as unrooted: " << endl << t1_r_as_u.toString() << endl;
    cout << "T1 unrooted as rooted: " << endl << t1_u_as_r.toString() << endl;
    cout << "T1 rooted: " << endl << t1_rooted.toString() << endl;
    cout << "T1 unrooted: " << endl << t1_unrooted.toString() << endl;
    cout << "Distance: " << Distance::getGeodesicDistance(t1_r_as_u, t1_rooted, false) << endl;
    cout << "Distance: " << Distance::getGeodesicDistance(t1_r_as_u, t1_unrooted, false) << endl;
    cout << "Distance: " << Distance::getGeodesicDistance(t1_u_as_r, t1_rooted, false) << endl;
    cout << "Distance: " << Distance::getGeodesicDistance(t1_u_as_r, t1_unrooted, false) << endl;
//
//    cout << Distance::getGeodesicDistance(smalltree1, smalltree2, false) << endl;
//    cout << Distance::getGeodesicDistance(prunedtree1, prunedtree2, false) << endl;
//    cout << Distance::getGeodesicDistance(smalltree1, prunedtree2, false) << endl;
//    cout << Distance::getGeodesicDistance(prunedtree1, smalltree2, false) << endl;
//    cout << Distance::getGeodesicDistance(smalltree1, smalltree1, false) << endl;
//    cout << Distance::getGeodesicDistance(prunedtree1, prunedtree1, false) << endl;
//    cout << Distance::getGeodesicDistance(smalltree1, prunedtree1, false) << endl;
//    cout << Distance::getGeodesicDistance(prunedtree1, smalltree1, false) << endl;
//    cout << Distance::getGeodesicDistance(smalltree2, smalltree2, false) << endl;
//    cout << Distance::getGeodesicDistance(prunedtree2, prunedtree2, false) << endl;
//    cout << Distance::getGeodesicDistance(smalltree2, prunedtree2, false) << endl;
//    cout << Distance::getGeodesicDistance(prunedtree2, smalltree2, false) << endl;

//    cout << "TESTING" << endl;
//    try {
//        auto rooted_dist = Distance::getGeodesicDistance(t1_rooted, t2_rooted, false);
//        cout << "rooted distance: " << rooted_dist << endl;
//    } catch (std::invalid_argument e) {
//        cout << e.what() << endl;
//    }
//
//    try {
//        cout << "unrooted distance: " << Distance::getGeodesicDistance(t1_unrooted, t2_unrooted, false) << endl;
//    } catch (std::invalid_argument e) {
//        cout << e.what() << endl;
//    }
//
    cout << "rooted, pruned distance: " << Distance::getGeodesicDistance(t1_rooted_pruned, t2_rooted_pruned, false) << endl;
    cout << "unrooted, pruned distance: " << Distance::getGeodesicDistance(t1_unrooted_pruned, t2_unrooted_pruned, false) << endl;
    cout << "rooted, pruned distance: " << Distance::getRobinsonFouldsDistance(t1_rooted_pruned, t2_rooted_pruned, false) << endl;
    cout << "unrooted, pruned distance: " << Distance::getRobinsonFouldsDistance(t1_unrooted_pruned, t2_unrooted_pruned, false) << endl;


    string n1("((a:3,b:4):.1,(c:5,((d:6,e:7):.2,f:8):.3):.4);");
    string n2("((a:3,c:4):.5,(d:5,((b:6,e:7):.2,f:8):.3):.4);");
    PhyloTree t1(n1, true);
    PhyloTree t2(n2, true);
    cout << "t1: " << t1.toString() << endl;
    cout << "t2: " << t2.toString() << endl;
    cout << Distance::getRobinsonFouldsDistance(t1,t2,false);
//    auto res = bigtree1.leaf_difference(smalltree2);
//    Tools::vector_print(res.first.begin(), res.first.end());
//    Tools::vector_print(res.second.begin(), res.second.end());
//    PhyloTreeEdge e(s1);
//    std::cout << e.toString() << std::endl;
//    PhyloTree t(tree, false);
//    for (auto &edge : t.getEdges()) {
//        std::cout << edge.toString() << std::endl;
//        std::cout << edge.getLength() << std::endl;
//        std::cout << "ID: " << edge.getOriginalID() <<std::endl;
//    }
//
//    for (auto len : t.getLeafEdgeLengths()) {
//        std::cout << len << std::endl;
//    }
//
//    PhyloTree bt(bigtree, false);
//    for (auto &edge : bt.getEdges()) {
//        std::cout << edge.toString() << std::endl;
//        std::cout << edge.getLength() << std::endl;
//        std::cout << "ID: " << edge.getOriginalID() <<std::endl;
//    }
//
//    for (auto len : bt.getLeafEdgeLengths()) {
//        std::cout << len << std::endl;
//    }
//
//    vector<PhyloTreeEdge> ptev;
//    int counter = 0;
//    for (size_t i=0; i < sv.size(); ++i) {
//        PhyloTreeEdge pte(sv[i]);
//        pte.setAttribute(lengths[i]);
//        pte.setOriginalID(counter++);
//        ptev.push_back(pte);
//    }
//    vector<string> leafnames{"a", "b", "c", "f"};
//    PhyloTree pt(ptev, leafnames, lengths);
//
//    for (auto &edge : pt.getEdges()) {
//        std::cout << edge.toString() << std::endl;
//        std::cout << edge.getLength() << std::endl;
//        std::cout << "ID: " << edge.getOriginalID() <<std::endl;
//    }
//
//    std::cout << "Leaf Edge Lengths" << std::endl;
//    for (auto len : pt.getLeafEdgeLengths()) {
//        std::cout << len << std::endl;
//    }

    return 0;
}

