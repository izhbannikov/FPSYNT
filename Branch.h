/* 
 * File:   ExhaustiveSearch.h
 * Author: ilya
 *
 * Created on 28 Октябрь 2011 г., 15:55
 */

#ifndef BRANCH_H
#define	BRANCH_H

#include "GraphNode.h"
#include "Sif.h"
//#include "Graph.h"


class Branch {
public:
    Branch();
    Branch(const Branch& orig);
    virtual ~Branch();
    
    void print();
    
    Branch *parent;
    
    Sif *value;//значение, содержащееся в ветви
    
    GraphNode *gNode; //ссылка на узел вычислительного графа, с которым связан данный узел
    
    double error_value;//значение ошибки, соответствующее sif value
    int epsilon;
    
    int status; /*0 - waiting, 1- active, 2- eliminated*/
    int nodeLevel;
private:
    
    
};

#endif	/* EXHAUSTIVESEARCH_H */

