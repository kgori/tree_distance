#include "EdgeAttribute.h"
#include <iostream>
#include <stdexcept>
#include <vector>

int main(int argc, char const *argv[])
{
    {
        auto a = EdgeAttribute();
        auto b = EdgeAttribute(vector<double>{1, 2, 3, 4});
        auto c = EdgeAttribute("5, 6, 7, 8");
        auto d = EdgeAttribute(c);
        cout << "a: " << a.toString() << " size = " << a.size() << endl;
        cout << "b: " << b.toString() << " size = " << b.size() << endl;
        cout << "c: " << c.toString() << " size = " << c.size() << endl;
        cout << "d: " << d.toString() << " size = " << d.size() << endl;
        cout << "c == d ? " << c.equals(d) << d.equals(c) << endl;
    }

    {
        auto a = EdgeAttribute("1,3,5,7");
        auto b = EdgeAttribute("2,5,8,9");
        cout << EdgeAttribute::difference(a,b).toString() << endl;
        cout << EdgeAttribute::add(a,b).toString() << endl;
        cout << EdgeAttribute::weightedPairAverage(a,b, 0.1).toString() << endl;
    }

    {
        auto z = EdgeAttribute::zeroAttribute(10);
        cout << z.toString() << endl;
    }

    {
        auto a = EdgeAttribute("3, 4");
        cout << a.toString() << " norm = " << a.norm() << endl;
        a.scaleBy(2);
        cout << a.toString() << " norm = " << a.norm() << endl;
    }

    {
        auto a = EdgeAttribute("-6");
        cout << a.toString() << endl;
        a.ensurePositive();
        cout << a.toString() << endl;
    }

    {
        auto a = EdgeAttribute("1,2,3");
        auto b = EdgeAttribute("6,7,8,9,0");
        try {
            EdgeAttribute::add(a,b);
        }
        catch (const std::invalid_argument& ia) {
            cout << ia.what() << endl;
        }
    }

    return 0;
}