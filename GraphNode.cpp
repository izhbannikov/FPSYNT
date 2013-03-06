/* 
 * File:   GraphNode.cpp
 * Author: ilya
 * 
 * Created on 6 Октябрь 2011 г., 15:38
 */

#include "GraphNode.h"


GraphNode::GraphNode() {
    this->tag = false;
    this->level = 0;
    this->id = 0;
    this->ready = false;
    //this->op1= new Sif(0,0,0,0,0);
    //this->op2= new Sif(0,0,0,0,0);
    this->value = new Sif(0,0,0,0,0);
    this->operation = 0;
    this->WL = 16;
    this->err = 0;
    this->epsilon = 0;
    
    
}

GraphNode::GraphNode(const GraphNode& orig) {
}

GraphNode::~GraphNode() {
}

void GraphNode::compute() {
    switch (this->operation) {
        case 0:
            this->combinations.clear();
            this->error_array.clear();
            /*this->value = */this->calculateSIFSum(this->op1->value,this->op2->value); 
            this->ready = true;
            break;
        case 1: 
            this->combinations.clear();
            this->error_array.clear();
            /*this->value = */this->calculateSIFMult(this->op1->value,this->op2->value); 
            this->ready = true;
            break;
        
    }
}


Sif *GraphNode::calculateSIFSum(Sif *op1, Sif *op2) {
    //printf("Addition:\n");
    
    Sif *o1, *o2;
    if(op1 == NULL) {
        o1 = new Sif(0,0,0,0,0); 
        o1->isZero = true;//вот такой вот топорный способ определения нулевого значения сиф
    } else {
        o1 = op1->Copy();//new Sif(0,op1->m_S,op1->m_I,op1->m_F,0);
    }
    if(op2 == NULL) {
        o2 = new Sif(0,0,0,0,0);
        o2->isZero = true;
    } else {
        o2 = op2->Copy();//new Sif(0,op2->m_S,op2->m_I,op2->m_F,0);
    }
    
    int deltaBP;
    if (o1->isZero || o2->isZero) {
            deltaBP = 0;
    } else {
        deltaBP = o1->L() - o2->L();
    }
    
    if (deltaBP<0) {
        o1->m_S = o1->m_S+abs(deltaBP);
    } else if (deltaBP>0) {
        o2->m_S = o2->m_S+abs(deltaBP);
    } 
    
    Sif *temp_res = new Sif(0,0,0,0,0);
    int c = 1; //carry-out
    temp_res->m_S = max(o1->m_S+o1->m_I, o2->m_S+o2->m_I) - max(o1->m_I, o2->m_I) - c;
    temp_res->m_I = max(o1->m_I, o2->m_I) + ((o1->isZero || o2->isZero) ? 0 : c);
    temp_res->m_F = max(o1->m_F, o2->m_F);
    
    
    int k_min, k_max;
    int S_min = 1;
    
    //сначала разбираемся с k_min
    k_min = -1*(temp_res->L() - temp_res->M() - S_min);
    Sif *res;
    if(k_min <= 0) {
        for(int i=k_min; i<=0; i++) {
            res = new Sif(0,0,0,0,0);
            res->m_S = temp_res->m_S + i;
            res->m_I = temp_res->m_I;
            res->m_F = WL - (res->L());
            this->combinations.push_back(res);
            //printf("(%d/%d/%d)\n",res->m_S,res->m_I,res->m_F);
            //вычисляем ошибку
            int tmp_epsilon = res->m_F - max(o1->m_F-this->op1->epsilon, o2->m_F-this->op2->epsilon);// + min(this->op1->epsilon,this->op2->epsilon);
            
            
            double tmp_err =  0.0;
            
            if(tmp_epsilon < 0) {
                tmp_err = this->calculateSumError(res,tmp_epsilon);
            } //else {tmp_epsilon = 0;}
            
            this->error_array.push_back(tmp_err);
            this->epsilon_array.push_back(tmp_epsilon);
        }
    } else {
        res = new Sif(0,0,0,0,0);
        res->m_S = 1;
        res->m_I = temp_res->m_I;
        res->m_F = WL - (res->L());
        this->combinations.push_back(res);
        //printf("(%d/%d/%d)\n",res->m_S,res->m_I,res->m_F);
        
        int tmp_epsilon = res->m_F - max(o1->m_F-this->op1->epsilon, o2->m_F-this->op2->epsilon);// + min(this->op1->epsilon,this->op2->epsilon);
        
            
        double tmp_err =  0.0;
            
        if(tmp_epsilon < 0) {
           tmp_err = this->calculateSumError(res,tmp_epsilon);
        }
        
        
        //double tmp_err =  this->calculateError(temp_res,res);
        this->error_array.push_back(tmp_err);
        this->epsilon_array.push_back(tmp_epsilon);
    }
    
    //впоследствии разберёмся и с этим, но позже
    k_max = this->WL - (temp_res->L() - temp_res->R());
    
    
    
    return res;
}

Sif *GraphNode::calculateSIFMult(Sif *op1, Sif *op2) {

    //printf("Multiplication:\n");
    //проверка форматов операндов перед умножением, для получения "валидного" результата
    if((op1->m_I + op2->m_I) > (this->WL - 1)) {
        //ошибка - увеличивай длину машинного слова, или уменьшай суммарную длину бит результата, 
        //оптимизируй, в общем!
        printf("Error, result does not fit machine word length. Increase a machine word length or decrease the number of bits in result.");
            
        exit(-1);
    }
    
    //дальше работаем только с "валидными" форматами !!!
    
    Sif *o1 = new Sif(0,op1->m_S,op1->m_I,op1->m_F,0);
    
    Sif *o2 = new Sif(0,op2->m_S,op2->m_I,op2->m_F,0);
    
    
    Sif *temp_res = new Sif();
    
    temp_res->m_S = o1->m_S + o2->m_S;
    temp_res->m_I = o1->m_I + o2->m_I;
    temp_res->m_F = o1->m_F + o2->m_F;
    
    //res = this->placeSIFResult(res);
    
    //this->combinations.push_back(res);
    
    
    int k_min, k_max;
    int S_min = 1;
    //int c=1;
    
    //сначала разбираемся с k_min
    k_min = -1*o1->m_S - o2->m_S + S_min;
    Sif *res;
    if(WL > (temp_res->m_I + temp_res->m_F)) {
        for(int i=k_min; i<=0; i++) {
            res = new Sif(0,0,0,0,0);
            res->m_S = temp_res->m_S + i;
            
            res->m_I = temp_res->m_I;
            res->m_F = WL - (res->L());
            if(res->m_S + res->m_I + temp_res->m_F > WL) break;
                
            this->combinations.push_back(res);
            //printf("(%d/%d/%d)\n",res->m_S,res->m_I,res->m_F);
            //вычисляем ошибку
            int tmp_epsilon = res->m_F - max(o1->m_F-this->op1->epsilon, o2->m_F-this->op2->epsilon);// + min(this->op1->epsilon,this->op2->epsilon);
            double tmp_err =  0.0;
            
            if(tmp_epsilon < 0) {
                tmp_err = this->calculateSumError(res,tmp_epsilon);
            } 
            
            
            this->error_array.push_back(tmp_err);
            this->epsilon_array.push_back(tmp_epsilon);
        }
//        int gg = this->combinations.size();
 //       int d=0;
    } else {
        
        res = new Sif(0,0,0,0,0);
        res->m_S = 1;
        res->m_I = temp_res->m_I;
        res->m_F = WL - (res->L());
         
        this->combinations.push_back(res);
        
        int tmp_epsilon = res->m_F - max(o1->m_F-this->op1->epsilon, o2->m_F-this->op2->epsilon);// + min(this->op1->epsilon,this->op2->epsilon);
        
            
        double tmp_err =  0.0;
            
        if(tmp_epsilon < 0) {
           tmp_err = this->calculateSumError(res,tmp_epsilon);
        }
        
        
        //double tmp_err =  this->calculateError(temp_res,res);
        this->error_array.push_back(tmp_err);
        this->epsilon_array.push_back(tmp_epsilon);
    }
    
    //впоследствии разберёмся и с этим, но позже
    k_max = 0;
    
    
    
    return res;
}

void GraphNode::printSIF() {
    //необходимо обработать нулл-значения операндов
    int s1 = this->op1 == NULL ? 0 : this->op1->value->m_S;
    int i1 = this->op1 == NULL ? 0 : this->op1->value->m_I;
    int f1 = this->op1 == NULL ? 0 : this->op1->value->m_F;
    int s2 = this->op2 == NULL ? 0 : this->op2->value->m_S;
    int i2 = this->op2 == NULL ? 0 : this->op2->value->m_I;
    int f2 = this->op2 == NULL ? 0 : this->op2->value->m_F;
    
    int preshift_op1 = (this->op1 == NULL) || (this->operation == 1) ? 0 : this->op1->shift_pos;
    int preshift_op2 = (this->op2 == NULL) || (this->operation == 1) ? 0 : this->op2->shift_pos;
    
    printf("Value: (%d/%d/%d) shift %d ; Op1: (%d/%d/%d) preshift %d ; Op2: (%d/%d/%d) preshift %d ;\n",this->value->m_S,this->value->m_I, this->value->m_F, this->shift_mult, s1,i1,f1, preshift_op1,s2,i2,f2,preshift_op2);
}

Sif *GraphNode::placeSIFResult(Sif *r) {
    
    Sif *res = new Sif(0,r->m_S,r->m_I,r->m_F,0);
    
    //Теперь помещаем это всё в машинное слово
    int overhead = WL - res->bits(); //разница между длиной машинного слова и длиной результата, в битах
    
    if (overhead >= 0) {
        res->m_S = res->m_S + abs(overhead);//результат укладывается в длину машинного слова
    } else {
        //неукладываемся в машинное слово, приходится усекать!
        //res->m_S-2-abs(overhead) - число знаковых бит которые надо удалить
        
        if ((res->m_S-2-abs(overhead)) >=0) {
            
            res->m_S = res->m_S - abs(overhead); //нормально, удаляем только знаковые биты
        
        } else {
            //подразумевается, что работаем ТОЛЬКО с валидными операндами,
            //у которых I_1 + I_2 <= WL-1
            
            if(res->m_I <=WL-2) {
                //оставляем 2 знаковых бита 
                res->m_S = 2;
                
            } else {
                //оставляем только один знаковый бит
                res->m_S = 1;
                
            }
            
            res->m_F = abs(WL - res->m_S - res->m_I);
        }
    }
    
    double tmp_err =  this->calculateError(r,res);
    this->err = tmp_err;
    this->error_array.push_back(tmp_err);
    
    return res;
    
}


double GraphNode::calculateError(Sif *op1, Sif *op2) {
    
    double err;
    for(int i=op1->m_F; i>=op2->m_F; i--) {
        err += pow(2,(-1)*i);
    }
    
    return err;
}

double GraphNode::calculateSumError(Sif *op, int epsilon) {
    
    double err;
    for(int i=op->m_F+abs(epsilon); i>=op->m_F; i--) {
        err += pow(2,(-1)*i);
    }
    
    return err;
}

GraphNode *GraphNode::Clone() {
    
    GraphNode *new_graph_node = new GraphNode();
    new_graph_node->tag = this->tag;
    new_graph_node->level = this->level;
    new_graph_node->id = this->id;
    new_graph_node->name = this->name;
    new_graph_node->ready = this->ready;
    new_graph_node->operation = this->operation;
    //new_graph_node->op1=op1 != NULL ? this->op1->Clone() : NULL;
    //new_graph_node->op2=op2 != NULL ? this->op2->Clone() : NULL;
    new_graph_node->op1_id = this->op1_id;// != NULL ? this->op1->Clone() : NULL;
    new_graph_node->op2_id = this->op2_id;// != NULL ? this->op2->Clone() : NULL;
    new_graph_node->value = new Sif(0,this->value->m_S,this->value->m_I, this->value->m_F,0);
    new_graph_node->WL = this->WL;
    new_graph_node->err = this->err;
    new_graph_node->epsilon = this->epsilon;
    
    //теперь разбираемся с узлами смежности
    //ищем айди
    for(int ii=0; ii<this->linkedNodes.size(); ii++) {
       //получаем очередной ай-ди линкованного узла
       new_graph_node->linkedNodes.push_back(this->linkedNodes[ii]);
    }
    new_graph_node->shift_pos = this->shift_pos;        
    new_graph_node->shift_mult = this->shift_mult;
    new_graph_node->init_err = this->init_err;
    
    return new_graph_node;
}


void GraphNode::print() {
    printf("Node: %d Name: %s Level: %d Ready: %d, Error: %16.16f ",this->id,this->name,this->level, this->ready,this->err);
    this->printSIF();
}