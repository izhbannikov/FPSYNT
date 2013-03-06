/* 
 * File:   Converter.h
 * Author: ilya
 *
 * Created on 22 Январь 2012 г., 18:22
 */

#ifndef CONVERTER_H
#define	CONVERTER_H

#include "Sif.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <deque>
#include <math.h>
#include <string.h>

class Converter {
public:
    Converter();
    Converter(const Converter& orig);
    virtual ~Converter();
    
    Sif *CastToSIF(double value);
    void write_data_to_file();
    char *CastToBinary(double value);
    char* itoa(int value, char* result, int base);
    deque<int> MakeTwoComplement(deque<int> op);
    long CastToInt(double value);
    long CastToUInt16(double value);
private:
    deque<int> invert(deque<int> op);
    deque<int> add_one(deque<int> op);
    
};

#endif	/* CONVERTER_H */

