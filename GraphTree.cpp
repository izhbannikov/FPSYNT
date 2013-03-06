/* 
 * File:   GraphTree.cpp
 * Author: ilya
 * 
 * Created on 3 Ноябрь 2011 г., 18:45
 */

#include "GraphTree.h"

GraphTree::GraphTree() {
}

GraphTree::GraphTree(const GraphTree& orig) {
}

GraphTree::~GraphTree() {
}


int** GraphTree::GetInputNodesArray() {
    
    int size = this->graph_nodes.size();
    
    int **array = new int*[size];
    
    //Создаём матрицу 2 на 2 и заполняем её нулями: 
    for(int i = 0; i < size; i++) {
        array[i] = new int[size];
        for(int j = 0; j < size; j++) {
            array[i][j] = 0;
        }
    }
    
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < this->graph_nodes[i]->linkedNodes.size(); j++) {
            array[i][this->graph_nodes[i]->linkedNodes[j]->g_id] = 1;
            
        }
        
        
    }
    
    
  return array;
}


void GraphTree::TopologicSort() {
    
    int currLevel = 0;
    
    GTreeLevel *level = new GTreeLevel();
    
    int var = this->graph_nodes.size();
    
    int **array = GetInputNodesArray();
    
    int *workArray = new int[this->graph_nodes.size()];
    
    while(var>0) {
        
        level = new GTreeLevel();
        level->num = currLevel;
        
        workArray = GetInDegree(array,workArray);
        
        for(int i = 0; i < this->graph_nodes.size(); i++) {
            if(workArray[i]==0) {
                
                workArray[i] = 65535;//помечаем ячейки с нулевыми полустепенями захода большим числом
                
                this->graph_nodes[i]->level = currLevel;
                level->g_levelNodes.push_back(this->graph_nodes[i]);
                
                //Исключаем из рассмотрения соотв строчки (у которых полустепени заходов нулевые) - помечаем такие строски двойками
                for(int j = 0; j < this->graph_nodes.size(); j++) {
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
int*  GraphTree::GetInDegree(int **array, int* workArray) {
    
    for(int j = 0; j < this->graph_nodes.size(); j++) {
        for(int i = 0; i < this->graph_nodes.size(); i++) {
            if ((array[i][1] != 2) && (workArray[j] != 65535)) {
                //вычисляем полустепени вхождения вершин
                workArray[j] = workArray[j] + array[i][j];
            } 
            
        }
        
    }
    
    
    
    return workArray;
}

