/* 
 * File:   Branch_p.cpp
 * Author: ilya
 * 
 * Created on 25 Ноябрь 2011 г., 14:00
 */

#include "Branch_p.h"

Branch_p::Branch_p() {
}

Branch_p::Branch_p(const Branch_p& orig) {
}

Branch_p::~Branch_p() {
}


void Branch_p::print() {
    printf("Node gNode_Id : %d, Node level: %d, Node err: %f, Node status: %d ", this->gNode_Id,this->nodeLevel, this->error_value,this->status);
    this->value->printSIF();
    
}
