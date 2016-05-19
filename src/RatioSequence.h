#ifndef __RATIO_SEQUENCE_H__
#define __RATIO_SEQUENCE_H__
#ifndef BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#define BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#endif
#include "Ratio.h"
#include <string>
#include <vector>

using namespace std;

class RatioSequence {

public:
    RatioSequence();

    RatioSequence(string ptA, string ptB);

    RatioSequence(const RatioSequence& other);

    static RatioSequence interleave(RatioSequence& rs1, RatioSequence& rs2);

//    static RatioSequence getRandomRS(size_t dim);

    Ratio& operator[](size_t index) { return _RatioSequence[index]; }

    const Ratio& operator[](size_t index) const { return _RatioSequence[index]; }

//    long getCombineCode();

    void setCombineCode(long c);

    Ratio getRatio(size_t i);

//    bool isAscending();

    double getDistance();

//    double getMinNonDesRSDistance();

    RatioSequence clone();

//    RatioSequence getCombinedRS(long combineCode);

    RatioSequence getNonDesRSWithMinDist();

//    RatioSequence getAscRSWithMinDist();

//    RatioSequence reverse();

    void push_back_value(Ratio item);

    void push_back(Ratio& item);

    void erase(size_t index);

//    void insert(vector<Ratio>::iterator index, Ratio item);

    vector<Ratio>::iterator begin();

    vector<Ratio>::iterator end();

    size_t size();

    string toString();

//    string toStringValueAndRatio();
//
//    string toStringValue();
//
//    string toStringVerbose(vector<string> leaf2NumMap);

//    string toStringCombType();

//
//    string toStringCombTypeAndValue();

private:
    long combineCode = 0;
    vector<Ratio> _RatioSequence;
};

#endif /* __RATIO_SEQUENCE_H__ */
