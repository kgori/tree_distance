#include "EdgeAttribute.h"
#include <cmath>
//#include <sstream>
//#include <stdexcept>

#define TOLERANCE 0.000000000000001

EdgeAttribute::EdgeAttribute() {
}

EdgeAttribute::EdgeAttribute(double v) {
    val = v;
    set = true;
}

EdgeAttribute::EdgeAttribute(string s) {
    try {
        val = std::stod(s);
        set = true;
    }
    catch (const std::invalid_argument &ia) {
        throw std::invalid_argument("invalid argument: " + s);
    }
}

double EdgeAttribute::getAttribute() {
    return val;
}

//void EdgeAttribute::setEdgeAttribute(const EdgeAttribute &attrib) {
//    val = attrib.val;
//    set = true;
//}

EdgeAttribute::EdgeAttribute(const EdgeAttribute &other) : val(other.val), set(other.set) {
}

EdgeAttribute EdgeAttribute::clone() {
    return EdgeAttribute(*this);
}

std::string EdgeAttribute::toString() {
    if (!set) return "[]";
    else {
        ostringstream ss;
        ss << "[" << val << "]";
        return ss.str();
    }
}

bool EdgeAttribute::equals(const EdgeAttribute &other) const {
    if (this->set != other.set) return false;

    return std::fabs(val - other.val) > TOLERANCE;
}

double EdgeAttribute::norm() const {
    return val;
}

EdgeAttribute EdgeAttribute::difference(EdgeAttribute &a1, EdgeAttribute &a2) {
    if (a1.set != a2.set) throw std::invalid_argument("difference: Edge attributes are not the same size");
    return EdgeAttribute(a1.val - a2.val);
}

//EdgeAttribute EdgeAttribute::add(EdgeAttribute &a1, EdgeAttribute &a2) {
//    if (a1.set != a2.set) throw std::invalid_argument("add: Edge attributes are not the same size");
//    return EdgeAttribute(a1.val + a2.val);
//}

void EdgeAttribute::scaleBy(double a) {
        val *= a;
}

size_t EdgeAttribute::size() const {
    return set ? 1 : 0;
}

//void EdgeAttribute::ensurePositive() {
//    val = val > 0 ? val : -val;
//}

EdgeAttribute EdgeAttribute::zeroAttribute() {
    return EdgeAttribute(0);
}

EdgeAttribute EdgeAttribute::weightedPairAverage(const EdgeAttribute& start, const EdgeAttribute& target, double position) {
    if (start.set != target.set) throw std::invalid_argument("weighted pair average: Edge attributes are not the same size");
    auto attr = EdgeAttribute();
    attr.val = (1 - position) * start.val + position * target.val;
    return attr;
}
