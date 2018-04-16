/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: lasse
 *
 * Created on September 5, 2017, 8:05 PM
 */

#include<cstdlib>
#include<unordered_map>
#include<iostream>
#include<string>
#include<utility>
#include<vector>
#include <algorithm>
#include"container.h"
#include"arraymap.h"

using namespace std;

/*
 * 
 */

int main(int argc, char** argv) {
    std::vector<int> v;
    v.push_back(5);
    std::vector<int>::const_iterator it = v.begin();
    
    std::cout << (*it) << std::endl;
    
    std::unordered_map<int, std::string> umap;
    ljl::array_map<int, std::string> map;
    
    map.emplace(1, "1");
    map.emplace(1, "NOT 1");
    //map[1] = "Def not 1";
    
    std::cout << map[1] << std::endl;
   
   
    return 0;
}