/* 
 * File:   Worker.h
 * Author: ilya
 *
 * Created on 25 Ноябрь 2011 г., 21:38
 */

#ifndef WORKER_H
#define	WORKER_H

#include <pthread.h>
#include <stdlib.h>
#include "Graph.h"
#include "Branch_p.h"
#include "GraphNode.h"


class Graph;
class Branch_p;
//Threaded class
class Worker
{
    
public :
    Worker() : m_stoprequested(false), m_running(false) {};
    //virtual ~Worker();
    Worker(const Worker& orig);
    
    // Create the thread and start work
    void start() 
    {
        m_running = true;
        //printf("Worker %d started...\n",this->id);
        pthread_create(&m_thread, 0, &start_thread, this);
        
    }
    
    void stop() 
    {
        m_running = false;
        
        pthread_detach(m_thread);
        pthread_cancel(m_thread);
        
        m_stoprequested = true;
        //printf("Worker %d finished...\n",this->id);
        
    }
    
    void ReceiveWorkTask(Graph *masterDFG, Branch_p* min_branch_err_p);
    bool m_running;
    int id;
private :
    bool m_stoprequested;
    //volatile bool m_running;
    pthread_t m_thread;
    pthread_mutex_t mutex;
    Branch_p *min_branch_err_p;
    Graph *masterDFG;
    // This is the static class function that serves as a C style function pointer
    // for the pthread_create call
    static void *start_thread(void *obj)
    {
        //All we do here is call the WorkerTask() function
        reinterpret_cast<Worker*>(obj)->WorkerTask();
        
    }
    /*Acual method for the Worker*/
    void WorkerTask();
    Graph *Modify_parallel(Branch_p *b_p,Graph *graph);
    void addNodesToBranch_parallel(GraphNode *g_node, Branch_p *branch_p, int nodeNum, int levNum, Graph *graph_copy);
};


#endif	/* WORKER_H */

