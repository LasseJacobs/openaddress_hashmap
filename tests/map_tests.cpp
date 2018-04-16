/*
 * File:   map_tests.cpp
 * Author: lasse
 *
 * Created on Apr 15, 2018, 10:41:32 AM
 */

#include "map_tests.h"
#include <string>
#include <exception>


CPPUNIT_TEST_SUITE_REGISTRATION(map_tests);

map_tests::map_tests() {
}

map_tests::~map_tests() {
}

void map_tests::setUp() {
}

void map_tests::tearDown() {
}

void map_tests::test_empty() {
    CPPUNIT_ASSERT(_map.empty());
}

void map_tests::test_size() {
    CPPUNIT_ASSERT(_map.size() == 0);
}

void map_tests::test_capacity() {
    CPPUNIT_ASSERT(_map.capacity() != 0);
}

void map_tests::test_emplace() {
    auto it = _map.emplace(4, "four");
    CPPUNIT_ASSERT(_map.size() == 1 && it.second);
}

void map_tests::test_emplace_double() {
    _map.emplace(4, "four");
    auto it = _map.emplace(4, "five");
    CPPUNIT_ASSERT(_map.size() == 1 && !it.second && (*it.first).second == "four");
}

void map_tests::test_emplace_multi() {
    _map.emplace(4, "four");
    _map.emplace(5, "five");
    CPPUNIT_ASSERT(_map.size() == 2);
}

void map_tests::test_clear() {
    _map.emplace(4, "four");
    _map.emplace(5, "five");
    _map.clear();
    CPPUNIT_ASSERT(_map.size() == 0);
}

void map_tests::test_at() {
    std::string result;
    try {
        _map.emplace(4, "four");
        result = _map.at(4);
    } catch(const std::exception& e) { 
        CPPUNIT_FAIL(e.what());
    }
    
    CPPUNIT_ASSERT(result == "four");
}

void map_tests::test_at_oor() {
    try {
        _map.at(4);
        CPPUNIT_FAIL("No exception thrown");
    } catch(const std::out_of_range& e) { 
        //Success
    }
}

void map_tests::test_find() {
    _map.emplace(4, "four");
    auto it = _map.find(4);
    CPPUNIT_ASSERT(it != _map.end() && (*it).second == "four");
}

void map_tests::test_find_none() {
    auto it = _map.find(4);
    CPPUNIT_ASSERT(it == _map.end());
}

void map_tests::test_subscript_assign() {
    _map[4] = "four";
    auto it = _map.find(4);
    CPPUNIT_ASSERT(it != _map.end() && (*it).second == "four" 
            && _map.size() == 1);
}

void map_tests::test_subscript_assign_double() {
    _map[4] = "four";
    _map[4] = "five";
    auto it = _map.find(4);
    CPPUNIT_ASSERT(it != _map.end() && (*it).second == "five" 
            && _map.size() == 1);
}

void map_tests::test_count() {
    _map.emplace(4, "four");
    CPPUNIT_ASSERT(_map.count(4) == 1);
}

void map_tests::test_count_none() {
    _map.emplace(5, "five");
    CPPUNIT_ASSERT(_map.count(4) == 0);
}

void map_tests::test_erase_by_key() {
    _map.emplace(4, "four");
    _map.erase(4);
    auto it = _map.find(4);
    CPPUNIT_ASSERT(it == _map.end() && _map.size() == 0);
}

void map_tests::test_erase_by_it() {
    _map.emplace(4, "four");
    auto it1 = _map.find(4);
    
    ljl::array_map<int, std::string>::const_iterator cit = it1;
    _map.erase(cit);
    auto it2 = _map.find(4);

    CPPUNIT_ASSERT(it2 == _map.end() && _map.size() == 0);
}

void map_tests::test_erase_none() {
    _map.emplace(4, "four");
    _map.erase(5);
    auto it = _map.find(4);
    CPPUNIT_ASSERT(it != _map.end() && _map.size() == 1);
}

void map_tests::test_rehash() {
    _map.emplace(4, "four");
    _map.emplace(5, "five");
    _map.emplace(6, "six");
    _map.emplace(7, "seven");
    
    _map.rehash(64);
    
    CPPUNIT_ASSERT(_map.size() == 4);
    CPPUNIT_ASSERT(_map.find(4) != _map.end());
    CPPUNIT_ASSERT_NO_THROW(_map.at(6));
}

void map_tests::test_rehash_by_assign() {
    int initial_cap = _map.capacity();
    for(int i = 0; i < initial_cap + 10; i++) {
        _map.emplace(i, "# " + std::to_string(i));
    }
    
    CPPUNIT_ASSERT(_map.size() == (initial_cap + 10));
    CPPUNIT_ASSERT(_map.find(4) != _map.end());
    CPPUNIT_ASSERT_NO_THROW(_map.at(6));
}

void map_tests::test_reserve() {
    _map.reserve(100);
    CPPUNIT_ASSERT(_map.capacity() >= 100);
}

/*
void map_tests::test_iterators() {
    ljl::array_map<int, int> int_map;
    for(int i = 0; i < 10; i++) {
        int_map.emplace(i, i*10);
    }
    
    int sum = 0;
    std::for_each(_map.begin(), _map.end(), [](std::pair<int int> &n){ sum += n; });
    CPPUNIT_ASSERT_EQUAL(sum, 550);
}
*/
