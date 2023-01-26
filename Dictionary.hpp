
#ifndef EX6_MEITAR453_DICTIONARY_HPP
#define EX6_MEITAR453_DICTIONARY_HPP
#include "HashMap.hpp"


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
    virtual bool erase(const string& key) final {
        if(this.contains_key(key)){ throw InvalidKey() ;}
        return this.erase(key);
    }
    bool update(const iterator& it_begin, const iterator& it_end) {
        while (it_begin->first != it_end->first){
            this->insert(it_begin->first, it_begin->second) ;
            it_begin += it_begin.operator+() ;
        }
        if (it_begin->second != it_end->second ){
            return false ;
        }
        return true ;
    }

};



#endif //EX6_MEITAR453_DICTIONARY_HPP
