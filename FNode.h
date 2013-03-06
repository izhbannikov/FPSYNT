/* 
 * File:   FNode.h
 * Author: ilya
 *
 * Created on 28 Октябрь 2011 г., 16:16
 */

#ifndef FNODE_H
#define	FNODE_H

#include <vector>
#include "Sif.h"
#include "GraphNode.h"

using namespace std;
class FNode {
public:
    FNode();
    FNode(const FNode& orig);
    virtual ~FNode();
    
    vector<FNode*> linkedNodeList;//Этот список хранит все FNodes которые являются потомками для этого узла
    
    Sif *value;//значение, содержащееся в FNode
    
    int level;
    
    int id;
    
    GraphNode *gNode; //ссылка на узел вычислительного графа, с которым связан данный узел
    
    double error_value;//значение ошибки, соответствующее sif value
    
    
private:

};

#endif	/* FNODE_H */

