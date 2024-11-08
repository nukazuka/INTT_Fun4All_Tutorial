/*
  A sample to use own function written in other header/source files.

  How To Compile
    1) Compile everything at once
      $ g++ sample5.cc function5.cc
      
    2) Split compilation (分割コンパイル) by hand
      $ g++ -o sample5.o -c sample5.cc
      $ g++ -o function5.o -c function5.cc
      $ g++ sample5.o function5.o
      $ ./a.out

    3) Split compilation with make
      $ make sample5
      $ ./sample5
*/

#include <iostream>
#include "function5.h"

int main()
{
  double value = 2.0;
  std::cout << "Input value: " << value << std::endl;
  std::cout << "Twice of " << value << " is " << ReturnTwice( value ) << std::endl;
  
  return 0;
}
