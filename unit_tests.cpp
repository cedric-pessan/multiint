
#include <gtest/gtest.h>

#include "multiint.hpp"

using std::string;

TEST(LargeIntegerTest, Instanciation)
{
   LargeInteger<1024> i;
   ASSERT_EQ( (string)i, "0" );
   
   LargeInteger<1024> i2( 125466776 );
   ASSERT_EQ( (string)i2, "125466776" );
   
   LargeInteger<1024> i3( -125466776 );
   ASSERT_EQ( (string)i3, "-125466776" );
   
   string s = "122435843953723954234958473942043735374349544738992998187456783424737538394220";
   LargeInteger<1024> i4( s );
   ASSERT_EQ( (string)i4, s );
   
   s = "-122435843953723954234958473942043735374349544738992998187456783424737538394220";
   LargeInteger<1024> i5( s );
   ASSERT_EQ( (string)i5, s );
   
   s = "-1232-4T5";
   ASSERT_THROW( i = LargeInteger<1024>( s ), number_format_error );
   
   try
     {
        i = LargeInteger<1024>( s );
     }
   catch( number_format_error e )
     {
        ASSERT_NE( e.what(), "" );
     }
}

TEST(LargeIntegerText, Assign)
{
   LargeInteger<1024> i;
   i = -125466776;
   ASSERT_EQ( (string)i, "-125466776" );
   
   string s = "122435843953723954234958473942043735374349544738992998187456783424737538394220";
   i = s;
   ASSERT_EQ( (string)i, s );
}
