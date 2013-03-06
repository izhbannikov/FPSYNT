/* 
 * File:   sif.cpp
 * Author: ilya
 * 
 * Created on 25 Октябрь 2011 г., 11:29
 */

#include "Sif.h"
#include <limits.h>  
#include <stdlib.h>  
#include <cmath>
#include <stdio.h>

Sif::Sif() {
}

Sif::Sif(const Sif& orig) {
}

Sif::~Sif() {
}

Sif::Sif( int ss, int s, int i, int f, int n ) {
    isZero = false;
  if( s < 0 )  {
      m_S = 0;
  } else if( s > USHRT_MAX ) {
      m_S = USHRT_MAX;
  } else {
      this->m_S = s;
  }

  if( i < 0 ) {
      m_I = 0;
  } else if( i > USHRT_MAX ) {
      m_I = USHRT_MAX;
  } else {
      this->m_I = i;
  }

  if( f < 0 ) {
      m_F = 0;
  } else if( f > USHRT_MAX ) {
      m_F = USHRT_MAX;
  } else {
      this->m_F = f;
  }

  if( n < SHRT_MIN ) m_n = SHRT_MIN;
  if( n > SHRT_MAX ) m_n = SHRT_MAX;
  
  
  
  
}


bool Sif::operator==( const Sif& rs ) const
{
  return( m_Ssign == rs.m_Ssign &&
	  m_S == rs.m_S &&
	  m_I == rs.m_I &&
	  m_F == rs.m_F &&
	  m_n == rs.m_n );
}

bool Sif::operator!=( const Sif& rs ) const
{
  return( m_Ssign != rs.m_Ssign ||
	  m_S != rs.m_S ||
	  m_I != rs.m_I ||
	  m_F != rs.m_F ||
	  m_n != rs.m_n );
}

//устанавливает требуемые значения знаковых битов, целых битов, фракциональных битов
const Sif& Sif::set( int ss, int s, int i, int f, int n )
{
  m_Ssign = ss;
  m_S = s < 0 ? 0 : s;
  m_I = i < 0 ? 0 : i;
  m_F = f < 0 ? 0 : f;
  m_n = n;

  return( *this );
}

// Return the smallest positive value which can be represented...
double Sif::spv() {
  if( bits() <= 0 || (m_I + m_F) <= 0 )
    return( 0.0 );
  
  double spvalue = 1.0; //фактически это разрешене или resolution
  for( int i=0; i < m_F; i++ )
  {
    spvalue /= 2.0;
  }

  // Apply the n shift...
  if( m_n != 0 )
  {
    if( m_n > 0 )
      spvalue *= (double) (1 << m_n);
    else
    {
      for( int i=m_n; i < 0; i++ )
	spvalue /= 2.0;
    }
  }

  if( m_Ssign == -1 )
    return( -spvalue );

  return( spvalue );
}

//Максимум, корый может представлять данное SIF
double Sif::max()
{
  if( m_Ssign < 0 )
  {
    return(-ldexp( 1.0, R() ) );
  }
  else
  {
    long long mantmask = ( ( 1LL << mantissabits() ) - 1LL );

    double maxval = ldexp( static_cast<double>( mantmask ), R() );


    return( maxval );
  }
}

double Sif::min()
{
  if( m_Ssign <= 0 )
  {
    return( - ldexp( static_cast<double>((1LL << mantissabits())), R() ) );
  }
  else
  {
    return( 0.0 );
  }
}


Sif *Sif::Copy() {
    Sif *copy = new Sif(0,this->m_S, this->m_I, this->m_F,0);
    copy->isZero = this->isZero;
    copy->m_Ssign = this->m_Ssign;
    copy->m_n = this->m_n;
    
    return copy;
}

void Sif::printSIF() {
    
    printf(" (%d/%d/%d) \n",this->m_S,this->m_I,this->m_F);
}