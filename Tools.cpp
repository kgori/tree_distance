#include "Tools.h"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <stdexcept>

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
