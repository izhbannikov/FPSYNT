/* 
 * File:   Branch_p.h
 * Author: ilya
 *
 * Created on 25 Ноябрь 2011 г., 14:00
 */
//#pragma once
#ifndef BRANCH_P_H
#define	BRANCH_P_H

#include <pthread.h>
#include "Sif.h"
#include "Graph.h"
#include "Worker.h"

class Graph;
class Worker;

class Branch_p {
public:
    Branch_p();
    Branch_p(const Branch_p& orig);
    virtual ~Branch_p();
    
    void print();
    
    Branch_p *parent;
    
    Sif *value;//значение, содержащееся в ветви
    
    int gNode_Id; //ссылка на узел вычислительного графа, с которым связан данный узел
    
    double error_value;//значение ошибки, соответствующее sif value
    int epsilon;
    
    int status; /*0 - waiting, 1- active, 2- eliminated*/
    int nodeLevel;
  
    Graph *dflGraph;
    
private:

};

#endif	/* BRANCH_P_H */

