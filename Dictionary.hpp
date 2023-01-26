
#ifndef EX6_MEITAR453_DICTIONARY_HPP
#define EX6_MEITAR453_DICTIONARY_HPP
#include "HashMap.hpp"
#include <iostream>
#include <string>
#include <stdexcept>
#include <iterator>

using std::string ;
using std::pair;
using std::vector ;
using iterator = std::vector<pair<string,string>>::const_iterator;

class InvalidKey: public std::invalid_argument {
public:
    using std::invalid_argument::invalid_argument;
    InvalidKey () : InvalidKey("Invalid key argument"){};
};


class Dictionary : public HashMap<std::string, std::string> {
public:
    using HashMap<string,string>::HashMap ;
    virtual bool erase(const string& key) override {
        if(!contains_key(key)){ throw InvalidKey() ;}
        return HashMap::erase(key);
    }
    bool update(const iterator& it_begin, const iterator& it_end) {


    }




};



#endif //EX6_MEITAR453_DICTIONARY_HPP
