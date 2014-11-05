#include "BipartiteGraph.h"
//#include "Bipartition.h"
//#include "EdgeAttribute.h"
//#include "PhyloTreeEdge.h"
#include "PhyloTree.h"
#include "Ratio.h"
#include "test_catch_helper.h"
#include "Distance.h"

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

TEST_CASE("EdgeAttribute") {
    SECTION("Construction") {
        auto a = EdgeAttribute();
        vector<double> v{1, 2, 3, 4};
        auto b = EdgeAttribute(0);
        auto c = EdgeAttribute(1);
        auto d = EdgeAttribute(0.5);
//        auto b = EdgeAttribute(v);
//        auto c = EdgeAttribute("5, 6, 7, 8");
//        auto d = EdgeAttribute(c);
        REQUIRE(a.toString() == "[]");
        REQUIRE(b.toString() == "[0]");
        REQUIRE(c.toString() == "[1]");
        REQUIRE(d.toString() == "[0.5]");
        REQUIRE(a.size() == 0);
        REQUIRE(b.size() == 1);
        REQUIRE(c.size() == 1);
//        REQUIRE(d.size() == c.size());
//        REQUIRE(c.equals(d));
    }

    SECTION("Arithmetic") {
//        auto a = EdgeAttribute("1,3,5,7");
//        auto b = EdgeAttribute("2,5,8,9");
//        REQUIRE(EdgeAttribute::difference(a, b).toString() == "[-1 -2 -3 -2]");
//        REQUIRE(EdgeAttribute::add(a, b).toString() == "[3 8 13 16]");
//        REQUIRE(EdgeAttribute::weightedPairAverage(a, b, 0.1).toString() == "[1.1 3.2 5.3 7.2]");
    }

    SECTION("Numerics") {
//        auto z = EdgeAttribute::zeroAttribute(10);
//        REQUIRE(z.toString() == "[0 0 0 0 0 0 0 0 0 0]");

//        auto a = EdgeAttribute("3, 4");
//        REQUIRE(a.toString() == "[3 4]");
//        REQUIRE(a.norm() == 5);
//        a.scaleBy(2);
//        REQUIRE(a.toString() == "[6 8]");
//        REQUIRE(a.norm() == 10);
//
//        auto b = EdgeAttribute("-6");
//        REQUIRE(b.toString() == "[-6]");
//        b.ensurePositive();
//        REQUIRE(b.toString() == "[6]");
    }

    {
//        auto a = EdgeAttribute("1,2,3");
//        auto b = EdgeAttribute("6,7,8,9,0");
//        REQUIRE_THROWS_AS(EdgeAttribute::add(a, b), std::invalid_argument);

    }
}

TEST_CASE("PhyloTreeEdge") {
    SECTION("Construction") {
        auto a = PhyloTreeEdge();
        REQUIRE(a.toString() == "[] ");
        REQUIRE(a.getOriginalID() == -1);

        auto bip = make_shared<Bipartition>("01001101");
        auto b = PhyloTreeEdge(bip->getPartition());
        REQUIRE(b.toString() == "[] 01001101");
        REQUIRE(b.getOriginalID() == -1);

        auto attrib = EdgeAttribute(0.5);
        auto c = PhyloTreeEdge(attrib);
        CHECK(c.toString() == attrib.toString() + " ");
        REQUIRE(c.getOriginalID() == -1);

        auto d = PhyloTreeEdge(attrib, 10);
        REQUIRE(d.toString() == attrib.toString() + " ");
        REQUIRE(d.getOriginalID() == 10);

        auto e = PhyloTreeEdge(attrib, bip, 11);
        REQUIRE(e.toString() == attrib.toString() + " " + "00000000");
        REQUIRE(e.getOriginalID() == 11);
        REQUIRE(e.isEmpty());

        auto f = PhyloTreeEdge(*bip, attrib, 12);
        REQUIRE(f.toString() == attrib.toString() + " " + bip->toString());
        REQUIRE(f.getOriginalID() == 12);

        auto edge = Bipartition("11011011");
        auto g = PhyloTreeEdge(edge.getPartition(), attrib, bip->getPartition(), 13);
        REQUIRE(g.toString() == attrib.toString() + " " + edge.toString());
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
        auto edges = vector<PhyloTreeEdge>({},{});
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
        vector<string> expected{"a","b","c","d","e","f","g"};
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
            CHECK(c_edges[i].getLength() == i+1);
            CHECK(d_edges[i] == d.getEdge(i));
            CHECK(d_edges[i].getLength() == i+2);
        }
    }

    SECTION("Edge Attributes") {
        double test;

        // unrooted
        string n1("(g:1,(a:1,(b:1,c:1):1):1,(f:1,(e:1,d:1):1):1);");
        string n2("(g:2,(a:2,(b:2,c:2):2):2,(d:2,(e:2,f:2):2):2);");
        auto a = PhyloTree(n1, false);
        auto b = PhyloTree(n2, false);
        auto a_attribs = a.getLeafEdgeAttribs();
        auto b_attribs = b.getLeafEdgeAttribs();
        CHECK(a_attribs.size() == a.numLeaves());
        CHECK(a_attribs.size() == 7);
        CHECK(b_attribs.size() == b.numLeaves());
        CHECK(b_attribs.size() == 7);

        // rooted
        string n3("((g:0.7,(a:0.1,(b:0.2,c:0.3):1):1):1,(f:0.6,(e:0.5,d:0.4):1):1);");
        string n4("((g:1.4,(a:0.2,(b:0.4,c:0.6):2):2):2,(d:0.8,(e:1.0,f:1.2):2):2);");
        auto c = PhyloTree(n3, true);
        auto d = PhyloTree(n4, true);
        auto c_attribs = c.getLeafEdgeAttribs();
        auto d_attribs = d.getLeafEdgeAttribs();
        CHECK(c_attribs.size() == c.numLeaves());
        CHECK(d_attribs.size() == d.numLeaves());
        CHECK(c_attribs.size() == 7);
        CHECK(d_attribs.size() == 7);

        for (size_t i = 0; i < 7; i++) {
            test = ((double) i + 1) / 10;
            CHECK(a_attribs[i].getAttribute() == 1);
            CHECK(b_attribs[i].getAttribute() == 2);
            CHECK(c_attribs[i].getAttribute() == test);
            CHECK(d_attribs[i].getAttribute() == 2 * test);
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
        vector<int> aVertices{0,1,2,3};
        vector<int> bVertices{0,1,2,3};
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

        auto t1 = PhyloTree(n1, true);
        auto t2 = PhyloTree(n2, true);
        auto t3 = PhyloTree(n3, true);
        auto t4 = PhyloTree(n4, true);
        CHECK(abs(Distance::getRobinsonFouldsDistance(t1, t2, false) - 8) < TOLERANCE);
        CHECK(abs(Distance::getRobinsonFouldsDistance(t1, t2, true) - 1) < TOLERANCE);
        CHECK(abs(Distance::getRobinsonFouldsDistance(t3, t4, false) - 8) < TOLERANCE);
        CHECK(abs(Distance::getRobinsonFouldsDistance(t3, t4, true) - 0.08333333333333333) < TOLERANCE);
    }

    SECTION("Weighted Robinson-Foulds") {
        string n1("((a:3,b:4):.1,(c:5,((d:6,e:7):.2,f:8):.3):.4);");
        string n2("((a:3,c:4):.5,(d:5,((b:6,e:7):.2,f:8):.3):.4);");
        string n3("(((((Sp1:0.237938301594,Sp2:0.237938301594):0.652150465533,Sp3:0.890088767127):1.0032766685,(Sp4:1.19159193346,(((Sp5:0.0499647780388,Sp6:0.0499647780388):0.625094094731,(Sp7:0.385992882857,(Sp8:0.0229771776543,Sp9:0.0229771776543):0.363015705203):0.289065989913):0.100330232606,Sp10:0.775389105376):0.416202828082):0.70177350217):1.37956852633,(((Sp11:0.143770218098,Sp12:0.143770218098):2.22399819333,(Sp13:0.879389128685,Sp14:0.879389128685):1.48837928275):0.882615734883,((((((Sp15:0.115537491571,Sp16:0.115537491571):1.14083013878,(Sp17:0.787001759928,((Sp18:0.109471903715,Sp19:0.109471903715):0.647746750411,Sp20:0.757218654126):0.029783105802):0.469365870425):0.210543598558,((Sp21:0.0928829593343,Sp22:0.0928829593343):0.596056952564,(Sp23:0.295394029748,Sp24:0.295394029748):0.39354588215):0.777971317013):0.273542049999,(((Sp25:0.750938536668,((Sp26:0.147146981319,Sp27:0.147146981319):0.197746581084,Sp28:0.344893562404):0.406044974265):0.224512201078,Sp29:0.975450737746):0.568823245523,(Sp30:0.423829830969,Sp31:0.423829830969):1.1204441523):0.196179295641):0.204967720146,(Sp32:1.88686786295,Sp33:1.88686786295):0.0585531361112):0.782778416912,((Sp34:0.453020826526,Sp35:0.453020826526):1.00864842727,Sp36:1.4616692538):1.26653016217):0.522184730348):0.0225498156455):0.118838892224,((Sp37:1.61330136153,((Sp38:1.11318444903,Sp39:1.11318444903):0.488085401285,(Sp40:1.42576644618,(((Sp41:0.0435502147095,Sp42:0.0435502147095):0.233013603141,Sp43:0.276563817851):0.175951351205,(Sp44:0.0424687878211,Sp45:0.0424687878211):0.410046381235):0.973251277121):0.175503404136):0.0120315112168):1.10970378837,((((Sp46:0.0108728387409,Sp47:0.0108728387409):0.096790292957,Sp48:0.107663131698):0.575955539077,Sp49:0.683618670775):0.35904620643,Sp50:1.0426648772):1.68034027269):0.66876770429):0.0;");
        string n4("(((((Sp1:0.237938301594,Sp2:0.237938301594):0.652150465533,(Sp4:1.19159193346,(Sp10:0.775389105376,(Sp7:0.385992882857,((Sp5:0.0499647780388,Sp6:0.0499647780388):0.625094094731,(Sp8:0.0229771776543,Sp9:0.0229771776543):0.363015705203):0.289065989913):0.100330232606):0.416202828082):0.70177350217):1.0032766685,Sp3:0.890088767127):1.37956852633,(((Sp11:0.143770218098,Sp12:0.143770218098):2.22399819333,(Sp13:0.879389128685,Sp14:0.879389128685):1.48837928275):0.882615734883,(((Sp34:0.453020826526,Sp35:0.453020826526):1.00864842727,Sp36:1.4616692538):1.26653016217,((Sp32:1.88686786295,Sp33:1.88686786295):0.0585531361112,((((Sp21:0.0928829593343,Sp22:0.0928829593343):0.596056952564,(Sp23:0.295394029748,Sp24:0.295394029748):0.39354588215):0.777971317013,((Sp15:0.115537491571,Sp16:0.115537491571):1.14083013878,((Sp20:0.757218654126,Sp17:0.787001759928):0.029783105802,(Sp18:0.109471903715,Sp19:0.109471903715):0.647746750411):0.469365870425):0.210543598558):0.273542049999,((Sp30:0.423829830969,Sp31:0.423829830969):1.1204441523,(Sp29:0.975450737746,((Sp26:0.147146981319,Sp27:0.147146981319):0.197746581084,(Sp25:0.750938536668,Sp28:0.344893562404):0.406044974265):0.224512201078):0.568823245523):0.196179295641):0.204967720146):0.782778416912):0.522184730348):0.0225498156455):0.118838892224,((Sp37:1.61330136153,((Sp38:1.11318444903,Sp39:1.11318444903):0.488085401285,(Sp40:1.42576644618,(((Sp41:0.0435502147095,Sp42:0.0435502147095):0.233013603141,Sp43:0.276563817851):0.175951351205,(Sp44:0.0424687878211,Sp45:0.0424687878211):0.410046381235):0.973251277121):0.175503404136):0.0120315112168):1.10970378837,((((Sp46:0.0108728387409,Sp47:0.0108728387409):0.096790292957,Sp48:0.107663131698):0.575955539077,Sp49:0.683618670775):0.35904620643,Sp50:1.0426648772):1.68034027269):0.66876770429):0.0;");

        auto t1 = PhyloTree(n1, true);
        auto t2 = PhyloTree(n2, true);
        auto t3 = PhyloTree(n3, true);
        auto t4 = PhyloTree(n4, true);
        CHECK(abs(Distance::getWeightedRobinsonFouldsDistance(t1, t2, false) - 6.4) < TOLERANCE);
        CHECK(abs(Distance::getWeightedRobinsonFouldsDistance(t1, t2, true) - 0.0935672514619883) < TOLERANCE);
        CHECK(abs(Distance::getWeightedRobinsonFouldsDistance(t3, t4, false) - 3.4563414769600005) < TOLERANCE);
        CHECK(abs(Distance::getWeightedRobinsonFouldsDistance(t3, t4, true) - 0.032045595869846046) < TOLERANCE);
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

        auto t1 = PhyloTree(s1, true);
        auto t2 = PhyloTree(s2, true);
        auto t3 = PhyloTree(s3, false);
        auto t4 = PhyloTree(s4, false);
        auto t5 = PhyloTree(s5, true);
        auto t6 = PhyloTree(s6, true);
        auto t7 = PhyloTree(s7, true);
        auto t8 = PhyloTree(s8, true);

        CHECK(abs(Distance::getGeodesicDistance(t1, t2, false) - 2.76188615828) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t1, t2, true) - 0.0977893234584) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t3, t4, false) - 4.35889894354) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t3, t4, true) - 0.438085827115) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t5, t6, false) - 4.472135955) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t5, t6, true) - 0.430331482912) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t7, t8, false) - 2.2413235148883968) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t7, t8, true) - 0.1522374775995074) < TOLERANCE);

        CHECK(abs(Distance::getGeodesicDistance(t1, t2, false) - Distance::getGeodesicDistance(s1, s2, false, true, true)) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t1, t2, true) - Distance::getGeodesicDistance(s1, s2, true, true, true)) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t3, t4, false) - Distance::getGeodesicDistance(s3, s4, false, false, false)) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t3, t4, true) - Distance::getGeodesicDistance(s3, s4, true, false, false)) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t5, t6, false) - Distance::getGeodesicDistance(s5, s6, false, true, true)) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t5, t6, true) - Distance::getGeodesicDistance(s5, s6, true, true, true)) < TOLERANCE);
        clock_t start = clock();
        for (size_t i = 0; i < 100; ++i) {
            Distance::getGeodesicDistance(t7, t8, false);
            Distance::getEuclideanDistance(t7, t8, false);
            Distance::getRobinsonFouldsDistance(t7, t8, false);
            Distance::getWeightedRobinsonFouldsDistance(t7, t8, false);
        }
        printf("Time taken: %.3f millisec\n", 1000 * (double)(clock() - start)/CLOCKS_PER_SEC);
        CHECK(abs(Distance::getGeodesicDistance(t7, t8, false) - 2.2413235148883968) < TOLERANCE);
        CHECK(abs(Distance::getGeodesicDistance(t7, t8, true) - 0.1522374775995074) < TOLERANCE);

    }
}
