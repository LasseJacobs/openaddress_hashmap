/* 
 * File:   main.cpp
 * Author: lasse
 *
 * Created on September 5, 2017, 8:05 PM
 */

#include<iostream>
#include"container.h"
#include"arraymap.h"

using namespace std;

/*
 * 
 */

int main(int argc, char** argv) {
    ljl::array_map<int, std::string> map;
    
    map.emplace(1, "1");
    map[2] = "two";
    
    std::cout << map.at(1) << std::endl;
    
    map.erase(1);
    try {
        std::cout << map.at(1) << std::endl;
    }
    catch(std::out_of_range e) {
        std::cout << "Element not found" << std::endl;
    }
   
    return 0;
}