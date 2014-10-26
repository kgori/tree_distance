#include "RatioSequence.h"
#include <random>

RatioSequence::RatioSequence(){

}

/*
ptA and ptB are coordinate strings in the format "1,2,3"
 */
RatioSequence::RatioSequence(string ptA, string ptB) {
    auto strvecA = Tools::string_split(ptA, ", ", "(,)");
    auto strvecB = Tools::string_split(ptB, ", ", "(,)");
    auto dblvecA = Tools::stringvec_to_doublevec(strvecA);
    auto dblvecB = Tools::stringvec_to_doublevec(strvecB);
    if (!(dblvecA.size() == dblvecB.size()))
        throw std::invalid_argument("Coordinates have different numbers of dimensions: (" + ptA + "),(" + ptB + ")");
    for (size_t i=0; i < dblvecA.size(); ++i) {
        this->push_back(Ratio(dblvecA[i], dblvecB[i]));
    }
}

RatioSequence RatioSequence::interleave(RatioSequence rs1, RatioSequence rs2) {
    RatioSequence combined1 = rs1.getNonDesRSWithMinDist();
    RatioSequence combined2 = rs2.getNonDesRSWithMinDist();

    RatioSequence interleavedRS = RatioSequence();
    size_t index1 = 0;  // index for stepping through combined1
    size_t index2 = 0;  // index for stepping through combined2
    while (index1 < combined1.size() && index2 < combined2.size()) {
        if (combined1[index1].getRatio() <= combined2[index2].getRatio()) {
            interleavedRS.push_back(combined1[index1]);
            index1++;
        } else {
            interleavedRS.push_back(combined2[index2]);
            index2++;
        }
    }
    // if we have finished adding rs2 but not rs1
    while (index1 < combined1.size()) {
        interleavedRS.push_back(combined1[index1]);
        index1++;
    }
    // if we have finished adding rs1 but not rs2
    while (index2 < combined2.size()) {
        interleavedRS.push_back(combined2[index2]);
        index2++;
    }
    return interleavedRS;
}

RatioSequence RatioSequence::getRandomRS(size_t dim) {
    std::default_random_engine dre;
    auto ud = std::uniform_real_distribution<>();
    vector<string> ptA_vec;
    vector<string> ptB_vec;
    double randval;

    for (size_t i=0; i < dim; ++i) {
        randval = (ud(dre) + 0.1) / 1.1;
        ptA_vec.push_back(Tools::double_to_string(randval));
        randval = (ud(dre) + 0.1) / 1.1;
        ptB_vec.push_back(Tools::double_to_string(randval));
    }
    return RatioSequence(Tools::string_join(ptA_vec, ","), Tools::string_join(ptB_vec, ","));
}

int RatioSequence::getCombineCode() {
    return combineCode;
}

void RatioSequence::setCombineCode(int c) {
    combineCode = c;
}

Ratio RatioSequence::getRatio(int i) {
    (*this)[i];
}

bool RatioSequence::isAscending() {
    return false;
}

double RatioSequence::getDistance() {
    return 0;
}

double RatioSequence::getMinNonDesRSDistance() {
    return 0;
}

RatioSequence RatioSequence::clone() {
    return RatioSequence();
}

RatioSequence RatioSequence::getCombinedRS(int combineCode) {
    return RatioSequence();
}

RatioSequence RatioSequence::getNonDesRSWithMinDist() {
    return RatioSequence();
}

RatioSequence RatioSequence::getAscRSWithMinDist() {
    return RatioSequence();
}

RatioSequence RatioSequence::reverse() {
    return RatioSequence();
}

string RatioSequence::toStringValueAndRatio() {
    return __1::basic_string<char, char_traits<char>, allocator<char>>();
}

string RatioSequence::toStringValue() {
    return __1::basic_string<char, char_traits<char>, allocator<char>>();
}

string RatioSequence::toStringVerbose(vector<string> leaf2NumMap) {
    return __1::basic_string<char, char_traits<char>, allocator<char>>();
}

string RatioSequence::toStringCombType() {
    return __1::basic_string<char, char_traits<char>, allocator<char>>();
}

string RatioSequence::toStringCombTypeAndValue() {
    return __1::basic_string<char, char_traits<char>, allocator<char>>();
}
