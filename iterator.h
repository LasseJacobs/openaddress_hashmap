/* 
 * File:   iterator.h
 * Author: lasse
 *
 * Created on March 21, 2018, 9:46 PM
 */

#ifndef ITERATOR_H
#define ITERATOR_H

#include <iterator>
#include <type_traits>  // remove_cv

#include "container.h"
#include "arraymap.h"

namespace ljl {

template<
    typename T, 
    typename UnqualifiedT = typename std::remove_cv<T>::type
>
class arraymap_iterator {
    template<typename K, typename V> friend class array_map;
    
public:
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    using iterator_category = std::forward_iterator_tag;
    
    arraymap_iterator() = default;
    arraymap_iterator(const arraymap_iterator& other) {
        _values = other._values;
        _current = other._current;
    }
    
    arraymap_iterator(smart_container<UnqualifiedT>* container) {
        _values = container;
        _current = 0;
        next_element();
    }
    
    arraymap_iterator(smart_container<UnqualifiedT>* container, unsigned int i) 
    { 
        _values = container;
        _current = i;
    }
    
    arraymap_iterator operator=(const arraymap_iterator& rhs) {
        _values = rhs._values;
        _current = rhs._current;
        
        return (*this);
    } 

    reference operator*() const {
        return (*_values)[_current];
    }
    
    reference operator->() const {
        return (*this);
    }

    arraymap_iterator& operator++() {
        next_element();
        return *this;
    }
    arraymap_iterator operator++(int) {
        arraymap_iterator temp = *this;
        next_element();
        
        return temp;
    }
    
    template<typename U>
    bool operator==(const arraymap_iterator<U>& rhs) const {
        return _values == rhs._values && _current == rhs._current;
    }
    template<typename U>
    bool operator!=(const arraymap_iterator<U>& rhs) const {
        return !(*this == rhs);
    }
    
    operator arraymap_iterator<const T>() const
    {
        return arraymap_iterator<const T>(_values, _current);
    }
    
private:
    smart_container<UnqualifiedT>* _values;
    unsigned int _current;
    
    void next_element() {
        while(++_current < _values->capacity()) {
            if(!_values->free(_current))
                break;
        }
    }
};

}

#endif /* ITERATOR_H */

