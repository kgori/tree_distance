#include "Ratio.h"
#include <cmath>
#include <iomanip>
//#include <iostream>

using namespace std;

Ratio::Ratio() : eLength(0), fLength(0) {
    std::cout << "Ratio::Ratio()" << std::endl;
}

Ratio::Ratio(vector<PhyloTreeEdge> e, vector<PhyloTreeEdge> f) :
        eEdges(e), fEdges(f), eLength(geoAvg(e)), fLength(geoAvg(f)) {
    std::cout << "Ratio::Ratio(vector<PhyloTreeEdge> e, vector<PhyloTreeEdge> f)" << std::endl;
}

Ratio::Ratio(double e, double f) : eLength(e), fLength(f) {
    std::cout << "Ratio::Ratio(double e, double f)" << std::endl;
}

Ratio::Ratio(const Ratio &other) : eLength(other.eLength), fLength(other.fLength),
        eEdges(other.eEdges), fEdges(other.fEdges) {
    std::cout << "Ratio::Ratio(const Ratio &other)" << std::endl;
}

Ratio Ratio::combine(Ratio r1, Ratio r2) {
    std::cout << "Ratio::combine(Ratio r1, Ratio r2)" << std::endl;
    Ratio r{};
    if ((r1.eEdges.size() == 0) && (r2.eEdges.size() == 0)) {
        r.setELength(geoAvg(r1.eLength, r2.eLength));
    }
    else {
        r.addAllEEdges(r1.eEdges);
        r.addAllEEdges(r2.eEdges);
    }

    if ((r1.fEdges.size() == 0) && (r2.fEdges.size() == 0)) {
        r.setFLength(geoAvg(r1.fLength, r2.fLength));
    }
    else {
        r.addAllFEdges(r1.fEdges);
        r.addAllFEdges(r2.fEdges);
    }
    return r;
}

double Ratio::geoAvg(double d1, double d2) {
    std::cout << "Ratio::geoAvg(double d1, double d2)" << std::endl;
    return sqrt(pow(d1, 2) + pow(d2, 2));
}

double Ratio::geoAvg(vector<PhyloTreeEdge> edges) {
    std::cout << "Ratio::geoAvg(vector<PhyloTreeEdge> edges)" << std::endl;
    double gAvg = 0.0;

    for (PhyloTreeEdge e: edges) {
        gAvg += pow(e.getLength(), 2);
    }

    return sqrt(gAvg);
}

vector<PhyloTreeEdge> Ratio::getEEdges() {
    std::cout << "Ratio::getEEdges()" << std::endl;
    return eEdges;
}

void Ratio::addEEdge(PhyloTreeEdge edge) {
    std::cout << "Ratio::addEEdge(PhyloTreeEdge edge)" << std::endl;
    eEdges.push_back(edge);
    eLength = geoAvg(eLength, edge.getLength());
}

void Ratio::addAllEEdges(vector<PhyloTreeEdge> edges) {
    std::cout << "Ratio::addAllEEdges(vector<PhyloTreeEdge> edges)" << std::endl;
    for (auto &e : edges) {
        eEdges.push_back(e);
    }
    eLength = geoAvg(eEdges);
}

double Ratio::getELength() {
    std::cout << "Ratio::getELength()" << std::endl;
    if (eEdges.size() == 0)
        return eLength;
    return geoAvg(eEdges);
}

void Ratio::setELength(double eLen) {
    std::cout << "Ratio::setELength(double eLen)" << std::endl;
    if (eEdges.size() == 0)
        eLength = eLen;
}

vector<PhyloTreeEdge> Ratio::getFEdges() {
    std::cout << "Ratio::getFEdges()" << std::endl;
    return fEdges;
}

void Ratio::addFEdge(PhyloTreeEdge edge) {
    std::cout << "Ratio::addFEdge(PhyloTreeEdge edge)" << std::endl;
    fEdges.push_back(edge);
    fLength = geoAvg(fLength, edge.getLength());
}

void Ratio::addAllFEdges(vector<PhyloTreeEdge> edges) {
    std::cout << "Ratio::addAllFEdges(vector<PhyloTreeEdge> edges)" << std::endl;
    for (auto &f : edges) {
        fEdges.push_back(f);
    }
    fLength = geoAvg(fEdges);
}

double Ratio::getFLength() {
    std::cout << "Ratio::getFLength()" << std::endl;
    if (fEdges.size() == 0)
        return fLength;
    return geoAvg(fEdges);
}

void Ratio::setFLength(double fLen) {
    std::cout << "Ratio::setFLength(double fLen)" << std::endl;
    if (fEdges.size() == 0)
        fLength = fLen;
}

double Ratio::getRatio() const {
    std::cout << "Ratio::getRatio()" << std::endl;
    return eLength / fLength;
}

double Ratio::getTime() {
    std::cout << "Ratio::getTime()" << std::endl;
    return eLength / (eLength + fLength);
}

Ratio Ratio::reverse() {
    std::cout << "Ratio::reverse()" << std::endl;
    vector<PhyloTreeEdge> evec, fvec;
    for (auto &e : eEdges) {
        evec.push_back(PhyloTreeEdge(e));
    }
    for (auto &f : fEdges) {
        fvec.push_back(PhyloTreeEdge(f));
    }
    return Ratio(fvec, evec);
}

bool Ratio::containsOriginalEEdge(const Bipartition &edge) {
    std::cout << "Ratio::containsOriginalEEdge(const Bipartition &edge)" << std::endl;
    for (PhyloTreeEdge &ratio_edge : eEdges) {
        if (ratio_edge.getOriginalEdge().equals(edge)) {
            return true;
        }
    }

    for (PhyloTreeEdge &ratio_edge : fEdges) {
        if (ratio_edge.getOriginalEdge().equals(edge)) {
            return true;
        }
    }
    return false;
}

string Ratio::toString() {
    std::cout << "Ratio::toString()" << std::endl;
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
    std::cout << "Ratio::clone()" << std::endl;
    return Ratio(*this);
}

string Ratio::toStringCombType() {
    std::cout << "Ratio::toStringCombType()" << std::endl;
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
    std::cout << "Ratio::toStringVerbose(vector<string> leaf2NumMap)" << std::endl;
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(8);

    ss << getRatio() << endl;

    ss << "Total length and corresponding edges dropped:" << endl;

    ss << eLength << "\t";

    // list the edges dropped
    for (int i = 0; i < eEdges.size(); i++) {
        if (i == 0) {   // nice formatting
            ss << Bipartition::toStringVerbose(*(eEdges[i].getPartition()), leaf2NumMap) << endl;
        } else {
            ss << "\t\t" << Bipartition::toStringVerbose(*(eEdges[i].getPartition()), leaf2NumMap) << endl;
        }
    }

    ss << "\nTotal length and corresponding edges added:" << endl;

    ss << fLength << "\t";

    // list the edges added
    for (int i = 0; i < fEdges.size(); i++) {
        if (i == 0) {   // nice formatting
            ss << Bipartition::toStringVerbose(*(fEdges[i].getPartition()), leaf2NumMap) << endl;
        } else {
            ss << "\t\t" << Bipartition::toStringVerbose(*(fEdges[i].getPartition()), leaf2NumMap) << endl;
        }
    }

    return ss.str();
}
