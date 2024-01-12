/*
  A typical sample for a function.
  Do as follows to compile it:
    $ g++ sample3.cc
  So an executable file "a.out" is created.
  Do as follows to execute it:
    $ ./a.out
*/
#include <iostream>

/*
  A declaration (宣言) of a function.
  It just tells
    - The name of this function
    - what is the argument
    - what is returned.
  But the actual behaviour is not written (it can be written technically.).
*/
double ReturnTwice( double value ) ;

/*
  A definition (定義) of the function.
  The actual behaviour is written here.
*/
double ReturnTwice( double value )
{
  return 2 * value;
}

int main()
{
  double value = 2.0;
  std::cout << "Input value: " << value << std::endl;
  std::cout << "Twice of " << value << " is " << ReturnTwice( value ) << std::endl;
  
  return 0;
}
