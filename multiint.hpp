/*
The MIT License (MIT)

Copyright (c) 2015 Cédric Pessan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef MULTIINT_HPP
#define MULTIINT_HPP

#include <string>
#include <stdint.h>
#include <algorithm>

typedef unsigned __int128 uint128_t;

template< int R > class IntegerWidthShouldBeMultipleOf64;

template<> class IntegerWidthShouldBeMultipleOf64< 0 >
{
};

template< int W > class LargeInteger : private IntegerWidthShouldBeMultipleOf64< W & 0x3F >
{
 private:
#ifdef __SIZEOF_INT128__
   static const int L = W / 64;
#else
   static const int L = W / 32;
#endif
   
 public:
   LargeInteger( int64_t i )
     {
        for( int k = 0; k < L-1; ++k ) num[ k ] = 0;
        if( i < 0 )
          {
             num[ L-1 ] = -i;
             negate();
          }
        else
          num[ L-1 ] = i;
     }
   
   LargeInteger()
     {
        for( int k = 0; k < L; ++k ) num[ k ] = 0;
     }
   
   LargeInteger operator*( int64_t i ) const
     {
        bool leftneg = isNegative();
        bool rightneg = ( i & ( 1LL << 63 ) ) != 0;
        bool neg = false;
        if( leftneg && !rightneg || !leftneg && rightneg ) neg = true;
        if( leftneg ) const_cast< LargeInteger* >(this)->negate();
        if( rightneg ) i = -i;
        
        LargeInteger res( 0 );
        uint64_t carry = 0;
        
        for( int k = L-1; k >= 0; --k )
          {
             uint128_t tmp = (uint128_t)i * (uint128_t)num[k] + (uint128_t)carry;
             carry = tmp >> 64;
             res.num[ k ] = tmp & 0xFFFFFFFFFFFFFFFFLL;
          }
        
        if( neg ) res.negate();
        if( leftneg ) const_cast< LargeInteger* >( this )->negate();
        return res;
     }
   
   LargeInteger operator*( uint64_t i ) const
     {
        LargeInteger res( 0 );
        uint64_t carry = 0;
        
        for( int k = L-1; k >= 0; --k )
          {
             uint128_t tmp = (uint128_t)i * (uint128_t)num[k] + (uint128_t)carry;
             carry = tmp >> 64;
             res.num[ k ] = tmp & 0xFFFFFFFFFFFFFFFFLL;
          }
        return res;
     }
   
   LargeInteger operator*( const LargeInteger& b ) const
     {
        bool leftneg = isNegative();
        bool rightneg = b.isNegative();
        bool neg = false;
        if( leftneg && !rightneg || !leftneg && rightneg ) neg = true;
        if( leftneg ) const_cast< LargeInteger* >(this)->negate();
        if( rightneg ) const_cast< LargeInteger* >( &b )->negate();
        
        LargeInteger res( 0 );
        
        for( int k = 0; k < L; ++k )
          {
             res += ( b * num[ k ] ) << ( L*8*8 - 64 * ( k+1 ) );
          }
        
        if( neg ) res.negate();
        if( leftneg ) const_cast< LargeInteger* >( this )->negate();
        if( rightneg ) const_cast< LargeInteger* >( &b )->negate();
        return res;
     }
   
   LargeInteger operator/( int64_t i ) const
     {
        bool leftneg = isNegative();
        bool rightneg = ( i & ( 1LL << 63 ) ) != 0;
        bool neg = false;
        if( leftneg && !rightneg || !leftneg && rightneg ) neg = true;
        if( leftneg ) const_cast< LargeInteger* >(this)->negate();
        if( rightneg ) i = -i;
        
        LargeInteger res( 0 );
        uint128_t r( 0 );
        
        for( int k = 0; k < L; ++k )
          {
             r = (uint128_t)r << 64 | (uint128_t)num[k];
             res.num[k] = r / i;
             r %= i;
          }
        
        res.r = r;
        if( neg ) res.negate();
        if( leftneg ) const_cast< LargeInteger* >( this )->negate();
        return res;
     }
   
   LargeInteger operator+( const LargeInteger& b ) const
     {
        LargeInteger res( 0 );
        uint64_t carry = 0;
        
        for( int k = L-1; k >= 0; --k )
          {
             uint128_t tmp = (uint128_t)num[ k ] + (uint128_t)b.num[ k ] + (uint128_t)carry;
             carry = tmp >> 64;
             res.num[ k ] = tmp & 0xFFFFFFFFFFFFFFFFLL;
          }
        
        return res;
     }
   
   LargeInteger operator<<( int l ) const
     {
        LargeInteger res = *this;
        while( l >= 64 )
          {
             for( int k=0; k<L-1; ++k )
               res.num[ k ] = res.num[ k+1 ];
             res.num[ L-1 ] = 0;
             l -= 64;
          }
        
        for( int k=0; k<L-1; ++k )
          {
             res.num[ k ] <<= l;
             res.num[ k ] |= ( ( res.num[ k+1 ] >> (64-l) ) & ( ( 1 << l ) - 1 ) );
          }
        res.num[ L-1 ] <<= l;
        return res;
     }
   
   LargeInteger& operator*=( const LargeInteger& b )
     {
        *this = *this * b;
        return *this;
     }
   
   LargeInteger& operator+=( const LargeInteger& b )
     {
        *this = *this + b;
        return *this;
     }
   
   LargeInteger& operator/=( int64_t i )
     {
        *this = *this / i;
        return *this;
     }
   
   operator std::string() const
     {
        if( isNegative() )
          {
             const_cast<LargeInteger*>(this)->negate();
             std::string s = "-";
             s += (std::string)(*this);
             const_cast<LargeInteger*>(this)->negate();
             return s;
          }
        
        if( !isPositive() ) return "0";
        
        std::string s;
        LargeInteger tmp = *this;
        while( tmp.isPositive() )
          {
             tmp /= 10;
             s += ( '0' + tmp.r );
          }
        std::reverse( s.begin(), s.end() );
        return s;
     }
   
   bool isNegative() const
     {
        return ( num[ 0 ] & ( 1LL << 63 ) ) != 0;
     }
   
   bool isPositive() const
     {
        for( int i = 0; i < L; ++i ) if( num[ i ] != 0 ) return true;
        return false;
     }
   
 private:
   uint64_t num[ L ];
   uint64_t r;
   
   void negate()
     {
        for( int k = 0; k < L; ++k ) num[ k ] = ~num[ k ];
        for( int k = L-1; k >=0; --k )
          {
             ++num[ k ];
             if( num[ k ] != 0 ) break;
          }
     }
};

#endif // MULTIINT_HPP