#include <cstdio>
#include <vector>
#include <string>
#include <stdexcept>
#include <iterator>
#include <iostream>
#include "exception"
#ifndef EX6_MEITAR453_HASHMAP_HPP
#define EX6_MEITAR453_HASHMAP_HPP
#define BASE_CAP 16
#define BAD_SIZE "vectors length not equal"
#define SQURE 2
#define HIGHLIM 0.75
#define LOWLIM 0.25
#define KEY_PROB "key not in table"
using std::vector ;
using std::pair ;
using std::string ;


template<typename KeyT, typename ValueT> class HashMap{
    typedef pair<KeyT,ValueT> single_pair ;
    typedef vector<single_pair> pair_vec ;
    typedef pair_vec pair_vec_array[] ;

protected:
    pair_vec_array *main_array_p_ ;
    int size_ ;
    int capacity_ ;

public:

    HashMap(): size_(0), capacity_(BASE_CAP), main_array_p_(new std::vector<std::pair<KeyT,
            ValueT>>[BASE_CAP]) {}

    HashMap (const vector<KeyT> &keys, const vector<ValueT> &values) : HashMap(){
        if (keys.size () != values.size ())
        {
            throw (std::domain_error{"vectors length not equal"});
        }
        size_ = 0 ;
        capacity_ = BASE_CAP ;
        for (size_t i = 0; i < keys.size (); i++) {
            if (contains_key(keys[i])) {
                at(keys[i]) = values[i];
            }
            else {
                insert(keys[i], values[i]);
            }
        }
    }


    HashMap (const HashMap &map_to_copy)
    {
        capacity_ = map_to_copy.capacity_;
        size_ = map_to_copy.size_;
        main_array_p_ = new vector<pair<KeyT, ValueT>>[capacity_];
        for (int i = 0; i < capacity_; i++)
        {
            main_array_p_[i] = map_to_copy.main_array_p_[i];
        }
    }

    virtual ~HashMap() {
        delete[] main_array_p_ ;
    }

    HashMap& operator= (const HashMap& data) {
        if (this == &data){
            return *this ;
        }
        std::swap(this->main_array_p_, data) ;
        return (*this) ;
    }

    bool operator== (const HashMap map) const{
        if (this->size_ != map.size_ || this->capacity_ != map.capacity_) {
            return false ;
        }
        for(int i = 0 ; i < capacity_; i++) {
            if (sizeof(main_array_p_[i]) != sizeof(map.in_data[i])) {
                return false ;
            }
            for (int j = 0 ; j < sizeof(main_array_p_[i]) ; j++ ) {
                if (main_array_p_[i][j].first != map.in_data[i][j].first ||
                                    main_array_p_[i][j].second != map.in_data[i][j].second){
                    return false ;
                }
            }
        }
        return true ;
    }

    bool operator!= (const HashMap map) const{
        return !(this->operator==(map)) ;
    }

    int bucket_size(KeyT key){
        return main_array_p_[hush_func(key)].size() ;
    }

    ValueT &operator[] (const KeyT &key) {
        // hash func -> v mod size = v & (size − 1)

        int indx = hush_func(key);
        int i = 0;
        while (main_array_p_[indx][i].first != key || i != size_ + 1) {
            i++;
        }
        if (i != size_) {
            return main_array_p_[indx][i].second;
        }
        return NULL ;
    }

    ValueT& operator[] (const KeyT &key) const{
        if(key >= capacity_ || key < 0){
            throw std::out_of_range(KEY_PROB) ;
        }
        return this[key] ;
    }

    int hush_func(const KeyT &key) const {
        return (int)(std::hash<KeyT>{} (key) && (capacity_ - 1)) ;
    }

    int size(){ return size_ ;}

    int capacity(){return capacity_ ; }

    bool empty(){return size_ != 0 ;}

    bool contains_key(KeyT &key_to_check) {
        int indx = hush_func(key_to_check) ;
        for (int i = 0; i < main_array_p_[indx]->size(); ++i) {
            if (main_array_p_[indx][i].first == key_to_check)
            {
                return true ;
            }
        }
        return false ;
    }
    bool contains_key(const KeyT key_to_check) const {
        int indx = hush_func(key_to_check) ;
        for (int i = 0; i < main_array_p_[indx]->size(); ++i) {
            if (main_array_p_[indx][i].first == key_to_check)
            {
                return true ;
            }
        }
        return false ;
    }

    double get_load_factor(){
        double load_factor = (((double)size_)/capacity_) ;
        return load_factor ;
    }

    void rehash(){
        pair_vec  new_vec = {} ;
        int j = 0 ;
        for (int i = 0; i < capacity_; ++i) {
            while(main_array_p_[i][j]) {
                new_vec.insert(main_array_p_[i][j]) ;
                j++ ;
            }
            j = 0 ;
        }
        pair_vec_array array_of_pairs = new_vec ;
        HashMap new_hash = new HashMap(array_of_pairs, sizeof(new_vec), sizeof(new_vec)^SQURE) ;
        swap(this, new_hash) ;
    }


    bool insert(KeyT key, ValueT value){
        if(contains_key(key)){
            return false;
        }
        int indx = hush_func(key) ;
        int i = 0 ;
        while(main_array_p_[indx][i]){
            i++ ;
        }
        main_array_p_[indx].emplace_back(pair_vec()) ;
        main_array_p_[indx][i].first = key ;
        main_array_p_[indx][i].second = value ;
        size_++ ;
        double load_factor = get_load_factor() ;
        double high_load_factor = HIGHLIM ;
        if (load_factor > high_load_factor) {
            rehash() ;
        }
        return true ;
    }

    virtual bool erase (const KeyT &key){
        if(!contains_key(key)){
            return false;
        }
        int indx = hush_func(key) ;
        int i = 0 ;
        while(main_array_p_[indx][i].first != key ){
            i++ ;
        }
        main_array_p_[indx].erase(i) ;
        size_-- ;
        if (get_load_factor() < LOWLIM) {
            rehash() ;
        }
        return true ;
    }

    ValueT& at (const KeyT key) const{
        ValueT val = operator[](key) ;
        if ( val == NULL){
            throw std::runtime_error(KEY_PROB);
        }
        return val ;
    }

    int bucket_index(KeyT key){
        if (contains_key(key)){
            return hush_func(key);
        }
        throw std::runtime_error(KEY_PROB);
    }

    void clear(){
        int j = 0 ;
        for (int i = 0; i < capacity_; ++i) {
            j = 0 ;
            while (main_array_p_[i][j]){
                main_array_p_[i].erase(j) ;
                j++ ;
            }
        }
    }


    class ConstIterator {
        friend class HashMap ;
    private:
        const HashMap<KeyT,ValueT>& map_;
        int current_bucket_;
        int current_pos_;
        explicit ConstIterator(const HashMap<KeyT,ValueT> &data) : map_(data), current_bucket_(0), current_pos_(0) {}
    public:
        typedef std::pair<KeyT, ValueT> value_type;
        typedef const value_type &reference;
        typedef const value_type *pointer;
        typedef std::ptrdiff_t difference_type;
        typedef std::forward_iterator_tag iterator_category;

        ConstIterator& operator++ () {
            if(current_pos_+ 1 < sizeof(map_[current_bucket_])){
                current_pos_++ ;
            } else if (current_bucket_+1 < sizeof(map_)){
                current_pos_ = 0 ;
                current_bucket_++ ;
            }
            return *this ;
        }

        ConstIterator operator++ (int){
            ConstIterator copy(*this) ;
            this->operator++() ;
            return *this ;
        }

        bool operator== (const ConstIterator &pair_to_check) const {
            bool flag = (&map_ == &pair_to_check.map_ && current_bucket_ ==
                    pair_to_check.current_bucket_ && current_pos_ == pair_to_check.current_pos_) ;
            return flag ;
        }

        bool operator!= (const ConstIterator &pair_to_check) const{
            return !this->operator==(pair_to_check) ;
        }

        reference operator* () const{
            return map_.main_array_p_[this->current_bucket_][this->current_pos_] ;
        }

        pointer operator-> () const{
            return &this->operator*() ;
        }
    };
    using const_iterator = ConstIterator;


    ConstIterator cbegin() const{
        int i = 0 ;
        while (!main_array_p_[i][0]){i++ ;}
        return main_array_p_[i][0] ;
    }

    ConstIterator cend() const{
        int indx = bucket_index(size_) ;
        int i = 0 ;
        while (main_array_p_[indx][i]){
            i++ ;
        }
        return main_array_p_[indx][i] ;
    }

    ConstIterator begin() const { return cbegin();};

    ConstIterator end() const { return cend();};

    void swap (HashMap other) {
        std::swap (this->main_array_p_, other.main_array_p_);
        std::swap (this->size_, other.size_);
        std::swap (this->capacity_, other.capacity_);
    }

};



#endif //EX6_MEITAR453_HASHMAP_HPP