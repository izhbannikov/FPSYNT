/* 
 * File:   Converter.cpp
 * Author: ilya
 * 
 * Created on 22 Январь 2012 г., 18:22
 */

#include "Converter.h"

Converter::Converter() {
}

Converter::Converter(const Converter& orig) {
}

Converter::~Converter() {
}

Sif* Converter::CastToSIF(double value) {
    
    //перевод целой части числа в двоичный формат, 8bits
    int q, r;
    double i_part;
    double f_part;
    f_part = modf(value,&i_part);
    
    vector<int>int_part;
    vector<int>full_word;
    if(i_part != 0) {
        q = ((int)i_part) / 2;
        r = ((int)i_part) % 2;
    
        int_part.push_back(r);
        full_word.push_back(r);
        while(!(q==0 && r==1)) {
                r = q % 2;
                q = q / 2;
        
                int_part.push_back(r);
                full_word.push_back(r);
        }
    }
    Sif *res = new Sif();
    res->m_S = 1;
    res->m_I = int_part.size();
    res->m_F = 8 - 1 - res->m_I;
    
    //перевод дробной части числа в двоичный формат
    vector<int>frac_part;
    double tmp_res = f_part*2;
    int f=0;
    if(f_part != 0) {
    while((f < res->m_F)) {
        f_part = modf(tmp_res,&i_part);
        frac_part.push_back(i_part);
        full_word.push_back(i_part);
        tmp_res = f_part*2;
        f++;
        
        if(f_part == 0)
            break;
    }
    }
    if(frac_part.size() < res->m_F) {
        res->m_F = frac_part.size();
        res->m_S = 8 - res->m_F - res->m_I;
    }
    
    //Проверка вычисленного значения и вычисление ошибки
    //Сначала дробную часть
    tmp_res = 0;
    //integer value:
    
    int integer_value = 0;
    for(int i = 0; i<frac_part.size(); i++) {
        tmp_res += frac_part[i]*pow(2,(-1)*(i+1));
        
        
    }
    printf("\n");
    //теперь целую:
    for(int i = 0; i<int_part.size(); i++) {
        tmp_res += int_part[i]*pow(2,i);
        
    }
    printf("Binary value: ");
    for(int i = 0; i < full_word.size(); i++) {
        //if(i <= res->m_I) {
            integer_value += full_word[full_word.size()-1-i]*pow(2,i);
        //} else {
        //    integer_value += full_word[i]*pow(2,(-1)*(i-res->m_I+1));
        //}
        printf("%d",full_word[i]);
    }
    printf("\n");
    double conv_err = 0.0;
    //conv_err = value - tmp_res;
    conv_err = value - integer_value*pow(2,(-1)*res->m_F);
    printf("Convertation error: %16.16f\n", conv_err);
    printf("Result SIf: ");
    res->printSIF();
    printf(" Integer value: %d, Scaling factor: 2^-%d", integer_value, res->m_F);
    return res;
    
}

void Converter::write_data_to_file() {
    //генерация функции синус
    /*
    FILE *f_a_re, *f_a_im, *f_b_re, *f_b_im, *f_c_re_f, *f_c_im_f;
    
    
    double a_re, a_im, c_re, c_im;
    double b_re, b_im;
    //int tmp_are[];
    //a
    f_a_re = fopen("a_re.txt","wt");
    f_a_im = fopen("a_im.txt","wt");
    f_b_re = fopen("b_re.txt","wt");
    f_b_im = fopen("b_im.txt","wt");
    f_c_re_f = fopen("c_re_float.txt","wt");
    f_c_im_f = fopen("c_im_float.txt","wt");
    
    for(int i = 0; i < 90; i++) {
        a_re = cos(i*3.1415926/180);
        //fprintf(f_a, "%s", CastToBinary(a_re)); 
        fprintf(f_a_re, "%d\n", CastToInt(a_re)); 
        a_im = sin(i*3.1415926/180);
        //fprintf(f_a, "%s\n", CastToBinary(a_im));
        fprintf(f_a_im, "%d\n", CastToInt(a_im));
    
        b_re = cos((i+90)*3.1415926/180);
        //fprintf(f_b, "%s", CastToBinary(b_re)); 
        fprintf(f_b_re, "%d\n", CastToInt(b_re)); 
        b_im = sin((i+90)*3.1415926/180);
        //fprintf(f_b, "%s\n", CastToBinary(b_im));
        fprintf(f_b_im, "%d\n", CastToInt(b_im)); 
        
        c_re = a_re*b_re - a_im*b_im ;
        c_im = a_im*b_re + a_re*b_im ;
        fprintf(f_c_re_f, "%f\n",c_re) ;
        fprintf(f_c_im_f, "%f\n",c_im) ;
    }
    fclose(f_a_re);
    fclose(f_a_im);
    
    fclose(f_b_re);
    fclose(f_b_im);
    
    fclose(f_c_re_f);
    fclose(f_c_im_f);
    */
    
    FILE *f_fir_in;
    double fir_in;
    f_fir_in = fopen("fir_in.txt","wt");
    for(int i = 0; i < 90; i++) {
        fir_in = sin(i*3.1415926/180);
        fprintf(f_fir_in, "%d\n", CastToInt(fir_in)); 
    }
    fclose(f_fir_in);
}

long Converter::CastToInt(double value) {
    //перевод целой части числа в двоичный формат
    int q, r;
    double i_part;
    double f_part;
    
    f_part = modf(fabs(value),&i_part);
    
    deque<int>int_part;
    deque<int>full_word;
    if(i_part != 0) {
        q = ((int)i_part) / 2;
        r = ((int)i_part) % 2;
    
        int_part.push_back(r);
        full_word.push_back(r);
        while(!(q==0 && r==1)) {
                r = q % 2;
                q = q / 2;
        
                int_part.push_back(r);
                full_word.push_back(r);
        }
    }
    
    //перевод дробной части числа в двоичный формат
    double tmp_res = f_part*2;
    int f=0;
    if(f_part != 0) {
        while(f < (16 - 1 - int_part.size())) {
                f_part = modf(tmp_res,&i_part);
                full_word.push_back(i_part);
                tmp_res = f_part*2;
                f++;
        
                if(f_part == 0)
                        break;
        }
    }
    
    while(full_word.size() < 16) {
        full_word.push_front(0);
    }
    
    long integer_value=0;
    
    for(int i = 0; i < full_word.size(); i++) {
        integer_value += full_word[full_word.size()-1-i]*pow(2,i);
        
    }
    if(value < 0) integer_value = -1*integer_value;
    
    return integer_value;
}


char* Converter::CastToBinary(double value) {
    
    //перевод целой части числа в двоичный формат
    int q, r;
    double i_part;
    double f_part;
    
    f_part = modf(fabs(value),&i_part);
    
    deque<int>int_part;
    deque<int>full_word;
    if(i_part != 0) {
        q = ((int)i_part) / 2;
        r = ((int)i_part) % 2;
    
        int_part.push_back(r);
        full_word.push_back(r);
        while(!(q==0 && r==1)) {
                r = q % 2;
                q = q / 2;
        
                int_part.push_back(r);
                full_word.push_back(r);
        }
    }
    
    //перевод дробной части числа в двоичный формат
    double tmp_res = f_part*2;
    int f=0;
    if(f_part != 0) {
        while(f < (16 - 1 - int_part.size())) {
                f_part = modf(tmp_res,&i_part);
                full_word.push_back(i_part);
                tmp_res = f_part*2;
                f++;
        
                if(f_part == 0)
                        break;
        }
    }
    
    while(full_word.size() < 16) {
        full_word.push_front(0);
    }
    
    if(value < 0) {
        //printf("kkk");
        //for(int i=0; i<full_word.size(); i++) {
        //        printf("%d",full_word[i]);
        //}
        full_word = this->MakeTwoComplement(full_word);
        //printf("\t");
        //for(int i=0; i<full_word.size(); i++) {
        //        printf("%d",full_word[i]);
        //}
        //printf("\n");
    }
    
    char t_res[2];
    char t_res1[2];
    t_res1[0]='0';
    t_res1[1]='\0';
    for(int i = 0; i < full_word.size(); i++) {
        itoa(full_word[i],t_res, 10);
        strncat(t_res1,t_res,1);
    }
    
    char res[full_word.size()+1];
    res[full_word.size()] = '\0';
    for(int i=1; i<=full_word.size(); i++) {
        res[i-1] = t_res1[i];
    }
    
    return res;
    
}

/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
char* Converter::itoa(int value, char* result, int base) {
        // check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }
	
        char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;
	
	do {
                tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );
	
	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	
        while(ptr1 < ptr) {
                tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	
        return result;
}

deque<int> Converter::invert(deque<int> op) {
    for(int i = 0; i < op.size(); i++) {
        if((int)op[i] == 1) {
                op[i] = 0;
        } else {
            op[i] = 1;
        }
    }
    
    return op;
}
//здесь просто добавляем единицу:
deque<int> Converter::add_one(deque<int> op) {
    for(int i=op.size()-1; i >= 0; i--) {
        if((int)op[i] == 0) {
            op[i] = 1;
            break;
        } else {
            op[i] = 0;
            if(i==0) {
                op.push_front(1);
                break;
            }
        }
    }
    
    return op;
}

deque<int> Converter::MakeTwoComplement(deque<int> op) {
    op = this->invert(op);
    op = this->add_one(op);
    
    return op;
}


long Converter::CastToUInt16(double value) {
    //перевод целой части числа в двоичный формат
    int q, r;
    double i_part;
    double f_part;
    short f_part_size=0;
    f_part = modf(fabs(value),&i_part);
    
    deque<int>int_part;
    deque<int>full_word;
    if(i_part != 0) {
        q = ((int)i_part) / 2;
   
        r = ((int)i_part) % 2;
    
        int_part.push_back(r);
        full_word.push_back(r);
        while(!(q==0 && r==1)) {
                r = q % 2;
                q = q / 2;
        
                int_part.push_back(r);
                full_word.push_back(r);
        }
    }
    
    long integer_value=0;
   //перевод дробной части числа в двоичный формат
    double tmp_res = f_part*2;
    int f=0;
    if(f_part != 0) {
        while(f < (15 - int_part.size())) {
                f_part = modf(tmp_res,&i_part);
                full_word.push_front(i_part);
                tmp_res = f_part*2;
                f++;
        
                if(f_part == 0)
                        break;
        }
    }
    
    while(full_word.size() < 15) {
        full_word.push_front(0);
    }
    
    
    
    for(int i = 0; i < full_word.size(); i++) {
        integer_value += full_word[i]*pow(2,i);
        printf("%d",full_word[i]);
    }
    if(value < 0) integer_value = -1*integer_value;
    int sf=15-int_part.size();
    printf("\n Integer value: %d, Scaling factor: 2^-%d, Double val: %f\n", integer_value,sf,integer_value*pow(2,(-1)*sf));
    
    
    return integer_value;
}
