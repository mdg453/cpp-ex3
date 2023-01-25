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
    typedef vector<pair<KeyT&,ValueT&>> buc_vec ;
    typedef buc_vec buc_vec_array[] ;
public:
    HashMap(int size, int capasity, buc_vec_array& data) :
            size_(size), capasity_(capasity), main_vec_(data) {}
    HashMap(const int size, const int capasity, const buc_vec_array& data) :
                    size_(size), capasity_(capasity), main_vec_(data) {}
//  HashMap(const int size, const int capasity, const KeyT& keys, const ValueT values) :
//                    size_(size), capasity_(capasity), main_vec_(buc_vec_array (buc_vec (array<pair<keys,values>)>) {}

    HashMap() : main_vec_(new buc_vec_array(16) ,size_ = 0  , capasity_ = 16  ) {}

    ~HashMap() {
        for (int i = 0; i < capasity_; ++i) {
            for (int j = 0; j < main_vec_[i].size() ; ++j) {
                delete main_vec_[i][j].first ;
                delete main_vec_[i][j].second ;
            }
        }
    }

    HashMap& operator= (const HashMap& data) {
        if (this == &data){
            return *this ;
        }
        std::swap(this->in_data, data) ;
        return (*this) ;
    }

    int hush_func(const KeyT &key) const {
        return (int)(std::hash<KeyT>{} (key) && (sizeof(main_vec_))) ;
    }

    int size(){ return size_ ;}
    int capacity(){return capasity_ ; }
    bool empty(){return size_ != 0 ;}

    bool contains_key(KeyT key_to_check) {
        int indx = hush_func(key_to_check) ;
        for (int i = 0; i < sizeof(main_vec_[indx]); ++i) {
            if (main_vec_[indx][i].first == key_to_check)
            {
                return true ;
            }
        }
        return false ;
    }

    double get_load_factor(){
        double load_factor = (((double)size_)/capasity_) ;
        return load_factor ;
    }

    void rehash(){
        buc_vec  new_vec = {} ;
        int j = 0 ;
        for (int i = 0; i < capasity_; ++i) {
            while(main_vec_[i][j]) {
                new_vec.insert(main_vec_[i][j]) ;
                j++ ;
            }
            j = 0 ;
        }
        buc_vec_array array_of_pairs = new_vec ;
        HashMap new_hash = new HashMap(array_of_pairs, sizeof(new_vec), sizeof(new_vec)^2) ;
        swap(this, new_hash) ;
    }

    bool insert(KeyT key, ValueT value){
        if(contains_key(key)){
            return false;
        }
        int indx = hush_func(key) ;
        int i = 0 ;
        while(main_vec_[indx][i]){
            i++ ;
        }
        main_vec_[indx].emplace_back(buc_vec()) ;
        main_vec_[indx][i].first = key ;
        main_vec_[indx][i].second = value ;
        size_++ ;
        double load_factor = get_load_factor() ;
        double high_load_factor = 0.75 ;
        if (load_factor > high_load_factor) {
            rehash() ;
        }
        return true ;
    }

    virtual bool erase (KeyT key){
        if(!contains_key(key)){
            return false;
        }
        int indx = hush_func(key) ;
        int i = 0 ;
        while(main_vec_[indx][i].first != key ){
            i++ ;
        }
        main_vec_[indx].erase(i) ;
        size_-- ;
        double load_factor = get_load_factor() ;
        double lower_load_factor = 0.25 ;
        if (load_factor < lower_load_factor) {
            rehash() ;
        }
        return true ;
    }

    ValueT operator[] (const KeyT key) const {
        // hash func -> v mod size = v & (size − 1)

        int indx = hush_func(key);
        int i = 0;
        while (main_vec_[indx][i].first != key || i != size_ + 1) {
            i++;
        }
        if (i != size_) {
            return main_vec_[indx][i].second;
        }
        return NULL ;
    }


    ValueT at (const KeyT key) const{
        ValueT val = operator[](key) ;
        if ( val == NULL){
            throw std::runtime_error("key not in table");
        }
        return val ;
    }

    bool operator== (const HashMap map) const{
        if (sizeof(main_vec_) != sizeof(map.in_data)) {
            return false ;
        }
        for(int i = 0 ; i < sizeof(main_vec_); i++) {
            if (sizeof(main_vec_[i]) != sizeof(map.in_data[i])) {
                return false ;
            }
            for (int j = 0 ; j < sizeof(main_vec_[i]) ; j++ ) {
                if (main_vec_[i][j].first != map.in_data[i][j].first || main_vec_[i][j].second != map.in_data[i][j].second){
                    return false ;
                }
            }
        }
        return true ;
    }

    bool operator!= (const HashMap map) const{
        return !(this == map) ;
    }

    int bucket_size(KeyT key){
        int indx = hush_func(key) ;
        return sizeof(main_vec_[indx]) ;
    }

    int bucket_index(KeyT key){
        if (contains_key(key)){
            return hush_func(key);
        }
        throw std::runtime_error("no suck key");
    }
    void clear(){
        int j = 0 ;
        for (int i = 0; i < capasity_; ++i) {
            j = 0 ;
            while (main_vec_[i][j]){
                main_vec_[i].erase(j) ;
                j++ ;
            }
        }
    }
    class const_iterator {
    public:
        const_iterator(const buc_vec_array& data, int size) : main_vec_(data), size_(size), current_bucket_(0), current_pos_(0) {}

        const pair<KeyT,ValueT>& operator*() const {
            return main_vec_[current_bucket_][current_pos_];
        }

        const_iterator& operator++() {
            current_pos_++;
            if (current_pos_ == main_vec_[current_bucket_].size()) {
                current_pos_ = 0;
                current_bucket_++;
                while (current_bucket_ < size_ && main_vec_[current_bucket_].empty())
                    current_bucket_++;
            }
            return *this;
        }

        bool operator!=(const const_iterator& other) const {
            return current_bucket_ != other.current_bucket_ || current_pos_ != other.current_pos_;
        }

    private:
        const buc_vec_array& main_vec_;
        int size_;
        int current_bucket_;
        int current_pos_;
    };


protected:
    int size_ ;
    int capasity_ ;
    buc_vec_array main_vec_ ;

};



#endif //EX6_MEITAR453_HASHMAP_HPP