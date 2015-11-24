multiint
========

Multiint is a lightweight c++ library that defines large integer
types. This is a header only library: include the header, typedef 
the number of bits you need for your type and just use like a normal
integer types. It includes support for streaming operators.

Why another large integer library
---------------------------------

The first version of this library was written because on one project i
did not have the possibility to use either gmp or boost. Moreover,
most opensource big integer libraries i found was using strings to
represent integers which is quite inefficient. So, at the time i
wrote a basic integer type containing only the operations i needed.
When the project was completed, the library did not miss many
operations to be used as a general purpose big integer library, so i
decided to complete it and publish it in the hope it can be useful to
someone else.

Usage example
-------------

The integer type takes as a parameter the number of bits of its binary
representation.

```
#include "multint.hpp"

typedef LargeInteger<1024> int1024_t;
```

Limitations
-----------

* The library uses the compiler provided __int128 type when available.
It is known to be available in gcc and clang. When this type is not
available, an local basic implementation is used that may be slower.

* The number of bits of the integer must be a multiple a 64. Using
another length will issue a compiler error.

Unit tests
----------

The CMake project provided with this library can be used to compile
unit tests. Unit tests are based on google testing framework and
require gmp library. If you intent to compile unit tests to not forget
to synchronize the gtest submodule:

```
git submodule init
git submodule update
```