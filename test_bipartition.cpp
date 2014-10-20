#include "Bipartition.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    {
        // test default constructor
        auto a = Bipartition();
        cout << "a is empty ? " << a.isEmpty() << endl;

        // test string constructor
        auto b = Bipartition("0100101");

        // test toString output
        cout << b.toString() << endl;

        // test copy constructor
        auto c = Bipartition(b);
        cout << c.toString() << endl;

        // test getting bitset
        auto bitset = c.getPartition();

        // test bitset constructor
        auto d = Bipartition(*bitset);
        cout << d.toString() << endl;
    }

    {
        auto a = Bipartition("1100101110");
        auto b = Bipartition("1100101110");
        auto c = Bipartition("0011010001");

        cout << "a == b ? " << a.equals(b) << b.equals(a) << endl;
        cout << "a == c ? " << a.equals(c) << c.equals(a) << endl;
        cout << "b == c ? " << b.equals(c) << c.equals(b) << endl;

        cout << "b Bipartition: " << b.toString() << endl;
        b.complement(b.size());
        cout << "b.complement:  " << b.toString() << endl;
        cout << "b == c ? " << b.equals(c) << c.equals(b) << endl;
    }

    {
        auto a = Bipartition("0101010101");
        auto b = Bipartition("1010101010");
        auto c = Bipartition("1111100000");

        cout << "a crosses b: " << a.crosses(b) << b.crosses(a) << endl;
        cout << "a crosses c: " << a.crosses(c) << c.crosses(a) << endl;
        cout << "b crosses c: " << b.crosses(c) << c.crosses(b) << endl;
    }

    {
        auto a = Bipartition("110011");
        auto b = Bipartition("100001");

        cout << "a contains b: " << a.contains(b) << endl;
        cout << "b contains a: " << b.contains(a) << endl;
        cout << "a properly contains b: " << a.properlyContains(b) << endl;
        cout << "b properly contains a: " << b.properlyContains(a) << endl;
    }
    return 0;
}