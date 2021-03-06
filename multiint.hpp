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
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cassert>

#define CPP11VERSION 199711L

#ifdef __SIZEOF_INT128__
#define USE_NATIVE_INT128
#endif

#ifdef USE_NATIVE_INT128
typedef unsigned __int128 uint128_t;
#else
class Basic128
{
 public:
   Basic128( uint64_t i )
     {
        num[ 0 ] = 0;
        num[ 1 ] = 0;
        num[ 2 ] = i >> 32;
        num[ 3 ] = i & 0xFFFFFFFF;
     }
   
   Basic128 operator+( const Basic128& i ) const
     {
        Basic128 res( 0 );
        uint32_t carry = 0;
        
        for( int k = 3; k >= 0; --k )
          {
             uint64_t tmp = (uint64_t)num[ k ] + (uint64_t)i.num[ k ] + (uint64_t)carry;
             carry = tmp >> 32;
             res.num[ k ] = tmp & 0xFFFFFFFF;
          }
        
        return res;
     }
   
   Basic128 operator*( uint32_t i ) const
     {
        Basic128 res( 0 );
        uint32_t carry = 0;
        
        for( int k = 3; k >= 0; --k )
          {
             uint64_t tmp = i * (uint64_t)num[k] + (uint64_t)carry;
             carry = tmp >> 32;
             res.num[ k ] = tmp & 0xFFFFFFFF;
          }
        return res;
     }
   
   Basic128 operator*( const Basic128& i ) const
     {
        Basic128 res( 0 );
        
        for( int k = 0; k < 4; ++k )
          res += ( i * num[ k ] ) << ( 128 - 32 * ( k+1 ) );
        
        return res;
     }
   
   int64_t operator/( uint64_t i ) const
     {
        Basic128 res( 0 );
        uint64_t r = 0;
        
        for( int k = 0; k < 4; ++k )
          {
             r = r << 32 | (uint64_t)num[k];
             res.num[k] = r / i;
             r %= i;
          }
        
        return ( (uint64_t)res.num[ 2 ] << 32 ) | res.num[ 3 ];
     }
   
   uint64_t operator&( uint64_t i ) const
     {
        return ( ( ( uint64_t )( num[ 2 ] ) << 32 ) | num[ 3 ] ) & i;
     }
   
   Basic128 operator|( const Basic128& i ) const
     {
        Basic128 res(0);
        for( int k = 0; k < 4; ++k )
          res.num[ k ] = num[ k ] | i.num[ k ];
        return res;
     }
   
   Basic128 operator<<( int l ) const
     {
        Basic128 res = *this;
        while( l > 0 )
          {
             res.num[ 0 ] = res.num[ 1 ];
             res.num[ 1 ] = res.num[ 2 ];
             res.num[ 2 ] = res.num[ 3 ];
             res.num[ 3 ] = 0;
             l -= 32;
          }
        return res;
     }
   
   uint64_t operator>>( int r ) const
     {
        assert( r == 64 );
        
        return ( ( uint64_t )num[ 0 ] << 32 ) | num[ 1 ];
     }
   
   Basic128 operator+=( const Basic128& i )
     {
        *this = *this + i;
        return *this;
     }
   
   Basic128 operator%=( uint64_t i )
     {
        uint64_t r = 0;
        
        for( int k = 0; k < 4; ++k )
          {
             r = r << 32 | num[ k ];
             r %= i;
          }
        
        num[ 0 ] = 0;
        num[ 1 ] = 0;
        num[ 2 ] = r >> 32;
        num[ 3 ] = r & 0xFFFFFFFF;
        return *this;
     }
   
 private:
   uint32_t num[ 4 ];
};

typedef Basic128 uint128_t;
#endif

class number_format_error : public std::runtime_error
{
 public:
   number_format_error( const std::string& reason )
     : std::runtime_error( reason )
       {
       }
};

/* This will generate an error at compilation time if the user try to
 * define a LargeInteger with a length that is not a multiple of 64.
 * This restriction is required as our integer are compound of 64 bits
 * integers. Moreover, supporting all length would add a lot of complexity
 * with possible performance impact. It should not be a big deal as we are
 * supposed to handle large numbers so a few bits more of less should not make
 * any difference :-)
 */
template< int R > class IntegerWidthShouldBeMultipleOf64;

template<> class IntegerWidthShouldBeMultipleOf64< 0 >
{
};

template< int W, typename u128 = uint128_t > class LargeInteger : private IntegerWidthShouldBeMultipleOf64< W & 0x3F >
{
 private:
   static const int L = W / 64;
   
 public:
   LargeInteger( int64_t i ) : r( NULL )
     {
        assign( i );
     }
   
   LargeInteger( uint64_t i ) : r( NULL )
     {
        assign( i );
     }
   
   LargeInteger( int32_t i ) : r( NULL )
     {
        assign( (int64_t)i );
     }
   
   LargeInteger( uint32_t i ) : r( NULL )
     {
        assign( (uint64_t)i );
     }
   
   LargeInteger( int16_t i ) : r( NULL )
     {
        assign( (int64_t)i );
     }
   
   LargeInteger( uint16_t i ) : r( NULL )
     {
        assign( (uint64_t)i );
     }
   
   LargeInteger( int8_t i ) : r( NULL )
     {
        assign( (int64_t)i );
     }
   
   LargeInteger( uint8_t i ) : r( NULL )
     {
        assign( (uint64_t)i );
     }
   
   LargeInteger() : r( NULL )
     {
        for( int k = 0; k < L; ++k ) num[ k ] = 0;
     }
   
   LargeInteger( const LargeInteger& b ) : r( NULL )
     {
        for( int k = 0; k < L; ++k ) num[ k ] = b.num[ k ];
        if( b.r ) r = new LargeInteger( *b.r );
     }
   
   ~LargeInteger()
     {
        if( r ) delete r;
     }
   
   LargeInteger( const std::string& s ) : r( NULL )
     {
        parse( s );
     }
   
   LargeInteger& operator=( int64_t i )
     {
        assign( i );
        return *this;
     }
   
   LargeInteger& operator=( uint64_t i )
     {
        assign( i );
        return *this;
     }
   
   LargeInteger& operator=( int32_t i )
     {
        assign( (int64_t)i );
        return *this;
     }
   
   LargeInteger& operator=( uint32_t i )
     {
        assign( (uint64_t)i );
        return *this;
     }
   
   LargeInteger& operator=( int16_t i )
     {
        assign( (int64_t)i );
        return *this;
     }
   
   LargeInteger& operator=( uint16_t i )
     {
        assign( (uint64_t)i );
        return *this;
     }
   
   LargeInteger& operator=( int8_t i )
     {
        assign( (int64_t)i );
        return *this;
     }
   
   LargeInteger& operator=( uint8_t i )
     {
        assign( (uint64_t)i );
        return *this;
     }
   
   LargeInteger& operator=( const LargeInteger& b )
     {
        if( r ) delete( r );
        r = NULL;
        for( int k = 0; k < L; ++k ) num[ k ] = b.num[ k ];
        if( b.r ) r = new LargeInteger( *b.r );
        return *this;
     }
   
   LargeInteger& operator=( const std::string& s )
     {
        parse( s );
        return *this;
     }
   
   LargeInteger operator+( const LargeInteger& b ) const
     {
        LargeInteger res( 0 );
        uint64_t carry = 0;
        
        for( int k = L-1; k >= 0; --k )
          {
             uint128_t tmp = (uint128_t)num[ k ] + (uint128_t)b.num[ k ] + (uint128_t)carry;
             carry = tmp >> 64;
             res.num[ k ] = tmp & 0xFFFFFFFFFFFFFFFFULL;
          }
        
        return res;
     }
   
   LargeInteger operator-( const LargeInteger& b ) const
     {
        return *this + -b;
     }
   
   const LargeInteger& operator+() const
     {
        return *this;
     }
   
   LargeInteger operator-() const
     {
        LargeInteger tmp( *this );
        tmp.negate();
        return tmp;
     }
   
   LargeInteger operator*( int64_t i ) const
     {
        bool leftneg = isNegative();
        bool rightneg = ( i & ( 1LL << 63 ) ) != 0;
        bool neg = false;
        if( leftneg && !rightneg || !leftneg && rightneg ) neg = true;
        const LargeInteger& left = leftneg ? -*this : *this;
        if( rightneg ) i = -i;
        
        LargeInteger res( 0 );
        uint64_t carry = 0;
        
        for( int k = L-1; k >= 0; --k )
          {
             uint128_t tmp = (uint128_t)i * (uint128_t)left.num[k] + (uint128_t)carry;
             carry = tmp >> 64;
             res.num[ k ] = tmp & 0xFFFFFFFFFFFFFFFFLL;
          }
        
        if( neg ) res.negate();
        return res;
     }
   
   LargeInteger operator*( uint64_t i ) const
     {
        bool neg = isNegative();
        const LargeInteger& left = neg ? -*this : *this;
        
        LargeInteger res( 0 );
        uint64_t carry = 0;
        
        for( int k = L-1; k >= 0; --k )
          {
             uint128_t tmp = (uint128_t)i * (uint128_t)left.num[k] + (uint128_t)carry;
             carry = tmp >> 64;
             res.num[ k ] = tmp & 0xFFFFFFFFFFFFFFFFLL;
          }
        if( neg ) res.negate();
        return res;
     }
   
   LargeInteger operator*( int32_t i ) const
     {
        return *this * (int64_t)i;
     }
   
   LargeInteger operator*( uint32_t i ) const
     {
        return *this * (uint64_t)i;
     }
   
   LargeInteger operator*( int16_t i ) const
     {
        return *this * (int64_t)i;
     }
   
   LargeInteger operator*( uint16_t i ) const
     {
        return *this * (uint64_t)i;
     }
   
   LargeInteger operator*( int8_t i ) const
     {
        return *this * (int64_t)i;
     }
   
   LargeInteger operator*( uint8_t i ) const
     {
        return *this * (uint64_t)i;
     }
   
   LargeInteger operator*( const LargeInteger& b ) const
     {
        bool leftneg = isNegative();
        bool rightneg = b.isNegative();
        bool neg = false;
        if( leftneg && !rightneg || !leftneg && rightneg ) neg = true;
        const LargeInteger& left = leftneg ? -*this : *this;
        const LargeInteger& right = rightneg ? -b : b;
        
        LargeInteger res( 0 );
        
        for( int k = 0; k < L; ++k )
          {
             res += ( right * left.num[ k ] ) << ( L*8*8 - 64 * ( k+1 ) );
          }
        
        if( neg ) res.negate();
        return res;
     }
   
   LargeInteger operator/( int64_t i ) const
     {
        bool leftneg = isNegative();
        bool rightneg = ( i & ( 1LL << 63 ) ) != 0;
        bool neg = false;
        if( leftneg && !rightneg || !leftneg && rightneg ) neg = true;
        const LargeInteger& left = leftneg ? -*this : *this;
        if( rightneg ) i = -i;
        
        LargeInteger res( 0 );
        uint128_t r( 0 );
        
        for( int k = 0; k < L; ++k )
          {
             r = (uint128_t)r << 64 | (uint128_t)left.num[k];
             res.num[k] = r / i;
             r %= i;
          }
        
        uint64_t r64 = r & 0xFFFFFFFFFFFFFFFFULL;
        res.r = new LargeInteger( r64 );
        if( neg ) res.negate();
        if(leftneg) res.r->negate();
        return res;
     }
   
   LargeInteger operator/( uint64_t i ) const
     {
        bool neg = isNegative();
        const LargeInteger& left = neg ? -*this : *this;
        
        LargeInteger res( 0 );
        uint128_t r( 0 );
        
        for( int k = 0; k < L; ++k )
          {
             r = (uint128_t)r << 64 | (uint128_t)left.num[k];
             res.num[k] = r / i;
             r %= i;
          }
        
        uint64_t r64 = r & 0xFFFFFFFFFFFFFFFFULL;
        res.r = new LargeInteger( r64 );
        if( neg )
          {
             res.negate();
             res.r->negate();
          }
        return res;
     }
   
   LargeInteger operator/( int32_t i ) const
     {
        return *this / (int64_t)i;
     }
   
   LargeInteger operator/( uint32_t i ) const
     {
        return *this / (uint64_t)i;
     }
   
   LargeInteger operator/( int16_t i ) const
     {
        return *this / (int64_t)i;
     }
   
   LargeInteger operator/( uint16_t i ) const
     {
        return *this / (uint64_t)i;
     }
   
   LargeInteger operator/( int8_t i ) const
     {
        return *this / (int64_t)i;
     }
   
   LargeInteger operator/( uint8_t i ) const
     {
        return *this / (uint64_t)i;
     }
   
   LargeInteger operator/( const LargeInteger& d ) const
     {
        bool leftneg = isNegative();
        bool rightneg = d.isNegative();
        bool neg = false;
        if( leftneg && !rightneg || !leftneg && rightneg ) neg = true;
        const LargeInteger& left = leftneg ? -*this : *this;
        const LargeInteger& right = rightneg ? -d : d;
        
        if( d == 0 ) return num[ 0 ] / d.num[ 0 ];
        
        LargeInteger res( 0 );
        LargeInteger r;
        
        for( int i = L*64; i >= 0; --i )
          {
             r <<= 1;
             r |= ( ( left & ( (LargeInteger)1 << i ) ) != 0 ) ? 1 : 0;
             if( r >= right )
               {
                  r -= right;
                  res |= ( (LargeInteger)1 << i );
               }
          }
        
        res.r = new LargeInteger( r );
        if( neg ) res.negate();
        if(leftneg) res.r->negate();
        return res;
     }
   
   uint64_t operator%( uint64_t d ) const
     {
        return ( *this / d ).r->num[ L-1 ];
     }
   
   int64_t operator%( int64_t d ) const
     {
        return *(int64_t*)(&( *this / d ).r->num[ L-1 ]);
     }
   
   uint32_t operator%( uint32_t d ) const
     {
        return ( *this / d ).r->num[ L-1 ];
     }
   
   int32_t operator%( int32_t d ) const
     {
        return *(int32_t*)(&( *this / d ).r->num[ L-1 ] );
     }
   
   uint16_t operator%( uint16_t d ) const
     {
        return ( *this / d ).r->num[ L-1 ];
     }
   
   int16_t operator%( int16_t d ) const
     {
        return *(int16_t*)(&( *this / d ).r->num[ L-1 ] );
     }
   
   uint8_t operator%( uint8_t d ) const
     {
        return ( *this / d ).r->num[ L-1 ];
     }
   
   int8_t operator%( int8_t d ) const
     {
        return *(int8_t*)(&( *this / d ).r->num[ L-1 ] );
     }
   
   LargeInteger operator%( const LargeInteger& d ) const
     {
        return *( *this / d ).r;
     }
   
   const LargeInteger& getRemaining() const
     {
        return *r;
     }
   
   LargeInteger& operator++()
     {
        *this += 1;
        return *this;
     }
   
   LargeInteger operator++( int )
     {
        LargeInteger tmp( *this );
        *this += 1;
        return tmp;
     }
   
   LargeInteger& operator--()
     {
        *this -= 1;
        return *this;
     }
   
   LargeInteger operator--( int )
     {
        LargeInteger tmp( *this );
        *this -= 1;
        return tmp;
     }
   
   bool operator==( const LargeInteger& b ) const
     {
        for( int k = 0; k < L; ++k ) 
          if( num[ k ] != b.num[ k ] ) 
            return false;
        return true;
     }
   
   bool operator!=( const LargeInteger& b ) const
     {
        return !( *this == b );
     }
   
   bool operator>( const LargeInteger& b ) const
     {
        return !( *this < b ) && ( *this != b );
     }
   
   bool operator<( const LargeInteger& b ) const
     {
        if( isNegative() != b.isNegative() ) return (int64_t)num[ 0 ] < (int64_t)b.num[ 0 ];
        
        for( int k = 0; k < L; ++k )
          {
             if( num[ k ] < b.num[ k ] )
               return true;
             else if( num[ k ] > b.num[ k ] )
               return false;
          }
        return false;
     }
   
   bool operator>=( const LargeInteger& b ) const
     {
        return !( *this < b );
     }
   
   bool operator<=( const LargeInteger& b ) const
     {
        return ( *this < b ) || ( *this == b );
     }
   
   LargeInteger operator~() const
     {
        LargeInteger res;
        for( int k = 0; k < L; ++k )
          res.num[ k ] = ~num[ k ];
        return res;
     }
   
   LargeInteger operator&( const LargeInteger& b ) const
     {
        LargeInteger res;
        for( int k = 0; k < L; ++k )
          res.num[ k ] = num[ k ] & b.num[ k ];
        return res;
     }
   
   LargeInteger operator|( const LargeInteger& b ) const
     {
        LargeInteger res;
        for( int k = 0; k < L; ++k )
          res.num[ k ] = num[ k ] | b.num[ k ];
        return res;
     }
   
   LargeInteger operator^( const LargeInteger& b ) const
     {
        LargeInteger res;
        for( int k = 0; k < L; ++k )
          res.num[ k ] = num[ k ] ^ b.num[ k ];
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
   
   LargeInteger operator>>( int r ) const
     {
        LargeInteger res = *this;
        while( r >= 64 )
          {
             for( int k=L-1; k>0; --k )
               res.num[ k ] = res.num[ k-1 ];
             res.num[ 0 ] = (int64_t)res.num[ 0 ] >> 63;
             r -= 64;
          }
        
        for( int k=L-1; k>0; --k )
          {
             res.num[ k ] >>= r;
             res.num[ k ] |= ( ( res.num[ k-1 ] & ( ( 1 << r ) - 1 ) ) << (64-r) );
          }
        res.num[ 0 ] = (int64_t)res.num[ 0 ] >> r;
        return res;
     }
   
   LargeInteger& operator+=( const LargeInteger& b )
     {
        *this = *this + b;
        return *this;
     }
   
   LargeInteger& operator-=( const LargeInteger& b )
     {
        *this = *this - b;
        return *this;
     }
   
   LargeInteger& operator*=( const LargeInteger& b )
     {
        *this = *this * b;
        return *this;
     }
   
   LargeInteger& operator*=( uint64_t i )
     {
        *this = *this * i;
        return *this;
     }
   
   LargeInteger& operator*=( int64_t i )
     {
        *this = *this * i;
        return *this;
     }
   
   LargeInteger& operator*=( uint32_t i )
     {
        *this = *this * i;
        return *this;
     }
   
   LargeInteger& operator*=( int32_t i )
     {
        *this = *this * i;
        return *this;
     }
   
   LargeInteger& operator*=( uint16_t i )
     {
        *this = *this * i;
        return *this;
     }
   
   LargeInteger& operator*=( int16_t i )
     {
        *this = *this * i;
        return *this;
     }
   
   LargeInteger& operator*=( uint8_t i )
     {
        *this = *this * i;
        return *this;
     }
   
   LargeInteger& operator*=( int8_t i )
     {
        *this = *this * i;
        return *this;
     }
   
   LargeInteger& operator/=( const LargeInteger& b )
     {
        *this = *this / b;
        return *this;
     }
   
   LargeInteger& operator/=( uint64_t i )
     {
        *this = *this / i;
        return *this;
     }
   
   LargeInteger& operator/=( int64_t i )
     {
        *this = *this / i;
        return *this;
     }
   
   LargeInteger& operator/=( uint32_t i )
     {
        *this = *this / i;
        return *this;
     }
   
   LargeInteger& operator/=( int32_t i )
     {
        *this = *this / i;
        return *this;
     }
   
   LargeInteger& operator/=( uint16_t i )
     {
        *this = *this / i;
        return *this;
     }
   
   LargeInteger& operator/=( int16_t i )
     {
        *this = *this / i;
        return *this;
     }
   
   LargeInteger& operator/=( uint8_t i )
     {
        *this = *this / i;
        return *this;
     }
   
   LargeInteger& operator/=( int8_t i )
     {
        *this = *this / i;
        return *this;
     }
   
   LargeInteger& operator%=( const LargeInteger& b )
     {
        *this = *this % b;
        return *this;
     }
   
   LargeInteger& operator%=( uint64_t i )
     {
        *this = *this % i;
        return *this;
     }
   
   LargeInteger& operator%=( int64_t i )
     {
        *this = *this % i;
        return *this;
     }
   
   LargeInteger& operator%=( uint32_t i )
     {
        *this = *this % i;
        return *this;
     }
   
   LargeInteger& operator%=( int32_t i )
     {
        *this = *this % i;
        return *this;
     }
   
   LargeInteger& operator%=( uint16_t i )
     {
        *this = *this % i;
        return *this;
     }
   
   LargeInteger& operator%=( int16_t i )
     {
        *this = *this % i;
        return *this;
     }
   
   LargeInteger& operator%=( uint8_t i )
     {
        *this = *this % i;
        return *this;
     }
   
   LargeInteger& operator%=( int8_t i )
     {
        *this = *this % i;
        return *this;
     }
   
   LargeInteger& operator&=( const LargeInteger& b )
     {
        *this = *this & b;
        return *this;
     }
   
   LargeInteger& operator|=( const LargeInteger& b )
     {
        *this = *this | b;
        return *this;
     }
   
   LargeInteger& operator^=( const LargeInteger& b )
     {
        *this = *this ^ b;
        return *this;
     }
   
   LargeInteger& operator<<=( int l )
     {
        *this = *this << l;
        return *this;
     }
   
   LargeInteger& operator>>=( int l )
     {
        *this = *this >> l;
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
             s += ( '0' + tmp.r->num[ L-1 ] );
          }
        std::reverse( s.begin(), s.end() );
        return s;
     }
   
   std::string toHexString() const
     {
        std::stringstream ss;
        ss << std::hex;
        
        for( int i = 0; i < L; ++i )
          {
             ss << num[ i ];
          }
        
        return ss.str().substr( ss.str().find_first_not_of( "0" ) );
     }
   
   std::string toOctString() const
     {
        std::stringstream ss;
        ss << std::oct;
        int tmp = 0;
        int count = (L*64)%3;
        count = ( ( count & 1 ) << 1 ) | ( ( count >> 1 ) & 1 );
        
        for( int i = 0; i < L; ++i )
          {
             for( int j = 0; j < 64; ++j )
               {
                  tmp <<= 1;
                  tmp |= ( num[ i ] >> ( 64 - j - 1 ) ) & 1;
                  if( ++count == 3 )
                    {
                       ss << tmp;
                       tmp = 0;
                       count = 0;
                    }
               }
          }
        
        return ss.str().substr( ss.str().find_first_not_of( "0" ) );
     }

#if __cplusplus > CPP11VERSION
   explicit operator uint64_t() const
     {
        return toUInt64();
     }
   
   explicit operator int64_t() const
     {
        return toInt64();
     }
   
   explicit operator uint32_t() const
     {
        return (uint32_t)toUInt64();
     }
   
   explicit operator int32_t() const
     {
        return (int32_t)toInt64();
     }
   
   explicit operator uint16_t() const
     {
        return (uint16_t)toUInt64();
     }
   
   explicit operator int16_t() const
     {
        return (int16_t)toInt64();
     }
   
   explicit operator uint8_t() const
     {
        return (uint8_t)toUInt64();
     }
   
   explicit operator int8_t() const
     {
        return (int8_t)toInt64();
     }
#endif
   
   uint64_t toUInt64() const
     {
        return num[ L-1 ];
     }
   
   int64_t toInt64() const
     {
        return (int64_t)num[ L-1 ];
     }
   
   uint32_t toUInt32() const
     {
        return (uint32_t)toUInt64();
     }
   
   int32_t toInt32() const
     {
        return (int32_t)toInt64();
     }
   
   uint16_t toUInt16() const
     {
        return (uint16_t)toUInt64();
     }
   
   int16_t toInt16() const
     {
        return (int16_t)toInt64();
     }
   
   uint8_t toUInt8() const
     {
        return (uint8_t)toUInt64();
     }
   
   int8_t toInt8() const
     {
        return (int8_t)toInt64();
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
   LargeInteger* r;
   
   void negate()
     {
        for( int k = 0; k < L; ++k ) num[ k ] = ~num[ k ];
        for( int k = L-1; k >=0; --k )
          {
             ++num[ k ];
             if( num[ k ] != 0 ) break;
          }
     }
   
   void assign( int64_t i )
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
   
   void assign( uint64_t i )
     {
        for( int k = 0; k < L-1; ++k ) num[ k ] = 0;
        num[ L-1 ] = i;
        if( r ) delete r;
        r = NULL;
     }
   
   void parse( const std::string& s )
     {
        if( s.length() == 0 ) *this = 0;
        if( s[ 0 ] == '0' )
          {
             if( s.length() > 1 && tolower( s[ 1 ] ) == 'x' ) parseHex( s );
             else parseOct( s );
             return;
          }
                
        LargeInteger tmp;
        int i = 0;
        if( s[ 0 ] == '-' ) ++i;
        
        for( ; i < s.length(); ++i )
          {
             if( s[ i ] < '0' || s[ i ] > '9' ) throw number_format_error( "Number could not be parsed" );
             tmp = tmp * 10 + ( s[ i ] - '0' );
          }
        
        if( s[ 0 ] == '-' ) tmp.negate();
        
        *this = tmp;
     }
   
   void parseHex( const std::string& s )
     {
        LargeInteger tmp;
        for( int i = 2; i < s.length(); ++i)
          {
             int val = 0;
             char c = tolower( s[ i ] );
             if( c >= '0' && c <= '9' ) val = c - '0';
             else if( c >= 'a' && c <= 'f' ) val = 10 + c - 'a';
             else throw number_format_error( "Number could not be parsed" );
             
             tmp = tmp * 16 + val;
          }
        
        *this = tmp;
     }
   
   void parseOct( const std::string& s )
     {
        LargeInteger tmp;
        for( int i = 1; i < s.length(); ++i )
          {
             if( s[ i ] < '0' || s[ i ] > '7' ) throw number_format_error( "Number could not be parsed" );
             tmp = tmp * 8 + ( s[ i ] - '0' );
          }
        
        *this = tmp;
     }
};

template< int W, typename u128, typename l > LargeInteger< W, u128 > operator+( l i, const LargeInteger< W, u128 >& j )
{
   return j + i;
}

template< int W, typename u128, typename l > LargeInteger< W, u128 > operator-( l i, const LargeInteger< W, u128 >& j )
{
   return -j + i;
}

template< int W, typename u128, typename l > LargeInteger< W, u128 > operator*( l i, const LargeInteger< W, u128 >& j )
{
   return j * i;
}

template< int W, typename u128, typename l > LargeInteger< W, u128 > operator/( l i, const LargeInteger< W, u128 >& j )
{
   return LargeInteger< W, u128 >( i ) / j;
}

template< int W, typename u128, typename l > l operator%( l i, const LargeInteger< W, u128 >& j )
{
   return (l)(LargeInteger< W, u128 >( i ) % j).toUInt64();
}

template< int W, typename u128 > std::ostream& operator<<( std::ostream& os, const LargeInteger< W, u128 >& i )
{
   std::string prefix;
   std::string number;
   
   if( ( os.flags() & os.basefield ) == os.dec )
     {
        if( ( os.flags() & os.showpos ) && !i.isNegative() ) prefix = "+";
        number = ( std::string )i;
        if( i.isNegative() )
          {
             prefix = "-";
             number = number.substr( 1 );
          }
     }
   else if( ( os.flags() & os.basefield ) == os.hex )
     {
        if( os.flags() & os.showbase ) prefix = "0x";
        number = i.toHexString();
        if( os.flags() & os.uppercase )
          {
             if( prefix.length() ) prefix = "0X";
             std::transform( number.begin(), number.end(), number.begin(), ::toupper );
          }
     }
   else if( ( os.flags() & os.basefield ) == os.oct )
     {
        if( os.flags() & os.showbase ) number = "0";
        number += i.toOctString();
     }
   
   if( os.width() > prefix.length() + number.length() )
     {
        std::string sfill( os.width() - prefix.length() - number.length(), os.fill() );
        
        if( ( os.flags() & os.adjustfield ) == os.internal )
          os << ( prefix + sfill + number );
        else if( ( os.flags() & os.adjustfield ) == os.left )
          os << ( prefix + number + sfill );
        else if( ( os.flags() & os.adjustfield ) == os.right )
          os << ( sfill + prefix + number );
     }
   else
     {
        os << ( prefix + number );
     }
   
   return os;
}

template< int W, typename u128 > std::istream& operator>>( std::istream& is, LargeInteger< W, u128 >& i )
{
   std::istream::sentry sen( is, false );
   if( sen )
     {
        std::string s;
        if( ( is.flags() & is.basefield ) == is.dec )
          {
             if( is.good() )
               {
                  if( is.peek() == '-' )
                    {
                       char c = is.get();
                       s += c;
                    }
                  else if( is.peek() == '+' ) is.get();
               }
             while( is.good() )
               {
                  char c = is.peek();
                  if( c == EOF ) break;
                  if( c >= '0' && c <= '9' ) s += c;
                  else break;
                  is.get();
               }
             i = s;
          }
        else if( ( is.flags() & is.basefield ) == is.hex )
          {
             s = "0x";
             if( is.good() && is.peek() == '0' )
               {
                  is.get();
                  if( is.good() && is.peek() == 'x' ) is.get();
                  else s += '0';
               }
             while( is.good() )
               {
                  char c = is.peek();
                  if( c == EOF ) break;
                  if( c >= '0' && c <= '9' ) s += c;
                  else if( tolower(c) >= 'a' && tolower(c) <= 'z' ) s += c;
                  else break;
                  is.get();
               }
             i = s;
          }
        else if( ( is.flags() & is.basefield ) == is.oct )
          {
             s = "0";
             if( is.good() && is.peek() == '0' )
               is.get();
             while( is.good() )
               {
                  char c = is.peek();
                  if( c == EOF ) break;
                  if( c >= '0' && c <= '7' ) s += c;
                  else break;
                  is.get();
               }
             i = s;
          }
     }
   
   return is;
}

#endif // MULTIINT_HPP
