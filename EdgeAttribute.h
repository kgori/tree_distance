#ifndef __EDGE_ATTRIBUTE_H__
#define __EDGE_ATTRIBUTE_H__

#include "Tools.h"
#include <string>
#include <vector>

using namespace std;

class EdgeAttribute {
public:
    EdgeAttribute();

    EdgeAttribute(const vector<double>& v);

    EdgeAttribute(string s);

    EdgeAttribute(const EdgeAttribute &other); // copy-constructor
    // ~EdgeAttribute();
    static EdgeAttribute difference(EdgeAttribute &a1, EdgeAttribute &a2);

    static EdgeAttribute add(EdgeAttribute &a1, EdgeAttribute &a2);

    static EdgeAttribute weightedPairAverage(const EdgeAttribute& start, const EdgeAttribute& target, double position);

    static EdgeAttribute zeroAttribute(size_t size);

    inline bool operator<(const EdgeAttribute &other) const {
        return this->norm() < other.norm();
    }

    inline bool operator>(const EdgeAttribute &other) const {
        return other < *this;
    }

    inline bool operator<=(const EdgeAttribute &other) const {
        return !(*this > other);
    }

    inline bool operator>=(const EdgeAttribute &other) const {
        return !(*this < other);
    }

    inline bool operator==(const EdgeAttribute &other) const {
        return this->norm() == other.norm();
    }

    inline bool operator!=(const EdgeAttribute &other) const {
        return !(*this == other);
    }

    double getAttribute();

    void setEdgeAttribute(const EdgeAttribute &attrib);

    EdgeAttribute clone();

    string toString();

    bool equals(const EdgeAttribute &other) const;

    double norm() const;

    void scaleBy(double a);

    size_t size() const;

    void ensurePositive();

private:
    vector<double> vect;
};

#endif /* __EDGE_ATTRIBUTE_H__ */
