/* 
 * File:   container.h
 * Author: lasse
 *
 * Created on April 11, 2018, 7:22 PM
 */

#ifndef CONTAINER_H
#define CONTAINER_H

#include<cassert>

namespace ljl {

template<typename T>
class container {
public:
    container() = delete;
    container(size_t capacity) {
        _data = new T[capacity];
        _capacity = capacity;
    }
    container(container<T>&& other) {
        _data = other._data;
        _capacity = other._capacity;
        
        other._data = nullptr;
        other._capacity = 0;
    }
    
    container<T>& operator=(container<T>&& rhs) {
        if(_data != nullptr)
            delete[] _data;
            
        _data = rhs._data;
        _capacity = rhs._capacity;
        
        rhs._data = nullptr;
        rhs._capacity = 0;
        
        return *this;
    }
    
    virtual T& operator[](unsigned int i) {
        assert(i >= 0 && i < _capacity);
        return _data[i];
    }
    virtual const T& operator[](unsigned int i) const {
        assert(i >= 0 && i < _capacity);
        return _data[i];
    }
    
    size_t capacity() const {
        return _capacity;
    }
    
    virtual ~container() {
        if(_data != nullptr)
            delete[] _data;
    }
        
private:
    T* _data;
    size_t _capacity;
};

template<typename T>
class smart_container : public container<T> {
public:    
    smart_container() = delete;
    smart_container(size_t capacity) : container<T>(capacity) {
        _empty = new bool[capacity];
        _removed = new bool[capacity];
        for(unsigned int i = 0; i < capacity; i++) {
            _empty[i] = true;
            _removed[i] = false;
        }
        
        _size = 0;
    }
    smart_container(smart_container<T>&& other) : container<T>(std::move(other)) {
        _empty = other._empty;
        other._empty = nullptr;
        _removed = other._removed;
        other._removed = nullptr;
        
        _size = other._size;
        other._size = 0;
    }
    
    smart_container<T>& operator=(smart_container<T>&& rhs) {
        container<T>::operator=(std::move(rhs));
        
        _empty = rhs._empty;
        rhs._empty = nullptr;
        _removed = rhs._removed;
        rhs._removed = nullptr;
        
        _size = rhs._size;
        rhs._size = 0;
        
        return *this;
    }
    T& operator[](unsigned int i) override {
        assert(i >= 0 && i < container<T>::capacity());

        if(free(i))
            _size++;
        
        _empty[i] = false;
        _removed[i] = false;
        
        return container<T>::operator[](i);
    }
    const T& operator[](unsigned int i) const override {
        return container<T>::operator[](i);
    }
    
    void remove(unsigned int i) {
        assert(i >= 0 && i < container<T>::capacity());
        if(free(i))
            return;
        
        _size--;
        _removed[i] = true;
    }
    
    bool empty(unsigned int i) const {
        assert(i >= 0 && i < container<T>::capacity());
        return _empty[i];
    }
    bool removed(unsigned int i) const {
        assert(i >= 0 && i < container<T>::capacity());
        return _removed[i];
    }
    bool free(unsigned int i) const {
        return empty(i) || removed(i);
    }
    
    size_t size() const {
        return _size;
    }
    
    virtual ~smart_container() {
        if(_empty != nullptr)
            delete[] _empty;
            
        if(_removed != nullptr)
            delete[] _removed;
    }
    
private:
    bool* _empty;
    bool* _removed;
    size_t _size;
};


}

#endif /* CONTAINER_H */

