/*
  A sample to use own header file.
*/
#include <iostream>

/* You header file is included here. File is searched in the current directory. */
#include "function4.h"

int main()
{
  double value = 2.0;
  std::cout << "Input value: " << value << std::endl;
  std::cout << "Twice of " << value << " is " << ReturnTwice( value ) << std::endl;
  
  return 0;
}
