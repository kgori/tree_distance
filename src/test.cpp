#include "BipartiteGraph.h"
#include "Distance.h"
#include "test_catch_helper.h"
#include "Tools.h"


#define TOLERANCE 0.0000001

TEST_CASE("Bipartition") {
    SECTION("Construction") {
        // test default constructor
        auto a = Bipartition();
        REQUIRE(a.isEmpty());

        // test string constructor
        auto b = Bipartition("0100101");
        REQUIRE(b.toString() == "0100101");

        // test copy constructor
        auto c = Bipartition(b);
        REQUIRE(c.toString() == b.toString());

        // test getting bitset
        auto bitset = c.getPartition();

        // test bitset constructor
        auto d = Bipartition(bitset);
        REQUIRE(d.toString() == c.toString());
    }

    SECTION("Equality") {
        auto a = Bipartition("1100101110");
        auto b = Bipartition("1100101110");
        auto c = Bipartition("0011010001");

        REQUIRE(a.equals(b));
        REQUIRE(!a.equals(c));
        REQUIRE(!b.equals(c));
        CHECK(a == b);
        CHECK(a != c);
        CHECK(b != c);
    }

    SECTION("Element-wise assignment") {
        auto a = Bipartition("11000000");
        auto b = Bipartition("00000000");
        b.addOne(0);
        b.addOne(1);
        REQUIRE(a.equals(b));
    }

    SECTION("Complement") {
        auto a = Bipartition("11000000");
        auto b = Bipartition("00110000");
        auto c = Bipartition("00111111");
        b.complement(4);
        c.complement(c.size());

        REQUIRE(a.equals(b));
        REQUIRE(a.equals(c));
    }

    SECTION("Logical") {
        auto a = Bipartition("11111");
        auto b = Bipartition("01100");
        a &= b;
        REQUIRE(a == b);
        REQUIRE(~b == Bipartition("10011"));
        REQUIRE(Bipartition("11111111").andNot(Bipartition("10100011")) == Bipartition("01011100"));
        REQUIRE((Bipartition("11111111") &= (Bipartition("10100011"))) == Bipartition("10100011"));
        REQUIRE(Bipartition("1100100111").andNot(Bipartition("1011011100")) == Bipartition("0100100011"));
        REQUIRE((Bipartition("1100100111") &= (Bipartition("1011011100"))) == Bipartition("1000000100"));
    }

    SECTION("Crosses") {
        auto a = Bipartition("0101010101");
        auto b = Bipartition("1010101010");
        auto c = Bipartition("1111100000");

        CHECK(!a.crosses(b));
        CHECK(!b.crosses(a));
        CHECK(a.crosses(c));
        CHECK(c.crosses(a));
        CHECK(b.crosses(c));
        CHECK(c.crosses(b));

        auto d = Bipartition("1100101110110");
        auto e = Bipartition("1010101100100");
        auto f = Bipartition("0011010001001");
        auto g = Bipartition("0101010011011");

        REQUIRE(d.crosses(e) == e.crosses(d));
        REQUIRE(d.crosses(f) == f.crosses(d));
        REQUIRE(d.crosses(g) == g.crosses(d));
        REQUIRE(e.crosses(f) == f.crosses(e));
        REQUIRE(e.crosses(g) == g.crosses(e));
        REQUIRE(f.crosses(g) == g.crosses(f));
    }

    SECTION("Contains") {
        auto a = Bipartition("110011");
        auto b = Bipartition("100001");
        auto c = Bipartition("001000");

        CHECK(a.contains(b));
        CHECK(!b.contains(a));
        CHECK(a.properlyContains(b));
        CHECK(!b.properlyContains(a));
        CHECK(c.contains(2));
        REQUIRE_FALSE(c.contains(3));
    }

    SECTION("Empty") {
        auto a = Bipartition("0000000");
        auto b = Bipartition("1100000");
        auto c = Bipartition("0011000");
        auto d = Bipartition("0011100");
        auto e = Bipartition("0001100");
        REQUIRE(a.isEmpty());
        REQUIRE_FALSE(b.isEmpty());
        REQUIRE_FALSE(c.isEmpty());
        REQUIRE_FALSE(d.isEmpty());
        REQUIRE_FALSE(e.isEmpty());
    }
}

TEST_CASE("PhyloTreeEdge") {
    SECTION("Construction") {
        auto a = PhyloTreeEdge();
        REQUIRE(a.toString() == "[0] ");
        REQUIRE(a.getOriginalID() == -1);

        auto bip = make_shared<Bipartition>("01001101");
        auto b = PhyloTreeEdge(bip->getPartition());
        REQUIRE(b.toString() == "[0] 01001101");
        REQUIRE(b.getOriginalID() == -1);

        auto attrib = 0.5;
        auto c = PhyloTreeEdge(attrib);
        CHECK(c.toString() == "[0.5] ");
        REQUIRE(c.getOriginalID() == -1);

        auto d = PhyloTreeEdge(attrib, 10);
        REQUIRE(d.toString() == "[0.5] ");
        REQUIRE(d.getOriginalID() == 10);

        auto e = PhyloTreeEdge(attrib, bip, 11);
        REQUIRE(e.toString() == "[0.5] 00000000");
        REQUIRE(e.getOriginalID() == 11);
        REQUIRE(e.isEmpty());

        auto f = PhyloTreeEdge(*bip, attrib, 12);
        REQUIRE(f.toString() == "[0.5] " + bip->toString());
        REQUIRE(f.getOriginalID() == 12);

        auto edge = Bipartition("11011011");
        auto g = PhyloTreeEdge(edge.getPartition(), attrib, bip->getPartition(), 13);
        REQUIRE(g.toString() == "[0.5] " + edge.toString());
        REQUIRE(g.getOriginalID() == 13);

        auto h = PhyloTreeEdge(g);
        REQUIRE(h.toString() == g.toString());
        REQUIRE(h.getOriginalID() == g.getOriginalID());
    }
    SECTION("Empty") {
        SECTION("Empty") {
            auto a = PhyloTreeEdge(Bipartition("0000000").getPartition());
            auto b = PhyloTreeEdge(Bipartition("1100000").getPartition());
            auto c = PhyloTreeEdge(Bipartition("0011000").getPartition());
            auto d = PhyloTreeEdge(Bipartition("0011100").getPartition());
            auto e = PhyloTreeEdge(Bipartition("0001100").getPartition());
            REQUIRE(a.isEmpty());
            REQUIRE_FALSE(b.isEmpty());
            REQUIRE_FALSE(c.isEmpty());
            REQUIRE_FALSE(d.isEmpty());
            REQUIRE_FALSE(e.isEmpty());
        }
    }
}


TEST_CASE("Tools") {
    SECTION("Next Index") {
        string s = "(a:1,(b:2,c:3));";
        CHECK(Tools::nextIndex(s, 0, ",)") == 4);
        CHECK(Tools::nextIndex(s, 1, ",)") == 4);
        CHECK(Tools::nextIndex(s, 2, ",)") == 4);
        CHECK(Tools::nextIndex(s, 3, ",)") == 4);
        CHECK(Tools::nextIndex(s, 4, ",)") == 9);
        CHECK(Tools::nextIndex(s, 5, ",)") == 9);
        CHECK(Tools::nextIndex(s, 6, ",)") == 9);
        CHECK(Tools::nextIndex(s, 7, ",)") == 9);
        CHECK(Tools::nextIndex(s, 8, ",)") == 9);
        CHECK(Tools::nextIndex(s, 9, ",)") == 13);
        CHECK(Tools::nextIndex(s, 10, ",)") == 13);
        CHECK(Tools::nextIndex(s, 11, ",)") == 13);
        CHECK(Tools::nextIndex(s, 12, ",)") == 13);
        CHECK(Tools::nextIndex(s, 13, ",)") == 14);
        CHECK(Tools::nextIndex(s, 14, ",)") == s.size());
        CHECK(Tools::nextIndex(s, 15, ",)") == s.size());
    }

    SECTION("Split and join") {
        string a("a|set|of|items");
        string b("--a, set: of\t| items--");
        vector<string> v{"a", "set", "of", "items"};
        CHECK(Tools::string_split(a, "|", "") == v);
        CHECK(Tools::string_split(b, ",:| \t", "-") == v);
        CHECK(Tools::string_join(v, "|") == a);
        CHECK(Tools::string_join(v, "*") == "a*set*of*items");
        CHECK(Tools::vector_to_string(v) == "a set of items");
        string c("(1,2,3,4)");
        string d(",1,,2,,,3,,,,4,,,,,");
        vector<string> w{"1", "2", "3", "4"};
        CHECK(Tools::string_split(c, ", ", "(,)") == w);
        CHECK(Tools::string_split(d, ", ", "(,)") == w);
    }

    SECTION("Double conversion") {
        double x = 1;
        CHECK(Tools::double_to_string(x) == "1.00000");
        vector<string> a{"1", "2", "3", "4"};
        vector<double> b{1.0, 2.0, 3.0, 4.0};
        CHECK(Tools::stringvec_to_doublevec(a) == b);

        vector<string> c{"a", "b", "c", "d"};
        CHECK_THROWS(Tools::stringvec_to_doublevec(c));
    }

    SECTION("Despace") {
        string s("string with\tvarious\rwhitespace\n\n\n");
        Tools::despace(s);
        CHECK(s.size() == 27);
        CHECK(s == "stringwithvariouswhitespace");
    }
}

TEST_CASE("PhyloTree") {
    SECTION("Construction") {
        auto edges = vector<PhyloTreeEdge>({},
        {
        });
        string n1("(a:1,(b:2,c:3):4);");
        string n2("(a:1, (b:2, c:3):4);");
        string n3("(a:1, b:2, c:3);");
        string n4("((a:3,b:4):.1,(c:5,((d:6,e:7):.2,f:8):.3):.4);");
        auto a = PhyloTree(n1, true);
        auto b = PhyloTree(n2, true);
        auto c = PhyloTree(n3, false);
        auto d = PhyloTree(n4, true);
        REQUIRE(a.getBranchLengthSum() == 10);
        REQUIRE(b.getBranchLengthSum() == 10);
        REQUIRE(c.getBranchLengthSum() == 6);
        REQUIRE(d.getBranchLengthSum() == 34);

        CHECK(n2 == "(a:1, (b:2, c:3):4);");
        CHECK(b.newick == n2);

        edges = d.getEdges();
        auto l2nm = d.getLeaf2NumMap();
        auto e = PhyloTree(edges, l2nm);
    }

    SECTION("Leaf to num maps") {
        string n1("(g:1,(a:1,(b:1,c:1):1):1,(f:1,(e:1,d:1):1):1);");
        string n2("(g:2,(a:2,(b:2,c:2):2):2,(d:2,(e:2,f:2):2):2);");
        string n3("((g:1,(a:1,(b:1,c:1):1):1):1,(f:1,(e:1,d:1):1):1);");
        string n4("((g:2,(a:2,(b:2,c:2):2):2):2,(d:2,(e:2,f:2):2):2);");

        auto a = PhyloTree(n1, false);
        auto b = PhyloTree(n2, false);
        vector<string> expected{"a", "b", "c", "d", "e", "f", "g"};
        REQUIRE(a.getLeaf2NumMap() == expected);
        REQUIRE(b.getLeaf2NumMap() == expected);

        // rooted
        auto c = PhyloTree(n3, true);
        auto d = PhyloTree(n4, true);
        REQUIRE(c.getLeaf2NumMap() == expected);
        REQUIRE(d.getLeaf2NumMap() == expected);
    }

    SECTION("Get edges") {
        // unrooted
        string n1("(g:1,(a:1,(b:1,c:1):2):3,(f:1,(e:1,d:1):4):5);");
        string n2("(g:2,(a:2,(b:2,c:2):3):4,(d:2,(e:2,f:2):5):6);");
        auto a = PhyloTree(n1, false);
        auto b = PhyloTree(n2, false);
        auto a_edges = a.getEdges();
        auto b_edges = b.getEdges();
        REQUIRE(a_edges.size() == 4);
        REQUIRE(b_edges.size() == 4);
        for (size_t i = 0; i < 4; ++i) {
            CHECK(a_edges[i] == a.getEdge(i));
            CHECK(a_edges[i].getLength() == i + 2);
            CHECK(b_edges[i] == b.getEdge(i));
            CHECK(b_edges[i].getLength() == i + 3);
        }

        // rooted
        string n3("((g:1,(a:1,(b:1,c:1):1):2):3,(f:1,(e:1,d:1):4):5);");
        string n4("((g:2,(a:2,(b:2,c:2):2):3):4,(d:2,(e:2,f:2):5):6);");
        auto c = PhyloTree(n3, true);
        auto d = PhyloTree(n4, true);
        auto c_edges = c.getEdges();
        auto d_edges = d.getEdges();
        REQUIRE(c_edges.size() == 5);
        REQUIRE(d_edges.size() == 5);
        for (size_t i = 0; i < 5; ++i) {
            CHECK(c_edges[i] == c.getEdge(i));
            CHECK(c_edges[i].getLength() == i + 1);
            CHECK(d_edges[i] == d.getEdge(i));
            CHECK(d_edges[i].getLength() == i + 2);
        }
    }

    SECTION("Edge Attributes") {
        double test;

        // unrooted
        string n1("(g:1,(a:1,(b:1,c:1):1):1,(f:1,(e:1,d:1):1):1);");
        string n2("(g:2,(a:2,(b:2,c:2):2):2,(d:2,(e:2,f:2):2):2);");
        auto a = PhyloTree(n1, false);
        auto b = PhyloTree(n2, false);
        auto a_attribs = a.getLeafEdgeLengths();
        auto b_attribs = b.getLeafEdgeLengths();
        CHECK(a_attribs.size() == a.numLeaves());
        CHECK(a_attribs.size() == 7);
        CHECK(b_attribs.size() == b.numLeaves());
//        CHECK(l);

        // rooted
        string n3("((g:0.7,(a:0.1,(b:0.2,c:0.3):1):1):1,(f:0.6,(e:0.5,d:0.4):1):1);");
        string n4("((g:1.4,(a:0.2,(b:0.4,c:0.6):2):2):2,(d:0.8,(e:1.0,f:1.2):2):2);");
        auto c = PhyloTree(n3, true);
        auto d = PhyloTree(n4, true);
        auto c_attribs = c.getLeafEdgeLengths();
        auto d_attribs = d.getLeafEdgeLengths();
        CHECK(c_attribs.size() == c.numLeaves());
        CHECK(d_attribs.size() == d.numLeaves());
        CHECK(c_attribs.size() == 7);
        CHECK(d_attribs.size() == 7);

        for (size_t i = 0; i < 7; i++) {
            test = ((double) i + 1) / 10;
            CHECK(a_attribs[i] == 1);
            CHECK(b_attribs[i] == 2);
            CHECK(c_attribs[i] == test);
            CHECK(d_attribs[i] == 2 * test);
        }
    }

    SECTION("Distance from origin") {
        string n1("(g:1,(a:1,(b:1,c:1):1):1,(f:1,(e:1,d:1):1):1);");
        string n2("(g:2,(a:2,(b:2,c:2):2):2,(d:2,(e:2,f:2):2):2);");
        string n3("((g:0.7,(a:0.1,(b:0.2,c:0.3):1):1):1,(f:0.6,(e:0.5,d:0.4):1):1);");
        string n4("((g:1.4,(a:0.2,(b:0.4,c:0.6):2):2):2,(d:0.8,(e:1.0,f:1.2):2):2);");
        auto a = PhyloTree(n1, false);
        auto b = PhyloTree(n2, false);
        auto c = PhyloTree(n3, true);
        auto d = PhyloTree(n4, true);

        REQUIRE(abs(a.getDistanceFromOrigin() - sqrt(11)) < TOLERANCE);
//        REQUIRE(abs(a.getDistanceFromOriginNoLeaves() - sqrt(4)) < TOLERANCE);
        REQUIRE(abs(b.getDistanceFromOrigin() - sqrt(44)) < TOLERANCE);
//        REQUIRE(abs(b.getDistanceFromOriginNoLeaves() - sqrt(16)) < TOLERANCE);
        REQUIRE(abs(c.getDistanceFromOrigin() - sqrt(6.4)) < TOLERANCE);
//        REQUIRE(abs(c.getDistanceFromOriginNoLeaves() - sqrt(5)) < TOLERANCE);
        REQUIRE(abs(d.getDistanceFromOrigin() - sqrt(25.6)) < TOLERANCE);
//        REQUIRE(abs(d.getDistanceFromOriginNoLeaves() - sqrt(20)) < TOLERANCE);

        REQUIRE(abs(a.getBranchLengthSum() - 11) < TOLERANCE);
        REQUIRE(abs(b.getBranchLengthSum() - 22) < TOLERANCE);
        REQUIRE(abs(c.getBranchLengthSum() - 7.8) < TOLERANCE);
        REQUIRE(abs(d.getBranchLengthSum() - 15.6) < TOLERANCE);
    }

    SECTION("Edges in common") {
        string n1("(g:1,(a:1,(b:1,c:1):1):1,(f:1,(e:1,d:1):1):1);");
        string n2("(g:2,(a:2,(b:2,c:2):2):2,(d:2,(e:2,f:2):2):2);");
        auto a = PhyloTree(n1, false);
        auto b = PhyloTree(n2, false);
        vector<PhyloTreeEdge> eic, enic;
        PhyloTree::getCommonEdges(a, b, eic);
        a.getEdgesNotInCommonWith(b, enic);
        b.getEdgesNotInCommonWith(a, enic);
        CHECK(eic.size() == 3);
        CHECK(enic.size() == 2);
    }

    SECTION("Newick") {
        string n3("(g:1,(a:1,(b:1,c:1):1):1,(f:1,(e:1,d:1):1):1);");
        string n4("(g:2,(a:2,(b:2,c:2):2):2,(d:2,(e:2,f:2):2):2);");
        string n5("(a:1, b:2, c:3, d:4, e:5, f:7);");
        string n6("(b:2, a:1, d:4, c:3, e:5);");
        auto a = PhyloTree(n3, false);
        auto b = PhyloTree(n4, false);
        auto star1 = PhyloTree(n5, false);
        auto star2 = PhyloTree(n6, false);

//        cout << a.getNewick(true) << endl;
//        cout << b.getNewick(true) << endl;
//        cout << star1.getNewick(true) << endl;
//        cout << star2.getNewick(true) << endl;
//        for (size_t i = 0; i < star1.getLeafEdgeAttribs().size(); ++i) {
//            cout << star1.getLeafEdgeAttribs()[i].getAttribute() << " ";
//        }
//        cout << endl;
//        cout << star1.getBranchLengthSum() << endl;
    }
}

TEST_CASE("Ratio") {
    SECTION("Construction") {
        auto a = Ratio();
        CHECK(a.getELength() == 0);
        CHECK(a.getFLength() == 0);
        CHECK(a.getEEdges() == vector<PhyloTreeEdge>());
        CHECK(a.getFEdges() == vector<PhyloTreeEdge>());

        auto b = Ratio(5.5, 10.9);
        CHECK(b.getELength() == 5.5);
        CHECK(b.getFLength() == 10.9);
        CHECK(b.getEEdges() == vector<PhyloTreeEdge>());
        CHECK(b.getFEdges() == vector<PhyloTreeEdge>());

        auto c = Ratio(b);
        CHECK(c.getELength() == 5.5);
        CHECK(c.getFLength() == 10.9);
        CHECK(c.getEEdges() == vector<PhyloTreeEdge>());
        CHECK(c.getFEdges() == vector<PhyloTreeEdge>());

        string n1("((a:3,b:4):.1,(c:5,((d:6,e:7):.2,f:8):.3):.4);");
        string n2("((a:3,c:4):.5,(d:5,((b:6,e:7):.2,f:8):.3):.4);");
        auto t1 = PhyloTree(n1, true);
        auto eEdges = t1.getEdges();
        auto t2 = PhyloTree(n2, true);
        auto fEdges = t2.getEdges();
        auto d = Ratio(eEdges, fEdges);
        CHECK(abs(d.getELength() - sqrt(0.3)) < TOLERANCE);
        CHECK(abs(d.getFLength() - sqrt(0.54)) < TOLERANCE);

        auto e = d.reverse();
        CHECK(abs(d.getELength() - e.getFLength()) < TOLERANCE);
        CHECK(abs(d.getFLength() - e.getELength()) < TOLERANCE);

        auto f = e.clone();
        CHECK(abs(f.getFLength() - e.getFLength()) < TOLERANCE);
        CHECK(abs(f.getELength() - e.getELength()) < TOLERANCE);

        auto g = Ratio::combine(d, e);
        CHECK(abs(g.getELength() - sqrt(0.84)) < TOLERANCE);
        CHECK(abs(g.getFLength() - sqrt(0.84)) < TOLERANCE);

        deque<Ratio> queue;
        {
            queue.push_front(d);
        }
        {
            auto h = queue.front();
            CHECK(h.getEEdges() == queue.front().getEEdges());
            CHECK(h.getFEdges() == queue.front().getFEdges());
            CHECK(h.getELength() == queue.front().getELength());
            CHECK(h.getFLength() == queue.front().getFLength());
            queue.pop_front();
        }
    }

    SECTION("Edge lengths") {
        auto a = Ratio();
        a.setELength(0.666);
        a.setFLength(666);
        REQUIRE(a.getELength() == 0.666);
        REQUIRE(a.getFLength() == 666);
    }

    SECTION("Contains") {
        string n1("((a:3,b:4):.1,(c:5,((d:6,e:7):.2,f:8):.3):.4);");
        string n2("((a:3,c:4):.5,(d:5,((b:6,e:7):.2,f:8):.3):.4);");
        auto t1 = PhyloTree(n1, true);
        auto eEdges = t1.getEdges();
        auto t2 = PhyloTree(n2, true);
        auto fEdges = t2.getEdges();
        auto a = Ratio(eEdges, fEdges);
        CHECK(a.containsOriginalEEdge(Bipartition("110000")));
        CHECK(a.containsOriginalEEdge(Bipartition("000110")));
        CHECK(a.containsOriginalEEdge(Bipartition("000111")));
        CHECK(a.containsOriginalEEdge(Bipartition("001111")));
        CHECK(a.containsOriginalEEdge(Bipartition("101000")));
        CHECK(a.containsOriginalEEdge(Bipartition("010010")));
        CHECK(a.containsOriginalEEdge(Bipartition("010011")));
        CHECK(a.containsOriginalEEdge(Bipartition("010111")));
        CHECK_FALSE(a.containsOriginalEEdge(Bipartition("000000")));
        CHECK_FALSE(a.containsOriginalEEdge(Bipartition("111111")));
        CHECK_FALSE(a.containsOriginalEEdge(Bipartition("001000")));
        CHECK_FALSE(a.containsOriginalEEdge(Bipartition("001100")));
    }
}

TEST_CASE("RatioSequence") {

}

TEST_CASE("Geodesic") {

}

TEST_CASE("Bipartite Graph") {
    SECTION("Vertex Cover") {
        vector<int> aVertices{0, 1, 2, 3};
        vector<int> bVertices{0, 1, 2, 3};
//        auto g = BipartiteGraph();
    }
}

TEST_CASE("Vertex") {
    auto a = Vertex(0);
    auto b = Vertex(0.1);
    auto c = Vertex(0.2);
    auto d = Vertex(0.3);
    auto e = Vertex(0.4);
    CHECK(a.weight == 0);
    CHECK(abs(b.weight - 0.01) < TOLERANCE);
    CHECK(abs(c.weight - 0.04) < TOLERANCE);
    CHECK(abs(d.weight - 0.09) < TOLERANCE);
    CHECK(abs(e.weight - 0.16) < TOLERANCE);
}

TEST_CASE("Distance") {
    SECTION("Robinson-Foulds") {
        string n1("((a:3,b:4):.1,(c:5,((d:6,e:7):.2,f:8):.3):.4);");
        string n2("((a:3,c:4):.5,(d:5,((b:6,e:7):.2,f:8):.3):.4);");
        string n3("(((((Sp1:0.237938301594,Sp2:0.237938301594):0.652150465533,Sp3:0.890088767127):1.0032766685,(Sp4:1.19159193346,(((Sp5:0.0499647780388,Sp6:0.0499647780388):0.625094094731,(Sp7:0.385992882857,(Sp8:0.0229771776543,Sp9:0.0229771776543):0.363015705203):0.289065989913):0.100330232606,Sp10:0.775389105376):0.416202828082):0.70177350217):1.37956852633,(((Sp11:0.143770218098,Sp12:0.143770218098):2.22399819333,(Sp13:0.879389128685,Sp14:0.879389128685):1.48837928275):0.882615734883,((((((Sp15:0.115537491571,Sp16:0.115537491571):1.14083013878,(Sp17:0.787001759928,((Sp18:0.109471903715,Sp19:0.109471903715):0.647746750411,Sp20:0.757218654126):0.029783105802):0.469365870425):0.210543598558,((Sp21:0.0928829593343,Sp22:0.0928829593343):0.596056952564,(Sp23:0.295394029748,Sp24:0.295394029748):0.39354588215):0.777971317013):0.273542049999,(((Sp25:0.750938536668,((Sp26:0.147146981319,Sp27:0.147146981319):0.197746581084,Sp28:0.344893562404):0.406044974265):0.224512201078,Sp29:0.975450737746):0.568823245523,(Sp30:0.423829830969,Sp31:0.423829830969):1.1204441523):0.196179295641):0.204967720146,(Sp32:1.88686786295,Sp33:1.88686786295):0.0585531361112):0.782778416912,((Sp34:0.453020826526,Sp35:0.453020826526):1.00864842727,Sp36:1.4616692538):1.26653016217):0.522184730348):0.0225498156455):0.118838892224,((Sp37:1.61330136153,((Sp38:1.11318444903,Sp39:1.11318444903):0.488085401285,(Sp40:1.42576644618,(((Sp41:0.0435502147095,Sp42:0.0435502147095):0.233013603141,Sp43:0.276563817851):0.175951351205,(Sp44:0.0424687878211,Sp45:0.0424687878211):0.410046381235):0.973251277121):0.175503404136):0.0120315112168):1.10970378837,((((Sp46:0.0108728387409,Sp47:0.0108728387409):0.096790292957,Sp48:0.107663131698):0.575955539077,Sp49:0.683618670775):0.35904620643,Sp50:1.0426648772):1.68034027269):0.66876770429):0.0;");
        string n4("(((((Sp1:0.237938301594,Sp2:0.237938301594):0.652150465533,(Sp4:1.19159193346,(Sp10:0.775389105376,(Sp7:0.385992882857,((Sp5:0.0499647780388,Sp6:0.0499647780388):0.625094094731,(Sp8:0.0229771776543,Sp9:0.0229771776543):0.363015705203):0.289065989913):0.100330232606):0.416202828082):0.70177350217):1.0032766685,Sp3:0.890088767127):1.37956852633,(((Sp11:0.143770218098,Sp12:0.143770218098):2.22399819333,(Sp13:0.879389128685,Sp14:0.879389128685):1.48837928275):0.882615734883,(((Sp34:0.453020826526,Sp35:0.453020826526):1.00864842727,Sp36:1.4616692538):1.26653016217,((Sp32:1.88686786295,Sp33:1.88686786295):0.0585531361112,((((Sp21:0.0928829593343,Sp22:0.0928829593343):0.596056952564,(Sp23:0.295394029748,Sp24:0.295394029748):0.39354588215):0.777971317013,((Sp15:0.115537491571,Sp16:0.115537491571):1.14083013878,((Sp20:0.757218654126,Sp17:0.787001759928):0.029783105802,(Sp18:0.109471903715,Sp19:0.109471903715):0.647746750411):0.469365870425):0.210543598558):0.273542049999,((Sp30:0.423829830969,Sp31:0.423829830969):1.1204441523,(Sp29:0.975450737746,((Sp26:0.147146981319,Sp27:0.147146981319):0.197746581084,(Sp25:0.750938536668,Sp28:0.344893562404):0.406044974265):0.224512201078):0.568823245523):0.196179295641):0.204967720146):0.782778416912):0.522184730348):0.0225498156455):0.118838892224,((Sp37:1.61330136153,((Sp38:1.11318444903,Sp39:1.11318444903):0.488085401285,(Sp40:1.42576644618,(((Sp41:0.0435502147095,Sp42:0.0435502147095):0.233013603141,Sp43:0.276563817851):0.175951351205,(Sp44:0.0424687878211,Sp45:0.0424687878211):0.410046381235):0.973251277121):0.175503404136):0.0120315112168):1.10970378837,((((Sp46:0.0108728387409,Sp47:0.0108728387409):0.096790292957,Sp48:0.107663131698):0.575955539077,Sp49:0.683618670775):0.35904620643,Sp50:1.0426648772):1.68034027269):0.66876770429):0.0;");
        string n5("(((Sp3:0.772674,Sp2:0.720991):0.629328,Sp1:0.24369):0.527364,(Sp4:0.064618,((Sp9:0.717141,Sp8:0.317641):0.504805,Sp10:0.784839):0.314846):0.0,(Sp6:0.884339,(Sp7:0.65591,Sp5:0.503117):0.276719):0.387619):0.0;");
        string n6("(((Sp4:1.13502051663020653294,((((Sp3:0.88169496183720930116,Sp2:0.99801256574751662232):0.52272444809873486182,Sp1:1.41505301691237672657):1.38235476745587315683,(Sp9:0.00000093904650884574,Sp8:0.00000093904650884574):0.35508510812969668047):0.51820259793242084978,Sp10:0.35696307523092679626):0.15172977041371080165):0.55567471244314503576,Sp6:0.44744303933382639782):0.15040231785032892597,Sp7:0.68773286572264702432,Sp5:0.53111502610885585884):0.0;");

        auto t1 = PhyloTree(n1, true);
        auto t2 = PhyloTree(n2, true);
        auto t3 = PhyloTree(n3, true);
        auto t4 = PhyloTree(n4, true);
        auto t5 = PhyloTree(n5, false);
        auto t6 = PhyloTree(n6, false);

        CHECK(abs(Distance::getRobinsonFouldsDistance(t1, t2, false) - 8) < TOLERANCE);
        CHECK(abs(Distance::getRobinsonFouldsDistance(t1, t2, true) - 1) < TOLERANCE);
        CHECK(abs(Distance::getRobinsonFouldsDistance(t3, t4, false) - 8) < TOLERANCE);
        CHECK(abs(Distance::getRobinsonFouldsDistance(t3, t4, true) - 0.08333333333333333) < TOLERANCE);
        CHECK(abs(Distance::getRobinsonFouldsDistance(t5, t6, false) - 4) < TOLERANCE);

    }

    SECTION("Weighted Robinson-Foulds") {
        string n1("((a:3,b:4):.1,(c:5,((d:6,e:7):.2,f:8):.3):.4);");
        string n2("((a:3,c:4):.5,(d:5,((b:6,e:7):.2,f:8):.3):.4);");
        string n3("(((((Sp1:0.237938301594,Sp2:0.237938301594):0.652150465533,Sp3:0.890088767127):1.0032766685,(Sp4:1.19159193346,(((Sp5:0.0499647780388,Sp6:0.0499647780388):0.625094094731,(Sp7:0.385992882857,(Sp8:0.0229771776543,Sp9:0.0229771776543):0.363015705203):0.289065989913):0.100330232606,Sp10:0.775389105376):0.416202828082):0.70177350217):1.37956852633,(((Sp11:0.143770218098,Sp12:0.143770218098):2.22399819333,(Sp13:0.879389128685,Sp14:0.879389128685):1.48837928275):0.882615734883,((((((Sp15:0.115537491571,Sp16:0.115537491571):1.14083013878,(Sp17:0.787001759928,((Sp18:0.109471903715,Sp19:0.109471903715):0.647746750411,Sp20:0.757218654126):0.029783105802):0.469365870425):0.210543598558,((Sp21:0.0928829593343,Sp22:0.0928829593343):0.596056952564,(Sp23:0.295394029748,Sp24:0.295394029748):0.39354588215):0.777971317013):0.273542049999,(((Sp25:0.750938536668,((Sp26:0.147146981319,Sp27:0.147146981319):0.197746581084,Sp28:0.344893562404):0.406044974265):0.224512201078,Sp29:0.975450737746):0.568823245523,(Sp30:0.423829830969,Sp31:0.423829830969):1.1204441523):0.196179295641):0.204967720146,(Sp32:1.88686786295,Sp33:1.88686786295):0.0585531361112):0.782778416912,((Sp34:0.453020826526,Sp35:0.453020826526):1.00864842727,Sp36:1.4616692538):1.26653016217):0.522184730348):0.0225498156455):0.118838892224,((Sp37:1.61330136153,((Sp38:1.11318444903,Sp39:1.11318444903):0.488085401285,(Sp40:1.42576644618,(((Sp41:0.0435502147095,Sp42:0.0435502147095):0.233013603141,Sp43:0.276563817851):0.175951351205,(Sp44:0.0424687878211,Sp45:0.0424687878211):0.410046381235):0.973251277121):0.175503404136):0.0120315112168):1.10970378837,((((Sp46:0.0108728387409,Sp47:0.0108728387409):0.096790292957,Sp48:0.107663131698):0.575955539077,Sp49:0.683618670775):0.35904620643,Sp50:1.0426648772):1.68034027269):0.66876770429):0.0;");
        string n4("(((((Sp1:0.237938301594,Sp2:0.237938301594):0.652150465533,(Sp4:1.19159193346,(Sp10:0.775389105376,(Sp7:0.385992882857,((Sp5:0.0499647780388,Sp6:0.0499647780388):0.625094094731,(Sp8:0.0229771776543,Sp9:0.0229771776543):0.363015705203):0.289065989913):0.100330232606):0.416202828082):0.70177350217):1.0032766685,Sp3:0.890088767127):1.37956852633,(((Sp11:0.143770218098,Sp12:0.143770218098):2.22399819333,(Sp13:0.879389128685,Sp14:0.879389128685):1.48837928275):0.882615734883,(((Sp34:0.453020826526,Sp35:0.453020826526):1.00864842727,Sp36:1.4616692538):1.26653016217,((Sp32:1.88686786295,Sp33:1.88686786295):0.0585531361112,((((Sp21:0.0928829593343,Sp22:0.0928829593343):0.596056952564,(Sp23:0.295394029748,Sp24:0.295394029748):0.39354588215):0.777971317013,((Sp15:0.115537491571,Sp16:0.115537491571):1.14083013878,((Sp20:0.757218654126,Sp17:0.787001759928):0.029783105802,(Sp18:0.109471903715,Sp19:0.109471903715):0.647746750411):0.469365870425):0.210543598558):0.273542049999,((Sp30:0.423829830969,Sp31:0.423829830969):1.1204441523,(Sp29:0.975450737746,((Sp26:0.147146981319,Sp27:0.147146981319):0.197746581084,(Sp25:0.750938536668,Sp28:0.344893562404):0.406044974265):0.224512201078):0.568823245523):0.196179295641):0.204967720146):0.782778416912):0.522184730348):0.0225498156455):0.118838892224,((Sp37:1.61330136153,((Sp38:1.11318444903,Sp39:1.11318444903):0.488085401285,(Sp40:1.42576644618,(((Sp41:0.0435502147095,Sp42:0.0435502147095):0.233013603141,Sp43:0.276563817851):0.175951351205,(Sp44:0.0424687878211,Sp45:0.0424687878211):0.410046381235):0.973251277121):0.175503404136):0.0120315112168):1.10970378837,((((Sp46:0.0108728387409,Sp47:0.0108728387409):0.096790292957,Sp48:0.107663131698):0.575955539077,Sp49:0.683618670775):0.35904620643,Sp50:1.0426648772):1.68034027269):0.66876770429):0.0;");
        string n5("(((Sp3:0.772674,Sp2:0.720991):0.629328,Sp1:0.24369):0.527364,(Sp4:0.064618,((Sp9:0.717141,Sp8:0.317641):0.504805,Sp10:0.784839):0.314846):0.0,(Sp6:0.884339,(Sp7:0.65591,Sp5:0.503117):0.276719):0.387619):0.0;");
        string n6("(((Sp4:1.13502051663020653294,((((Sp3:0.88169496183720930116,Sp2:0.99801256574751662232):0.52272444809873486182,Sp1:1.41505301691237672657):1.38235476745587315683,(Sp9:0.00000093904650884574,Sp8:0.00000093904650884574):0.35508510812969668047):0.51820259793242084978,Sp10:0.35696307523092679626):0.15172977041371080165):0.55567471244314503576,Sp6:0.44744303933382639782):0.15040231785032892597,Sp7:0.68773286572264702432,Sp5:0.53111502610885585884):0.0;");

        auto t1 = PhyloTree(n1, true);
        auto t2 = PhyloTree(n2, true);
        auto t3 = PhyloTree(n3, true);
        auto t4 = PhyloTree(n4, true);
        auto t5 = PhyloTree(n5, false);
        auto t6 = PhyloTree(n6, false);
        CHECK(abs(Distance::getWeightedRobinsonFouldsDistance(t1, t2, false) - 6.4) < TOLERANCE);
        CHECK(abs(Distance::getWeightedRobinsonFouldsDistance(t1, t2, true) - 0.0935672514619883) < TOLERANCE);
        CHECK(abs(Distance::getWeightedRobinsonFouldsDistance(t3, t4, false) - 3.4563414769600005) < TOLERANCE);
        CHECK(abs(Distance::getWeightedRobinsonFouldsDistance(t3, t4, true) - 0.032045595869846046) < TOLERANCE);
        CHECK(abs(Distance::getWeightedRobinsonFouldsDistance(t5, t6, false) - 6.9776459344674295) < TOLERANCE);
    }

    SECTION("Euclidean") {
        string n1("((a:3,b:4):.1,(c:5,((d:6,e:7):.2,f:8):.3):.4);");
        string n2("((a:3,c:4):.5,(d:5,((b:6,e:7):.2,f:8):.3):.4);");
        string n3("(((((Sp1:0.237938301594,Sp2:0.237938301594):0.652150465533,Sp3:0.890088767127):1.0032766685,(Sp4:1.19159193346,(((Sp5:0.0499647780388,Sp6:0.0499647780388):0.625094094731,(Sp7:0.385992882857,(Sp8:0.0229771776543,Sp9:0.0229771776543):0.363015705203):0.289065989913):0.100330232606,Sp10:0.775389105376):0.416202828082):0.70177350217):1.37956852633,(((Sp11:0.143770218098,Sp12:0.143770218098):2.22399819333,(Sp13:0.879389128685,Sp14:0.879389128685):1.48837928275):0.882615734883,((((((Sp15:0.115537491571,Sp16:0.115537491571):1.14083013878,(Sp17:0.787001759928,((Sp18:0.109471903715,Sp19:0.109471903715):0.647746750411,Sp20:0.757218654126):0.029783105802):0.469365870425):0.210543598558,((Sp21:0.0928829593343,Sp22:0.0928829593343):0.596056952564,(Sp23:0.295394029748,Sp24:0.295394029748):0.39354588215):0.777971317013):0.273542049999,(((Sp25:0.750938536668,((Sp26:0.147146981319,Sp27:0.147146981319):0.197746581084,Sp28:0.344893562404):0.406044974265):0.224512201078,Sp29:0.975450737746):0.568823245523,(Sp30:0.423829830969,Sp31:0.423829830969):1.1204441523):0.196179295641):0.204967720146,(Sp32:1.88686786295,Sp33:1.88686786295):0.0585531361112):0.782778416912,((Sp34:0.453020826526,Sp35:0.453020826526):1.00864842727,Sp36:1.4616692538):1.26653016217):0.522184730348):0.0225498156455):0.118838892224,((Sp37:1.61330136153,((Sp38:1.11318444903,Sp39:1.11318444903):0.488085401285,(Sp40:1.42576644618,(((Sp41:0.0435502147095,Sp42:0.0435502147095):0.233013603141,Sp43:0.276563817851):0.175951351205,(Sp44:0.0424687878211,Sp45:0.0424687878211):0.410046381235):0.973251277121):0.175503404136):0.0120315112168):1.10970378837,((((Sp46:0.0108728387409,Sp47:0.0108728387409):0.096790292957,Sp48:0.107663131698):0.575955539077,Sp49:0.683618670775):0.35904620643,Sp50:1.0426648772):1.68034027269):0.66876770429):0.0;");
        string n4("(((((Sp1:0.237938301594,Sp2:0.237938301594):0.652150465533,(Sp4:1.19159193346,(Sp10:0.775389105376,(Sp7:0.385992882857,((Sp5:0.0499647780388,Sp6:0.0499647780388):0.625094094731,(Sp8:0.0229771776543,Sp9:0.0229771776543):0.363015705203):0.289065989913):0.100330232606):0.416202828082):0.70177350217):1.0032766685,Sp3:0.890088767127):1.37956852633,(((Sp11:0.143770218098,Sp12:0.143770218098):2.22399819333,(Sp13:0.879389128685,Sp14:0.879389128685):1.48837928275):0.882615734883,(((Sp34:0.453020826526,Sp35:0.453020826526):1.00864842727,Sp36:1.4616692538):1.26653016217,((Sp32:1.88686786295,Sp33:1.88686786295):0.0585531361112,((((Sp21:0.0928829593343,Sp22:0.0928829593343):0.596056952564,(Sp23:0.295394029748,Sp24:0.295394029748):0.39354588215):0.777971317013,((Sp15:0.115537491571,Sp16:0.115537491571):1.14083013878,((Sp20:0.757218654126,Sp17:0.787001759928):0.029783105802,(Sp18:0.109471903715,Sp19:0.109471903715):0.647746750411):0.469365870425):0.210543598558):0.273542049999,((Sp30:0.423829830969,Sp31:0.423829830969):1.1204441523,(Sp29:0.975450737746,((Sp26:0.147146981319,Sp27:0.147146981319):0.197746581084,(Sp25:0.750938536668,Sp28:0.344893562404):0.406044974265):0.224512201078):0.568823245523):0.196179295641):0.204967720146):0.782778416912):0.522184730348):0.0225498156455):0.118838892224,((Sp37:1.61330136153,((Sp38:1.11318444903,Sp39:1.11318444903):0.488085401285,(Sp40:1.42576644618,(((Sp41:0.0435502147095,Sp42:0.0435502147095):0.233013603141,Sp43:0.276563817851):0.175951351205,(Sp44:0.0424687878211,Sp45:0.0424687878211):0.410046381235):0.973251277121):0.175503404136):0.0120315112168):1.10970378837,((((Sp46:0.0108728387409,Sp47:0.0108728387409):0.096790292957,Sp48:0.107663131698):0.575955539077,Sp49:0.683618670775):0.35904620643,Sp50:1.0426648772):1.68034027269):0.66876770429):0.0;");

        auto t1 = PhyloTree(n1, true);
        auto t2 = PhyloTree(n2, true);
        auto t3 = PhyloTree(n3, true);
        auto t4 = PhyloTree(n4, true);
        CHECK(abs(Distance::getEuclideanDistance(t1, t2, false) - 2.615339366124404) < TOLERANCE);
        CHECK(abs(Distance::getEuclideanDistance(t1, t2, true) - 0.09260058256224009) < TOLERANCE);
        CHECK(abs(Distance::getEuclideanDistance(t3, t4, false) - 1.5848550562104533) < TOLERANCE);
        CHECK(abs(Distance::getEuclideanDistance(t3, t4, true) - 0.10764815276134682) < TOLERANCE);
    }

    SECTION("Geodesic") {
        string s1("((a:3,b:4):.1,(c:5,((d:6,e:7):.2,f:8):.3):.4);");
        string s2("((a:3,c:4):.5,(d:5,((b:6,e:7):.2,f:8):.3):.4);");
        string s3("(g:1,(a:1,(b:1,c:1):1):1,(f:1,(e:1,d:1):1):1);");
        string s4("(g:2,(a:2,(b:2,c:2):2):2,(d:2,(e:2,f:2):2):2);");
        string s5("((g:1,(a:1,(b:1,c:1):1):1):1,(f:1,(e:1,d:1):1):1);");
        string s6("((g:2,(a:2,(b:2,c:2):2):2):2,(d:2,(e:2,f:2):2):2);");
        string s7("(((((Sp1:0.237938301594,Sp2:0.237938301594):0.652150465533,Sp3:0.890088767127):1.0032766685,(Sp4:1.19159193346,(((Sp5:0.0499647780388,Sp6:0.0499647780388):0.625094094731,(Sp7:0.385992882857,(Sp8:0.0229771776543,Sp9:0.0229771776543):0.363015705203):0.289065989913):0.100330232606,Sp10:0.775389105376):0.416202828082):0.70177350217):1.37956852633,(((Sp11:0.143770218098,Sp12:0.143770218098):2.22399819333,(Sp13:0.879389128685,Sp14:0.879389128685):1.48837928275):0.882615734883,((((((Sp15:0.115537491571,Sp16:0.115537491571):1.14083013878,(Sp17:0.787001759928,((Sp18:0.109471903715,Sp19:0.109471903715):0.647746750411,Sp20:0.757218654126):0.029783105802):0.469365870425):0.210543598558,((Sp21:0.0928829593343,Sp22:0.0928829593343):0.596056952564,(Sp23:0.295394029748,Sp24:0.295394029748):0.39354588215):0.777971317013):0.273542049999,(((Sp25:0.750938536668,((Sp26:0.147146981319,Sp27:0.147146981319):0.197746581084,Sp28:0.344893562404):0.406044974265):0.224512201078,Sp29:0.975450737746):0.568823245523,(Sp30:0.423829830969,Sp31:0.423829830969):1.1204441523):0.196179295641):0.204967720146,(Sp32:1.88686786295,Sp33:1.88686786295):0.0585531361112):0.782778416912,((Sp34:0.453020826526,Sp35:0.453020826526):1.00864842727,Sp36:1.4616692538):1.26653016217):0.522184730348):0.0225498156455):0.118838892224,((Sp37:1.61330136153,((Sp38:1.11318444903,Sp39:1.11318444903):0.488085401285,(Sp40:1.42576644618,(((Sp41:0.0435502147095,Sp42:0.0435502147095):0.233013603141,Sp43:0.276563817851):0.175951351205,(Sp44:0.0424687878211,Sp45:0.0424687878211):0.410046381235):0.973251277121):0.175503404136):0.0120315112168):1.10970378837,((((Sp46:0.0108728387409,Sp47:0.0108728387409):0.096790292957,Sp48:0.107663131698):0.575955539077,Sp49:0.683618670775):0.35904620643,Sp50:1.0426648772):1.68034027269):0.66876770429):0.0;");
        string s8("(((((Sp1:0.237938301594,Sp2:0.237938301594):0.652150465533,(Sp4:1.19159193346,(Sp10:0.775389105376,(Sp7:0.385992882857,((Sp5:0.0499647780388,Sp6:0.0499647780388):0.625094094731,(Sp8:0.0229771776543,Sp9:0.0229771776543):0.363015705203):0.289065989913):0.100330232606):0.416202828082):0.70177350217):1.0032766685,Sp3:0.890088767127):1.37956852633,(((Sp11:0.143770218098,Sp12:0.143770218098):2.22399819333,(Sp13:0.879389128685,Sp14:0.879389128685):1.48837928275):0.882615734883,(((Sp34:0.453020826526,Sp35:0.453020826526):1.00864842727,Sp36:1.4616692538):1.26653016217,((Sp32:1.88686786295,Sp33:1.88686786295):0.0585531361112,((((Sp21:0.0928829593343,Sp22:0.0928829593343):0.596056952564,(Sp23:0.295394029748,Sp24:0.295394029748):0.39354588215):0.777971317013,((Sp15:0.115537491571,Sp16:0.115537491571):1.14083013878,((Sp20:0.757218654126,Sp17:0.787001759928):0.029783105802,(Sp18:0.109471903715,Sp19:0.109471903715):0.647746750411):0.469365870425):0.210543598558):0.273542049999,((Sp30:0.423829830969,Sp31:0.423829830969):1.1204441523,(Sp29:0.975450737746,((Sp26:0.147146981319,Sp27:0.147146981319):0.197746581084,(Sp25:0.750938536668,Sp28:0.344893562404):0.406044974265):0.224512201078):0.568823245523):0.196179295641):0.204967720146):0.782778416912):0.522184730348):0.0225498156455):0.118838892224,((Sp37:1.61330136153,((Sp38:1.11318444903,Sp39:1.11318444903):0.488085401285,(Sp40:1.42576644618,(((Sp41:0.0435502147095,Sp42:0.0435502147095):0.233013603141,Sp43:0.276563817851):0.175951351205,(Sp44:0.0424687878211,Sp45:0.0424687878211):0.410046381235):0.973251277121):0.175503404136):0.0120315112168):1.10970378837,((((Sp46:0.0108728387409,Sp47:0.0108728387409):0.096790292957,Sp48:0.107663131698):0.575955539077,Sp49:0.683618670775):0.35904620643,Sp50:1.0426648772):1.68034027269):0.66876770429):0.0;");
        string s9("(((((Sp1:1.27913501775,Sp2:1.27913501775):0.503519437631,(((((Sp3:0.174706128894,Sp4:0.174706128894):0.278800970217,Sp5:0.453507099111):0.127576167435,Sp6:0.581083266546):0.386999844658,(((Sp7:0.326360375774,(Sp8:0.0687696356441,Sp9:0.0687696356441):0.25759074013):0.0959845651045,Sp10:0.422344940879):0.221924386113,(Sp11:0.163484266197,Sp12:0.163484266197):0.480785060795):0.323813784212):0.480748594377,(Sp13:0.0510925892695,(Sp14:0.0189164946479,Sp15:0.0189164946479):0.0321760946216):1.39773911631):0.333822749805):2.04546638535,(((((Sp16:0.978372114238,(Sp17:0.63042759148,Sp18:0.63042759148):0.347944522758):0.511938566725,Sp19:1.49031068096):0.301021081869,(Sp20:0.842620988974,Sp21:0.842620988974):0.948710773858):1.59221652851,(((((Sp22:1.09930721834,(((Sp23:0.199396014183,Sp24:0.199396014183):0.417527575814,Sp25:0.616923589997):0.113215896736,(Sp26:0.549418597187,Sp27:0.549418597187):0.180720889546):0.36916773161):0.560230687455,((Sp28:1.15743938659,(Sp29:1.08783477542,Sp30:1.08783477542):0.0696046111671):0.0589976010252,(((Sp31:0.698057408415,(((((Sp32:0.0566740344693,Sp33:0.0566740344693):0.0129389126625,Sp34:0.0696129471317):0.0705659261503,Sp35:0.140178873282):0.0557916607637,Sp36:0.195970534046):0.0421968479637,Sp37:0.238167382009):0.459890026406):0.0365566616792,Sp38:0.734614070094):0.152504586853,Sp39:0.887118656947):0.329318330665):0.443100918186):0.432218957209,(Sp40:0.758611681751,(Sp41:0.565122074383,Sp42:0.565122074383):0.193489607368):1.33314518126):0.842167587485,(((Sp43:1.38348455055,(((Sp44:0.907793337814,(Sp45:0.3994934584,(Sp46:0.0532441043793,Sp47:0.0532441043793):0.346249354021):0.508299879414):0.167552053274,((Sp48:0.117750473553,Sp49:0.117750473553):0.641514308353,(Sp50:0.296361468881,Sp51:0.296361468881):0.462903313025):0.316080609181):0.0820858564681,(Sp52:0.359216626145,Sp53:0.359216626145):0.798214621411):0.226053302992):0.126930948011,Sp54:1.51041549856):1.09531917588,(((Sp55:0.238069584341,Sp56:0.238069584341):0.775674872089,((Sp57:0.164150310957,Sp58:0.164150310957):0.192020430855,Sp59:0.356170741812):0.657573714618):0.107510009587,((Sp60:0.0535101772097,Sp61:0.0535101772097):0.5268732017,Sp62:0.58038337891):0.540871087106):1.48448020842):0.328189776051):0.439960964476,(((Sp63:0.41694933649,Sp64:0.41694933649):2.01515845131,((((Sp65:0.421772541702,((Sp66:0.0818380748323,Sp67:0.0818380748323):0.174170545333,Sp68:0.256008620165):0.165763921537):1.12820274871,((Sp69:0.353985084324,(Sp70:0.0481188529638,Sp71:0.0481188529638):0.30586623136):0.0974830229145,Sp72:0.451468107238):1.09850718318):0.170789563213,(Sp73:0.992767955167,((Sp74:0.239444608479,Sp75:0.239444608479):0.0541533437269,(Sp76:0.0445966871034,Sp77:0.0445966871034):0.249001265102):0.699170002961):0.727996898461):0.175657129874,(((Sp78:0.974433492321,(Sp79:0.102350375018,Sp80:0.102350375018):0.872083117303):0.665885328411,(Sp81:1.49175923285,Sp82:1.49175923285):0.148559587883):0.0943007476136,(Sp83:0.333084685621,Sp84:0.333084685621):1.40153488272):0.161802415157):0.5356858043):0.291431308105,((Sp85:0.169032398939,Sp86:0.169032398939):2.46685093527,(((Sp87:0.465671983323,Sp88:0.465671983323):0.850127256512,(Sp89:0.57902198436,((((Sp90:0.0262818778227,Sp91:0.0262818778227):0.209738754281,Sp92:0.236020632104):0.0269033482636,Sp93:0.262923980368):0.0369359780471,Sp94:0.299859958415):0.279162025945):0.736777255475):0.811845404108,Sp95:2.12764464394):0.508238690267):0.0876557616971):0.65034631906):0.00966287637107):0.335602981855,(((Sp96:0.225350675979,Sp97:0.225350675979):0.259776840063,Sp98:0.485127516041):1.10771026192,Sp99:1.59283777796):2.12631349523):0.108969567543):0.0269800204399,((((((Sp100:0.34344008276,Sp101:0.34344008276):1.89222900415,((Sp102:0.66351462302,(Sp103:0.189363881069,Sp104:0.189363881069):0.474150741951):0.861558660228,(Sp105:0.964480877406,((Sp106:0.142822137458,(Sp107:0.10933546825,Sp108:0.10933546825):0.0334866692084):0.0799346751851,(Sp109:0.14672597146,Sp110:0.14672597146):0.076030841183):0.741724064762):0.560592405842):0.710595803662):0.0903246607965,(Sp111:2.10767581502,(((Sp112:0.0403357241915,Sp113:0.0403357241915):0.763525649747,Sp114:0.803861373939):0.252709994825,(Sp115:0.071618285436,Sp116:0.071618285436):0.984953083328):1.05110444625):0.218317932691):0.32915750147,(((((Sp117:0.259448014129,Sp118:0.259448014129):0.265122761895,Sp119:0.524570776024):0.651445835968,Sp120:1.17601661199):0.657587484071,((Sp121:1.14867611138,((Sp122:0.482856289615,Sp123:0.482856289615):0.162279078864,((Sp124:0.106692974306,(Sp125:0.00944967989553,Sp126:0.00944967989553):0.0972432944107):0.215832172067,(Sp127:0.314179330997,Sp128:0.314179330997):0.00834581537632):0.322610222106):0.503540742897):0.437145577357,(Sp129:0.0156109890856,Sp130:0.0156109890856):1.57021069965):0.247782407329):0.0460724755596,((Sp131:0.334703310869,Sp132:0.334703310869):0.686652279118,Sp133:1.02135558999):0.858320981635):0.775474677555):0.470012290737,(((((Sp134:0.875541614643,((Sp135:0.132420294593,(Sp136:0.0809918962359,Sp137:0.0809918962359):0.0514283983568):0.252340736537,Sp138:0.38476103113):0.490780583513):0.146367464988,Sp139:1.02190907963):0.614888088207,((Sp140:0.245027971969,Sp141:0.245027971969):0.377756872911,(Sp142:0.283700827557,Sp143:0.283700827557):0.339084017323):1.01401232296):0.842150817584,(Sp144:0.639261403016,Sp145:0.639261403016):1.83968658241):0.106704423767,Sp146:2.58565240919):0.539511130725):0.3830311766,(((Sp147:2.70810435364,((Sp148:0.789376932649,((Sp149:0.0947595009639,Sp150:0.0947595009639):0.0239810982641,Sp151:0.118740599228):0.670636333421):0.572783643036,Sp152:1.36216057568):1.34594377795):0.0823751992798,(((Sp153:1.95153530956,((Sp154:0.422813488863,Sp155:0.422813488863):0.481967979215,(Sp156:0.494474699927,Sp157:0.494474699927):0.410306768151):1.04675384148):0.550784953018,((Sp158:0.714729971934,Sp159:0.714729971934):1.04654908186,Sp160:1.7612790538):0.741041208778):0.00645629105752,(((Sp161:1.40148168493,Sp162:1.40148168493):0.308326557034,Sp163:1.70980824197):0.148093825391,(((Sp164:0.191458210359,Sp165:0.191458210359):0.510549615609,(Sp166:0.193506022965,Sp167:0.193506022965):0.508501803003):0.750385556799,Sp168:1.45239338277):0.405508684589):0.650874486277):0.281702999282):0.506069520636,Sp169:3.29654907355):0.211645642962):0.346906144662):0.473615447119,(((((Sp170:1.46610997688,Sp171:1.46610997688):0.123201112472,Sp172:1.58931108935):0.993298508052,((Sp173:0.710855310766,Sp174:0.710855310766):0.467725391794,((Sp175:0.431625041839,Sp176:0.431625041839):0.731571957278,((Sp177:0.327844498402,Sp178:0.327844498402):0.538475292787,(Sp179:0.291142082579,Sp180:0.291142082579):0.57517770861):0.296877207928):0.0153837034442):1.40402889484):0.644018538001,((Sp181:1.01749443377,Sp182:1.01749443377):0.471874142901,((Sp183:1.2712792487,(Sp184:1.05386924653,((Sp185:0.31114863844,(Sp186:0.123554697721,Sp187:0.123554697721):0.187593940719):0.494654204946,((Sp188:0.0806215599401,Sp189:0.0806215599401):0.213853865445,Sp190:0.294475425385):0.511327418001):0.248066403145):0.217410002169):0.162471425245,(Sp191:0.78153800227,(Sp192:0.642641958406,Sp193:0.642641958406):0.138896043864):0.652212671675):0.0556179027307):1.73725955873):0.419805697964,((Sp194:0.464490259405,Sp195:0.464490259405):1.80798975463,((Sp196:1.35040721455,Sp197:1.35040721455):0.871830987079,((Sp198:0.567797422118,Sp199:0.567797422118):0.00140042348511,Sp200:0.569197845603):1.65304035603):0.0502418124):1.37395381934):0.682282474926):0.0;");
        string s10("(((((Sp1:0.26477902124,Sp2:0.26477902124):1.77279070711,((Sp3:1.23917055472,((Sp4:0.21375223815,Sp5:0.21375223815):1.02234928776,((Sp6:0.762864032946,((Sp7:0.122313116293,Sp8:0.122313116293):0.086701184473,Sp9:0.209014300766):0.55384973218):0.38379751507,Sp10:1.14666154802):0.0894399778973):0.00306902880917):0.620007724838,((((Sp11:0.144771368307,Sp12:0.144771368307):0.298744211973,Sp13:0.44351558028):0.456798040473,Sp14:0.900313620753):0.133963561025,((Sp15:0.781182031223,(Sp16:0.385189606748,Sp17:0.385189606748):0.395992424475):0.195261093576,Sp18:0.976443124799):0.0578340569798):0.824901097782):0.17839144879):0.534924561244,(Sp19:1.59530033696,(Sp20:1.45482777331,((Sp21:0.961142236058,(Sp22:0.592515374022,Sp23:0.592515374022):0.368626862036):0.265186706811,Sp24:1.22632894287):0.228498830443):0.140472563645):0.977193952637):1.72117152818,((((((Sp25:0.613349942764,Sp26:0.613349942764):0.605291012977,Sp27:1.21864095574):0.487267938031,((Sp28:0.431481068174,Sp29:0.431481068174):1.24668833373,(Sp30:1.03411526602,(Sp31:0.449429276811,Sp32:0.449429276811):0.584685989208):0.644054135884):0.0277394918695):0.158636011276,((Sp33:1.31647343952,(Sp34:0.174193501345,Sp35:0.174193501345):1.14227993818):0.0534707968825,(Sp36:0.586537125193,Sp37:0.586537125193):0.783407111212):0.494600668644):0.638079404114,((Sp38:0.634091411145,(Sp39:0.556628277001,(Sp40:0.330825628466,Sp41:0.330825628466):0.225802648535):0.0774631341447):1.18515562432,(Sp42:0.175983921701,Sp43:0.175983921701):1.64326311377):0.683377273695):0.946329360539,(((Sp44:0.390392424727,Sp45:0.390392424727):0.217606210801,(Sp46:0.420350512287,Sp47:0.420350512287):0.187648123241):2.42964575148,((((Sp48:0.921271251347,(Sp49:0.516444465139,(Sp50:0.167553806207,Sp51:0.167553806207):0.348890658932):0.404826786208):0.0501988254069,(Sp52:0.229851032234,Sp53:0.229851032234):0.74161904452):1.24996134417,(Sp54:1.33056466833,((((Sp55:0.162333066375,Sp56:0.162333066375):0.30365400463,Sp57:0.465987071006):0.118158163048,Sp58:0.584145234054):0.725072346905,((((((Sp59:0.0677894626526,Sp60:0.0677894626526):0.204508463407,Sp61:0.27229792606):0.283465367974,Sp62:0.555763294034):0.519289268771,((Sp63:1.06909904236,Sp64:1.06909904236):0.00333708426913,Sp65:1.07243612663):0.00261643617739):0.158166199064,((Sp66:0.539865118359,Sp67:0.539865118359):0.217821870368,(((Sp68:0.1495710333,Sp69:0.1495710333):0.417429952731,Sp70:0.567000986031):0.0346304067253,(Sp71:0.324187795925,Sp72:0.324187795925):0.277443596831):0.15605559597):0.475531773143):0.0755372033497,Sp73:1.30875596522):0.000461615739492):0.0213470873667):0.890866752602):0.276283021461,((Sp74:1.72304416302,(Sp75:1.00766497007,Sp76:1.00766497007):0.715379192948):0.657369708915,((Sp77:0.226483737526,(Sp78:0.222025230038,Sp79:0.222025230038):0.00445850748802):1.99762164943,(Sp80:1.3539136427,(((((Sp81:0.0360862474604,(Sp82:0.0285329623022,Sp83:0.0285329623022):0.00755328515819):0.108982741287,Sp84:0.145068988747):0.529544781394,((Sp85:0.123063951608,Sp86:0.123063951608):0.0480443351745,Sp87:0.171108286782):0.503505483359):0.135512977032,(Sp88:0.693468027211,(Sp89:0.0722130037359,Sp90:0.0722130037359):0.621255023475):0.116658719962):0.0808142301248,(Sp91:0.663849583459,((Sp92:0.136357414092,Sp93:0.136357414092):0.287841092669,Sp94:0.424198506761):0.239651076699):0.227091393838):0.4629726654):0.870191744257):0.156308484983):0.11730057045):0.539929944623):0.411309282691):0.844712148069):0.746902477536,(((((((Sp95:0.768588963519,Sp96:0.768588963519):0.693432527725,Sp97:1.46202149124):0.644953606036,(Sp98:1.2192447012,Sp99:1.2192447012):0.887730396076):0.586027299845,((Sp100:0.860208262772,Sp101:0.860208262772):0.454566361319,Sp102:1.31477462409):1.37822777303):1.33679129576,(((Sp103:1.54052949731,((Sp104:1.4232184509,((Sp105:0.104856684306,Sp106:0.104856684306):0.558789858734,(Sp107:0.197095761746,Sp108:0.197095761746):0.466550781294):0.759571907858):0.0488709517055,((Sp109:0.32566767691,Sp110:0.32566767691):0.710779269235,(Sp111:0.843838715301,(Sp112:0.508484574768,Sp113:0.508484574768):0.335354140532):0.192608230844):0.435642456459):0.0684400947101):0.693348903294,Sp114:2.23387840061):0.984372734492,((Sp115:0.0982081935444,Sp116:0.0982081935444):1.61711752687,Sp117:1.71532572042):1.50292541468):0.811542557784):0.164325213608,((Sp118:2.95232563601,(((Sp119:0.181433098258,Sp120:0.181433098258):0.298156410464,Sp121:0.479589508722):0.446387266232,((((Sp122:0.0535461491016,Sp123:0.0535461491016):0.00761276298214,Sp124:0.0611589120838):0.662820442775,Sp125:0.723979354859):0.0696595850072,(Sp126:0.374564853656,Sp127:0.374564853656):0.41907408621):0.132337835088):2.02634886105):0.0658966012466,((((Sp128:0.873869843706,Sp129:0.873869843706):0.0752003925332,(Sp130:0.921748961346,Sp131:0.921748961346):0.0273212748931):0.599657322515,(Sp132:0.703173332738,(Sp133:0.0588254554238,Sp134:0.0588254554238):0.644347877314):0.845554226016):1.31126944335,((((Sp135:0.420471971378,Sp136:0.420471971378):0.196314121932,Sp137:0.61678609331):1.71513369874,(((((Sp138:0.468310559234,Sp139:0.468310559234):0.468397866907,Sp140:0.936708426141):0.477062287576,(Sp141:0.543965780271,(Sp142:0.395973668305,Sp143:0.395973668305):0.147992111965):0.869804933446):0.0145160980512,(((Sp144:0.374535413269,(Sp145:0.137423439574,Sp146:0.137423439574):0.237111973695):0.651924752152,(Sp147:0.282745526277,Sp148:0.282745526277):0.743714639144):0.186765731427,((Sp149:0.436528304551,Sp150:0.436528304551):0.14913383144,Sp151:0.58566213599):0.627563760858):0.21506091492):0.737374437197,(Sp152:1.67785476486,Sp153:1.67785476486):0.487806484108):0.166258543085):0.453587219291,(((Sp154:0.624734005168,((Sp155:0.286202003825,(Sp156:0.253555415662,Sp157:0.253555415662):0.0326465881626):0.294398941412,(Sp158:0.441885260543,(Sp159:0.141601584771,Sp160:0.141601584771):0.300283675772):0.138715684694):0.0441330599313):0.120653752093,(Sp161:0.655205107847,(Sp162:0.261074111524,(Sp163:0.0867574623694,Sp164:0.0867574623694):0.174316649154):0.394130996323):0.0901826494141):1.38514301397,(Sp165:1.86219727514,Sp166:1.86219727514):0.268333496091):0.654976240109):0.0744899907606):0.158225235153):1.17589666924):0.171847379944,(((((Sp167:0.579914914715,Sp168:0.579914914715):0.454628207868,(Sp169:0.721946978125,(Sp170:0.169585313358,Sp171:0.169585313358):0.552361664767):0.312596144457):1.86593347148,((((Sp172:0.110915112535,Sp173:0.110915112535):0.0842578077741,Sp174:0.195172920309):0.187256692459,(Sp175:0.343377184606,Sp176:0.343377184606):0.0390524281611):1.24633313939,(((Sp177:0.109666443501,Sp178:0.109666443501):0.0810599119451,Sp179:0.190726355446):0.142226484624,Sp180:0.33295284007):1.29580991209):1.2717138419):0.255212827139,((Sp181:0.180477283067,Sp182:0.180477283067):1.67201747523,(Sp183:0.440452819612,Sp184:0.440452819612):1.41204193868):1.3031946629):0.109946382558,((Sp185:0.0366220815296,Sp186:0.0366220815296):2.09781806899,((((((Sp187:0.0346881095431,Sp188:0.0346881095431):0.0799759925947,(Sp189:0.0084839570952,Sp190:0.0084839570952):0.106180145043):0.0480432755536,(Sp191:0.0770294495392,Sp192:0.0770294495392):0.0856779281521):0.795838704082,Sp193:0.958546081774):0.0120100620426,((Sp194:0.317501361746,Sp195:0.317501361746):0.42778645548,(Sp196:0.13883735008,Sp197:0.13883735008):0.606450467146):0.22526832659):0.00302600093111,((Sp198:0.110854440051,Sp199:0.110854440051):0.254751775566,Sp200:0.365606215617):0.60797592913):1.16085800578):1.13119565323):1.10033048268):0.67460200887):0.0;");

        auto t1 = PhyloTree(s1, true);
        auto t2 = PhyloTree(s2, true);
        auto t3 = PhyloTree(s3, false);
        auto t4 = PhyloTree(s4, false);
        auto t5 = PhyloTree(s5, true);
        auto t6 = PhyloTree(s6, true);
        auto t7 = PhyloTree(s7, true);
        auto t8 = PhyloTree(s8, true);
        auto t9 = PhyloTree(s9, true);
        auto t10 = PhyloTree(s10, true);

        CHECK(abs(Distance::getGeodesicDistance(t1, t2, false) - 2.76188615828) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t1, t2, true) - 0.0977893234584) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t3, t4, false) - 4.35889894354) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t3, t4, true) - 0.438085827115) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t5, t6, false) - 4.472135955) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t5, t6, true) - 0.430331482912) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t7, t8, false) - 2.2413235148883968) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t7, t8, true) - 0.1522374775995074) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t9, t10, false) - 19.904540675743142) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t9, t10, true) - 0.6871102863171641) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t1, t2, false) - Distance::getGeodesicDistance(s1, s2, false, true, true)) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t1, t2, true) - Distance::getGeodesicDistance(s1, s2, true, true, true)) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t3, t4, false) - Distance::getGeodesicDistance(s3, s4, false, false, false)) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t3, t4, true) - Distance::getGeodesicDistance(s3, s4, true, false, false)) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t5, t6, false) - Distance::getGeodesicDistance(s5, s6, false, true, true)) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t5, t6, true) - Distance::getGeodesicDistance(s5, s6, true, true, true)) < TOLERANCE);
        clock_t start = clock();
        for (size_t i = 0; i < 100; ++i) {
            Distance::getGeodesicDistance(t9, t10, false);
//            Distance::getEuclideanDistance(t9, t10, false);
//            Distance::getRobinsonFouldsDistance(t9, t10, false);
//            Distance::getWeightedRobinsonFouldsDistance(t9, t10, false);
        }
        printf("Time taken: %.3f millisec\n", 1000 * (double) (clock() - start) / CLOCKS_PER_SEC);
        CHECK(abs(Distance::getGeodesicDistance(t7, t8, false) - 2.2413235148883968) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t7, t8, true) - 0.1522374775995074) < TOLERANCE);

    }
}
