/* 
 * File:   Worker.cpp
 * Author: ilya
 * 
 * Created on 25 Ноябрь 2011 г., 21:38
 */

#include "Worker.h"
/*
Worker::Worker(){
    
}
*/


void Worker::ReceiveWorkTask(Graph *masterDFG, Branch_p* min_branch_err_p) {
    this->masterDFG = masterDFG;
    this->min_branch_err_p = min_branch_err_p;
    pthread_mutex_init(&mutex, NULL);
}


void Worker::WorkerTask() {
    
   
    min_branch_err_p->dflGraph = Modify_parallel(min_branch_err_p,min_branch_err_p->dflGraph);
   
    for(int i=0; i<min_branch_err_p->dflGraph->nodes.size(); i++) {
        for(int j=0; j<min_branch_err_p->dflGraph->nodes.size(); j++) {
            if(min_branch_err_p->dflGraph->nodes[i]->op1_id == min_branch_err_p->dflGraph->nodes[j]->id) {
                min_branch_err_p->dflGraph->nodes[i]->op1 = min_branch_err_p->dflGraph->nodes[j]->Clone();
            }
            if(min_branch_err_p->dflGraph->nodes[i]->op2_id == min_branch_err_p->dflGraph->nodes[j]->id) {
                min_branch_err_p->dflGraph->nodes[i]->op2 = min_branch_err_p->dflGraph->nodes[j]->Clone();
            }
        }
    }
   
   
    GraphNode *node;
    for(int j= 0; j<min_branch_err_p->dflGraph->levels[min_branch_err_p->nodeLevel+1]->levelNodes.size(); j++) {
        node = min_branch_err_p->dflGraph->levels[min_branch_err_p->nodeLevel+1]->levelNodes[j];
        if((node->op1 != NULL) && (node->op2 != NULL)) {
          node->compute();
        } 
        
    }
            
    addNodesToBranch_parallel(min_branch_err_p->dflGraph->levels[min_branch_err_p->nodeLevel+1]->levelNodes[0], min_branch_err_p, 0,min_branch_err_p->nodeLevel+1,min_branch_err_p->dflGraph);
    
    
    if(min_branch_err_p->nodeLevel == min_branch_err_p->dflGraph->levels.size() -2) {
        min_branch_err_p->status == 3;
        this->masterDFG->OptResults.push_back(min_branch_err_p->dflGraph);  
        
    }
    
    pthread_mutex_lock(&masterDFG->mutex);
        this->masterDFG->w_busy--;
    pthread_mutex_unlock(&masterDFG->mutex);
    
    stop();

}


Graph *Worker::Modify_parallel(Branch_p *b_p,Graph *graph) {
    if (b_p->parent != NULL) {
        for(int i=0; i<graph->nodes.size(); i++) {
                if(graph->nodes[i]->id == b_p->gNode_Id) {
                        graph->nodes[i]->value = b_p->value;
                        graph->nodes[i]->err = b_p->error_value;
                        graph->nodes[i]->epsilon = b_p->epsilon;
                }
        }
        this->Modify_parallel(b_p->parent,graph);
    }
    
    return graph;
}


void Worker::addNodesToBranch_parallel(GraphNode *g_node, Branch_p *branch_p, int nodeNum, int levNum, Graph *graph_copy) {
     
     
     
    for(int i=0; i<g_node->combinations.size(); i++) {
       Branch_p *new_branch_p = new Branch_p();
       new_branch_p->nodeLevel = levNum;
       new_branch_p->value = g_node->combinations[i];//устанавливаем сиф-значение
       new_branch_p->parent = branch_p;
       new_branch_p->gNode_Id = g_node->id;
       new_branch_p->dflGraph = graph_copy->Clone();
       
       if(g_node->error_array[i] == 0) {
          if(g_node->op1->err > g_node->op2->err) {
              g_node->err = g_node->op1->err;
              new_branch_p->error_value = g_node->op1->err;
              
          } else {
              g_node->err = g_node->op2->err;
              new_branch_p->error_value = g_node->op2->err;
          }
        } else {
           if(g_node->op1->err > g_node->op2->err) {
              if(g_node->op1->err > g_node->error_array[i]) {
                 g_node->err = g_node->op1->err;
                 new_branch_p->error_value = g_node->err;
              } else {
                 g_node->err = g_node->error_array[i];
                 new_branch_p->error_value = g_node->err;
              }
                
           } else {
              if(g_node->op2->err > g_node->error_array[i]) {
                 g_node->err = g_node->op2->err;
                 new_branch_p->error_value = g_node->err;
              } else {
                 g_node->err = g_node->error_array[i];
                 new_branch_p->error_value = g_node->err;
              }   
           }
        }
        
        g_node->value = g_node->combinations[i];
        nodeNum++;
        GraphNode *new_gnode;
        g_node->epsilon = g_node->epsilon_array[i];
        new_branch_p->epsilon = g_node->epsilon;
        
        if(nodeNum < (new_branch_p->dflGraph->levels[levNum]->levelNodes.size())) {
            new_gnode = new GraphNode();
            new_gnode = new_branch_p->dflGraph->levels[levNum]->levelNodes[nodeNum];
            this->addNodesToBranch_parallel(new_gnode,new_branch_p,nodeNum, levNum,new_branch_p->dflGraph);
            nodeNum--;
        } else {
              //pthread_mutex_lock(&this->masterDFG->mutex);
                this->masterDFG->task_pull_p.push_back(new_branch_p);
              //pthread_mutex_unlock(&this->masterDFG->mutex);
        }
          
    }
    
     
}

