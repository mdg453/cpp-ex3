#include <cstdio>
#include <vector>
#include <iostream>
#ifndef EX6_MEITAR453_HASHMAP_HPP
#define EX6_MEITAR453_HASHMAP_HPP
#define BASE_CAP 16
#define BAD_SIZE "vectors length not equal"
using std::vector ;
using std::pair ;
using std::string ;

template<typename KeyT, typename ValueT> class HashMap{
    typedef vector<pair<KeyT&,ValueT&>> buc_vec ;
    typedef buc_vec buc_vec_array[] ;
protected:
    buc_vec_array *main_vec_ ;
    int size_ = 0  ;
    int capacity_ = BASE_CAP ;



public:

    HashMap(): size_(0), capacity_(BASE_CAP), main_vec_(new vector<pair<KeyT, ValueT>>[BASE_CAP]) {}

    HashMap (const std::vector<KeyT> &keys, const std::vector<ValueT> &values) : HashMap () {
        if (keys.size () != values.size ())
        {
            throw (std::domain_error{BAD_SIZE});
        }
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
        map_to_copy = new vector<pair<KeyT, ValueT>>[capacity_];
        for (int i = 0; i < capacity_; i++)
        {
            main_vec_[i] = map_to_copy.main_vec_[i];
        }


    }

    ~HashMap() {
        for (int i = 0; i < capacity_; ++i) {
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
        return (int)(std::hash<KeyT>{} (key) && (capacity_ - 1)) ;
    }

    int size(){ return size_ ;}
    int capacity(){return capacity_ ; }
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
        double load_factor = (((double)size_)/capacity_) ;
        return load_factor ;
    }

    void rehash(){
        buc_vec  new_vec = {} ;
        int j = 0 ;
        for (int i = 0; i < capacity_; ++i) {
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

    bool erase (KeyT& key){
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
        for (int i = 0; i < capacity_; ++i) {
            j = 0 ;
            while (main_vec_[i][j]){
                main_vec_[i].erase(j) ;
                j++ ;
            }
        }
    }
    class const_iterator {
        friend class HashMap ;
    private:
        const HashMap<KeyT,ValueT>& map_;
        int current_bucket_;
        int current_pos_;
    public:

        typedef std::pair<KeyT, ValueT> value_type;
        typedef const value_type &reference;
        typedef const value_type *pointer;
        typedef int difference_type;
        typedef std::forward_iterator_tag iterator_category;
    private:
        const_iterator(const HashMap<KeyT,ValueT> &data, int size) : map_(data), current_bucket_(0), current_pos_(0) {}

    public:
        const_iterator cbegin() const{
            return map_.main_vec_ ;
        }
        const_iterator cend() const{
            return const_iterator(*this, this->map_.capacity_ , 0 ) ;
        }
        const_iterator begin(){
            return cbegin() ;
        }
        const_iterator end(){
            return cend() ;
        }

        const_iterator& operator++ () {
            if(current_pos_+ 1 < sizeof(map_[current_bucket_])){
                current_pos_++ ;
            } else if (current_bucket_+1 < sizeof(map_)){
                current_pos_ = 0 ;
                current_bucket_++ ;
            }
        }

        const_iterator operator++ (int j){
            for (int i = 0; i < j; ++i) {
                this->operator++() ;
            }
            return this ;
        }

        bool operator== (const const_iterator &pair_to_check) const {
            bool flag = (&map_ == &pair_to_check.map_ && current_bucket_ ==
                    pair_to_check.current_bucket_ && current_pos_ == pair_to_check.current_pos_) ;
            return flag ;
        }

        bool operator!= (const const_iterator &pair_to_check) const{
            return !this->operator==(pair_to_check) ;
        }

        reference operator* () const{
            return map_.main_vec_[this->current_bucket_][this->current_pos_] ;
        }

        pointer operator-> () const{
            return &this->operator*() ;
        }
    };
};



#endif //EX6_MEITAR453_HASHMAP_HPP