/* 
 * File:   GTreeLevel.h
 * Author: ilya
 *
 * Created on 3 Ноябрь 2011 г., 19:34
 */

#ifndef GTREELEVEL_H
#define	GTREELEVEL_H

#include "Graph.h"
#include <stdio.h>

#include <vector>

class GTreeLevel {
public:
    GTreeLevel();
    GTreeLevel(const GTreeLevel& orig);
    virtual ~GTreeLevel();
    
    bool ready;
    int num;
    
    vector<Graph*> g_levelNodes;
    
private:

};

#endif	/* GTREELEVEL_H */

