#ifndef EX6_MEITAR453_DICTIONARY_HPP
#define EX6_MEITAR453_DICTIONARY_HPP
#include "HashMap.hpp"
#define KEY_ERR "Invalid key argument"
using std::string;
using std::pair;
using std::vector ;
using iterator = vector<pair<string,string>>::const_iterator;
class InvalidKey: public std::invalid_argument {
public:
    using std::invalid_argument::invalid_argument;
    InvalidKey() : std::invalid_argument(KEY_ERR) {};
    explicit InvalidKey (const string &err_mssg) : std::invalid_argument(err_mssg){};
};
class Dictionary :
                        public HashMap<string , string> {
public:
    using HashMap<std::basic_string<char>,std::basic_string<char>>::HashMap ;


    bool erase(const string &key) override {
        if(contains_key(key)){
            return HashMap<std::basic_string<char>,std::basic_string<char>>::erase(key) ;
        }
        throw InvalidKey() ;
    }

    void update(const iterator & it_begin, const iterator & it_end) {
        while (it_begin!=it_end)
        {
            if(contains_key((it_begin->first)))
            {
                at(it_begin->first) =  (*it_begin).second;
                return;
            }
            insert(it_begin->first, it_begin->second);
            it_begin.operator+(1);
        }
    }

};



#endif //EX6_MEITAR453_DICTIONARY_HPP
