#include "Bipartition.h"
#include "EdgeAttribute.h"
#include "PhyloTreeEdge.h"
#include "PhyloTree.h"
#include "Ratio.h"
#include "test_catch_helper.h"

#define TOLERANCE 0.00000001

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
        auto d = Bipartition(*bitset);
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
    }

    SECTION("Contains") {
        auto a = Bipartition("110011");
        auto b = Bipartition("100001");

        CHECK(a.contains(b));
        CHECK(!b.contains(a));
        CHECK(a.properlyContains(b));
        CHECK(!b.properlyContains(a));
    }
}

TEST_CASE("EdgeAttribute") {
    SECTION("Construction") {
        auto a = EdgeAttribute();
        auto b = EdgeAttribute(vector<double>{1, 2, 3, 4});
        auto c = EdgeAttribute("5, 6, 7, 8");
        auto d = EdgeAttribute(c);
        REQUIRE(a.toString() == "[]");
        REQUIRE(b.toString() == "[1 2 3 4]");
        REQUIRE(c.toString() == "[5 6 7 8]");
        REQUIRE(d.toString() == c.toString());
        REQUIRE(a.size() == 0);
        REQUIRE(b.size() == 4);
        REQUIRE(c.size() == 4);
        REQUIRE(d.size() == c.size());
        REQUIRE(c.equals(d));
    }

    SECTION("Arithmetic") {
        auto a = EdgeAttribute("1,3,5,7");
        auto b = EdgeAttribute("2,5,8,9");
        REQUIRE(EdgeAttribute::difference(a, b).toString() == "[-1 -2 -3 -2]");
        REQUIRE(EdgeAttribute::add(a, b).toString() == "[3 8 13 16]");
        REQUIRE(EdgeAttribute::weightedPairAverage(a, b, 0.1).toString() == "[1.1 3.2 5.3 7.2]");
    }

    SECTION("Numerics") {
        auto z = EdgeAttribute::zeroAttribute(10);
        REQUIRE(z.toString() == "[0 0 0 0 0 0 0 0 0 0]");

        auto a = EdgeAttribute("3, 4");
        REQUIRE(a.toString() == "[3 4]");
        REQUIRE(a.norm() == 5);
        a.scaleBy(2);
        REQUIRE(a.toString() == "[6 8]");
        REQUIRE(a.norm() == 10);

        auto b = EdgeAttribute("-6");
        REQUIRE(b.toString() == "[-6]");
        b.ensurePositive();
        REQUIRE(b.toString() == "[6]");
    }

    {
        auto a = EdgeAttribute("1,2,3");
        auto b = EdgeAttribute("6,7,8,9,0");
        REQUIRE_THROWS_AS(EdgeAttribute::add(a, b), std::invalid_argument);

    }
}

TEST_CASE("PhyloTreeEdge") {
    SECTION("Construction") {
        auto a = PhyloTreeEdge();
        REQUIRE(a.toString() == "[] ");
        REQUIRE(a.getOriginalID() == -1);

        auto bip = Bipartition("01001101");
        auto b = PhyloTreeEdge(*(bip.getPartition()));
        REQUIRE(b.toString() == "[] 01001101");
        REQUIRE(b.getOriginalID() == -1);

        auto attrib = EdgeAttribute("0.5, 0.6, 0.7, 0.8");
        auto c = PhyloTreeEdge(attrib);
        REQUIRE(c.toString() == attrib.toString() + " ");
        REQUIRE(c.getOriginalID() == -1);

        auto d = PhyloTreeEdge(attrib, 10);
        REQUIRE(d.toString() == attrib.toString() + " ");
        REQUIRE(d.getOriginalID() == 10);

        auto e = PhyloTreeEdge(attrib, bip, 11);
        REQUIRE(e.toString() == attrib.toString() + " ");
        REQUIRE(e.getOriginalID() == 11);

        auto f = PhyloTreeEdge(bip, attrib, 12);
        REQUIRE(f.toString() == attrib.toString() + " " + bip.toString());
        REQUIRE(f.getOriginalID() == 12);

        auto edge = Bipartition("11011011");
        auto g = PhyloTreeEdge(*(edge.getPartition()), attrib, *(bip.getPartition()), 13);
        REQUIRE(g.toString() == attrib.toString() + " " + edge.toString());
        REQUIRE(g.getOriginalID() == 13);

        auto h = PhyloTreeEdge(g);
        REQUIRE(h.toString() == g.toString());
        REQUIRE(h.getOriginalID() == g.getOriginalID());
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
        vector<string> a{"1", "2", "3", "4"};
        vector<double> b{1.0, 2.0, 3.0, 4.0};
        CHECK(Tools::stringvec_to_doublevec(a) == b);

        vector<string> c{"a", "b", "c", "d"};
        CHECK_THROWS(Tools::stringvec_to_doublevec(c));
    }
}

TEST_CASE("PhyloTree") {
    SECTION("Construction") {
        auto edges = vector<PhyloTreeEdge>({},{});
        auto d = PhyloTree("(a:1,(b:2,c:3):4);", true);
        auto e = PhyloTree("(a:1, (b:2, c:3):4);", true);
        auto f = PhyloTree("(a:1, b:2, c:3);", false);
        auto g = PhyloTree("((a:3,b:4):.1,(c:5,((d:6,e:7):.2,f:8):.3):.4);", true);
        REQUIRE(d.getBranchLengthSum() == 10);
        REQUIRE(e.getBranchLengthSum() == 10);
        REQUIRE(f.getBranchLengthSum() == 6);
        REQUIRE(g.getBranchLengthSum() == 34);
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

        auto t1 = PhyloTree("((a:3,b:4):.1,(c:5,((d:6,e:7):.2,f:8):.3):.4);", true);
        auto eEdges = t1.getEdges();
        auto t2 = PhyloTree("((a:3,c:4):.5,(d:5,((b:6,e:7):.2,f:8):.3):.4);", true);
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
    }
    SECTION("Edge lengths") {
        auto a = Ratio();
        a.setELength(0.666);
        a.setFLength(666);
        REQUIRE(a.getELength() == 0.666);
        REQUIRE(a.getFLength() == 666);
    }

    SECTION("Contains") {
        auto t1 = PhyloTree("((a:3,b:4):.1,(c:5,((d:6,e:7):.2,f:8):.3):.4);", true);
        auto eEdges = t1.getEdges();
        auto t2 = PhyloTree("((a:3,c:4):.5,(d:5,((b:6,e:7):.2,f:8):.3):.4);", true);
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