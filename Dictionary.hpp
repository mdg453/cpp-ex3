//
// Created by meita on 17/01/2023.
//

#ifndef EX6_MEITAR453_DICTIONARY_HPP
#define EX6_MEITAR453_DICTIONARY_HPP
#include "HashMap.hpp"
#include <iostream>
#include <string>
using std::string ;
using std::pair;
using std::vector ;

class Dictionary : public HashMap<string, string> {
public:
    using HashMap<string,string>::HashMap ;

    virtual bool erase (const string& key) override{
        if (!HashMap::contains_key(key)) {
            return false;
        } else {
            int indx = hush_func(key);
            int i = 0;
            while (main_vec_[indx][i].first != key) {
                i++;
            }
            main_vec_[indx].erase(i);
            size_--;
            double load_factor = get_load_factor();
            double lower_load_factor = 0.25;
            if (load_factor < lower_load_factor) {
                rehash();
            }
            return true;
        }
    }




};



#endif //EX6_MEITAR453_DICTIONARY_HPP
