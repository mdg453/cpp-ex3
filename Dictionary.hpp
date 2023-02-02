#ifndef EX6_MEITAR453_DICTIONARY_HPP
#define EX6_MEITAR453_DICTIONARY_HPP
#include "HashMap.hpp"
#define KEY_ERR "Invalid key argument"
using std::string;
using std::pair;
using std::vector ;
using iterator = HashMap<string,string>::const_iterator ;

class InvalidKey: public std::invalid_argument {
public:
    using std::invalid_argument::invalid_argument;
    InvalidKey() : std::invalid_argument(KEY_ERR) {};
    explicit InvalidKey (const string &err_mssg) : std::invalid_argument(err_mssg){};
};


class Dictionary : public HashMap<std::string, std::string> {
public:
    using HashMap<string,string>::HashMap ;
//    Dictionary() = default ;
//    Dictionary(const vector<string>& keys, const vector<string>& values):
//            HashMap<string,string>(keys, values){};
    bool erase(const string& key) final {
        if(HashMap<string ,string>::contains_key(key)){
            return HashMap<std::basic_string,std::basic_string>::erase(key) ;
        }
        throw InvalidKey() ;
    }

    void update(const iterator & it_begin, const iterator & it_end) {
        while (it_begin!=it_end)
        {
            if(contains_key((it_begin->first))
            {
                HashMap<std::string, std::string>::at ((*it_begin).first) =
                        (*begin_iterator).second;
            }
            HashMap<std::string, std::string>::insert((*begin_iterator).first,
                                                      (*begin_iterator).second);
            it_begin++;
        }
    }

};



#endif //EX6_MEITAR453_DICTIONARY_HPP
