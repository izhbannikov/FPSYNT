/* 
 * File:   sif.h
 * Author: ilya
 *
 * Created on 25 Октябрь 2011 г., 11:29
 */

#ifndef SIF_H
#define	SIF_H
#include <vector>

using namespace std;

class Sif {
public:
    Sif();
    Sif(const Sif& orig);
    virtual ~Sif();
    
    Sif( int ss, int s, int i, int f, int n );
    

    char  m_Ssign;  // +/- in front of S: 1 = plus, -1 = minus, 0 = none
    unsigned short m_S;
    unsigned short m_I;
    unsigned short m_F;
    short m_n;
                
    bool operator == ( const Sif& rs ) const;
    bool operator != ( const Sif& rs ) const;

    const Sif& set( int ss, int s, int i, int f, int n=0 );
  
    int L() { 
        return( m_S + m_I /*+ m_n*/ ); 
    }
                
    int M() { 
        return( m_I /*+ m_n*/ ); 
    }
                
    int R() { 
        return( /*m_n*/ - m_F ); 
    }

    double spv();  // наименьшее положительное значение

    double max();
    double min();

    //возвращает позицию десятчной запятой
    int binaryPoint() const {
        return( m_S + m_I /*+ m_n*/ );
    }

    //Возвращает число используемых бит (фактически длина слова)
    int bits() const {
        return( m_S + m_I + m_F );
    }


    int mantissabits() const { return( m_I + m_F ); }

    void   normalize();  // Make n as close to 0 as possible

    bool isSigned() { 
        return( (m_Ssign != 1) && (m_S != 0) );
    }
  
    bool isUnsigned() {
        return( (m_Ssign == 1) || (m_S == 0) );
    }
    
    bool isZero;
    
    
    Sif *Copy();
    
    void printSIF();
    
private:

};

#endif	/* SIF_H */

