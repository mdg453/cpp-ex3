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
    typedef vector<single_pair> pair_vec_array ;

    protected:
        pair_vec_array* main_array_p_{} ;
        int size_ ;
        int capacity_ ;

    public:

        HashMap(): size_(0), capacity_(BASE_CAP), main_array_p_(new pair_vec_array[16]) {}

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




        HashMap (const HashMap *map_to_copy) {
            capacity_ = map_to_copy->capacity_;
            size_ = map_to_copy->size_;
            main_array_p_ = new vector<pair<KeyT, ValueT>>[capacity_];
            for (int i = 0; i < capacity_; i++)
            {
                main_array_p_[i] = map_to_copy->main_array_p_[i];
            }
        }

        virtual ~HashMap() {
            delete[] main_array_p_ ;
        }

        HashMap& operator= (const HashMap& data) {
            if (this == &data){
                return *this ;
            }
            main_array_p_ = data.main_array_p_;
            capacity_ = data.capacity() ;
            size_ = data.size() ;
//            std::swap(&main_array_p_, data.main_array_p_) ;
            return (*this) ;
        }

        bool operator== (const HashMap map) const{
            if (this->size_ != map.size_ || this->capacity_ != map.capacity_) {
                return false ;
            }
            for(int i = 0 ; i < capacity_; i++) {
                if (main_array_p_[i].size() != map.main_array_p_[i].size()) {
                    return false ;
                }
                for (int j = 0 ; j < sizeof(main_array_p_[i]) ; j++ ) {
                    if (main_array_p_[i][j].first != map.main_array_p_[i][j].first ||
                                        main_array_p_[i][j].second != map.main_array_p_[i][j].second){
                        return false ;
                    }
                }
            }
            return true ;
        }

        bool operator!= (const HashMap map) const{
            return !(this->operator==(map)) ;
        }

        int bucket_size(KeyT key) const{
            return main_array_p_[hush_func(key)].size() ;
        }

        ValueT &operator[] (const KeyT &key) {
            // hash func -> v mod size = v & (size − 1)

            int indx = hush_func(key);
            int i = 0;
            while (!(main_array_p_[indx][i].first == key) || i != main_array_p_[indx].size() ) {
                i++;
            }
            if (i != main_array_p_[indx].size()) {
                return main_array_p_[indx][i].second;
            }
            insert(key, ValueT()) ;
            return at(key);
        }

        ValueT operator[] (const KeyT &key) const{
            ValueT val = this->operator[](key) ;
            return val ;
        }

        int hush_func(const KeyT &key) const {
            return (int)(std::hash<KeyT>{} (key) && (capacity_ - 1)) ;
        }

        int size() const{ return size_ ;}

        int capacity() const{return capacity_ ; }

        bool empty() const{return size_ != 0 ;}

        bool contains_key (KeyT &key_to_check){
            int indx = hush_func(key_to_check) ;
            for (int i = 0; i < main_array_p_[indx].size(); ++i) {
                if (main_array_p_[indx][i].first == key_to_check)
                {
                    return true ;
                }
            }
            return false ;
        }
        bool contains_key(const KeyT key_to_check) const {
            int indx = hush_func(key_to_check) ;
            for (int i = 0; i < main_array_p_[indx].size(); ++i) {
                if (main_array_p_[indx][i].first == key_to_check)
                {
                    return true ;
                }
            }
            return false ;
        }

        double get_load_factor() const{
            double load_factor = (((double)size_)/capacity_) ;
            return load_factor ;
        }

        void rehash(){
            vector<KeyT> new_vec_keys = {} ;
            vector<ValueT> new_vec_val= {} ;
            for (int i = 0; i < capacity_; ++i) {
                for(int j = 0 ; j < main_array_p_[i].size(); ++j ) {
                    new_vec_keys.emplace_back(main_array_p_[i][j].first) ;
                    new_vec_val.emplace_back(main_array_p_[i][j].second) ;
                }
            }
            HashMap new_hash = new HashMap(new_vec_keys,new_vec_val) ;
            main_array_p_ = new_hash.main_array_p_ ;
            capacity_ = new_hash.capacity() ;
            size_ = new_hash.size() ;
        }


        bool insert(KeyT key, ValueT value){
            if(contains_key(key)){
                return false;
            }
            int indx = hush_func(key) ;
            size_t i = main_array_p_[indx].size() ;
            main_array_p_[indx].emplace_back(single_pair()) ;
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

        virtual bool erase(const KeyT &key) {
            int indx = hush_func(key);
            int i = 0;
            for (auto &pair : main_array_p_[indx]) {
                if (pair.first == key) {
                    main_array_p_[indx].erase(main_array_p_[indx].begin() + i);
                    size_--;
                    if (get_load_factor() < LOWLIM) {
                        rehash();
                    }
                    return true;
                }
                i++;
            }
            return false;
        }

        ValueT& at (const KeyT key) const{
            int indx = hush_func(key);
            int i = 0;
            while (!(main_array_p_[indx][i].first == key) || i != size_ ) {
                i++;
            }
            if (i != size_) {
                return main_array_p_[indx][i].second;
            }
            else {
                throw std::out_of_range(KEY_PROB);
            }
        }

        int bucket_index(KeyT key) const{
            if (contains_key(key)){
                return hush_func(key);
            }
            throw std::runtime_error(KEY_PROB);
        }

        void clear(){
            int j = 0 ;
            for (int i = 0; i < capacity_; ++i) {
                 main_array_p_[i].clear() ;
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
                if(current_pos_+ 1 < map_.main_array_p_[current_bucket_].size()){
                    current_pos_++ ;
                } else if (current_bucket_+1 < map_.capacity()){
                    current_pos_ = 0 ;
                    current_bucket_++ ;
                }
                return *this ;
            }

            ConstIterator operator++ (int) {
                ConstIterator &copy(*this) ;
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


        ConstIterator cbegin() const {
            int i = 0 ;
            while (main_array_p_[i].empty()){i++ ;}
            ConstIterator it(*this) ;
            return it ;
        }

        ConstIterator cend() const{
            ConstIterator it(*this) ;
            int i = 0 ;
            while(i < capacity_){
                ++it ;
            }
            return it ;
        }

        ConstIterator begin() const{ return cbegin();};

        ConstIterator end() const { return cend();};

        void swap (HashMap other) {
            std::swap (this->main_array_p_, other.main_array_p_);
            std::swap (this->size_, other.size_);
            std::swap (this->capacity_, other.capacity_);
        }

};



#endif //EX6_MEITAR453_HASHMAP_HPP