#ifndef BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#define BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#endif
#include "Tools.h"
#include "PhyloTree.h"
#include <iomanip>
//#include <utility>
#define PRECISION 5

bool Tools::is_leaf(const bitset_t split) {
    return (split.count() == 1 || split.count() == split.size() - 1);
}

size_t Tools::leaf_index(const bitset_t split) {
    if (split.count() == 1) {
        return split.size() - split.find_first() - 1;
    }
    else if (split.count() == split.size() - 1) {
        return split.size() - (~split).find_first() - 1;
    }
    else {
        throw std::invalid_argument("Not a leaf");
    }
}

size_t Tools::leaf_index_nothrow(const bitset_t split) {
    if (split.count() == split.size() - 1) {
        return split.size() - (~split).find_first() - 1;
    } else {
        return split.size() - split.find_first() - 1;
    }
}

std::vector<std::string> Tools::string_split(std::string &to_split, const std::string &delimiters, const std::string &to_trim) {
    std::vector<std::string> results;
    boost::trim_if(to_split, boost::is_any_of(to_trim));
    boost::split(results, to_split, boost::is_any_of(delimiters), boost::token_compress_on);
    return results;
}

std::vector<std::string> Tools::string_split(std::string &to_split, const char *delimiters, const char *to_trim) {
    std::vector<std::string> results;
    boost::trim_if(to_split, boost::is_any_of(to_trim));
    boost::split(results, to_split, boost::is_any_of(delimiters), boost::token_compress_on);
    return results;
}

std::vector<double> Tools::stringvec_to_doublevec(std::vector<std::string> stringvec) {
    std::vector<double> doublevec(stringvec.size());
    std::transform(stringvec.begin(), stringvec.end(), doublevec.begin(), [](const std::string &val) {
        try {
            return std::stod(val);
        }
        catch (const std::invalid_argument &ia) {
            throw std::invalid_argument("invalid argument: " + val);
        }
    });
    return doublevec;
}

std::string Tools::string_join(std::vector<std::string> to_join, std::string delimiter) {
    return boost::algorithm::join(to_join, delimiter);
}

std::string Tools::string_join(std::deque<std::string> to_join, std::string delimiter) {
    return boost::algorithm::join(to_join, delimiter);
}

std::string Tools::double_to_string(double d) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(PRECISION);
    ss << d;
    std::string s = ss.str();
    return s;
}

//size_t Tools::nextIndex(string const &t, size_t i, string s) {
//    size_t pos = t.find_first_of(s, i + 1);
//    return pos == std::string::npos ? t.size() : pos;
//}

string Tools::substring(string s, size_t begin, size_t end) {
    if (begin > end) throw std::invalid_argument("'begin' can't be larger than 'end'");
    try {
        return s.substr(begin, end - begin);
    }
    catch (std::out_of_range &range_error) {
        return "";
    }
}

void Tools::despace(string &s) {
    s.erase(std::remove_if(s.begin(), s.end(), [](char x) {
        return std::isspace(x);
    }), s.end());
}

size_t Tools::nextIndex(const string &t, size_t i, const char *s) {
    size_t pos = t.find_first_of(s, i + 1);
    return pos == std::string::npos ? t.size() : pos;
}
