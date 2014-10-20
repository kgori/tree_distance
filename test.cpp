#include "Bipartition.h"
#include "EdgeAttribute.h"
#include "PhyloTreeEdge.h"
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

//    {
//        auto a = Bipartition("0101010101");
//        auto b = Bipartition("1010101010");
//        auto c = Bipartition("1111100000");
//
//        cout << "a crosses b: " << a.crosses(b) << b.crosses(a) << endl;
//        cout << "a crosses c: " << a.crosses(c) << c.crosses(a) << endl;
//        cout << "b crosses c: " << b.crosses(c) << c.crosses(b) << endl;
//    }
//
//    {
//        auto a = Bipartition("110011");
//        auto b = Bipartition("100001");
//
//        cout << "a contains b: " << a.contains(b) << endl;
//        cout << "b contains a: " << b.contains(a) << endl;
//        cout << "a properly contains b: " << a.properlyContains(b) << endl;
//        cout << "b properly contains a: " << b.properlyContains(a) << endl;
//    }
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

//int main(int argc, char const *argv[]) {
////    test_bipartition();
////    test_edge_attribute();
//    test_phylotree_edge();
//    return 0;
//}
