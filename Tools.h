#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Tools
{
public:
    template<typename T>
    static bool vector_equal(std::vector<T> x, std::vector<T> y) {
        std::sort(x.begin(), x.end());
        std::sort(y.begin(), y.end());
        return x ==  y;
    }

    template<typename T>
    static void vector_print(std::vector<T> v) {
        for (size_t i=0; i < v.size() - 1; ++i) {
            std::cout << v[i] << " ";
        }
        std::cout << v[v.size() - 1] << std::endl;
    }

    static vector<string> string_split(string to_split, string delimiters=", \t\n", string to_trim="[]");

    static vector<double> stringvec_to_doublevec(vector<string> stringvec);
};
