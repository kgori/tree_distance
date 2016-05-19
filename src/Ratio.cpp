#ifndef BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#define BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#endif
#include "Ratio.h"
#include <cmath>
#include <iomanip>
#include <sstream>

using namespace std;

Ratio::Ratio() : eLength(0), fLength(0) {
}

Ratio::Ratio(vector<PhyloTreeEdge>& e, vector<PhyloTreeEdge>& f) :
        eEdges(e), fEdges(f), eLength(geoAvg(e)), fLength(geoAvg(f)) {
}

Ratio::Ratio(double e, double f) : eLength(e), fLength(f) {
}

Ratio::Ratio(const Ratio &other) : eLength(other.eLength), fLength(other.fLength),
                                   eEdges(other.eEdges), fEdges(other.fEdges) {
}

Ratio Ratio::combine(Ratio& r1, Ratio& r2) {
    Ratio r{};
    if ((r1.eEdges.size() == 0) && (r2.eEdges.size() == 0)) {
        r.setELength(geoAvg(r1.eLength, r2.eLength));
    }
    else {
        r.setAllEEdges(r1.eEdges);
        r.addAllEEdges(r2.eEdges);
    }

    if ((r1.fEdges.size() == 0) && (r2.fEdges.size() == 0)) {
        r.setFLength(geoAvg(r1.fLength, r2.fLength));
    }
    else {
        r.setAllFEdges(r1.fEdges);
        r.addAllFEdges(r2.fEdges);
    }
    return r;
}

double Ratio::geoAvg(double d1, double d2) {
    return sqrt(pow(d1, 2) + pow(d2, 2));
}

double Ratio::geoAvg(vector<PhyloTreeEdge>& edges) {
    double gAvg = 0.0;

    for (PhyloTreeEdge &e: edges) {
        gAvg += pow(e.getLength(), 2);
    }

    return sqrt(gAvg);
}

const vector<PhyloTreeEdge>& Ratio::getEEdges() {
    return eEdges;
}

void Ratio::addEEdge(PhyloTreeEdge& edge) {
    eEdges.push_back(edge);
    eLength = geoAvg(eLength, edge.getLength());
}

void Ratio::addAllEEdges(vector<PhyloTreeEdge>& edges) {
    eEdges.reserve(eEdges.size() + edges.size());
    for (auto &e : edges) {
        eEdges.push_back(e);
    }
    eLength = geoAvg(eEdges);
}

void Ratio::setAllEEdges(vector<PhyloTreeEdge>& edges) {
    eEdges = edges;
    eLength = geoAvg(eEdges);
}

void Ratio::setAllFEdges(vector<PhyloTreeEdge>& edges) {
    fEdges = edges;
    fLength = geoAvg(fEdges);
}

double Ratio::getELength() {
    if (eEdges.size() == 0)
        return eLength;
    return geoAvg(eEdges);
}

void Ratio::setELength(double eLen) {
    if (eEdges.size() == 0)
        eLength = eLen;
}

const vector<PhyloTreeEdge>& Ratio::getFEdges() {
    return fEdges;
}

void Ratio::addFEdge(PhyloTreeEdge& edge) {
    fEdges.push_back(edge);
    fLength = geoAvg(fLength, edge.getLength());
}

void Ratio::addAllFEdges(vector<PhyloTreeEdge>& edges) {
    fEdges.reserve(fEdges.size() + edges.size());
    for (auto &f : edges) {
        fEdges.push_back(f);
    }
    fLength = geoAvg(fEdges);
}

double Ratio::getFLength() {
    if (fEdges.size() == 0)
        return fLength;
    return geoAvg(fEdges);
}

void Ratio::setFLength(double fLen) {
    if (fEdges.size() == 0)
        fLength = fLen;
}

double Ratio::getRatio() const {
    return eLength / fLength;
}

double Ratio::getTime() {
    return eLength / (eLength + fLength);
}

Ratio Ratio::reverse() {
    vector<PhyloTreeEdge> evec, fvec;
    evec.reserve(eEdges.size());
    fvec.reserve(fEdges.size());
    for (auto &e : eEdges) {
        evec.push_back(PhyloTreeEdge(e));
    }
    for (auto &f : fEdges) {
        fvec.push_back(PhyloTreeEdge(f));
    }
    return Ratio(fvec, evec);
}

bool Ratio::containsOriginalEEdge(const Bipartition &edge) {

    for (PhyloTreeEdge &ratio_edge : eEdges) {
        if (ratio_edge.getOriginalEdge()->equals(edge)) {
            return true;
        }
    }

    for (PhyloTreeEdge &ratio_edge : fEdges) {
        if (ratio_edge.getOriginalEdge()->equals(edge)) {
            return true;
        }
    }
    return false;
}

string Ratio::toString() {
    std::ostringstream ss;
    for (auto &e : eEdges) {
        ss << e.toString() << " ";
    }
    ss << getELength() << " / " << getFLength() << " ";
    for (auto &f : fEdges) {
        ss << f.toString() << " ";
    }

    return ss.str();
}

Ratio Ratio::clone() {
    return Ratio(*this);
}

string Ratio::toStringCombType() {
    ostringstream ss;
    ss << "{";

    for (size_t i = 0; i < eEdges.size(); i++) {
        ss << eEdges[i].getOriginalID();
        if (i < eEdges.size() - 1) {
            ss << ",";
        }
    }
    ss << "}/{";

    for (size_t i = 0; i < fEdges.size(); i++) {
        ss << fEdges[i].getOriginalID();
        if (i < fEdges.size() - 1) {
            ss << ",";
        }
    }
    ss << "}";
    return ss.str();
}

string Ratio::toStringVerbose(vector<string> leaf2NumMap) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(8);

    ss << getRatio() << endl;

    ss << "Total length and corresponding edges dropped:" << endl;

    ss << eLength << "\t";

    // list the edges dropped
    for (int i = 0; i < eEdges.size(); i++) {
        if (i == 0) {   // nice formatting
            ss << Bipartition::toStringVerbose(eEdges[i].getPartition(), leaf2NumMap) << endl;
        } else {
            ss << "\t\t" << Bipartition::toStringVerbose(eEdges[i].getPartition(), leaf2NumMap) << endl;
        }
    }

    ss << "\nTotal length and corresponding edges added:" << endl;

    ss << fLength << "\t";

    // list the edges added
    for (int i = 0; i < fEdges.size(); i++) {
        if (i == 0) {   // nice formatting
            ss << Bipartition::toStringVerbose(fEdges[i].getPartition(), leaf2NumMap) << endl;
        } else {
            ss << "\t\t" << Bipartition::toStringVerbose(fEdges[i].getPartition(), leaf2NumMap) << endl;
        }
    }

    return ss.str();

}
