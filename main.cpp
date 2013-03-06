#include <stdio.h>
#include "GraphNode.h"
#include "Graph.h"
#include <stdlib.h>
#include <vector>
#include <deque>
#include "GraphTree.h"
#include "timer.h"
#include <math.h>
#include <string.h>
#include "Converter.h"
#include "Statistics.h"
#include <pthread.h>


using namespace std;

typedef struct {
    int node_id;
    string linked_node_id;
    string name;
    short ss;
    short S;
    short I;
    short F;
    short sf;
    string op1;
    string op2;
    short operation;
    int op1_id;
    int op2_id;
} NodeStruct;



int main(int argc, char* argv[]) {
    
    double start, finish, elapsed;

    /*Reading input file : */
    std::fstream read_file(argv[1]);
    std::string line;
    
    int ii=0;
    int cnt = 0;
    
    vector<NodeStruct> record_block;
    
    while ( getline(read_file,line) ) {
       
       ii++;
       
       if(ii==1) {
         NodeStruct node_struct;
         node_struct.node_id = atoi(line.c_str());
         record_block.push_back(node_struct);
         
       }
       
       if(ii==2) {
           
           record_block[cnt].linked_node_id = line; //atoi(line.c_str());
       }
       
       if(ii==3) {
         record_block[cnt].name = line.c_str();
       }
       
       if(ii==4) {
           record_block[cnt].ss = atoi(line.c_str());
       }
       
       if(ii==5) {
           record_block[cnt].S = atoi(line.c_str());
       }
       
       if(ii==6) {
           record_block[cnt].I = atoi(line.c_str());
       }
       
       if(ii==7) {
           record_block[cnt].F = atoi(line.c_str());
       }
       
       if(ii==8) {
           record_block[cnt].sf = atoi(line.c_str());
       }
       
       if(ii==9) {
           record_block[cnt].op1 = line.c_str();
       }
       
       if(ii==10) {
           record_block[cnt].op2 = line;
       }
       
       if(ii==11) {
           record_block[cnt].operation = atoi(line.c_str());
       }
       
       if(ii==12) {
           record_block[cnt].op1_id = atoi(line.c_str());
       }
       
       if(ii==13) {
           record_block[cnt].op2_id = atoi(line.c_str());
           cnt++;
       }
       
       if(ii==14) {
           ii=0;
           
       }
       
    }
    
    Graph *graph = new Graph();
    graph->g_id = 0;
    //Automatically creating graph nodes
    for(int i = 0; i<record_block.size(); i++) {
        GraphNode *node = new GraphNode();
        node->id = i;
        graph->nodes.push_back(node);
    }
    
    for(int i = 0; i<record_block.size(); i++) {
        if(record_block[i].linked_node_id != "NULL") {
            graph->nodes[i]->linkedNodes.push_back(graph->nodes[ atoi(record_block[i].linked_node_id.c_str() ) ] );
        }
        
        graph->nodes[i]->name = (char*)record_block[i].name.c_str();
        graph->nodes[i]->value = new Sif(record_block[i].ss, record_block[i].S, record_block[i].I, record_block[i].F, record_block[i].sf);
        
        if(record_block[i].op1 == "NULL") {
            graph->nodes[i]->op1 = NULL;
        } else {
            graph->nodes[i]->op1 = graph->nodes[atoi(record_block[i].op1.c_str())];
        }
        
        if(record_block[i].op2 == "NULL") {
            graph->nodes[i]->op2 = NULL;
        } else {
            graph->nodes[i]->op2 = graph->nodes[atoi(record_block[i].op2.c_str())];
        }
        
        graph->nodes[i]->operation = record_block[i].operation;
        graph->nodes[i]->op1_id = record_block[i].op1_id;
        graph->nodes[i]->op2_id = record_block[i].op2_id;
        
        
        
    }
    
    Converter *converter = new Converter();
    //converter->CastToUInt16(0.15);
    //converter->CastToUInt16(0.05);
    //converter->CastToUInt16(0.45);
    //converter->CastToUInt16(0.35);
    //converter->write_data_to_file();
    
    /*Здесь происходит проверка гипотезы*/
    //Statistics *stat = new Statistics();
    //stat->pattern(8,0,10000);
    
    
    
    //printf("\n");
    
  //  converter->CastToSIF(0.15);
  //  converter->CastToSIF(0.05);
  //  converter->CastToSIF(0.45);
  //  converter->CastToSIF(0.35);
    
    
    
    
    ///
    //====================
    /*First we are doing a topological sorting of the initial dataflow graph:*/
    graph->TopologicSort();
    /*Printing the initial dataflow graph*/
    printf("Printing the initial dataflow graph:\n");
    graph->printGraph();
    
    //graph->ExhaustSearch2();
    
    /*Branch-and-bound method, parallel realization:*/
    /*
    printf("Branch-and-bound method, parallel realization:\n");
    graph->OptResults.clear();
    GET_TIME(start);
    graph->BB_parallel();
    GET_TIME(finish);
    elapsed = finish - start;
    printf("Elapsed time = %e seconds\n", elapsed);
    printf("-----BB Parallel searching results-----\n");
    vector<Graph*>::iterator res_i;
    //for(res_i = graph->OptResults.begin(); res_i != graph->OptResults.end(); ++res_i) {
    //    ((Graph*)*res_i)->printGraph();
    //}
    graph->OptResults.front()->printGraph();
    start = finish = elapsed = 0;
    printf("---End of Branch-and-bound method, Parallel realization---");
    printf("-------------------------------------:\n");
    */
    
    /*Branch-and-bound method, serial realization:*/
    /*
    printf("Branch-and-bound method, serial realization:\n");
    graph->OptResults.clear();
    GET_TIME(start);
    graph->BB();
    GET_TIME(finish);
    elapsed = finish - start;
    printf("Elapsed time = %e seconds\n", elapsed);
    printf("-----BB Serial searching results-----\n");
    //for(res_i = graph->OptResults.begin(); res_i != graph->OptResults.end(); ++res_i) {
    //    ((Graph*)*res_i)->printGraph();
    //}
    graph->OptResults.front()->printGraph();
    start = finish = elapsed = 0;
    printf("---End of Branch-and-bound method, serial realization---");
    printf("-------------------------------------:\n");
    */
    
    /*Exhaustive search*/
    printf("-----------Exhaustive search---------:\n");
    graph->OptResults.clear();
    GET_TIME(start);
    graph->ExhaustSearch();
    GET_TIME(finish);
    elapsed = finish - start;
    printf("Elapsed time = %e seconds\n", elapsed);
    start = finish = elapsed = 0;
    printf("-------End of Exhaustive search------:\n");
    printf("-------------------------------------:\n");
    
    
    return 0;

}


