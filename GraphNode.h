/* 
 * File:   graphNode.h
 * Author: ilya
 *
 * Created on 6 Октябрь 2011 г., 15:38
 */

#ifndef GRAPHNODE_H
#define	GRAPHNODE_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "Sif.h"
#include <math.h>
#include <algorithm> // sort, max_element, random_shuffle, remove_if, lower_bound 
#include <iostream>
//#include "Graph.h"

using namespace std;


class GraphNode {
public:
    GraphNode();
    GraphNode(const GraphNode& orig);
    virtual ~GraphNode();
    
    vector <GraphNode*> linkedNodes;
    
    bool tag;
    int level;
    int id;
    char* name;
    
    bool ready;
    
    void compute();
    
    int operation; //0 - "+", 1 - "*", 2 - "/" , -1 - нет операции
    //Sif *op1; 
    //Sif *op2;
    GraphNode *op1;
    GraphNode *op2;
    Sif *value;
    
    void setOperand(GraphNode *op);
    
    Sif *calculateSIFSum(Sif *op1, Sif *op2);
    Sif *calculateSIFMult(Sif *op1, Sif *op2);
    Sif *placeSIFResult(Sif *res);
    
    double calculateError(Sif *op1, Sif *op2);
    double calculateSumError(Sif *op, int epsilon);
    
    void printSIF();
    
    int WL;
    
    vector<Sif*> combinations;
    vector<double> error_array;
    vector<int> epsilon_array;
    int epsilon;
    
    
    double err;//промежуточная ошибка вычислений
    
    double op1_err; //ошибка, привнесенная первым операндом
    double op2_err;//ошибка, привнесённая вторым операндом
    
    GraphNode *Clone();
    
    void print();
    
    int op1_id;
    int op2_id;
    
    int shift_pos;//число позиций на которые надо сдвинуть операнд
    //если shift_pos отрицательно - сдвинуть влево, иначе - вправо.
    
    //для сдвига после операции умножение:
    int shift_mult;
    
    double init_err;
    
};

#endif	/* GRAPHNODE_H */

