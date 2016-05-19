#ifndef __TOOLS_H__
#define __TOOLS_H__
#ifndef BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#define BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#endif

#include <algorithm>
#include "boost/algorithm/string.hpp"
#include "boost/dynamic_bitset.hpp"
#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using bitset_t = boost::dynamic_bitset<>;

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

    /*
     * Fills an OutputIterator to a container with items drawn from an Iterator to an input container,
     * EXCEPT at indices specified by an IndexIterator to an index container
     */
    template <typename Iterator, typename IndexIterator, typename OutputIterator>
    static void prune_container(Iterator begin, Iterator end, IndexIterator index_begin, IndexIterator index_end,
            OutputIterator inserter) {
        auto front = begin;
        for (; begin!=end; begin++) {
            if (index_begin == index_end) {
                (*inserter++) = (*begin);
            }
            else if (std::distance(front, begin)==(*index_begin)) {
                index_begin++;
            }
            else {
                (*inserter++) = (*begin);
            }
        }
    }

    template<typename Iterator>
    static void vector_print(Iterator begin, Iterator end) {
        if (begin == end) return;
        for (; begin != end-1; begin++) {
            std::cout << (*begin) << " ";
        }
        std::cout << (*begin) << std::endl;
    }

    /*
     * Remove taxa at indices in `missing` from PhyloTree `tree`,
     * returning a new pruned tree
     */
    template <typename Container>
    static bitset_t prune_bitset(const bitset_t& original, const Container& missing) {
        size_t original_size = original.size();
        size_t result_size = original_size - missing.size();
        bitset_t result(result_size);
        size_t original_index = 0, result_index = 0, missing_index = 0;
        for (; original_index < original.size(); original_index++) {
            if (original_index == missing[missing_index]) { // skip this one
                missing_index++;
                continue;
            }
            else {
                result[result_size - result_index++ - 1] = original[original_size - original_index - 1];
            }
        }
        return std::move(result);
    }

    static bool is_leaf(const bitset_t split);

    static size_t leaf_index(const bitset_t split);

    static size_t leaf_index_nothrow(const bitset_t split);
};

#endif /* __TOOLS_H__ */
