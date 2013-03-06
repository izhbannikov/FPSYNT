/* 
 * File:   Graph.cpp
 * Author: ilya
 * 
 * Created on 13 Октябрь 2011 г., 12:02
 */


#include "Graph.h"
#include "GraphLevel.h"
#include <cmath>
#include "Sif.h"
#include "GraphTree.h"


Graph::Graph() {
    treeSize = 0;
    this->g_id = 0;
    this->level = 0;
    /*First, we are creating workers:*/
    for(int i=0; i<6; i++) {
        Worker *w = new Worker();
        w->id = i;
        this->workers.push_back(w);
        
    }
}

Graph::Graph(const Graph& orig) {
    
    
}

Graph::~Graph() {
}

/*This method creates the Adjacency Matrix of initial dataflow graph:*/
int** Graph::GetInputNodesArray() {
    
    int size = this->nodes.size();
    
    int **array = new int*[size];
    
    //Создаём матрицу 2 на 2 и заполняем её нулями: 
    //Сложность при этом: Общее количество итераций внутреннего цикла равно size*size
    //Общее количество итераций внешнего цикла равно size
    for(int i = 0; i < size; i++) {
        array[i] = new int[size];
        for(int j = 0; j < size; j++) {
            array[i][j] = 0;
        }
    }
    
    //Сложность при этом: Общее количество итераций внутреннего цикла равно size*2
    //Общее количество итераций внешнего цикла равно size
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < this->nodes[i]->linkedNodes.size(); j++) {
            array[i][this->nodes[i]->linkedNodes[j]->id] = 1;
            
        }
        
        
    }
    
    //Сложность всей функции при этом:
    //size*size или nodes^2
    
  return array;
}

/*Topological Sorting method: */
//тогда сложность топологической сортировки равна nodes^4 потому, что имеется во внутреннем цикле вызов
//функции GetInDegree и поэтому сложности процедур перемножаются.
void Graph::TopologicSort() {
    
    int currLevel = 0;
    
    GraphLevel *level = new GraphLevel();
    
    int var = this->nodes.size();
    
    //Сложность GetInputNodesArray функции при этом:
    //size*size или nodes^2
    int **array = GetInputNodesArray();
    
    int *workArray = new int[this->nodes.size()];
    
    //сложность цикла while равна числу узлов nodes
    while(var>0) {
        
        level = new GraphLevel();
        level->num = currLevel;
        
        //сложность при этом: 
        //nodes^2
        workArray = GetInDegree(array,workArray);
        
        //сложность цикла при этом в самом худшем случае равна: nodes
        for(int i = 0; i < this->nodes.size(); i++) {
            if(workArray[i]==0) {
                
                workArray[i] = 65535;//помечаем ячейки с нулевыми полустепенями захода большим числом
                
                this->nodes[i]->level = currLevel;
                level->levelNodes.push_back(this->nodes[i]);
                
                //Исключаем из рассмотрения соотв строчки (у которых полустепени заходов нулевые) - помечаем такие строски двойками
                //сложность цикла при этом в самом худшем случае равна: nodes^2
                for(int j = 0; j < this->nodes.size(); j++) {
                    array[i][j] = 2;
                }
                var--;
            } else if ((workArray[i]!=0)&&(workArray[i]!=65535)) {
                workArray[i] = 0;
            }
        }
        
        this->levels.push_back(level);
        
        currLevel++;
        
        
    }
      
}

/*This function finds an Input Half-Degree of a graph node*/
//сложность при этом: 
//nodes^2
int*  Graph::GetInDegree(int **array, int* workArray) {
    
    for(int j = 0; j < this->nodes.size(); j++) {
        for(int i = 0; i < this->nodes.size(); i++) {
            if ((array[i][1] != 2) && (workArray[j] != 65535)) {
                //вычисляем полустепени вхождения вершин
                workArray[j] = workArray[j] + array[i][j];
            } 
            
        }
        
    }
    
    
    
    return workArray;
}



/*Printing DFG*/
void Graph::printGraph() {
    
    for(int i =0; i<this->nodes.size();i++) {
        printf("Node: %d Name: %s Level: %d Ready: %d, Error:  %16.16f ",this->nodes[i]->id,this->nodes[i]->name,this->nodes[i]->level, this->nodes[i]->ready,this->nodes[i]->err);
        this->nodes[i]->printSIF();
        
        
    }
    
    printf("------End of graph-------\n");
}


/*Copying of the DFG:*/
Graph *Graph::Clone() {
    Graph *gf = NULL;
    gf = new Graph();
    
    for(int k=0; k<this->nodes.size(); k++) {
            GraphNode *gn = this->nodes[k]->Clone();
            gf->nodes.push_back(gn);
            
    }
    
    //gf->printGraph();
   
    //теперь копируем уровни
    for(int l=0; l<this->levels.size(); l++) {
            GraphLevel *gl = new GraphLevel();
            gl->num = this->levels[l]->num;
            gl->ready = this->levels[l]->ready;
            gl->allowChanges = this->levels[l]->allowChanges;
            //ищем айди
            for(int ii=0; ii<this->levels[l]->levelNodes.size(); ii++) {
                //получаем очередной ай-ди узла
                int l_id = this->levels[l]->levelNodes[ii]->id;
                //ищем этот ай-ди и добавляем этот узел 
                for(int kk = 0; kk<gf->nodes.size();kk++) {
                    if (gf->nodes[kk]->id == l_id) gl->levelNodes.push_back(gf->nodes[kk]);
                }
            }
            gf->levels.push_back(gl);
    }
    //gf->printGraph();
    //Теперь разбираемся с операндами:
    for(int i=0; i<gf->nodes.size(); i++) {
        //берём узел:
        for(int j=0; j<gf->nodes.size(); j++) {
                //берём узел:
            if(gf->nodes[i]->op1_id == gf->nodes[j]->id) {
                gf->nodes[i]->op1 = gf->nodes[j]->Clone();
            }
            if(gf->nodes[i]->op2_id == gf->nodes[j]->id) {
                gf->nodes[i]->op2 = gf->nodes[j]->Clone();
            }
        }
    }
    
    
    //gf->printGraph();
    return gf;
    
    
}

/*Exhaustive Search Method realization*/
void Graph::ExhaustSearch() {
    /*First, we need obtain a set of levels of initial DFG*/
    /*Then we have to start from Level #1*/
    /*Main Loop of Exhaustive Search method: */
    /*Loop thru all levels : */
    for(int i=1; i < this->levels.size()-1; i++) {
        /*If level is not ready:*/
        /*Compute nodes*/
        /*If level is not ready. We do not consider levels that are ready.*/
        if (this->levels[i]->ready == false ) {
            
            /*Loop thru all nodes of this level:*/
            for(int j= 0; j<this->levels[i]->levelNodes.size(); j++) {
                GraphNode *node = this->levels[i]->levelNodes[j];
                if((node->op1 != NULL) && (node->op2 != NULL)) {
                    node->compute(); //Calculating the SIF-partitioning
                } 
            }
            
            this->g_nodes = this->levels[i]->levelNodes;
            this->levels[i]->allowChanges = true;
            
            /*Here we are building the Combinatorial Tree:*/
            this->f_tree = new FTree(); /*f_tree is a combinatorial tree*/
            FNode *f_node = new FNode(); /*A node of a combinatorial tree*/
            f_tree->root = f_node; /*A root of a combinatorial tree*/
            f_tree->f_nodes.push_back(f_node);
            f_node->id = id;/*Calculating the id of a node of the combinatorial tree*/
            this->N = 0;
            treeSize++; /*Increasing the size of the combinatorial tree*/
            
            //если взять среднее количество элементов в массиве combinations[]
            //равное 2, то сложность будет 2^N, где N - высота комбинаторного древа
            //можно взять высоту комбинаторного дерева равную 5
            //тогда сложность алгоритма будет 2^5
            //высота комбинаторного  дерева зависит от количества узлов и 
            //количества уровней. примем за среднее количество узлов на уровне 
            //равным 4 и количество уровней равным 8. Тогда общее число узлов 
            //будет равным 32 и сложность алгоритма будет 2^32.
            //в общем случае: 2^nodes
            /*Recursive method to add nodes to the combinatorial tree : */
            this->addLinkedNodes(this->levels[i]->levelNodes[0],f_node, i,0);
            
            f_tree->TopologicSort();
            
            this->Traverse(f_tree->root);
            
            break;
        }
    }
    
    int optimum_index = 0;
    for(int i=1; i<this->OptResults.size(); i++) {
        if(this->OptResults[optimum_index]->levels[this->levels.size()-2]->levelNodes[0]->err < this->OptResults[i]->levels[this->levels.size()-2]->levelNodes[0]->err) {
            optimum_index = i;
        }
        this->OptResults[i]->printGraph();
    }
    
    printf("-----Exhaustive searching results-----\n");
    this->OptResults[optimum_index]->printGraph();
    
}


/*This method recursively adds new nodes to the Combinatorial Tree:*/
/*First node "fnode" is a root*/
void Graph::addLinkedNodes(GraphNode *g_node, FNode *fnode,int levNum, int nodeNum) {
    /*This is actually a Depth-searching :*/
    /*Loop thru all possible SIF combinations of the particular graph node : */
    for(int i=0; i<g_node->combinations.size(); i++) {
        //формируем новый узел комбинаторного дерева :
        FNode *new_fnode = new FNode();
        new_fnode->value = g_node->combinations[i];//устанавливаем сиф-значение
        
        /*Processing the errors related to that particular SIF combinations of the graph node : */
        if(g_node->error_array[i] == 0) {
                if(g_node->op1->err > g_node->op2->err) {
                    g_node->err = g_node->op1->err;
                    new_fnode->error_value = g_node->op1->err;
                } else {
                    g_node->err = g_node->op2->err;
                    new_fnode->error_value = g_node->op2->err;
                }
        } else {
            if(g_node->op1->err > g_node->op2->err) {
                if(g_node->op1->err > g_node->error_array[i]) {
                    g_node->err = g_node->op1->err;
                    new_fnode->error_value = g_node->err;
                } else {
                    g_node->err = g_node->error_array[i];
                    new_fnode->error_value = g_node->err;
                }
                
            } else {
                 if(g_node->op2->err > g_node->error_array[i]) {
                    g_node->err = g_node->op2->err;
                    new_fnode->error_value = g_node->err;
                 } else {
                    g_node->err = g_node->error_array[i];
                    new_fnode->error_value = g_node->err;
                }   
           }
        }
        
        fnode->linkedNodeList.push_back(new_fnode);
        f_tree->f_nodes.push_back(new_fnode);
        id++;
        new_fnode->id = id;
        new_fnode->gNode = g_node;
        
        g_node->value = g_node->combinations[i];
        
        if(nodeNum == (this->levels[levNum]->levelNodes.size()-1)) {
            g_node->epsilon = g_node->epsilon_array[i];
            
            levNum++;
            
            for(int j= 0; j<this->levels[levNum]->levelNodes.size(); j++) {
                GraphNode *node = this->levels[levNum]->levelNodes[j];
                if((node->op1 != NULL) && (node->op2 != NULL)) {
                    node->compute();
                } 
            }
            
            this->addLinkedNodes(this->levels[levNum]->levelNodes[0],new_fnode,levNum,0);
                
            levNum--;
           
        } else {
            nodeNum++;
            
            GraphNode *new_gnode = new GraphNode();
                
            new_gnode = this->levels[levNum]->levelNodes[nodeNum];
            treeSize++;
                
            this->addLinkedNodes(new_gnode,new_fnode,levNum,nodeNum);
            nodeNum--;
        }
    }
    
}

/*This method recursively traveses the Combinatorial Tree and build a new copy of Dataflow Graph : */
void Graph::Traverse(FNode *fnode) {
    
    for(int i=0; i<fnode->linkedNodeList.size(); i++) {
        this->Visit(fnode->linkedNodeList[i]);
        this->Traverse(fnode->linkedNodeList[i]);
        
        if(this->endIndicator == (this->f_tree->levels.size()-1)) {
            
            this->OptResults.push_back(this->Clone());
        }
        this->endIndicator--;
    }
    
    
}

void Graph::Visit(FNode *fnode) {
    
    fnode->gNode->value = fnode->value;
    fnode->gNode->err = fnode->error_value;
    //здесь надо разобраться со сдвигами и направлениями:
    //сначала разбираемся со сложением:
    if(fnode->gNode->operation == 0) {
        if(fnode->gNode->op1 != NULL) {
                //определяем число битовых позиций на которые необходимо сдвинуть операнд:
                fnode->gNode->op1->shift_pos = fnode->gNode->op1->value->m_F - fnode->gNode->value->m_F;
        }
        if(fnode->gNode->op2 != NULL) {
                //определяем число битовых позиций на которые необходимо сдвинуть операнд:
                fnode->gNode->op2->shift_pos = fnode->gNode->op2->value->m_F - fnode->gNode->value->m_F;
        }
    } else {
        //теперь разбираемся с умножением:
        if(fnode->gNode->operation == 1) {
            if(fnode->gNode->op1 != NULL && fnode->gNode->op2 != NULL) {
                //определяем число битовых позиций на которые необходимо сдвинуть операнд:
                fnode->gNode->shift_mult = fnode->gNode->op1->value->m_F + fnode->gNode->op2->value->m_F - fnode->gNode->value->m_F;
            }
        }
    }
    
    //теперь разбираемся с init_err:
    if(fnode->gNode->op1 != NULL && fnode->gNode->op2 != NULL) {
        //if (fnode->gNode->op1->init_err > fnode->gNode->op2->init_err) {
        //    fnode->gNode->init_err = fnode->gNode->op1->init_err;
        //} else {
        //    fnode->gNode->init_err = fnode->gNode->op2->init_err;
        //}
        fnode->gNode->init_err = fnode->gNode->op1->init_err + fnode->gNode->op2->init_err;
    }
    
    fnode->gNode->err += fnode->gNode->init_err;
    
    this->endIndicator++;   
        
}

/*Branch-and-bound method, serial realization : */
void Graph::BB() {
     /*Go to the first level*/
     /*For this level we determine nodes*/
     /*Loop for all nodes of this level:*/
     for(int j= 0; j<this->levels[1]->levelNodes.size(); j++) {
        GraphNode *node = this->levels[1]->levelNodes[j];
        if((node->op1 != NULL) && (node->op2 != NULL)) {
                node->compute();
        } 
     }
     
     /*Main algorithm of serial BB-search :*/
     /*STEP-1*/
     /*Creating a start pull of tasks*/
     Branch *root = new Branch();
     root->parent = NULL;
     addNodesToBranch(this->levels[1]->levelNodes[0], root, 0,1);
     
     
     
     /*STEP-2*/
     /*
      * 1). Get zero task from Task Pull. Mark Branch as "Active".
      * 2). Check another nodes with the same level.
      * 3). If "Active" branch's error is greater then:
      *         -> Eliminate "Active branch", mark "Active" branch with lower error.
      * 4). Else
      *         -> Eliminate just checked branch and check another branch
      * 5). If the error of "Active" branch is equal of just checked branch - mark just checked branch as "Active" and goto another branch. 
      * 6). After checking of all nodes -> proceed "Active" branch serially.
     */
     for(int k=0; k<this->task_pull.size(); k++) {
     
        //1). Get zero task from Task Pull. Mark Branch as "Active".
        Branch *min_branch_err = task_pull[k];
        if(min_branch_err->status != 2) {
            min_branch_err->status= 1;
            for(int z=k;z<task_pull.size();z++) {
                //2). Check another nodes with the same level.
                if(min_branch_err->nodeLevel == task_pull[z]->nodeLevel) {
                    if(min_branch_err->error_value > task_pull[z]->error_value) {
                        //3). If "Active" branch's error is greater then:
                        //-> Eliminate "Active branch", mark "Active" branch with lower error.
                        min_branch_err->status = 2;
                        min_branch_err = task_pull[z];
                        min_branch_err->status = 1;
                        //k=z;
                    }  else {
                        if (min_branch_err->error_value == task_pull[z]->error_value) {
                           task_pull[z]->status = 1;
                           //k=z;
                        } else {
                           //* 4). Eliminate just checked branch and check another branch
                           //*-> Eliminate just checked branch and check another branch
                           task_pull[z]->status = 2;
                           
                        }
                    }
               }
           }
         
           //printf("Task pull size: %d\n",this->task_pull.size());
           
           //After checking of all nodes -> give "Active" branch to first-available worker
           this->Modify(min_branch_err);
           
           /*Goto next level*/
           for(int j= 0; j<this->levels[min_branch_err->nodeLevel+1]->levelNodes.size(); j++) {
                GraphNode *node = this->levels[min_branch_err->nodeLevel+1]->levelNodes[j];
                if((node->op1 != NULL) && (node->op2 != NULL)) {
                        node->compute();
                } 
           }
                
           addNodesToBranch(this->levels[min_branch_err->nodeLevel+1]->levelNodes[0], min_branch_err, 0,min_branch_err->nodeLevel+1);
           
           if(min_branch_err->nodeLevel == this->levels.size()-2) {
                min_branch_err->status == 3;
                this->OptResults.push_back(this->Clone());  
        
           }
     
        }
     }
     
     printf("Task pull FINAL size: %d\n",this->task_pull.size());
      
      printf("-----BB Serial searching results-----\n");
      vector<Graph*>::iterator res_i;
      //for(res_i = this->OptResults.begin(); res_i != this->OptResults.end(); ++res_i) {
      //    ((Graph*)*res_i)->printGraph();
      //}
     
}

/*Making branch, serial edition*/
void Graph::addNodesToBranch(GraphNode *g_node, Branch *branch, int nodeNum, int levNum) {
     
     
     
    for(int i=0; i<g_node->combinations.size(); i++) {
       Branch *new_branch = new Branch();
       new_branch->nodeLevel = levNum;
       new_branch->value = g_node->combinations[i];
       new_branch->parent = branch;
       
       if(g_node->error_array[i] == 0) {
                if(g_node->op1->err > g_node->op2->err) {
                    g_node->err = g_node->op1->err;
                    new_branch->error_value = g_node->op1->err;
                } else {
                    g_node->err = g_node->op2->err;
                    new_branch->error_value = g_node->op2->err;
                }
        } else {
            if(g_node->op1->err > g_node->op2->err) {
                if(g_node->op1->err > g_node->error_array[i]) {
                    g_node->err = g_node->op1->err;
                    new_branch->error_value = g_node->err;
                } else {
                    g_node->err = g_node->error_array[i];
                    new_branch->error_value = g_node->err;
                }
                
            } else {
                 if(g_node->op2->err > g_node->error_array[i]) {
                    g_node->err = g_node->op2->err;
                    new_branch->error_value = g_node->err;
                 } else {
                    g_node->err = g_node->error_array[i];
                    new_branch->error_value = g_node->err;
                }   
           }
        }
        
        new_branch->gNode = g_node;
        
        g_node->value = g_node->combinations[i];
        
        nodeNum++;
        GraphNode *new_gnode;
        
        g_node->epsilon = g_node->epsilon_array[i];
        new_branch->epsilon = g_node->epsilon;
        
        if(nodeNum < (this->levels[levNum]->levelNodes.size())) {
            new_gnode = new GraphNode();
            new_gnode = this->levels[levNum]->levelNodes[nodeNum];
            this->addNodesToBranch(new_gnode,new_branch,nodeNum, levNum);
            nodeNum--;
        } else {
            this->task_pull.push_back(new_branch);
        }
       
    }
    
     
}

void Graph::Modify(Branch *b) {
    if (b->parent != NULL) {
        b->gNode->value = b->value;
        b->gNode->err =b->error_value;
        b->gNode->epsilon = b->epsilon;
        this->Modify(b->parent);
    }
}



/*Branch-and-bound searching, parallel realization. Master thread - is the main thread. Workers are created with pthreads*/
void Graph::BB_parallel() {
     /*Go to the first level*/
     /*For this level we determine nodes*/
     /*Loop for all nodes of this level:*/
     Graph *graph_copy = this->Clone();
     for(int j= 0; j<graph_copy->levels[1]->levelNodes.size(); j++) {
        GraphNode *node = graph_copy->levels[1]->levelNodes[j];
        if((node->op1 != NULL) && (node->op2 != NULL)) {
                node->compute();
        } 
     }
     
     /*Main algorithm of parallel BB-search :*/
     /*STEP-1*/
     /*Creating a start pull of tasks*/
     Branch_p *root = new Branch_p();
     root->parent = NULL;
     addNodesToBranch_parallel(graph_copy->levels[1]->levelNodes[0], root, 0,1,graph_copy);
     
     /*STEP-2*/
     /*
      * 1). Get zero task from Task Pull. Mark Branch as "Active".
      * 2). Check another nodes with the same level.
      * 3). If "Active" branch's error is greater then:
      *         -> Eliminate "Active branch", mark "Active" branch with lower error.
      * 4). Else
      *         -> Eliminate just checked branch and check another branch
      * 5). If the error of "Active" branch is equal of just checked branch - mark just checked branch as "Active" and goto another branch. 
      * 6). After checking of all nodes -> give "Active" branch to first-available worker
     */
     Branch_p *min_branch_err_p;
     for(int k=0; k<this->task_pull_p.size(); ++k) {
         this->freeWorkerFound = false;
         
        //1). Get zero task from Task Pull. Mark Branch as "Active".
        min_branch_err_p = task_pull_p[k];
        if(min_branch_err_p->status != 2) {
            
            //if (min_branch_err_p->status != 1) {
            min_branch_err_p->status= 1;
            int lvl = min_branch_err_p->nodeLevel;
            for(int z=k;z<task_pull_p.size()-w_busy;z++) {
                //2). Check another nodes with the same level.
                //if(lvl == ((Branch_p*)*i1)->nodeLevel) {
                if(lvl == task_pull_p[z]->nodeLevel) {
                    if(min_branch_err_p->error_value > task_pull_p[z]->error_value){//((Branch_p*)*i1)->error_value) {
                        //3). If "Active" branch's error is greater then:
                        //-> Eliminate "Active branch", mark "Active" branch with lower error.
                        min_branch_err_p->status = 2;
                        min_branch_err_p = task_pull_p[z];//((Branch_p*)*i1);
                        min_branch_err_p->status = 1;
                        k=z;
                        
                    }  else {
                        if (min_branch_err_p->error_value ==  task_pull_p[z]->error_value) {//((Branch_p*)*i1)->error_value) {
                           //((Branch_p*)*i1)->status = 1;
                           task_pull_p[z]->status = 1;
                           //k=z;
                        } else {
                           //* 4). Eliminate just checked branch and check another branch
                           //*-> Eliminate just checked branch and check another branch
                           //((Branch_p*)*i1)->status = 2;
                           task_pull_p[z]->status = 2;
                        }
                    }
               }
           }
            
            for(int jj=0; jj<this->workers.size();jj++) {
                if(!this->workers[jj]->m_running) {
                                        this->w_busy++;
                                        this->workers[jj]->ReceiveWorkTask(this,min_branch_err_p);
                                        this->workers[jj]->start();
                                        this->freeWorkerFound = true;
                                        break;
                                }
                        }
                
                
                if(this->freeWorkerFound == false) {
                                for(;;) {
                                        for(int jj=0; jj<this->workers.size();jj++) {
                                                if(!this->workers[jj]->m_running) {
                                                        this->w_busy++;
                                                        this->workers[jj]->ReceiveWorkTask(this,min_branch_err_p);
                                                        this->workers[jj]->start();
                                                        this->freeWorkerFound = true;
                    
                                                        break;
                                                }
                                        }
                                        if(this->freeWorkerFound==true) break;
                                } 
                        }
                
                
           
        }
        //Wait for finishing of all workers:
        //printf("Wait...%d\n",this->w_busy);
        //printf("K...%d\n",k);
        if(k == this->task_pull_p.size()-1) {
           //printf("W...%d\n",this->w_busy);
           while (this->w_busy > 0) { 
               //printf("Wait...\n");
               if(k<this->task_pull_p.size()-1) {
                   break;
               }
           }    
        }
        
      }
     
     
      printf("Task pull FINAL size: %d\n",this->task_pull_p.size());
      
      
      
     
}

/*Creating a branch, parallel realization*/
void Graph::addNodesToBranch_parallel(GraphNode *g_node, Branch_p *branch_p, int nodeNum, int levNum, Graph *graph_copy) {
     
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
            this->task_pull_p.push_back(new_branch_p);
              
        }
          
    }
    
     
}
 

//=== нерекурсивные методы ======//
/*Exhaustive Search Method realization*/
void Graph::ExhaustSearch2() {
    /*First, we need obtain a set of levels of initial DFG*/
    /*Then we have to start from Level #1*/
    /*Main Loop of Exhaustive Search method: */
    /*Here we are building the Combinatorial Tree:*/
    int last_id = 0;
    this->f_tree = new FTree();
    FNode *root_node = new FNode();
    f_tree->root = root_node;
    f_tree->f_nodes.push_back(root_node);
    root_node->id = last_id;
    this->N = 0;
    treeSize++;
    GraphNode *start_node = new GraphNode();
    start_node = this->levels[1]->levelNodes[0];
    start_node->compute();
    
    //только для стартового узла:
    for(int i=0; i<start_node->combinations.size(); i++) {
        //формируем новый узел
        FNode *new_fnode = new FNode();
        new_fnode->value = start_node->combinations[i];//устанавливаем сиф-значение
        
        //printf("%d\n",g_node->combinations.size());
        
        if(start_node->error_array[i] == 0) {
           if(start_node->op1->err > start_node->op2->err) {
              start_node->err = start_node->op1->err;
              new_fnode->error_value = start_node->op1->err;
           } else {
              start_node->err = start_node->op2->err;
              new_fnode->error_value = start_node->op2->err;
           }
        } else {
            if(start_node->op1->err > start_node->op2->err) {
                if(start_node->op1->err > start_node->error_array[i]) {
                    start_node->err = start_node->op1->err;
                    new_fnode->error_value = start_node->err;
                } else {
                    start_node->err = start_node->error_array[i];
                    new_fnode->error_value = start_node->err;
                }
                
            } else {
                 if(start_node->op2->err > start_node->error_array[i]) {
                    start_node->err = start_node->op2->err;
                    new_fnode->error_value = start_node->err;
                 } else {
                    start_node->err = start_node->error_array[i];
                    new_fnode->error_value = start_node->err;
                }   
           }
        }
        
        root_node->linkedNodeList.push_back(new_fnode);
        f_tree->f_nodes.push_back(new_fnode);
        last_id++;
        new_fnode->id = last_id;
        new_fnode->gNode = start_node;
       
       // s_node->value = g_node->combinations[i];
    
    }
    
    int from = 1;
    int to = last_id;
    //цикл по уровням графа
    for(int i=1; i < 2; i++) {//this->levels.size()-1; i++) {//пока что по одному уровню
        /*Loop for all nodes of this level:*/
        for(int j= 1; j<this->levels[i]->levelNodes.size(); j++) {
           
           GraphNode *node = this->levels[i]->levelNodes[j];
           if((node->op1 != NULL) && (node->op2 != NULL)) {
              node->compute(); //вычисляем значение sif
           } else {
               printf("Error: check the operands of node %s\n", node->name);
               break;
           }
           
           // цил по возможным значениям узла:
           for(int n = from; n > to; n++) {
             for(int k= 1; k<node->combinations.size(); k++) {
                FNode *new_fnode = new FNode();
                new_fnode->value = start_node->combinations[i];//устанавливаем сиф-значение
        
                //printf("%d\n",g_node->combinations.size());
        
                if(node->error_array[i] == 0) {
                        if(start_node->op1->err > start_node->op2->err) {
                                start_node->err = start_node->op1->err;
                                new_fnode->error_value = start_node->op1->err;
                        } else {
                                start_node->err = start_node->op2->err;
                                new_fnode->error_value = start_node->op2->err;
                        }
                } else {
                        if(start_node->op1->err > start_node->op2->err) {
                                if(start_node->op1->err > start_node->error_array[i]) {
                                        start_node->err = start_node->op1->err;
                                        new_fnode->error_value = start_node->err;
                                } else {
                                        start_node->err = start_node->error_array[i];
                                        new_fnode->error_value = start_node->err;
                                }
                
                        } else {
                                if(start_node->op2->err > start_node->error_array[i]) {
                                        start_node->err = start_node->op2->err;
                                        new_fnode->error_value = start_node->err;
                                } else {
                                        start_node->err = start_node->error_array[i];
                                        new_fnode->error_value = start_node->err;
                                }   
                        }
                 }
        
                 this->f_tree->f_nodes[n]->linkedNodeList.push_back(new_fnode);
                 f_tree->f_nodes.push_back(new_fnode);
                 last_id++;
                 new_fnode->id = last_id;
                 new_fnode->gNode = node;
             }
          }
           
          from = to + node->combinations.size();
          to = last_id;
          start_node = node;
        }
        
        break;
  
    }

}

/*This method recursively adds new nodes to the Combinatorial Tree:*/
/*First node "fnode" is a root*/
/*
void Graph::addLinkedNodes2(GraphNode *start_node, FNode *root,int levNum, int nodeNum) {
    for(int i=0; i<start_node->combinations.size(); i++) {
        //формируем новый узел
        FNode *new_fnode = new FNode();
        new_fnode->value = g_node->combinations[i];//устанавливаем сиф-значение
        
        //printf("%d\n",g_node->combinations.size());
        
        if(g_node->error_array[i] == 0) {
                if(g_node->op1->err > g_node->op2->err) {
                    g_node->err = g_node->op1->err;
                    new_fnode->error_value = g_node->op1->err;
                } else {
                    g_node->err = g_node->op2->err;
                    new_fnode->error_value = g_node->op2->err;
                }
        } else {
            if(g_node->op1->err > g_node->op2->err) {
                if(g_node->op1->err > g_node->error_array[i]) {
                    g_node->err = g_node->op1->err;
                    new_fnode->error_value = g_node->err;
                } else {
                    g_node->err = g_node->error_array[i];
                    new_fnode->error_value = g_node->err;
                }
                
            } else {
                 if(g_node->op2->err > g_node->error_array[i]) {
                    g_node->err = g_node->op2->err;
                    new_fnode->error_value = g_node->err;
                 } else {
                    g_node->err = g_node->error_array[i];
                    new_fnode->error_value = g_node->err;
                }   
           }
        }
        
        fnode->linkedNodeList.push_back(new_fnode);
        f_tree->f_nodes.push_back(new_fnode);
        id++;
        new_fnode->id = id;
        new_fnode->gNode = g_node;
        
        g_node->value = g_node->combinations[i];
        
        if(nodeNum == (this->levels[levNum]->levelNodes.size()-1)) {
            g_node->epsilon = g_node->epsilon_array[i];
            
            levNum++;
            
            for(int j= 0; j<this->levels[levNum]->levelNodes.size(); j++) {
                GraphNode *node = this->levels[levNum]->levelNodes[j];
                if((node->op1 != NULL) && (node->op2 != NULL)) {
                    node->compute();
                } 
            }
            
            this->addLinkedNodes(this->levels[levNum]->levelNodes[0],new_fnode,levNum,0);
                
            levNum--;
           
        } else {
            nodeNum++;
            
            GraphNode *new_gnode = new GraphNode();
                
            new_gnode = this->levels[levNum]->levelNodes[nodeNum];
            treeSize++;
                
            this->addLinkedNodes(new_gnode,new_fnode,levNum,nodeNum);
            nodeNum--;
        }
    }
    
}
 */