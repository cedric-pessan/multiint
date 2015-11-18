
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

TEST(LargeIntegerTest, Assign)
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
   
   ASSERT_EQ( (string)( 4354657576 + i1 ), mpz_class( mpz_class( 4354657576 ) + i1gmp ).get_str() );
   
   ASSERT_EQ( (string)( -4354657576 + i1 ), mpz_class( mpz_class( -4354657576 ) + i1gmp ).get_str() );
   
   string s2 = "122435843953723954234958473942043735374349544738992998187456783424737538394220";
   LargeInteger<1024> i2 = s2;
   mpz_class i2gmp( s2 );
   ASSERT_EQ( (string)( i1 + i2 ), mpz_class( i1gmp + i2gmp ).get_str() );
   
   string s3 = "-122435843953723954234958473942043735374349544738992998187456783424737538394220";
   LargeInteger<1024> i3 = s3;
   mpz_class i3gmp( s3 );
   ASSERT_EQ( (string)( i1 + i3 ), mpz_class( i1gmp + i3gmp ).get_str() );
}

TEST(LargeIntegerTest, Subtraction)
{
   string s1 = "2324562324354654768987455344234356324354656757858568764654657657587686786786";
   LargeInteger<1024> i1 = s1;
   mpz_class i1gmp( s1 );
   ASSERT_EQ( (string)( i1 - 4354657576 ), mpz_class( i1gmp - mpz_class( 4354657576 ) ).get_str() );
   
   ASSERT_EQ( (string)( i1 - -4354657576 ), mpz_class( i1gmp - mpz_class( -4354657576 ) ).get_str() );
   
   ASSERT_EQ( (string)( 4354657576 - i1 ), mpz_class( mpz_class( 4354657576 ) - i1gmp ).get_str() );
   
   ASSERT_EQ( (string)( -4354657576 - i1 ), mpz_class( mpz_class( -4354657576 ) - i1gmp ).get_str() );
   
   string s2 = "122435843953723954234958473942043735374349544738992998187456783424737538394220";
   LargeInteger<1024> i2 = s2;
   mpz_class i2gmp( s2 );
   ASSERT_EQ( (string)( i1 - i2 ), mpz_class( i1gmp - i2gmp ).get_str() );
   
   string s3 = "-122435843953723954234958473942043735374349544738992998187456783424737538394220";
   LargeInteger<1024> i3 = s3;
   mpz_class i3gmp( s3 );
   ASSERT_EQ( (string)( i1 - i3 ), mpz_class( i1gmp - i3gmp ).get_str() );
}

TEST(LargeIntegerTest, UnaryPlus)
{
   string s1 = "2324562324354654768987455344234356324354656757858568764654657657587686786786";
   LargeInteger<1024> i1 = s1;
   ASSERT_EQ( (string)( +i1 ), s1 );
   
   string s2 = "-2324562324354654768987455344234356324354656757858568764654657657587686786786";
   LargeInteger<1024> i2 = s2;
   ASSERT_EQ( (string)( +i2 ), s2 );
}

TEST(LargeIntegerTest, UnaryMinus)
{
   string s1 = "2324562324354654768987455344234356324354656757858568764654657657587686786786";
   string s2 = "-2324562324354654768987455344234356324354656757858568764654657657587686786786";
   LargeInteger<1024> i1 = s1;
   ASSERT_EQ( (string)( -i1 ), s2 );
      
   LargeInteger<1024> i2 = s2;
   ASSERT_EQ( (string)( -i2 ), s1 );
}

TEST(LargeIntegerTest, Multiplication)
{
   string s1 = "2324562324354654768987455344234356324354656757858568764654657657587686786786";
   LargeInteger<1024> i1 = s1;
   mpz_class i1gmp( s1 );
   ASSERT_EQ( (string)( i1 * 4354657576 ), mpz_class( i1gmp * mpz_class( 4354657576 ) ).get_str() );
   
   ASSERT_EQ( (string)( i1 * -4354657576 ), mpz_class( i1gmp * mpz_class( -4354657576 ) ).get_str() );
   
   ASSERT_EQ( (string)( 4354657576 * i1 ), mpz_class( mpz_class( 4354657576 ) * i1gmp ).get_str() );
   
   ASSERT_EQ( (string)( -4354657576 * i1 ), mpz_class( mpz_class( -4354657576 ) * i1gmp ).get_str() );
   
   string s2 = "-2324562324354654768987455344234356324354656757858568764654657657587686786786";
   LargeInteger<1024> i2 = s2;
   mpz_class i2gmp( s2 );
   ASSERT_EQ( (string)( i2 * 4354657576 ), mpz_class( i2gmp * mpz_class( 4354657576 ) ).get_str() );
   
   ASSERT_EQ( (string)( i2 * -4354657576 ), mpz_class( i2gmp * mpz_class( -4354657576 ) ).get_str() );
   
   ASSERT_EQ( (string)( 4354657576 * i2 ), mpz_class( mpz_class( 4354657576 ) * i2gmp ).get_str() );
   
   ASSERT_EQ( (string)( -4354657576 * i2 ), mpz_class( mpz_class( -4354657576 ) * i2gmp ).get_str() );
   
   string s3 = "122435843953723954234958473942043735374349544738992998187456783424737538394220";
   LargeInteger<1024> i3 = s3;
   mpz_class i3gmp( s3 );
   ASSERT_EQ( (string)( i1 * i3 ), mpz_class( i1gmp * i3gmp ).get_str() );
   
   string s4 = "-122435843953723954234958473942043735374349544738992998187456783424737538394220";
   LargeInteger<1024> i4 = s4;
   mpz_class i4gmp( s4 );
   ASSERT_EQ( (string)( i1 * i4 ), mpz_class( i1gmp * i4gmp ).get_str() );
   
   ASSERT_EQ( (string)( i2 * i4 ), mpz_class( i2gmp * i4gmp ).get_str() );
}

TEST(LargeIntegerTest, Division)
{
   string s1 = "2324562324354654768987455344234356324354656757858568764654657657587686786786";
   LargeInteger<1024> i1 = s1;
   mpz_class i1gmp( s1 );
   ASSERT_EQ( (string)( i1 / 4354657576 ), mpz_class( i1gmp / mpz_class( 4354657576 ) ).get_str() );
   
   ASSERT_EQ( (string)( i1 / -4354657576 ), mpz_class( i1gmp / mpz_class( -4354657576 ) ).get_str() );
   
   ASSERT_EQ( (string)( 4354657576 / i1 ), mpz_class( mpz_class( 4354657576 ) / i1gmp ).get_str() );
   
   ASSERT_EQ( (string)( -4354657576 / i1 ), mpz_class( mpz_class( -4354657576 ) / i1gmp ).get_str() );
   
   string s2 = "-2324562324354654768987455344234356324354656757858568764654657657587686786786";
   LargeInteger<1024> i2 = s2;
   mpz_class i2gmp( s2 );
   ASSERT_EQ( (string)( i2 / 4354657576 ), mpz_class( i2gmp / mpz_class( 4354657576 ) ).get_str() );
   
   ASSERT_EQ( (string)( i2 / -4354657576 ), mpz_class( i2gmp / mpz_class( -4354657576 ) ).get_str() );
   
   ASSERT_EQ( (string)( 4354657576 / i2 ), mpz_class( mpz_class( 4354657576 ) / i2gmp ).get_str() );
   
   ASSERT_EQ( (string)( -4354657576 / i2 ), mpz_class( mpz_class( -4354657576 ) / i2gmp ).get_str() );
   
   string s3 = "122435843953723954234958473942035374349544738992998187456783424737538394220";
   LargeInteger<1024> i3 = s3;
   mpz_class i3gmp( s3 );
   ASSERT_EQ( (string)( i1 / i3 ), mpz_class( i1gmp / i3gmp ).get_str() );
   
   string s4 = "-122435843953723954234958473942035374349544738992998187456783424737538394220";
   LargeInteger<1024> i4 = s4;
   mpz_class i4gmp( s4 );
   ASSERT_EQ( (string)( i1 / i4 ), mpz_class( i1gmp / i4gmp ).get_str() );
   
   ASSERT_EQ( (string)( i2 / i4 ), mpz_class( i2gmp / i4gmp ).get_str() );
   
   ::testing::FLAGS_gtest_death_test_style = "threadsafe";
   ASSERT_DEATH( i1 / LargeInteger<1024>(), "" );
}

TEST(LargeIntegerTest, Modulo)
{
   string s1 = "2324562324354654768987455344234356324354656757858568764654657657587686786786";
   LargeInteger<1024> i1 = s1;
   mpz_class i1gmp( s1 );
   ASSERT_EQ( i1 % 4354657576 , atoll(mpz_class( i1gmp % mpz_class( 4354657576 ) ).get_str().c_str() ) );
   
   ASSERT_EQ( i1 % -4354657576, atoll(mpz_class( i1gmp % mpz_class( -4354657576 ) ).get_str().c_str() ) );
   
   ASSERT_EQ( 4354657576 % i1, atoll(mpz_class( mpz_class( 4354657576 ) % i1gmp ).get_str().c_str() ) );
   
   ASSERT_EQ( -4354657576 % i1, atoll(mpz_class( mpz_class( -4354657576 ) % i1gmp ).get_str().c_str() ) );
   
   string s2 = "-2324562324354654768987455344234356324354656757858568764654657657587686786786";
   LargeInteger<1024> i2 = s2;
   mpz_class i2gmp( s2 );
   ASSERT_EQ( i2 % 4354657576, atoll(mpz_class( i2gmp % mpz_class( 4354657576 ) ).get_str().c_str() ) );
   
   ASSERT_EQ( i2 % -4354657576, atoll(mpz_class( i2gmp % mpz_class( -4354657576 ) ).get_str().c_str() ) );
   
   ASSERT_EQ( 4354657576 % i2, atoll(mpz_class( mpz_class( 4354657576 ) % i2gmp ).get_str().c_str() ) );
   
   ASSERT_EQ( -4354657576 % i2, atoll(mpz_class( mpz_class( -4354657576 ) % i2gmp ).get_str().c_str() ) );
   
   string s3 = "122435843953723954234958473942035374349544738992998187456783424737538394220";
   LargeInteger<1024> i3 = s3;
   mpz_class i3gmp( s3 );
   ASSERT_EQ( (string)( i1 % i3 ), mpz_class( i1gmp % i3gmp ).get_str() );
   
   string s4 = "-122435843953723954234958473942035374349544738992998187456783424737538394220";
   LargeInteger<1024> i4 = s4;
   mpz_class i4gmp( s4 );
   ASSERT_EQ( (string)( i1 % i4 ), mpz_class( i1gmp % i4gmp ).get_str() );
   
   ASSERT_EQ( (string)( i2 % i4 ), mpz_class( i2gmp % i4gmp ).get_str() );
   
   ::testing::FLAGS_gtest_death_test_style = "threadsafe";
   ASSERT_DEATH( i1 % LargeInteger<1024>(), "" );
   
   ASSERT_EQ( (string)( i1 / i3 ).getRemaining(), mpz_class( i1gmp % i3gmp ).get_str() );
}

TEST(LargeIntegerTest, Increments)
{
   string s1 = "2324562324354654768987455344234356324354656757858568764654657657587686786786";
   LargeInteger<1024> i1 = s1;
   ASSERT_EQ( (string)++i1, (string)(LargeInteger<1024>(s1) + 1 ) );
   ASSERT_EQ( (string)i1, (string)(LargeInteger<1024>(s1) + 1 ) );
   ASSERT_EQ( (string)i1++, (string)(LargeInteger<1024>(s1) + 1 ) );
   ASSERT_EQ( (string)i1, (string)(LargeInteger<1024>(s1) + 2 ) );
}

TEST(LargeIntegerTest, Decrements)
{
   string s1 = "2324562324354654768987455344234356324354656757858568764654657657587686786786";
   LargeInteger<1024> i1 = s1;
   ASSERT_EQ( (string)--i1, (string)(LargeInteger<1024>(s1) - 1 ) );
   ASSERT_EQ( (string)i1, (string)(LargeInteger<1024>(s1) - 1 ) );
   ASSERT_EQ( (string)i1--, (string)(LargeInteger<1024>(s1) - 1 ) );
   ASSERT_EQ( (string)i1, (string)(LargeInteger<1024>(s1) - 2 ) );
}

TEST(LargeIntegerTest, Equality)
{
   string s1 = "2324562324354654768987455344234356324354656757858568764654657657587686786786";
   string s2 = "122435843953723954234958473942043735374349544738992998187456783424737538394220";
   LargeInteger<1024> i1 = s1;
   LargeInteger<1024> i2 = s2;
   LargeInteger<1024> i3 = s1;
   ASSERT_EQ( i1 == i2, false );
   ASSERT_EQ( i1 == i3, true );
   ASSERT_EQ( LargeInteger<1024>( "1234" ) == 1234, true );
   ASSERT_EQ( LargeInteger<1024>( "1234" ) == -1234, false );
}

TEST(LargeIntegerTest, Unequality)
{
   string s1 = "2324562324354654768987455344234356324354656757858568764654657657587686786786";
   string s2 = "122435843953723954234958473942043735374349544738992998187456783424737538394220";
   LargeInteger<1024> i1 = s1;
   LargeInteger<1024> i2 = s2;
   LargeInteger<1024> i3 = s1;
   ASSERT_EQ( i1 != i2, true );
   ASSERT_EQ( i1 != i3, false );
   ASSERT_EQ( LargeInteger<1024>( "1234" ) != 1234, false );
   ASSERT_EQ( LargeInteger<1024>( "1234" ) != -1234, true );
}

TEST(LargeIntegerTest, Greater)
{
   string s1 = "2324562324354654768987455344234356324354656757858568764654657657587686786786";
   string s2 = "122435843953723954234958473942043735374349544738992998187456783424737538394220";
   string s3 = "-2324562324354654768987455344234356324354656757858568764654657657587686786786";
   string s4 = "-122435843953723954234958473942043735374349544738992998187456783424737538394220";
   LargeInteger<1024> i1 = s1;
   LargeInteger<1024> i2 = s2;
   LargeInteger<1024> i3 = s3;
   LargeInteger<1024> i4 = s4;
   LargeInteger<1024> i5 = s1;
   ASSERT_EQ( i1 > i2, false );
   ASSERT_EQ( i1 > i3, true );
   ASSERT_EQ( i1 > i5, false );
   ASSERT_EQ( i2 > i1, true );
   ASSERT_EQ( i3 > i1, false );
   ASSERT_EQ( i3 > i4, true );
   ASSERT_EQ( i4 > i3, false );
   ASSERT_EQ( LargeInteger<1024>( "1234" ) > 1234, false );
   ASSERT_EQ( LargeInteger<1024>( "1234" ) > -1234, true );
}

TEST(LargeIntegerTest, Less)
{
   string s1 = "2324562324354654768987455344234356324354656757858568764654657657587686786786";
   string s2 = "122435843953723954234958473942043735374349544738992998187456783424737538394220";
   string s3 = "-2324562324354654768987455344234356324354656757858568764654657657587686786786";
   string s4 = "-122435843953723954234958473942043735374349544738992998187456783424737538394220";
   LargeInteger<1024> i1 = s1;
   LargeInteger<1024> i2 = s2;
   LargeInteger<1024> i3 = s3;
   LargeInteger<1024> i4 = s4;
   LargeInteger<1024> i5 = s1;
   ASSERT_EQ( i1 < i2, true );
   ASSERT_EQ( i1 < i3, false );
   ASSERT_EQ( i1 < i5, false );
   ASSERT_EQ( i2 < i1, false );
   ASSERT_EQ( i3 < i1, true );
   ASSERT_EQ( i3 < i4, false );
   ASSERT_EQ( i4 < i3, true );
   ASSERT_EQ( LargeInteger<1024>( "1234" ) < 1234, false );
   ASSERT_EQ( LargeInteger<1024>( "1234" ) < -1234, false );
}

TEST(LargeIntegerTest, GreaterOrEqual)
{
   string s1 = "2324562324354654768987455344234356324354656757858568764654657657587686786786";
   string s2 = "122435843953723954234958473942043735374349544738992998187456783424737538394220";
   string s3 = "-2324562324354654768987455344234356324354656757858568764654657657587686786786";
   string s4 = "-122435843953723954234958473942043735374349544738992998187456783424737538394220";
   LargeInteger<1024> i1 = s1;
   LargeInteger<1024> i2 = s2;
   LargeInteger<1024> i3 = s3;
   LargeInteger<1024> i4 = s4;
   LargeInteger<1024> i5 = s1;
   ASSERT_EQ( i1 >= i2, false );
   ASSERT_EQ( i1 >= i3, true );
   ASSERT_EQ( i1 >= i5, true );
   ASSERT_EQ( i2 >= i1, true );
   ASSERT_EQ( i3 >= i1, false );
   ASSERT_EQ( i3 >= i4, true );
   ASSERT_EQ( i4 >= i3, false );
   ASSERT_EQ( LargeInteger<1024>( "1234" ) >= 1234, true );
   ASSERT_EQ( LargeInteger<1024>( "1234" ) >= -1234, true );
}

TEST(LargeIntegerTest, LessOrEqual)
{
   string s1 = "2324562324354654768987455344234356324354656757858568764654657657587686786786";
   string s2 = "122435843953723954234958473942043735374349544738992998187456783424737538394220";
   string s3 = "-2324562324354654768987455344234356324354656757858568764654657657587686786786";
   string s4 = "-122435843953723954234958473942043735374349544738992998187456783424737538394220";
   LargeInteger<1024> i1 = s1;
   LargeInteger<1024> i2 = s2;
   LargeInteger<1024> i3 = s3;
   LargeInteger<1024> i4 = s4;
   LargeInteger<1024> i5 = s1;
   ASSERT_EQ( i1 <= i2, true );
   ASSERT_EQ( i1 <= i3, false );
   ASSERT_EQ( i1 <= i5, true );
   ASSERT_EQ( i2 <= i1, false );
   ASSERT_EQ( i3 <= i1, true );
   ASSERT_EQ( i3 <= i4, false );
   ASSERT_EQ( i4 <= i3, true );
   ASSERT_EQ( LargeInteger<1024>( "1234" ) <= 1234, true );
   ASSERT_EQ( LargeInteger<1024>( "1234" ) <= -1234, false );
}

TEST(LargeIntegerTest, BitwiseNot)
{
   string s1 = "0x12e243F58439537239542349584A739420437353743b49544738992998187456c783424737538394220";
   string s2 = "0xed1dbc0a7bc6ac8dc6abdcb6a7b58c6bdfbc8cac8bc4b6abb8c766d667e78ba9387cbdb8c8ac7c6bddf";
   string s3 = "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";
   LargeInteger<1024> i1 = s1;
   LargeInteger<1024> i2 = s2;
   LargeInteger<1024> i3 = s3;
   ASSERT_EQ( (~i1 & i3 ) == i2, true );
}

TEST(LargeIntegerTest, BitwiseAnd)
{
   string s1 = "0x12e243F58439537239542349584A739420437353743b49544738992998187456c783424737538394220";
   string s2 = "0x00000002324562324354654768987455344234356324354656757858568764654657657587686786786";
   string s3 = "0x00000000000142320154214148087014204230116020014446301808100064444603404507400384200";
   LargeInteger<1024> i1 = s1;
   LargeInteger<1024> i2 = s2;
   LargeInteger<1024> i3 = s3;
   ASSERT_EQ( (i1 & i2 ) == i3, true );
}

TEST(LargeIntegerTest, BitwiseOr)
{
   string s1 = "0x12e243F58439537239542349584A739420437353743b49544738992998187456c783424737538394220";
   string s2 = "0x00000002324562324354654768987455344234356324354656757858568764654657657587686786786";
   string s3 = "0x12e243f7b67d73727b54674f78da77d534437777773f7d56577df979de9f7477c7d76777b77be7967a6";
   LargeInteger<1024> i1 = s1;
   LargeInteger<1024> i2 = s2;
   LargeInteger<1024> i3 = s3;
   ASSERT_EQ( (i1 | i2 ) == i3, true );
}

TEST(LargeIntegerTest, BitwiseXor)
{
   string s1 = "0x12e243F58439537239542349584A739420437353743b49544738992998187456c783424737538394220";
   string s2 = "0x00000002324562324354654768987455344234356324354656757858568764654657657587686786786";
   string s3 = "0x12e243f7b67c31407a00460e30d207c114014766171f7c12114de171ce9f103381d42732b03be4125a6";
   LargeInteger<1024> i1 = s1;
   LargeInteger<1024> i2 = s2;
   LargeInteger<1024> i3 = s3;
   ASSERT_EQ( (i1 ^ i2 ) == i3, true );
}

TEST(LargeIntegerTest, LeftShift)
{
   string s1 = "0x12e243F58439537239542349584A739420437353743b49544738992998187456c783424737538394220";
   int shift = 230;
   
   LargeInteger<1024> i1 = s1;
   LargeInteger<1024> i2 = s1;
   for( int i=0; i<shift; ++i ) i2 *= 2;
   ASSERT_EQ( ( i1 << shift ), i2 );
}

TEST(LargeIntegerTest, RightShift)
{
   string s1 = "0x12e243F58439537239542349584A739420437353743b49544738992998187456c783424737538394220";
   int shift = 230;
   
   LargeInteger<1024> i1 = s1;
   LargeInteger<1024> i2 = s1;
   for( int i=0; i<shift; ++i ) i2 /= 2;
   ASSERT_EQ( ( i1 >> shift ), i2 );
   
   string s2 = "-122435843953723954234958473942043735374349544738992998187456783424737538394220";
   i1 = s2;
   shift = 95;
   ASSERT_EQ( (string)( i1 >> shift ), mpz_class( mpz_class( s2 ) >> shift ).get_str() );
}

TEST(LargeIntegerTest, Cast)
{
   LargeInteger<1024> i1( 1234567 );
   ASSERT_EQ( ( int64_t )( i1 ), 1234567 );
   
   LargeInteger<1024> i2( -1234567 );
   ASSERT_EQ( ( int64_t )( i2 ), -1234567 );
}

TEST(LargeIntegerTest, OutputStreams)
{
   string s = "122435843953723954234958473942043735374349544738992998187456783424737538394220";
   LargeInteger<1024> i( s );
   
   std::stringstream ss;
   ss << i;
   ASSERT_EQ( s, ss.str() );
   
   string s2 = "0x12e243f58439537239542349584a739420437353743b49544738992998187456c783424737538394220";
   LargeInteger<1024> i2( s2 );
   ss.str( "" );
   ss << std::hex << i2;
   ASSERT_EQ( s2.substr( 2 ), ss.str() );
   
   string s3 = "0122435743753723754234757473742043735374347544737772777177456773424737537374220";
   LargeInteger<1024> i3( s3 );
   ss.str( "" );
   ss << std::oct << i3;
   ASSERT_EQ( s3.substr( 1 ), ss.str() );
   
   ss.str( "" );
   ss << std::dec << std::showbase << i;
   ASSERT_EQ( s, ss.str() );
   
   ss.str( "" );
   ss << std::hex << std::showbase << i2;
   ASSERT_EQ( s2, ss.str() );
   
   ss.str( "" );
   ss << std::oct << std::showbase << i3;
   ASSERT_EQ( s3, ss.str() );
   
   ss.str( "" );
   ss << std::dec << std::showpos << i;
   ASSERT_EQ( string("+") + s, ss.str() );
   
   ss.str( "" );
   ss << std::hex << std::showpos << i2;
   ASSERT_EQ( s2, ss.str() );
   
   ss.str( "" );
   ss << std::oct << std::showpos << i3;
   ASSERT_EQ( s3, ss.str() );
   
   ss.str( "" );
   string sup2 = s2;
   std::transform( sup2.begin(), sup2.end(), sup2.begin(), ::toupper );
   ss << std::hex << std::noshowpos << std::showbase << std::uppercase << i2;
   ASSERT_EQ( sup2, ss.str() );
   
   ss.str( "" );
   ss << std::noshowbase << i2;
   ASSERT_EQ( sup2.substr( 2 ), ss.str() );
   
   ss.str( "" );
   ss << std::dec << std::internal << std::showbase << std::setfill( '*' ) << std::setw( 4 ) << i;
   ASSERT_EQ( s, ss.str() );
   
   ss.str( "" );
   ss << std::setw( 100 ) << i;
   ASSERT_EQ( string( 100 - s.length(), '*' ) + s, ss.str() );
   
   ss.str( "" );
   ss << std::showpos << std::setw( 100 ) << i;
   ASSERT_EQ( string( "+" ) + string( 100 - s.length() - 1, '*' ) + s, ss.str() );
   
   string s4 = "-122435843953723954234958473942043735374349544738992998187456783424737538394220";
   LargeInteger<1024> i4( s4 );
   ss.str( "" );
   ss << std::setw( 100 ) << i4;
   ASSERT_EQ( string( "-" ) + string( 100 - s4.length(), '*' ) + s4.substr( 1 ), ss.str() );
   
   ss.str( "" );
   ss << std::hex << std::nouppercase << std::setw( 100 ) << i2;
   ASSERT_EQ( string( "0x" ) + string( 100 - s2.length(), '*' ) + s2.substr( 2 ), ss.str() );
   
   ss.str( "" );
   ss << std::oct << std::setw( 100 ) << i3;
   ASSERT_EQ( string( 100 - s3.length(), '*' ) + s3, ss.str() );
   
   ss.str( "" );
   ss << std::noshowpos << std::dec << std::left << std::setw( 100 ) << i;
   ASSERT_EQ( s + string( 100 - s.length(), '*' ), ss.str() );
   
   ss.str( "" );
   ss << std::showpos << std::setw( 100 ) << i;
   ASSERT_EQ( string( "+" ) + s + string( 100 - s.length() - 1, '*' ), ss.str() );
   
   ss.str( "" );
   ss << std::setw( 100 ) << i4;
   ASSERT_EQ( string( "-" ) + s4.substr( 1 ) + string( 100 - s4.length(), '*' ), ss.str() );
   
   ss.str( "" );
   ss << std::hex << std::setw( 100 ) << i2;
   ASSERT_EQ( s2 + string( 100 - s2.length(), '*' ), ss.str() );
   
   ss.str( "" );
   ss << std::oct << std::setw( 100 ) << i3;
   ASSERT_EQ( s3 + string( 100 - s3.length(), '*' ), ss.str() );
   
   ss.str( "" );
   ss << std::noshowpos << std::dec << std::right << std::setw( 100 ) << i;
   ASSERT_EQ( string( 100 - s.length(), '*' ) + s, ss.str() );
   
   ss.str( "" );
   ss << std::showpos << std::setw( 100 ) << i;
   ASSERT_EQ( string( 100 - s.length() - 1, '*' ) + '+' + s, ss.str() );
   
   ss.str( "" );
   ss << std::setw( 100 ) << i4;
   ASSERT_EQ( string( 100 - s4.length(), '*' ) + s4, ss.str() );
   
   ss.str( "" );
   ss << std::hex << std::setw( 100 ) << i2;
   ASSERT_EQ( string( 100 - s2.length(), '*' ) + s2, ss.str() );
   
   ss.str( "" );
   ss << std::oct << std::setw( 100 ) << i3;
   ASSERT_EQ( string( 100 - s3.length(), '*' ) + s3, ss.str() );
}

TEST(LargeIntegerTest, InputStreams)
{
   string s = "122435843953723954234958473942043735374349544738992998187456783424737538394220";
   LargeInteger<1024> i( s );
   string s2 = "0x12e243f58439537239542349584a739420437353743b49544738992998187456c783424737538394220";
   LargeInteger<1024> i2( s2 );
   string s3 = "0122435743753723754234757473742043735374347544737772777177456773424737537374220";
   LargeInteger<1024> i3( s3 );
   string s4 = "-122435843953723954234958473942043735374349544738992998187456783424737538394220";
   LargeInteger<1024> i4( s4 );
   
   LargeInteger<1024> j;
   std::stringstream ss;
   ss << s;
   ss >> j;
   ASSERT_EQ( i, j );
   
   ss.str( "" );
   ss.clear();
   ss << "123e456";
   ss >> j;
   ASSERT_EQ( "123", (string)j );
   
   ss.str( "" );
   ss.clear();
   ss << s2;
   ss >> std::hex >> j;
   ASSERT_EQ( i2, j );
   
   ss.str( "" );
   ss.clear();
   ss << s2.substr( 2 );
   ss >> std::hex >> j;
   ASSERT_EQ( i2, j );
   
   ss.str( "" );
   ss.clear();
   ss << s2;
   ss >> std::hex >> j;
   ASSERT_EQ( i2, j );
   
   ss.str( "" );
   ss.clear();
   ss << s3.substr( 1 );
   ss >> std::oct >> j;
   ASSERT_EQ( i3, j );
   
   ss.str( "" );
   ss.clear();
   ss << s3;
   ss >> std::oct >> j;
   ASSERT_EQ( i3, j );
   
   ss.str( "" );
   ss.clear();
   ss << s4;
   ss >> std::dec >> j;
   ASSERT_EQ( i4, j );
   
   ss.str( "" );
   ss.clear();
   ss << "+" << s;
   ss >> std::dec >> j;
   ASSERT_EQ( i, j );
   
   ss.str( s + " " + s4 );
   ss.clear();
   LargeInteger<1024> k;
   ss >> j >> k;
   ASSERT_EQ( i, j );
   ASSERT_EQ( i4, k );
   
   ss.str( s + " " + s4 );
   ss.clear();
   ss >> std::noskipws >> j >> k;
   ASSERT_EQ( i, j );
   ASSERT_EQ( LargeInteger<1024>( 0 ), k );
}
