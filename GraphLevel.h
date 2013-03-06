/* 
 * File:   GraphLevel.h
 * Author: ilya
 *
 * Created on 23 Октябрь 2011 г., 21:34
 */

#ifndef GRAPHLEVEL_H
#define	GRAPHLEVEL_H

//#include "Graph.h"
#include <stdio.h>
#include "GraphNode.h"
#include <vector>
#include "FNode.h"


using namespace std;

class GraphLevel {
public:
    GraphLevel();
    GraphLevel(const GraphLevel& orig);
    virtual ~GraphLevel();
    
    vector<GraphNode*> levelNodes;
    vector<FNode*> f_levelNodes;
    bool ready;
    int num;
    bool allowChanges;
    
    //vector<Graph*> g_levelNodes;
    
private:

};

#endif	/* GRAPHLEVEL_H */

