/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   map_tests.h
 * Author: lasse
 *
 * Created on Apr 15, 2018, 10:41:32 AM
 */

#ifndef MAP_TESTS_H
#define MAP_TESTS_H

#include <cppunit/extensions/HelperMacros.h>

#include <string>
#include "../arraymap.h"

class map_tests : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(map_tests);

    CPPUNIT_TEST(test_empty);
    CPPUNIT_TEST(test_size);
    CPPUNIT_TEST(test_capacity);
    CPPUNIT_TEST(test_clear);
    CPPUNIT_TEST(test_emplace);
    CPPUNIT_TEST(test_emplace_double);
    CPPUNIT_TEST(test_emplace_multi);
    CPPUNIT_TEST(test_at);
    CPPUNIT_TEST(test_at_oor);
    CPPUNIT_TEST(test_find);
    CPPUNIT_TEST(test_find_none);
    CPPUNIT_TEST(test_subscript_assign);
    CPPUNIT_TEST(test_subscript_assign_double);
    CPPUNIT_TEST(test_count);
    CPPUNIT_TEST(test_count_none);
    CPPUNIT_TEST(test_erase_by_key);
    CPPUNIT_TEST(test_erase_by_it);
    CPPUNIT_TEST(test_erase_none);
    CPPUNIT_TEST(test_rehash);
    CPPUNIT_TEST(test_rehash_by_assign);
    CPPUNIT_TEST(test_reserve);
    //CPPUNIT_TEST(test_iterators);

    CPPUNIT_TEST_SUITE_END();

public:
    map_tests();
    virtual ~map_tests();
    void setUp();
    void tearDown();

private:
    ljl::array_map<int, std::string> _map;
    
    void test_empty();
    void test_size();
    void test_capacity();
    void test_clear();
    void test_emplace();
    void test_emplace_double();
    void test_emplace_multi();
    void test_at();
    void test_at_oor();
    void test_find();
    void test_find_none();
    void test_subscript_assign();
    void test_subscript_assign_double();
    void test_count();
    void test_count_none();
    void test_erase_by_key();
    void test_erase_by_it();
    void test_erase_none();
    void test_rehash();
    void test_rehash_by_assign();
    void test_reserve();
    //void test_iterators();
};

#endif /* MAP_TESTS_H */

