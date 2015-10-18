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

#ifdef __SIZEOF_INT128__
#define USE_NATIVE_INT128
#endif

#ifdef USE_NATIVE_INT128
typedef unsigned __int128 uint128_t;
#else
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
             res.num[ k ] = tmp & 0xFFFFFFFFFFFFFFFFLL;
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
        
        res.r = new LargeInteger( (uint64_t)r );
        if( neg ) res.negate();
        return res;
     }
   
   LargeInteger operator/( uint64_t i ) const
     {
        LargeInteger res( 0 );
        uint128_t r( 0 );
        
        for( int k = 0; k < L; ++k )
          {
             r = (uint128_t)r << 64 | (uint128_t)num[k];
             res.num[k] = r / i;
             r %= i;
          }
        
        res.r = new LargeInteger( (uint64_t)r );
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
   
   LargeInteger& operator-=( const LargeInteger& b )
     {
        *this = *this - b;
        return *this;
     }
   
   LargeInteger& operator/=( int64_t i )
     {
        *this = *this / i;
        return *this;
     }
   
   LargeInteger& operator|=( const LargeInteger& b )
     {
        *this = *this | b;
        return *this;
     }
   
   LargeInteger& operator<<=( int l )
     {
        *this = *this << l;
        return *this;
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
   
   bool operator<( const LargeInteger& b ) const
     {
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

#endif // MULTIINT_HPP