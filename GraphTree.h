/* 
 * File:   GraphTree.h
 * Author: ilya
 *
 * Created on 3 Ноябрь 2011 г., 18:45
 */

#ifndef GRAPHTREE_H
#define	GRAPHTREE_H

#include "Graph.h"
#include "vector"
#include "GTreeLevel.h"

using namespace std;

class GraphTree {
public:
    GraphTree();
    GraphTree(const GraphTree& orig);
    virtual ~GraphTree();
    
    void TopologicSort();
    vector<GTreeLevel*> levels;
    
    
    
    vector<Graph*> graph_nodes;//узлы, которые состоят из графов
    
    
    
private:
    int** GetInputNodesArray();
    int*  GetInDegree(int **array, int* workArray);
    
    
};

#endif	/* GRAPHTREE_H */

