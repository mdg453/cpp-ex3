//
// Created by meita on 17/01/2023.
//
#include <stdio.h>
#include <vector>
#include <iostream>
#ifndef EX6_MEITAR453_HASHMAP_HPP
#define EX6_MEITAR453_HASHMAP_HPP
using std::vector ;
using std::pair ;



template <typename KeyT, typename ValueT> class HashMap{
    typedef vector<pair<KeyT,ValueT>> buc_vec ;
    typedef buc_vec buc_vec_array[] ;
public:

    explicit HashMap(const buc_vec_array& data) : in_data(data) {}
    explicit HashMap() : in_data(new buc_vec_array(16)) {}
    virtual ~HashMap() {}

    HashMap& operator= (const HashMap& data) {
        if (this == &data){
            return *this ;
        }
        std::swap(this->in_data, data) ;
        return (*this) ;
    }

    ValueT operator[] (KeyT key){

    }


private:
    buc_vec_array in_data;
};


#endif //EX6_MEITAR453_HASHMAP_HPP