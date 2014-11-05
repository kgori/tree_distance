#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const string EMPTY("");

class Tools {
public:
    template<typename T>
    static bool vector_equal(std::vector<T> x, std::vector<T> y) {
        std::sort(x.begin(), x.end());
        std::sort(y.begin(), y.end());
        return x == y;
    }

    template<typename T>
    static void vector_print(std::vector<T> v) {
        for (size_t i = 0; i < v.size() - 1; ++i) {
            std::cout << v[i] << " ";
        }
        std::cout << v[v.size() - 1] << std::endl;
    }

    template<typename T>
    static void deque_print(std::deque<T> v) {
        for (size_t i = 0; i < v.size() - 1; ++i) {
            std::cout << v[i] << " ";
        }
        std::cout << v[v.size() - 1] << std::endl;
    }

    template<typename T>
    static std::string vector_to_string(std::vector<T> v) {
        std::stringstream ss{};
        for (size_t i = 0; i < v.size() - 1; ++i) {
            ss << v[i] << " ";
        }
        ss << v[v.size() - 1];
        return ss.str();
    }

    template<typename T, template<typename> class C>
    static void remove_element_at_index(C<T> &c, size_t index) {
        c.erase(c.begin() + index);
    }

    template<typename T>
    static void deque_remove_element_at_index(deque<T> &c, size_t index) {
        c.erase(c.begin() + index);
    }

    template<typename T>
    static void vector_remove_element_at_index(vector<T> &c, size_t index) {
        c.erase(c.begin() + index);
    }

    static vector<string> string_split(string &to_split, const string &delimiters, const string &to_trim);

    static vector<string> string_split(std::string &to_split, const char *delimiters = " ,/n/t", const char *to_trim = "[]");

    static string string_join(vector<string> to_join, string delimiter = EMPTY);

    static string string_join(deque<string> to_join, string delimiter = EMPTY);

    static vector<double> stringvec_to_doublevec(vector<string> stringvec);

    static string double_to_string(double d);

//    static size_t nextIndex(const string& t, size_t i, string s);

    static size_t nextIndex(const string &t, size_t i, const char *s);

    static string substring(string s, size_t begin, size_t end);

    static void despace(string &s);
};

#endif /* __TOOLS_H__ */
