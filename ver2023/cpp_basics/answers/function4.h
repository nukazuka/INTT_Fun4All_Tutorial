/*
  A simple header file for practice.
*/

/*
  This is so-called "include guard".
  Declaration must be done only once. If you include this header file twice or more,
  an error is occues. Include guard prevents you from including this header multiple times.
*/
#ifndef Header_ReturnTwice /* If the keyword "Header_ReturnTwice" is not defined yet, you can go to the following lines */
#define Header_ReturnTwice /* The keyword is defined here. You'll never come back because of #ifndef. */

// A declaration (宣言) of a function
double ReturnTwice( double value ) ;

// A definition (定義) of the function
double ReturnTwice( double value )
{
  return 2 * value;
}

// end of Header_ReturnTwice
#endif
