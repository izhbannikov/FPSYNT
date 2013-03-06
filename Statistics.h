/* 
 * File:   Statistics.h
 * Author: ilya
 *
 * Created on 24 Январь 2012 г., 9:18
 */

#ifndef STATISTICS_H
#define	STATISTICS_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <deque>
#include <math.h>
#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;

class Statistics {
public:
    Statistics();
    Statistics(const Statistics& orig);
    virtual ~Statistics();
    void read_file();
    
    void pattern(int op_length, int k, int n);
    
private:
    deque<int> add_one(deque<int> op);
    deque<long> CastToBinary(int value);
};

#endif	/* STATISTICS_H */

