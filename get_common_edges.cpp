#include "Distance.h"
#include <algorithm>
#include <cmath>
#include <unordered_map>
using namespace std;

int main(int argc, char const *argv[])
{
    string s1("((((a:1,b:1):1,c:1):1,d:1):1,(i:1,(j:1,k:1):1):1,(l:1,(h:1,(g:1,(e:1,f:1):1):1):1):1);");
    string s2("((((a:2,b:2):2,c:2):2,d:2):2,(h:2,(g:2,(e:2,f:2):2):2):2,(i:2,(j:2,(k:2,l:2):2):2):2);");
    string s5("(((((Sp1:0.237938301594,Sp2:0.237938301594):0.652150465533,Sp3:0.890088767127):1.0032766685,(Sp4:1.19159193346,(((Sp5:0.0499647780388,Sp6:0.0499647780388):0.625094094731,(Sp7:0.385992882857,(Sp8:0.0229771776543,Sp9:0.0229771776543):0.363015705203):0.289065989913):0.100330232606,Sp10:0.775389105376):0.416202828082):0.70177350217):1.37956852633,(((Sp11:0.143770218098,Sp12:0.143770218098):2.22399819333,(Sp13:0.879389128685,Sp14:0.879389128685):1.48837928275):0.882615734883,((((((Sp15:0.115537491571,Sp16:0.115537491571):1.14083013878,(Sp17:0.787001759928,((Sp18:0.109471903715,Sp19:0.109471903715):0.647746750411,Sp20:0.757218654126):0.029783105802):0.469365870425):0.210543598558,((Sp21:0.0928829593343,Sp22:0.0928829593343):0.596056952564,(Sp23:0.295394029748,Sp24:0.295394029748):0.39354588215):0.777971317013):0.273542049999,(((Sp25:0.750938536668,((Sp26:0.147146981319,Sp27:0.147146981319):0.197746581084,Sp28:0.344893562404):0.406044974265):0.224512201078,Sp29:0.975450737746):0.568823245523,(Sp30:0.423829830969,Sp31:0.423829830969):1.1204441523):0.196179295641):0.204967720146,(Sp32:1.88686786295,Sp33:1.88686786295):0.0585531361112):0.782778416912,((Sp34:0.453020826526,Sp35:0.453020826526):1.00864842727,Sp36:1.4616692538):1.26653016217):0.522184730348):0.0225498156455):0.118838892224,((Sp37:1.61330136153,((Sp38:1.11318444903,Sp39:1.11318444903):0.488085401285,(Sp40:1.42576644618,(((Sp41:0.0435502147095,Sp42:0.0435502147095):0.233013603141,Sp43:0.276563817851):0.175951351205,(Sp44:0.0424687878211,Sp45:0.0424687878211):0.410046381235):0.973251277121):0.175503404136):0.0120315112168):1.10970378837,((((Sp46:0.0108728387409,Sp47:0.0108728387409):0.096790292957,Sp48:0.107663131698):0.575955539077,Sp49:0.683618670775):0.35904620643,Sp50:1.0426648772):1.68034027269):0.66876770429):0.0;");
    string s6("(((((Sp1:0.237938301594,Sp2:0.237938301594):0.652150465533,(Sp4:1.19159193346,(Sp10:0.775389105376,(Sp7:0.385992882857,((Sp5:0.0499647780388,Sp6:0.0499647780388):0.625094094731,(Sp8:0.0229771776543,Sp9:0.0229771776543):0.363015705203):0.289065989913):0.100330232606):0.416202828082):0.70177350217):1.0032766685,Sp3:0.890088767127):1.37956852633,(((Sp11:0.143770218098,Sp12:0.143770218098):2.22399819333,(Sp13:0.879389128685,Sp14:0.879389128685):1.48837928275):0.882615734883,(((Sp34:0.453020826526,Sp35:0.453020826526):1.00864842727,Sp36:1.4616692538):1.26653016217,((Sp32:1.88686786295,Sp33:1.88686786295):0.0585531361112,((((Sp21:0.0928829593343,Sp22:0.0928829593343):0.596056952564,(Sp23:0.295394029748,Sp24:0.295394029748):0.39354588215):0.777971317013,((Sp15:0.115537491571,Sp16:0.115537491571):1.14083013878,((Sp20:0.757218654126,Sp17:0.787001759928):0.029783105802,(Sp18:0.109471903715,Sp19:0.109471903715):0.647746750411):0.469365870425):0.210543598558):0.273542049999,((Sp30:0.423829830969,Sp31:0.423829830969):1.1204441523,(Sp29:0.975450737746,((Sp26:0.147146981319,Sp27:0.147146981319):0.197746581084,(Sp25:0.750938536668,Sp28:0.344893562404):0.406044974265):0.224512201078):0.568823245523):0.196179295641):0.204967720146):0.782778416912):0.522184730348):0.0225498156455):0.118838892224,((Sp37:1.61330136153,((Sp38:1.11318444903,Sp39:1.11318444903):0.488085401285,(Sp40:1.42576644618,(((Sp41:0.0435502147095,Sp42:0.0435502147095):0.233013603141,Sp43:0.276563817851):0.175951351205,(Sp44:0.0424687878211,Sp45:0.0424687878211):0.410046381235):0.973251277121):0.175503404136):0.0120315112168):1.10970378837,((((Sp46:0.0108728387409,Sp47:0.0108728387409):0.096790292957,Sp48:0.107663131698):0.575955539077,Sp49:0.683618670775):0.35904620643,Sp50:1.0426648772):1.68034027269):0.66876770429):0.0;");
    auto t1 = PhyloTree(s5, false);
    auto t2 = PhyloTree(s6, false);


    vector<EdgeAttribute> algo1attrs, algo2attrs, algo3attrs;
    clock_t start = clock();
    // ALGO1
    vector<PhyloTreeEdge> t1_edges;
    vector<PhyloTreeEdge> t2_edges;
    vector<PhyloTreeEdge> algo1_edges, algo2_edges, algo3_edges;
    t1.getEdges(t1_edges);
    t2.getEdges(t2_edges);
    EdgeAttribute l_attr, r_attr;
    Bipartition search_split;
    std::sort(t1_edges.begin(), t1_edges.end());
    std::sort(t2_edges.begin(), t2_edges.end());

    // Adapted std::set_intersection algorithm
    auto first1 = t1_edges.begin();
    auto first2 = t2_edges.begin();
    auto last1 = t1_edges.end();
    auto last2 = t2_edges.end();
    auto common_bkinstr = std::back_inserter(algo1_edges);

    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            if (first1->isCompatibleWith(t2_edges)) {
                algo1_edges.push_back(PhyloTreeEdge(first1->asSplit(), first1->getAttribute(), first1->getOriginalID()));
            }
            ++first1; // first1 not in list2
        } else {
            if (!(*first2 < *first1)) { // first1 == first2
                l_attr = first1->getAttribute();
                search_split = first1->asSplit();
                r_attr = t2.getAttribOfSplit(search_split);
                auto commonAttrib = EdgeAttribute::difference(l_attr, r_attr);
                *common_bkinstr++ = PhyloTreeEdge(first1->asSplit(), commonAttrib, first1->getOriginalID());
                ++first1;
            }
            else { // first2 not in list1
                if (first2->isCompatibleWith(t1_edges)) {
                    algo1_edges.push_back(PhyloTreeEdge(first2->asSplit(), first2->getAttribute(), first2->getOriginalID()));
                }
            }
            ++first2;
        }
    }



    clock_t end = clock();
    printf("Time taken: %.3f millisec\n", 1000 * (double)(end - start)/CLOCKS_PER_SEC);
    cout << "algo1attrs.size  " << algo1attrs.size() << endl;
    cout << "algo1_edges.size " << algo1_edges.size() << endl;

    start = clock();
    //ALGO2

    if (t1.getLeaf2NumMap() != t2.getLeaf2NumMap()) {
        throw runtime_error("leaf2NumMaps are not equal");
    }
    auto splits = t2.getSplits();
    for (auto &e1 : t1.getEdges()) {
        if (!e1.isZero()) {
            search_split = e1.asSplit();
            if (std::find(splits.begin(), splits.end(), search_split) != splits.end()) {
                l_attr = e1.getAttribute();
                r_attr = t2.getAttribOfSplit(search_split);
                auto commonAttrib = EdgeAttribute::difference(l_attr, r_attr);
                algo2attrs.push_back(commonAttrib);
                algo2_edges.push_back(PhyloTreeEdge(search_split, commonAttrib, e1.getOriginalID()));
            }
            else if (e1.isCompatibleWith(splits)) {
                auto commonAttrib = e1.getAttribute();
                algo2attrs.push_back(commonAttrib);
                algo2_edges.push_back(PhyloTreeEdge(search_split, commonAttrib, e1.getOriginalID()));
            }
        }
    }
    splits = t1.getSplits();
    for (auto &e2 : t2.getEdges()) {
        if (!e2.isZero()) {
            search_split = e2.asSplit();
            if (e2.isCompatibleWith(splits) && std::find(splits.begin(), splits.end(), search_split) == splits.end()) {
                auto commonAttrib = e2.getAttribute();
                algo2attrs.push_back(commonAttrib);
                algo2_edges.push_back(PhyloTreeEdge(search_split, commonAttrib, e2.getOriginalID()));
            }
        }
    }
    end = clock();
    printf("Time taken: %.3f millisec\n", 1000 * (double)(end - start)/CLOCKS_PER_SEC);
    cout << "algo2attrs.size  " << algo2attrs.size() << endl;
    cout << "algo2_edges.size " << algo2_edges.size() << endl;
    start = clock();
    //ALGO3
    EdgeAttribute l_attr2, r_attr2;
    Bipartition search_split2;
    if (t1.getLeaf2NumMap() != t2.getLeaf2NumMap()) {
        throw runtime_error("leaf2NumMaps are not equal");
    }
    auto _t1_edges = t1.getEdges();
    auto _t2_edges = t2.getEdges();
    auto t1_splits = t1.getSplits();
    auto t2_splits = t2.getSplits();
    unordered_map<unsigned long, size_t> t1_split_map;
    unordered_map<unsigned long, size_t> t2_split_map;

    for (auto& s : t2_splits) {
        ++t2_split_map[s.getPartition().to_ulong()];
    }

    for (size_t i = 0; i < _t1_edges.size(); ++i) {
        auto e1 = _t1_edges[i];
        search_split = t1_splits[i];
        ++t1_split_map[search_split.getPartition().to_ulong()];
        if (!e1.isZero()) {
            if (t2_split_map.count(search_split.getPartition().to_ulong()) != 0) {
                l_attr2 = e1.getAttribute();
                r_attr2 = t2.getAttribOfSplit(search_split);
                algo3_edges.push_back(PhyloTreeEdge(search_split, EdgeAttribute::difference(l_attr2, r_attr2), e1.getOriginalID()));
            }
            else if (e1.isCompatibleWith(t2_splits)) {
                algo3_edges.push_back(PhyloTreeEdge(search_split, e1.getAttribute(), e1.getOriginalID()));
            }
        }
    }

    for (size_t i = 0; i < _t2_edges.size(); ++i) {
        auto e2 = _t2_edges[i];
        search_split = t2_splits[i];
        if (!e2.isZero()) {
            if (e2.isCompatibleWith(t1_splits) && t1_split_map.count(search_split.getPartition().to_ulong()) == 0) {
                algo3_edges.push_back(PhyloTreeEdge(search_split, e2.getAttribute(), e2.getOriginalID()));
            }
        }
    }
    end = clock();
    printf("Time taken: %.3f millisec\n", 1000 * (double)(end - start)/CLOCKS_PER_SEC);
    cout << "algo3_edges.size " << algo3_edges.size() << endl;

//    vector<PhyloTreeEdge> all;
//    for (auto edge : t1.getEdges()) {
//        all.push_back(edge);
//    }
//    for (auto edge : t2.getEdges()) {
//        all.push_back(edge);
//    }
//    for (size_t i = 0; i < all.size(); ++i) {
//        cout << "all "  << all[i].toStringVerbose(t1.getLeaf2NumMap()) << endl;
//    }
//
//
//    clock_t start = clock();
//    vector<PhyloTreeEdge> common;
//    vector<Bipartition> edges1;
//    vector<Bipartition> edges2;
//    EdgeAttribute l_attr, r_attr;
//
//    t1.getSplits(edges1);
//    t2.getSplits(edges2);
//
//    std::sort(edges1.begin(), edges1.end());
//    std::sort(edges2.begin(), edges2.end());
//    auto first1 = edges1.begin();
//    auto first2 = edges2.begin();
//    auto last1 = edges1.end();
//    auto last2 = edges2.end();
//    auto d_first = std::back_inserter(common);
//
//    first1 = edges1.begin();
//    first2 = edges2.begin();
//    while (first1 != last1 && first2 != last2) {
//        if (*first1 < *first2) {
//            ++first1;
//        } else  {
//            if (!(*first2 < *first1)) {
//                //first1->length -= first2->length;
//                *d_first++ = PhyloTreeEdge(*first1, EdgeAttribute(first1->length - first2->length), first1->ID);
//                ++first1;
//            }
//            ++first2;
//        }
//    }
//    PhyloTree::getCommonEdges2(t1,t2, common);
//    clock_t end = clock();
//    printf("Time taken: %.3f millisec\n", 1000 * (double)(end - start)/CLOCKS_PER_SEC);
//    for (size_t i = 0; i < common.size(); ++i) {
//        cout << "common "  << common[i].toStringVerbose(t1.getLeaf2NumMap())  << endl;
//    }
//    cout << endl;

//    start = clock();
//    vector<PhyloTreeEdge> chk;
//    PhyloTree::getCommonEdges(t1, t2, chk);
//    clock_t end = clock();
//    printf("Time taken: %.3f millisec\n", 1000 * (double)(end - start)/CLOCKS_PER_SEC);
//    for (size_t i = 0; i < chk.size(); ++i) {
//        cout << "chk "  << chk[i].toStringVerbose(t1.getLeaf2NumMap()) << endl;
//    }
//    cout << endl;
//
//    cout << "EQUALITY? " << ((common == chk) ? "TRUE" : "FALSE") << endl;
//    cout << "EQUALITY? " << (Tools::vector_equal(common, chk) ? "TRUE" : "FALSE") << endl;
//
//    cout << "WRF check" << endl;
//    double wrf_value = 0;
//
//    // Collect edges-in-common and edges-not-in-common...
//    vector<PhyloTreeEdge> eic;
//    PhyloTree::getCommonEdges2(t1, t2, eic);
//    vector<PhyloTreeEdge> enic;
//    t1.getEdgesNotInCommonWith(t2, enic);
//    t2.getEdgesNotInCommonWith(t1, enic);
//    // ... and leaves
//    vector<EdgeAttribute> leaves1 = t1.getLeafEdgeAttribs();
//    vector<EdgeAttribute> leaves2 = t2.getLeafEdgeAttribs(); // Assuming these are the same leaves
//
//    // Collect length differences for internal edges...
//    for (PhyloTreeEdge &pte : eic) {
//        wrf_value += pte.getLength();
//    }
//
//    for (PhyloTreeEdge &pte : enic) {
//        wrf_value += pte.getLength();
//    }
//
//    // ... and leaves
//    for (int i=0; i < leaves1.size() ; i++) {
//        wrf_value += abs(leaves1[i].norm() - leaves2[i].norm());
//    }

//    cout << "wrf value new = " << wrf_value << endl;
//    s5 = string("(((((Sp1:0.237938301594,Sp2:0.237938301594):0.652150465533,Sp3:0.890088767127):1.0032766685,(Sp4:1.19159193346,(((Sp5:0.0499647780388,Sp6:0.0499647780388):0.625094094731,(Sp7:0.385992882857,(Sp8:0.0229771776543,Sp9:0.0229771776543):0.363015705203):0.289065989913):0.100330232606,Sp10:0.775389105376):0.416202828082):0.70177350217):1.37956852633,(((Sp11:0.143770218098,Sp12:0.143770218098):2.22399819333,(Sp13:0.879389128685,Sp14:0.879389128685):1.48837928275):0.882615734883,((((((Sp15:0.115537491571,Sp16:0.115537491571):1.14083013878,(Sp17:0.787001759928,((Sp18:0.109471903715,Sp19:0.109471903715):0.647746750411,Sp20:0.757218654126):0.029783105802):0.469365870425):0.210543598558,((Sp21:0.0928829593343,Sp22:0.0928829593343):0.596056952564,(Sp23:0.295394029748,Sp24:0.295394029748):0.39354588215):0.777971317013):0.273542049999,(((Sp25:0.750938536668,((Sp26:0.147146981319,Sp27:0.147146981319):0.197746581084,Sp28:0.344893562404):0.406044974265):0.224512201078,Sp29:0.975450737746):0.568823245523,(Sp30:0.423829830969,Sp31:0.423829830969):1.1204441523):0.196179295641):0.204967720146,(Sp32:1.88686786295,Sp33:1.88686786295):0.0585531361112):0.782778416912,((Sp34:0.453020826526,Sp35:0.453020826526):1.00864842727,Sp36:1.4616692538):1.26653016217):0.522184730348):0.0225498156455):0.118838892224,((Sp37:1.61330136153,((Sp38:1.11318444903,Sp39:1.11318444903):0.488085401285,(Sp40:1.42576644618,(((Sp41:0.0435502147095,Sp42:0.0435502147095):0.233013603141,Sp43:0.276563817851):0.175951351205,(Sp44:0.0424687878211,Sp45:0.0424687878211):0.410046381235):0.973251277121):0.175503404136):0.0120315112168):1.10970378837,((((Sp46:0.0108728387409,Sp47:0.0108728387409):0.096790292957,Sp48:0.107663131698):0.575955539077,Sp49:0.683618670775):0.35904620643,Sp50:1.0426648772):1.68034027269):0.66876770429):0.0;");
//    s6 = string("(((((Sp1:0.237938301594,Sp2:0.237938301594):0.652150465533,(Sp4:1.19159193346,(Sp10:0.775389105376,(Sp7:0.385992882857,((Sp5:0.0499647780388,Sp6:0.0499647780388):0.625094094731,(Sp8:0.0229771776543,Sp9:0.0229771776543):0.363015705203):0.289065989913):0.100330232606):0.416202828082):0.70177350217):1.0032766685,Sp3:0.890088767127):1.37956852633,(((Sp11:0.143770218098,Sp12:0.143770218098):2.22399819333,(Sp13:0.879389128685,Sp14:0.879389128685):1.48837928275):0.882615734883,(((Sp34:0.453020826526,Sp35:0.453020826526):1.00864842727,Sp36:1.4616692538):1.26653016217,((Sp32:1.88686786295,Sp33:1.88686786295):0.0585531361112,((((Sp21:0.0928829593343,Sp22:0.0928829593343):0.596056952564,(Sp23:0.295394029748,Sp24:0.295394029748):0.39354588215):0.777971317013,((Sp15:0.115537491571,Sp16:0.115537491571):1.14083013878,((Sp20:0.757218654126,Sp17:0.787001759928):0.029783105802,(Sp18:0.109471903715,Sp19:0.109471903715):0.647746750411):0.469365870425):0.210543598558):0.273542049999,((Sp30:0.423829830969,Sp31:0.423829830969):1.1204441523,(Sp29:0.975450737746,((Sp26:0.147146981319,Sp27:0.147146981319):0.197746581084,(Sp25:0.750938536668,Sp28:0.344893562404):0.406044974265):0.224512201078):0.568823245523):0.196179295641):0.204967720146):0.782778416912):0.522184730348):0.0225498156455):0.118838892224,((Sp37:1.61330136153,((Sp38:1.11318444903,Sp39:1.11318444903):0.488085401285,(Sp40:1.42576644618,(((Sp41:0.0435502147095,Sp42:0.0435502147095):0.233013603141,Sp43:0.276563817851):0.175951351205,(Sp44:0.0424687878211,Sp45:0.0424687878211):0.410046381235):0.973251277121):0.175503404136):0.0120315112168):1.10970378837,((((Sp46:0.0108728387409,Sp47:0.0108728387409):0.096790292957,Sp48:0.107663131698):0.575955539077,Sp49:0.683618670775):0.35904620643,Sp50:1.0426648772):1.68034027269):0.66876770429):0.0;");
//    t1 = PhyloTree(s5, false);
//    t2 = PhyloTree(s6, false);
//
//    wrf_value = 0;
//    wrf_value = Distance::getWeightedRobinsonFouldsDistance(t1, t2, false);
//    cout << "wrf value old = " << wrf_value << endl;
    return 0;
}
