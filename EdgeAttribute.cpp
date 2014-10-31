#include "EdgeAttribute.h"
#include <cmath>
//#include <sstream>
//#include <stdexcept>

#define TOLERANCE 0.000000000000001

EdgeAttribute::EdgeAttribute() {
}

EdgeAttribute::EdgeAttribute(const vector<double>& v) {
    vect = v;
}

EdgeAttribute::EdgeAttribute(string s) {
    std::vector<string> sv = Tools::string_split(s);
    std::vector<double> dv = Tools::stringvec_to_doublevec(sv);
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

double EdgeAttribute::norm() const {
    double norm = 0.0;
    for (auto &e : vect) {
        norm += e * e;
    }
    return std::sqrt(norm);
}

EdgeAttribute EdgeAttribute::difference(EdgeAttribute &a1, EdgeAttribute &a2) {
    if (a1.size() != a2.size()) throw std::invalid_argument("difference: Edge attributes are not the same size");
    std::vector<double> diff;
    for (size_t i = 0; i < a1.size(); ++i) {
        diff.push_back(a1.vect[i] - a2.vect[i]);
    }
    return EdgeAttribute(diff);
}

EdgeAttribute EdgeAttribute::add(EdgeAttribute &a1, EdgeAttribute &a2) {
    if (a1.size() != a2.size()) throw std::invalid_argument("add: Edge attributes are not the same size");
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
        vect[0] = (double) std::fabs(vect[0]);
    }
}

EdgeAttribute EdgeAttribute::zeroAttribute(size_t size) {
    std::vector<double> zeros(size, 0.0);
    return EdgeAttribute(zeros);
}

EdgeAttribute EdgeAttribute::weightedPairAverage(const EdgeAttribute& start, const EdgeAttribute& target, double position) {
    size_t start_size = start.size();
    if (start_size != target.size()) throw std::invalid_argument("weighted pair average: Edge attributes are not the same size");
    auto attr = EdgeAttribute();
    attr.vect.resize(start_size, 0.0);
    for (size_t i = 0; i < start_size; ++i) {
        attr.vect[i] = (1 - position) * start.vect[i] + position * target.vect[i];
    }
    return attr;
}
