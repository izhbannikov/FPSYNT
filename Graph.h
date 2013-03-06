/* 
 * File:   Graph.h
 * Author: ilya
 *
 * Created on 13 Октябрь 2011 г., 12:02
 */
//#pragma once
#ifndef GRAPH_H
#define	GRAPH_H

#include <stdio.h>
#include "GraphNode.h"
#include <vector>
#include "Branch.h"
#include "Sif.h"
#include "FTree.h"
#include "FNode.h"
#include "GraphLevel.h"
#include <deque>
#include "Branch_p.h"
#include "Worker.h"
#include <pthread.h>



using namespace std;

class Branch_p;
class Worker;

class Graph {
public:
    Graph();
    Graph(const Graph& orig);
    virtual ~Graph();

    vector<GraphNode*> nodes;

    void TopologicSort();
    
    Graph *Clone();
    
    vector<GraphLevel*> levels;
    
    //vector<Graph*> exhaustiveSearchCollection; /*Holds a set of copies of dataflow graph which were obrained during Exhaustive search*/
    
    
    void printGraph();
    
    /*Actually Exhaustive search method declaration*/
    void ExhaustSearch();
    
    int g_id;
    
    vector<Graph*> linkedNodes;
    int level;
    
    
    /*Branch-and-bound method, serial edition, declaration :*/
    void BB();
    
    /*Branch-and-bound method, parallel edition, declaration :*/
    void BB_parallel();
    deque<Branch_p*> task_pull_p; /*Task pull for parallel Branch-and-bound realization*/
    
    int w_busy; /*Number of workers, which are busy*/
    pthread_mutex_t mutex;/*A mutext, needed for exclusive access to the Task Pull*/
    
    vector<Graph*> OptResults;
    
    //-----------нерекурсивные методы -------//
   void ExhaustSearch2();
   
    
private:
    int** GetInputNodesArray();
    int*  GetInDegree(int **array, int* workArray);
    
    int treeSize; 
    vector<GraphNode*> g_nodes; 
    FTree *f_tree; /*Combinatorial Tree*/
    void addLinkedNodes(GraphNode *g_node, FNode *fnode,int levNum,int nodeNum); /*Recursive method for making a combinatorial tree*/
    void Traverse(FNode *fnode);
    int N;
    int id;
    
    vector<Worker*> workers;/*Array of workers*/
    bool freeWorkerFound;/*Free worker found indicator*/
    
    int endIndicator;
    
   void Visit(FNode *fnode);
   
   deque<Branch*> task_pull;
   
   
   
   void addNodesToBranch(GraphNode *g_node, Branch *branch, int nodeNum, int levNum);
   void Modify(Branch *b); /*Graph modification method*/
   
   
   
   void addNodesToBranch_parallel(GraphNode *g_node, Branch_p *branch_p, int nodeNum, int levNum, Graph *graph_copy);
    
   void addLinkedNodes2(GraphNode *g_node, FNode *fnode,int levNum,int nodeNum);
   
    
   
};

#endif	/* GRAPH_H */

