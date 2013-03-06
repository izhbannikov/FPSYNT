/* 
 * File:   ExhaustiveSearch.cpp
 * Author: ilya
 * 
 * Created on 28 Октябрь 2011 г., 15:55
 */

#include "Branch.h"
#include <vector>

using namespace std;



Branch::Branch() {
    
}

Branch::Branch(const Branch& orig) {
}

Branch::~Branch() {
}


void Branch::print() {
    printf("Node name: %s, Node level: %d, Node err: %f, Node status: %d ", this->gNode->name,this->nodeLevel, this->error_value,this->status);
    this->value->printSIF();
    
}