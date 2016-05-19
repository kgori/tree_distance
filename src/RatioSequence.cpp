#ifndef BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#define BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#endif
#include "RatioSequence.h"
#include "Tools.h"
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
    size_t len = dblvecA.size();
    _RatioSequence.reserve(len);
    for (size_t i=0; i < len; ++i) {
        this->push_back_value(Ratio(dblvecA[i], dblvecB[i]));
    }
}

RatioSequence RatioSequence::interleave(RatioSequence& rs1, RatioSequence& rs2) {
    RatioSequence combined1 = rs1.getNonDesRSWithMinDist();
    RatioSequence combined2 = rs2.getNonDesRSWithMinDist();

    RatioSequence interleavedRS = RatioSequence();
    interleavedRS._RatioSequence.reserve(combined1.size() + combined2.size());
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

//RatioSequence RatioSequence::getRandomRS(size_t dim) {
//    std::default_random_engine dre;
//    auto ud = std::uniform_real_distribution<>();
//    vector<string> ptA_vec;
//    vector<string> ptB_vec;
//    double randval;
//
//    for (size_t i=0; i < dim; ++i) {
//        randval = (ud(dre) + 0.1) / 1.1;
//        ptA_vec.push_back(Tools::double_to_string(randval));
//        randval = (ud(dre) + 0.1) / 1.1;
//        ptB_vec.push_back(Tools::double_to_string(randval));
//    }
//    return RatioSequence(Tools::string_join(ptA_vec, ","), Tools::string_join(ptB_vec, ","));
//}

//long RatioSequence::getCombineCode() {
//    return combineCode;
//}

void RatioSequence::setCombineCode(long c) {
    combineCode = c;
}

Ratio RatioSequence::getRatio(size_t i) {
    return (*this)[i];
}

//bool RatioSequence::isAscending() {
//    return std::is_sorted(this->begin(), this->end());
//}

double RatioSequence::getDistance() {
    double dist_sqd = 0.0;
    for(auto &ratio : *this) {
        dist_sqd += pow(ratio.getELength() + ratio.getFLength(), 2);
    }
    return sqrt(dist_sqd);
}

//double RatioSequence::getMinNonDesRSDistance() {
//    return getNonDesRSWithMinDist().getDistance();
//}

RatioSequence RatioSequence::clone() {
    return RatioSequence(*this);
}

//RatioSequence RatioSequence::getCombinedRS(long combineCode) {
//    RatioSequence combinedRS = RatioSequence(); // new ratio sequence formed by combining.
//    Ratio ratioToCombineWith = _RatioSequence.front(); // the ratio that we will combine the next ratio with if it is indicated we should combine the ratios
//
//    // loop though the rs.size() - 1 ways to combine the ratios
//    // each is represented as 2^j
//    for (size_t j = 0; j < _RatioSequence.size() - 1; j++) {
//        // if i contains j
//        if ((combineCode & (long) pow(2, j)) == pow(2, j)) {
//            //combine ratio j and ratio j+1
//            ratioToCombineWith = Ratio::combine(ratioToCombineWith, _RatioSequence[j + 1]);
//        } else {
//            // move ratio j (and whatever earlier ratios it is combined with) into the vector combinedRS
//            combinedRS.push_back(ratioToCombineWith);
//            // ratio j+1 becomes the ratioToCombineWith
//            ratioToCombineWith = _RatioSequence[j + 1];
//        }
//    }
//    // add last ratio to the ratio sequence
//    combinedRS.push_back(ratioToCombineWith);
//
//    return combinedRS;
//}

RatioSequence RatioSequence::getNonDesRSWithMinDist() {
    if (this->size() < 2) {
        return *this;
    }
    RatioSequence combinedRS(*this);
    size_t i = 0;   // index for stepping through rs
    long combineCode = 0;
    vector<int> ccArray(this->size(), 0);

    int a = 0; // array index

    while (i < combinedRS.size() - 1) {
        if (combinedRS[i].getRatio() > combinedRS[i + 1].getRatio()) {
            Ratio combinedRatio = Ratio::combine(combinedRS[i], combinedRS[i + 1]);
            combinedRS.erase(i);
            combinedRS.erase(i);
            combinedRS._RatioSequence.insert(combinedRS._RatioSequence.begin() + i, combinedRatio);
            ccArray[a] = 1;
            if (i > 0) {
                i--;
                // go back in the array to the last non-combined ratio
                while (ccArray[a] == 1) {
                    a--;
                }
            } else {
                // we must advance a
                while ((a < this->size() - 1) && (ccArray[a] != 2)) {
                    a++;
                }
            }
        } else {
            ccArray[a] = 0;
            // the ratios are not-descending, so go on to the next pair
            i++;
            // we must jump ahead to the next ratio not considered for combination
            while ((a < this->size() - 1) && (ccArray[a] != 2)) {
                a++;
            }
        }
    }

    for (size_t k = 0; k < this->size() - 1; k++) {
        if (ccArray[k] == 1) {
            combineCode += pow(2, k);
        }
    }
    combinedRS.setCombineCode(combineCode);

    return combinedRS;
}

//RatioSequence RatioSequence::getAscRSWithMinDist() {
//    if (this->size() < 2) {
//        return *this;
//    }
//    RatioSequence combinedRS(*this);
//    size_t i = 0;   // index for stepping through rs
//    long combineCode = 0;
//    vector<int> ccArray(this->size(), 2);  // array storing which ratios have been combined
//                                          // initialize array to 0.  Will mark a 1 if ratios combined.
//
//    size_t a = 0; // array index
//
//    while (i < combinedRS.size() - 1) {
//        if (combinedRS[i].getRatio() >= combinedRS[i + 1].getRatio()) {
//            Ratio combinedRatio = Ratio::combine(combinedRS[i], combinedRS[i + 1]);
//            combinedRS.erase(i);
//            combinedRS.erase(i);
//            combinedRS._RatioSequence.insert(combinedRS._RatioSequence.begin() + i, combinedRatio);
//            ccArray[a] = 1;
//            if (i > 0) {
//                i--;
//                // go back in the array to the last non-combined ratio
//                while (ccArray[a] == 1) {
//                    a--;
//                }
//            } else {
//                // we must advance a
//                while ((a < this->size() - 1) && (ccArray[a] != 2)) {
//                    a++;
//                }
//            }
//        } else {
//            ccArray[a] = 0;
//            // the ratios are not-descending, so go on to the next pair
//            i++;
//            // we must jump ahead to the next ratio not considered for combination
//            while ((a < this->size() - 1) && (ccArray[a] != 2)) {
//                a++;
//            }
//        }
//    }
//
//    for (size_t k = 0; k < this->size() - 1; k++) {
//        if (ccArray[k] == 1) {
//            combineCode += pow(2, k);
//        }
//    }
//    combinedRS.setCombineCode(combineCode);
//
//    return combinedRS;
//}

//RatioSequence RatioSequence::reverse() {
//    RatioSequence revRS = RatioSequence();
//    for (auto i = _RatioSequence.size() - 1; i > 0; --i) {
//        revRS.push_back(_RatioSequence[i].reverse());
//    }
//    revRS.push_back(_RatioSequence[0].reverse());
//    return revRS;
//}

//string RatioSequence::toStringValueAndRatio() {
//    return __1::basic_string<char, char_traits<char>, allocator<char>>();
//}
//
//string RatioSequence::toStringValue() {
//    return __1::basic_string<char, char_traits<char>, allocator<char>>();
//}
//
//string RatioSequence::toStringVerbose(vector<string> leaf2NumMap) {
//    return __1::basic_string<char, char_traits<char>, allocator<char>>();
//}
//
//string RatioSequence::toStringCombType() {
//    return __1::basic_string<char, char_traits<char>, allocator<char>>();
//}
//
//string RatioSequence::toStringCombTypeAndValue() {
//    return __1::basic_string<char, char_traits<char>, allocator<char>>();
//}
size_t RatioSequence::size() { return _RatioSequence.size(); }

void RatioSequence::push_back(Ratio& item) {
    _RatioSequence.push_back(item);
}

void RatioSequence::push_back_value(Ratio item) {
    _RatioSequence.push_back(std::move(item));
}

void RatioSequence::erase(size_t index) {
    Tools::vector_remove_element_at_index(_RatioSequence, index);
}

//void RatioSequence::insert(vector<Ratio>::iterator index, Ratio item) {
//    _RatioSequence.insert(index, item);
//}

vector<Ratio>::iterator RatioSequence::begin() {
    return _RatioSequence.begin();
}

vector<Ratio>::iterator RatioSequence::end() {
    return _RatioSequence.end();
}

RatioSequence::RatioSequence(const RatioSequence &other) {
    _RatioSequence = other._RatioSequence;
    combineCode = other.combineCode;
}

string RatioSequence::toString() {
    ostringstream ss;
    for (size_t i = 0; i < _RatioSequence.size() - 1; ++i) {
        ss << _RatioSequence[i].toString() << " ";
    }
    ss << _RatioSequence.back().toString();
    return ss.str();
}

//string RatioSequence::toStringCombType() {
//    string s = "";
//
//    for (int i = 0; i < this->size(); i++) {
//        s += _RatioSequence[i].toStringCombType() + ";";
//    }
//    return s;
//}

//string RatioSequence::toStringVerbose(vector<string> leaf2NumMap) {
//    stringstream ss;
//    for (int i = 0; i < _RatioSequence.size(); i++) {
//        ss << "\nRatio " << i << ":  ";
//        ss << _RatioSequence[i].toStringVerbose(leaf2NumMap);
//    }
//
//    return ss.str();
//}
