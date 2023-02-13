#include <cstdio>
#include <vector>
#include <string>
#include <stdexcept>
#include <iterator>
#include <iostream>
#include "exception"
#include <math.h>

#ifndef EX6_MEITAR453_HASHMAP_HPP
#define EX6_MEITAR453_HASHMAP_HPP
#define BASE_CAP 16
#define BAD_SIZE "vectors length not equal"
#define SQURE 2
#define HIGHLIM 0.75
#define LOWLIM 0.25
#define KEY_PROB "key not in table"
#define SWEET16 16
using std::vector;
using std::pair;
using std::string;


template<typename KeyT, typename ValueT>
class HashMap {
    typedef pair<KeyT, ValueT> single_pair;
    typedef vector<single_pair> pair_vec_array;

protected:
    pair_vec_array *main_array_p_{};
    size_t size_;
    size_t capacity_;

public:

    HashMap() : size_(0), capacity_(BASE_CAP), main_array_p_(new pair_vec_array[BASE_CAP]) {}


    HashMap(const vector<KeyT> &keys, const vector<ValueT> &values) : HashMap() {
        if (keys.size() != values.size()) {
            throw (std::domain_error{"vectors length not equal"});
        }
        for (size_t i = 0; i < keys.size(); i++) {
            if (contains_key(keys[i])) {
                at(keys[i]) = values[i];
            } else {
                insert(keys[i], values[i]);
            }
        }
    }


    HashMap(const HashMap *map_to_copy) : HashMap() {
        for (int i = 0; i < map_to_copy->capacity(); ++i) {
            for (int j = 0; j < map_to_copy->main_array_p_[i].size(); ++j) {
                insert(map_to_copy->main_array_p_[i][j].first,
                       map_to_copy->main_array_p_[i][j].second);
            }
        }
    }

    virtual ~HashMap() = default;
//            delete[] main_array_p_ ;
//        }

    HashMap &operator=(const HashMap &data) {
        if (this == &data) {
            return *this;
        }
        for (int i = 0; i < capacity(); ++i) {
            main_array_p_[i] = data.main_array_p_[i];
        }
        capacity_ = data.capacity();
        size_ = data.size();
        return (*this);
    }

    bool operator==(const HashMap &map) const {
        if (this->size_ != map.size_) {
            return false;
        }
        for (int i = 0; i < map.capacity_; i++) {
            if (main_array_p_[i] != map.main_array_p_[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const HashMap map) const {
        return !(this->operator==(map));
    }

    int bucket_size(KeyT key) const {
        size_t h_f = hush_func(key);
        if (h_f >= capacity() || h_f < 0) {
            throw std::out_of_range(KEY_PROB);
        }
        for (auto it = main_array_p_[h_f].cbegin(); it !=
                                                    main_array_p_[h_f].cend(); it++) {
            if (it->first == key) {
                return main_array_p_[h_f].size();
            }
        }
        throw std::out_of_range(KEY_PROB);
    }

    ValueT &operator[](const KeyT &key) {
        // hash func -> v mod size = v & (size − 1)

        int indx = hush_func(key);
        for (size_t j = 0; j < main_array_p_[indx].size(); ++j) {
            if (main_array_p_[indx][j].first == key) {
                return main_array_p_[indx][j].second;
            }
        }
        this->insert(key, ValueT());
        return this->at(key);
    }

    ValueT operator[](const KeyT &key) const {
        return this->at(key);
    }

    int hush_func(const KeyT &key) const {
        return (int) (std::hash<KeyT>{}(key) & (capacity_ - 1));
    }

    int size() const { return size_; }

    int capacity() const { return capacity_; }

    bool empty() const { return size_ == 0; }

    bool contains_key(KeyT &key_to_check) {
        int indx = hush_func(key_to_check);
        for (size_t i = 0; i < main_array_p_[indx].size(); ++i) {
            if (main_array_p_[indx][i].first == key_to_check) {
                return true;
            }
        }
        return false;
    }

    bool contains_key(const KeyT key_to_check) const {
        int indx = hush_func(key_to_check);
        for (size_t i = 0; i < main_array_p_[indx].size(); ++i) {
            if (main_array_p_[indx][i].first == key_to_check) {
                return true;
            }
        }
        return false;
    }

    double get_load_factor() const {
        double load_factor = (((double) size_) / capacity_);
        return load_factor;
    }

    void rehash() {
        vector<KeyT> new_vec_keys = {};
        vector<ValueT> new_vec_val = {};
        for (size_t i = 0; i < capacity_; ++i) {
            for (size_t j = 0; j < main_array_p_[i].size(); ++j) {
                new_vec_keys.emplace_back(main_array_p_[i][j].first);
                new_vec_val.emplace_back(main_array_p_[i][j].second);
            }
        }
        for (int i = 0; i < capacity_; ++i) {
            main_array_p_[i].clear();
        }
        while (new_vec_keys.size() > HIGHLIM * capacity_) {
            capacity_ *= 2;
        }
        while (new_vec_keys.size() < LOWLIM * capacity_ && capacity_ > 1) {
            capacity_ /= 2;
        }
        size_ = 0;
        main_array_p_ = new pair_vec_array[capacity_];
        for (size_t i = 0; i < new_vec_keys.size(); ++i) {
            insert(new_vec_keys[i], new_vec_val[i]);
        }
    }


    bool insert(KeyT key, ValueT value) {
        if (contains_key(key)) {
            return false;
        }
        int indx = hush_func(key);
        main_array_p_[indx].emplace_back(single_pair(key, value));
        size_ += 1;
        if (get_load_factor() > HIGHLIM) {
            rehash();
        }
        return true;
    }

    virtual bool erase(const KeyT &key) {
        int indx = hush_func(key);
        int i = 0;
        for (auto &pair: main_array_p_[indx]) {
            if (pair.first == key) {
                main_array_p_[indx].erase(main_array_p_[indx].begin() + i);
                size_ -= 1;
                if (get_load_factor() <= LOWLIM) {
                    rehash();
                }
                return true;
            }
            i++;
        }
        return false;
    }

    ValueT &at(const KeyT key) const {
        size_t indx = hush_func(key);
        for (size_t j = 0; j < main_array_p_[indx].size(); ++j) {
            if (main_array_p_[indx].size() > 0) {
                if (main_array_p_[indx][j].first == key) {
                    return main_array_p_[indx][j].second;
                }
            }
        }
        throw std::out_of_range(KEY_PROB);
    }

    int bucket_index(KeyT key) const {
        if (contains_key(key)) {
            return hush_func(key);
        }
        throw std::runtime_error(KEY_PROB);
    }

    void clear() {
        for (int i = 0; i < capacity_; ++i) {
            main_array_p_[i].clear();
        }
        size_ = 0;
    }


    class ConstIterator {
        friend class HashMap;

    private:
        const HashMap<KeyT, ValueT> &map_;
        size_t current_bucket_;
        size_t current_pos_;

        explicit ConstIterator(const HashMap<KeyT, ValueT> &data, int current_bucket,
                                                                        int current_pos) :
                map_(data), current_bucket_(current_bucket), current_pos_(current_pos) {
            auto *buk_p = &(this->map_.main_array_p_[current_bucket]);
            while (current_bucket_ < map_.capacity_ && buk_p->empty()){
                ++current_bucket_ ;
                buk_p = &(this->map_.main_array_p_[current_bucket]) ;
            }
        }

//        explicit ConstIterator(const HashMap<KeyT, ValueT> &data) :
//                map_(data), current_bucket_(0), current_pos_(0) {}

    public:
        typedef std::pair<KeyT, ValueT> value_type;
        typedef const value_type &reference;
        typedef const value_type *pointer;
        typedef std::ptrdiff_t difference_type;
        typedef std::forward_iterator_tag iterator_category;

        ConstIterator &operator++() {
            if (current_bucket_ < map_.capacity_){
                pair_vec_array *vec_p = &map_.main_array_p_[current_bucket_] ;
                current_pos_++ ;
                if (current_pos_  >= vec_p->size()) {
                    current_pos_ = 0;
                    do {
                        current_bucket_++;
                        vec_p = &map_.main_array_p_[current_bucket_];
                    } while(current_bucket_ < map_.capacity() && vec_p->empty());
                }
            }
            return *this ;
        }

        ConstIterator operator++(int) {
            const ConstIterator copy = (*this);
            this->operator++();
            return copy;
        }

        bool operator==(const ConstIterator &pair_to_check) const {
            bool flag = (&map_ == &pair_to_check.map_ && current_bucket_ ==
                                                         pair_to_check.current_bucket_ &&
                         current_pos_ == pair_to_check.current_pos_);
            return flag;
        }

        bool operator!=(const ConstIterator &pair_to_check) const {
            return !this->operator==(pair_to_check);
        }

        reference operator*() const {
            return map_.main_array_p_[this->current_bucket_][this->current_pos_];
        }

        pointer operator->() const {
            return &this->operator*();
        }
    };

    using const_iterator = ConstIterator;


    ConstIterator cbegin() const {
        return ConstIterator(*this, 0, 0);
    }

    ConstIterator cend() const {
        return ConstIterator(*this, capacity_, 0);
    }

    ConstIterator begin() const { return cbegin(); };

    ConstIterator end() const { return cend(); };

    void swap(HashMap other) {
        std::swap(this->main_array_p_, other.main_array_p_);
        std::swap(this->size_, other.size_);
        std::swap(this->capacity_, other.capacity_);
    }

};


#endif //EX6_MEITAR453_HASHMAP_HPP