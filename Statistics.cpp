/* 
 * File:   Statistics.cpp
 * Author: ilya
 * 
 * Created on 24 Январь 2012 г., 9:18
 */

#include "Statistics.h"

Statistics::Statistics() {
}

Statistics::Statistics(const Statistics& orig) {
}

Statistics::~Statistics() {
}

void Statistics::read_file() {
  string line;
  ifstream myfile ("c_re_float.txt");
  float c_re_float[90];
  
  if (myfile.is_open())
  {
      char item[20];
      //float cost;
    
      for(int i=0; i<89; i++) {
        getline (myfile,line);
      //  line >> c_re_float[i];
        //c_re_float[i] = strtod(line,NULL);
    }
    myfile.close();
  }
}

void Statistics::pattern(int op_length, int k, int n) {
    deque<long> op1_bin, op2_bin;
    double op1_int, op2_int;
    
    for(int i=0; i<op_length; i++) {
        op1_bin.push_back(1);
        op2_bin.push_back(1);
        op1_int += op1_bin[i]*pow(2,i);
        op2_int += op2_bin[i]*pow(2,i);
    }
    
    int size = op1_bin.size();
    printf("Direct calculation (looking at groving result length):\n");
    for(int i=1; i<=n; i++) {
        
        op1_int += op2_int;
        op1_bin = CastToBinary(op1_int);
        if(size < op1_bin.size()) {
            printf("Overflow occured in position: %d\n", i);
            size = op1_bin.size();
        }
        
    }
    
    printf("Calculation from equation:\n");
    for(int i=1; i<=14; i++) {
        printf("Overflow occured: %d\n",(int)floor(pow(2,op_length-1+i)/op2_int));
        
    }
    
    
}

deque<long> Statistics::CastToBinary(int value) {
    //перевод целой части числа в двоичный формат
    int q, r;
    
    deque<long>int_part;
    if(value != 0) {
        q = (value) / 2;
        r = (value) % 2;
    
        int_part.push_back(r);
        while(!(q==0 && r==1)) {
                r = q % 2;
                q = q / 2;
        
                int_part.push_back(r);
                
        }
    }
    
    //for(int i=int_part.size()-1; i>=0; i--) {
    //    printf("%d", int_part[i]);
    //}
    //printf("\n");
    
    return int_part;
    
}


