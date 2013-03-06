/* 
 * File:   FGraph.cpp
 * Author: ilya
 * 
 * Created on 28 Октябрь 2011 г., 16:24
 */

#include "FTree.h"
#include "Graph.h"
#include <vector>

using namespace std;



FTree::FTree() {
    n = 0;
}

FTree::FTree(const FTree& orig) {
}

FTree::~FTree() {
}

int** FTree::GetInputNodesArray() {
    
    int size = this->f_nodes.size();
    
    int **array = new int*[size];
    
    //Создаём матрицу 2 на 2 и заполняем её нулями: 
    for(int i = 0; i < size; i++) {
        array[i] = new int[size];
        for(int j = 0; j < size; j++) {
            array[i][j] = 0;
        }
    }
    
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < this->f_nodes[i]->linkedNodeList.size(); j++) {
            array[i][this->f_nodes[i]->linkedNodeList[j]->id] = 1;
            
        }
        
        
    }
    /*
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            printf("%d\t", array[i][j]);
         }
        printf("\n");
    }
    */
    
  return array;
}


void FTree::TopologicSort() {
    
    int currLevel = 0;
    
    GraphLevel *level = new GraphLevel();
    
    int var = this->f_nodes.size();
    
    int **array = GetInputNodesArray();
    
    int *workArray = new int[this->f_nodes.size()];
    
    while(var>0) {
        
        level = new GraphLevel();
        level->num = currLevel;
        
        workArray = GetInDegree(array,workArray);
        
        for(int i = 0; i < this->f_nodes.size(); i++) {
            if(workArray[i]==0) {
                
                workArray[i] = 65535;//помечаем ячейки с нулевыми полустепенями захода большим числом
                
                this->f_nodes[i]->level = currLevel;
                level->f_levelNodes.push_back(this->f_nodes[i]);
                
                //Исключаем из рассмотрения соотв строчки (у которых полустепени заходов нулевые) - помечаем такие строски двойками
                for(int j = 0; j < this->f_nodes.size(); j++) {
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

//эта функция считает полустепени вхождения вершин графа и сохраняет их в одномерном массиве
int*  FTree::GetInDegree(int **array, int* workArray) {
    
    for(int j = 0; j < this->f_nodes.size(); j++) {
        for(int i = 0; i < this->f_nodes.size(); i++) {
            if ((array[i][1] != 2) && (workArray[j] != 65535)) {
                //вычисляем полустепени вхождения вершин
                workArray[j] = workArray[j] + array[i][j];
            } 
            
        }
        
    }
    
    
    
    return workArray;
}




Sif** FTree::GetShiftMatrix(int row_num, int col_num) {
    this->shiftMatrix = new Sif*[row_num];
    
    //Создаём матрицу  и заполняем её нулями: 
    for(int i = 0; i < row_num; i++) {
        this->shiftMatrix[i] = new Sif[col_num];
        for(int j = 0; j < col_num; j++) {
            this->shiftMatrix[i][j].set(0,0,0,0,0);
        }
    }
    
    
    GetRow(this->root,col_num);
    
    /*
    for(int i = 0; i < row_num; i++) {
        //for(int j = 0; j < col_num; j++) {
            this->shiftMatrix[i][0].printSIF();
         //}
        printf("\n");
    }
    **/
    this->n = 0;
  return this->shiftMatrix;
}


void  FTree::GetRow(FNode *f_node,int col_num) {
    
    //int n=0;
    
    
    for(int i=0; i<f_node->linkedNodeList.size(); i++) {
        
        this->temp_arr.push_back(f_node->linkedNodeList[i]->value);
        
        GetRow(f_node->linkedNodeList[i],col_num);
        
    }
    
    
    if(temp_arr.size() == col_num) {
        for(int i=0; i<temp_arr.size(); i++) {
                this->shiftMatrix[n][i].set(0, temp_arr[i]->m_S, temp_arr[i]->m_I, temp_arr[i]->m_F, 0);
        }
        n++;
    }
    
    
    
    temp_arr.pop_back();
    
    
}



//получаем массив ошибок
double** FTree::GetErrorMatrix(int row_num, int col_num) {
    this->errorMatrix = new double*[row_num];
    
    //Создаём матрицу  и заполняем её нулями: 
    for(int i = 0; i < row_num; i++) {
        this->errorMatrix[i] = new double[col_num];
        for(int j = 0; j < col_num; j++) {
            this->errorMatrix[i][j] = 0.0;
        }
    }
    
    
    GetErrorRow(this->root,col_num);
    
    /*
    for(int i = 0; i < row_num; i++) {
        //for(int j = 0; j < col_num; j++) {
            this->shiftMatrix[i][0].printSIF();
         //}
        printf("\n");
    }
    **/
    
  return this->errorMatrix;
}


void  FTree::GetErrorRow(FNode *f_node,int col_num) {
    
    //int n=0;
    
    
    for(int i=0; i<f_node->linkedNodeList.size(); i++) {
        
        this->temp_error_arr.push_back(f_node->linkedNodeList[i]->error_value);
        
        GetErrorRow(f_node->linkedNodeList[i],col_num);
        
    }
    
    
    if(temp_error_arr.size() == col_num) {
        for(int i=0; i<temp_error_arr.size(); i++) {
                this->errorMatrix[n][i] = temp_error_arr[i];
        }
        n++;
    }
    
    
    
    temp_error_arr.pop_back();
    
    
}

//примитивная печать графа
void FTree::printGraph() {
    for(int i =1; i<this->f_nodes.size();i++) {
        printf("Node: %d Level: %d Error: %f ",this->f_nodes[i]->id,this->f_nodes[i]->level,this->f_nodes[i]->error_value);
        this->f_nodes[i]->value->printSIF();
        printf("\n");
    }
    printf("\n------End of combinatorial tree-------\n");
}

