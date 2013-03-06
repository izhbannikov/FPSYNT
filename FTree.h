/* 
 * File:   FGraph.h
 * Author: ilya
 * 
 * Класс представляет собой комбинаторное дерево. Это дерево состоит из узлов, 
 * являющихся различными вариациями сиф-разбиения узлов исходного вычислительного
 * графа.
 *
 * Created on 28 Октябрь 2011 г., 16:24
 */

#ifndef FTREE_H
#define	FTREE_H

#include "FNode.h"
#include <vector>
#include "GraphLevel.h"
#include "Sif.h"
//#include "Graph.h"

using namespace std;

class FTree {
public:
    FTree();
    FTree(const FTree& orig);
    virtual ~FTree();
    
    FNode *root;
    
    vector<FNode*> f_nodes;
    
    vector<GraphLevel*> levels;
    
    void TopologicSort();
    void printGraph();
    
    Sif** GetShiftMatrix(int row_num, int col_num);
    
    double **GetErrorMatrix(int row_num, int col_num);
    
    
    
    
private:
    int** GetInputNodesArray();
    int*  GetInDegree(int **array, int* workArray);
    
    void GetRow(FNode *f_node,int col_num);
    void GetErrorRow(FNode *f_node,int col_num);
    
    Sif **shiftMatrix;
    double **errorMatrix;
    
    vector<Sif*> temp_arr;
    vector<double> temp_error_arr;
    
    
    
    int n;
};

#endif	/* FGRAPH_H */

