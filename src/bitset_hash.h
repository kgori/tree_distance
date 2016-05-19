//
// Created by Kevin Gori on 19/05/2016.
//
#ifndef BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#define BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
#endif
#include <boost/dynamic_bitset.hpp>
#include <boost/functional/hash.hpp>

#ifndef CGTP_BITSET_HASH_H
#define CGTP_BITSET_HASH_H

using bitset_t = boost::dynamic_bitset<>;

struct BitsetHash {
    size_t operator()(const bitset_t &bitset) const {
        return boost::hash_value(bitset.m_bits);
    }
};

#endif //CGTP_BITSET_HASH_H
