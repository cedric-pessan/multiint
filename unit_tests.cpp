
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
}

TEST(LargeIntegerText, Copy)
{
}
