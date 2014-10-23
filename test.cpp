#include "Bipartition.h"
#include "EdgeAttribute.h"
#include "PhyloTreeEdge.h"
#include "PhyloTree.h"
#include "test_catch_helper.h"

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
        a&=b;
        REQUIRE(a == b);
        REQUIRE(~b == Bipartition("10011"));
        REQUIRE(Bipartition("11111111").andNot(Bipartition("10100011")) == Bipartition("01011100"));
        REQUIRE((Bipartition("11111111")&=(Bipartition("10100011"))) == Bipartition("10100011"));
        REQUIRE(Bipartition("1100100111").andNot(Bipartition("1011011100")) == Bipartition("0100100011"));
        REQUIRE((Bipartition("1100100111")&=(Bipartition("1011011100"))) == Bipartition("1000000100"));
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

//    {
//        auto a = EdgeAttribute("1,2,3");
//        auto b = EdgeAttribute("6,7,8,9,0");
//        try {
//            EdgeAttribute::add(a, b);
//        }
//        catch (const std::invalid_argument &ia) {
//            cout << ia.what() << endl;
//        }
//    }
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
        cout << Tools::nextIndex(s, 16, ",)") << endl;
        cout << Tools::nextIndex(s, 17, ",)") << endl;
    }
}

TEST_CASE("PhyloTree") {
    SECTION("Construction") {
        auto edges = vector<PhyloTreeEdge>({},{});
//        auto a = PhyloTree();
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