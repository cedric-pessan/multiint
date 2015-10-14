
#include <gtest/gtest.h>
#include <gmpxx.h>

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
   
   s = "0x12e243F58439537239542349584A739420437353743b49544738992998187456c783424737538394220";
   LargeInteger<1024> i6( s );
   ASSERT_EQ( (string)i6, mpz_class( s ).get_str() );
   
   s = "0X12e243F58439537239542349584A739420437353743b49544738992998187456c783424737538394220";
   LargeInteger<1024> i7( s );
   ASSERT_EQ( (string)i7, mpz_class( s ).get_str() );
   
   s = "0x12e243F58439537239542349584A739420437353743b4954473u992998187456c783424737538394220";
   ASSERT_THROW( i = LargeInteger<1024>( s ), number_format_error );
   
   s = "0122435743753723754234757473742043735374347544737772777177456773424737537374220";
   LargeInteger<1024> i8( s );
   ASSERT_EQ( (string)i8, mpz_class( s ).get_str() );
   
   s = "0122435843953723954234958473942043735374349544738992998187456783424737538394220";
   ASSERT_THROW( i = LargeInteger<1024>( s ), number_format_error );
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

TEST(LargeIntegerTest, Addition)
{
   string s1 = "2324562324354654768987455344234356324354656757858568764654657657587686786786";
   LargeInteger<1024> i1 = s1;
   mpz_class i1gmp( s1 );
   ASSERT_EQ( (string)( i1 + 4354657576 ), mpz_class( i1gmp + mpz_class( 4354657576 ) ).get_str() );
   
   ASSERT_EQ( (string)( i1 + -4354657576 ), mpz_class( i1gmp + mpz_class( -4354657576 ) ).get_str() );
   
   string s2 = "122435843953723954234958473942043735374349544738992998187456783424737538394220";
   LargeInteger<1024> i2 = s2;
   mpz_class i2gmp( s2 );
   ASSERT_EQ( (string)( i1 + i2 ), mpz_class( i1gmp + i2gmp ).get_str() );
   
   string s3 = "-122435843953723954234958473942043735374349544738992998187456783424737538394220";
   LargeInteger<1024> i3 = s3;
   mpz_class i3gmp( s3 );
   ASSERT_EQ( (string)( i1 + i3 ), mpz_class( i1gmp + i3gmp ).get_str() );
}
