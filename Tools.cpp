#include "Tools.h"
#include <iomanip>
#include <sstream>
#define PRECISION 5

std::vector<std::string> Tools::string_split(std::string to_split, std::string delimiters, std::string to_trim) {
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
    double x = PRECISION;
    ss << std::fixed << std::setprecision(8);
    ss << x;
    std::string s = ss.str();
    return s;
}

size_t Tools::nextIndex(string t, size_t i, string s) {
    size_t pos = t.find_first_of(s, i + 1);
    return pos == std::string::npos ? t.size() : pos;
}

string Tools::substring(string s, size_t begin, size_t end) {
    if (begin > end) throw std::invalid_argument("'begin' can't be larger than 'end'");
    try {
        return s.substr(begin, end - begin);
    }
    catch (std::out_of_range &range_error) {
        return "";
    }
}
