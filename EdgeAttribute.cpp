#include "EdgeAttribute.h"
#include <boost/algorithm/string.hpp>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>

#define TOLERANCE 0.000000000000001

std::vector<std::string> string_to_vect(std::string s) {
    std::vector<std::string> results;
    boost::trim_if(s, boost::is_any_of("[]"));
    boost::split(results, s, boost::is_any_of(" ,\t"), boost::token_compress_on);
    return results;
}

std::vector<double> string_vector_to_double_vector(std::vector<std::string> stringVector) {
    std::vector<double> doubleVector(stringVector.size());
    std::transform(stringVector.begin(), stringVector.end(), doubleVector.begin(), [](const std::string &val) {
        try {
            return std::stod(val);
        }
        catch (const std::invalid_argument &ia) {
            throw std::invalid_argument("invalid argument: " + val);
        }
    });
    return doubleVector;
}

EdgeAttribute::EdgeAttribute() {
}

EdgeAttribute::EdgeAttribute(vector<double> v) {
    vect = v;
}

EdgeAttribute::EdgeAttribute(string s) {
    std::vector<string> sv = string_to_vect(s);
    std::vector<double> dv = string_vector_to_double_vector(sv);
    vect = dv;
}

double EdgeAttribute::getAttribute() {
    return vect[0];
}

void EdgeAttribute::setEdgeAttribute(const EdgeAttribute &attrib) {
    vect = attrib.vect;
}

EdgeAttribute::EdgeAttribute(const EdgeAttribute &other) {
    vect = other.vect;
}

EdgeAttribute EdgeAttribute::clone() {
    return EdgeAttribute(*this);
}

std::string EdgeAttribute::toString() {
    if (vect.size() == 0) return "[]";
    else {
        ostringstream ss;
        ss << "[";
        if (vect.size() > 1) {
            for (size_t i = 0; i < (vect.size() - 1); ++i) {
                ss << vect[i] << " ";
            }
        }
        ss << vect[vect.size() - 1] << "]";
        std::string s{ss.str()};
        return s;
    }
}

bool EdgeAttribute::equals(const EdgeAttribute &other) const {
    if (vect.size() != other.size()) return false;
    for (size_t i = 0; i < vect.size(); ++i) {
        if (std::fabs(vect[i] - other.vect[i]) > TOLERANCE) {
            return false;
        }
    }
    return true;
}

double EdgeAttribute::norm() {
    double norm = 0.0;
    for (auto e : vect) {
        norm += e * e;
    }
    return std::sqrt(norm);
}

EdgeAttribute EdgeAttribute::difference(EdgeAttribute a1, EdgeAttribute a2) {
    if (a1.size() != a2.size()) throw std::invalid_argument("Edge attributes are not the same size");
    std::vector<double> diff;
    for (size_t i = 0; i < a1.size(); ++i) {
        diff.push_back(a1.vect[i] - a2.vect[i]);
    }
    return EdgeAttribute(diff);
}

EdgeAttribute EdgeAttribute::add(EdgeAttribute a1, EdgeAttribute a2) {
    if (a1.size() != a2.size()) throw std::invalid_argument("Edge attributes are not the same size");
    std::vector<double> diff;
    for (size_t i = 0; i < a1.size(); ++i) {
        diff.push_back(a1.vect[i] + a2.vect[i]);
    }
    return EdgeAttribute(diff);
}

void EdgeAttribute::scaleBy(double a) {
    for (auto &e : vect) {
        e *= a;
    }
}

size_t EdgeAttribute::size() const {
    return vect.size();
}

void EdgeAttribute::ensurePositive() {
    if (vect.size() == 1) {
        vect[0] = std::fabs(vect[0]);
    }
}

EdgeAttribute EdgeAttribute::zeroAttribute(size_t size) {
    std::vector<double> zeros(size, 0.0);
    return EdgeAttribute(zeros);
}

EdgeAttribute EdgeAttribute::weightedPairAverage(EdgeAttribute start, EdgeAttribute target, double position) {
    if (start.size() != target.size()) throw std::invalid_argument("Edge attributes are not the same size");
    std::vector<double> point(start.size(), 0.0);
    for (size_t i = 0; i < start.size(); ++i) {
        point[i] = (1 - position) * start.vect[i] + position * target.vect[i];
    }
    return EdgeAttribute(point);
}