/* 
 * File:   arraymap.h
 * Author: lasse
 *
 * Created on March 21, 2018, 8:42 PM
 */

#ifndef ARRAYMAP_H
#define ARRAYMAP_H

#include <utility>
#include <cmath>
#include <exception>
#include "container.h"
#include "iterator.h"

namespace ljl {

template<typename K, typename V>
class array_map {
public:
    using key_type = K;
    using mapped_type = V;
    using value_type = std::pair<K, V>;
    using size_type = size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = arraymap_iterator<value_type>;
    using const_iterator = arraymap_iterator<const value_type>;
    
    array_map() : _values(32) {
        _maxLoad = 0.70f;
    }
    
    /**
     * Checks if the container has no elements
     * 
     * @return true if the container is empty, false otherwise
     */
    bool empty() const {
        return size() == 0;
    }
    /**
     * Returns the number of elements in the container
     * 
     * @return The number of elements in the container.
     */
    size_type size() const {
        return _values.size();
    }
    /**
     * Return the capacity of the container
     * 
     * @return The capacity of the container.
     */
    size_type capacity() const {
        return _values.capacity();
    }
    /**
     * Removes all elements from the container.
     * Invalidates any references, pointers, or iterators referring to 
     * contained elements. May also invalidate past-the-end iterators.
     */
    void clear() {
        smart_container<value_type> empty_container(32);
        _values = std::move(empty_container);
    }
    
    /**
     * Inserts a new element into the container.
     * If rehashing occurs due to the insertion, all iterators are 
     * invalidated.
     * 
     * @param key - element key to emplace
     * @param value - element value to emplace
     * @return Returns a pair consisting of an iterator to the 
     * inserted element, or the already-existing element if no 
     * insertion happened, and a bool denoting whether the 
     * insertion took place. True for Insertion, False for No 
     * Insertion.
     */
    std::pair<iterator, bool> emplace(
            const key_type& key, 
            const mapped_type& value) 
    {
        size_type i = find_element(key);
        if(i != _values.capacity())
            return std::make_pair(iterator(&_values, i), false);
        
        if(load_factor() > _maxLoad) {
            rehash(_values.capacity() * 2);
        }
            
        i = hash(key);
        while (!_values.free(i)) {
            i = (i == _values.capacity()-1) ? 0 : i + 1;
        }
        _values[i] = std::make_pair(key, value);
            
        return std::make_pair(iterator(&_values, i), true);
    }

    /**
     * Returns a reference to the mapped value of the element 
     * with key equivalent to key. If no such element exists, 
     * an exception of type std::out_of_range is thrown.
     * 
     * @param key - the key of the element to find
     * @return Reference to the mapped value of the requested element
     */
    V& at(const K& key) {
        return const_cast<V&>(static_cast<const array_map&>(*this).at(key));
    }
    
    const V& at(const K& key) const {
        size_type i = find_element(key);
        if(i == _values.capacity())
            throw std::out_of_range("Key not found");
        
        return _values[i].second;
    }
    
    /**
     * Finds an element with key equivalent to key.
     * 
     * @param key - key value of the element to search for
     * @return Iterator to an element with key equivalent 
     * to key. If no such element is found, past-the-end 
     * (see end()) iterator is returned.
     */
    iterator find(const K& key) {
        return iterator(&_values, find_element(key));
    }
    
    const_iterator find(const K& key) const {
        return const_iterator(&_values, find_element(key));
    }

    /**
     * Returns a reference to the value that is mapped to a 
     * key equivalent to key, performing an insertion if such 
     * key does not already exist.
     * 
     * @param key - the key of the element to find
     * @return Reference to the mapped value of the new element 
     * if no element with key key existed. Otherwise a 
     * reference to the mapped value of the existing element 
     * whose key is equivalent to key.
     */
    V& operator[](const K& key) {
        int i = find_element(key);
        if(i != _values.capacity())
            return _values[i].second;
        
        if(load_factor() > _maxLoad) {
            rehash(_values.capacity() * 2);
        }
            
        i = hash(key);
        while (!_values.free(i)) {
            i = (i == _values.capacity()-1) ? 0 : i + 1;
        }
        
        _values[i].first = key;
        return _values[i].second;
    }
    
    /**
     * Returns the number of elements with key that compares 
     * equal to the specified argument key, which is either 
     * 1 or 0 since this container does not allow duplicates.
     * 
     * @param key - key value of the elements to count
     * @return Number of elements with key key, that is 
     * either 1 or 0.
     */
    size_type count(const K& key) const {
        if(find_element(key) == _values.capacity())
            return 0;
        return 1;
    }
    
    /**
     * Removes specified element at pos.
     * 
     * @param pos - iterator to the element to remove
     * @return Iterator following the last removed element.
     */
    iterator erase(const_iterator pos) {
        _values.remove(pos._current);
        pos++;
        
        return iterator(&_values, pos._current);
    }
    
    /**
     * Removes the element (if one exists) with the key 
     * equivalent to key.
     * 
     * @param key - key value of the elements to remove
     * @return Number of elements removed.
     */
    size_type erase(const key_type& key) {
        int i = find_element(key);
        if(i == _values.capacity()) {
            return 0;
        }
        
        _values.remove(i);
        return 1;
    }
    
    /**
     * Returns the average number of elements per bucket, 
     * that is, size() divided by capacity().
     * 
     * @return The load factor of the container.
     */
    float load_factor() const {
        return (float)_values.size() / (float)_values.capacity();
    }
    /**
     * Manages the maximum load factor.
     * 
     * @return Returns current maximum load factor. 
     */
    float max_load_factor() const {
        return _maxLoad;
    }
    /**
     * Sets the maximum load factor to ml.
     * 
     * @param ml - new maximum load factor setting
     */
    void max_load_factor(float ml) {
        _maxLoad = ml;
    }
    
    /**
     * Sets the capacity of the container to count and rehashes 
     * the container, i.e. puts the elements into appropriate 
     * place considering that total number of spots has changed. 
     * If the new capacity makes load factor more than 
     * maximum load factor (count < size() / max_load_factor()), 
     * then the new number of buckets is at least size() / max_load_factor().
     * 
     * @param count - new capacity of the container
     */
    void rehash(size_type count) {
        smart_container<value_type> new_values(count);
        std::swap(_values, new_values);
    
        for(unsigned int i = 0; i < new_values.capacity(); i++)
        {
            if(!new_values.free(i))
            {
                const reference entry = new_values[i];
                emplace(entry.first, entry.second);
            }
        }
    }
    
    /**
     * Sets the number of buckets to the number needed to accomodate
     * at least count elements without exceeding maximum load factor 
     * and rehashes the container.
     * 
     * @param count - new capacity of the container
     */
    void reserve(size_type count) {
        rehash(std::ceil(count / max_load_factor()));
    }
    
    /**
     * Returns an iterator to the first element of the container.
     * If the container is empty, the returned iterator will be 
     * equal to end().
     * 
     * @return Returns an iterator to the first element of 
     * the container.
     */
    iterator begin() {
        return iterator(&_values);
    }
    const_iterator begin() const {
        return const_iterator(&_values);
    }
    const_iterator cbegin() const {
        return begin();
    }
    
    /**
     * Returns an iterator to the element following the last 
     * element of the container.
     * This element acts as a placeholder; attempting to access 
     * it results in undefined behavior.
     * 
     * @return Returns an iterator to the element following the last 
     * element of the container.
     */
    iterator end() {
        return iterator(&_values, _values.capacity());
    }
    const_iterator end() const {
        return const_iterator(&_values, _values.capacity());
    }
    const_iterator cend() const {
        return end();
    }
        
private:
    float _maxLoad;
    smart_container<value_type> _values;

    size_type hash(const key_type& key) const {
        size_t hash = std::hash<key_type>{}(key);
        return hash;
    }
    
    size_type find_element(const key_type& key) const {
        int i = hash(key);
        while (!_values.empty(i)) {
            if (!_values.removed(i) && _values[i].first == key) 
                return i;
            
            i = (i == _values.size()-1) ? 0 : i + 1;
        }
        return _values.capacity();
    }
};

}

#endif /* ARRAYMAP_H */

