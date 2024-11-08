/*
  A typical sample for beginners, so-called Hello, world.
  Do as follows to compile it:
    $ g++ sample2.cc
  So an executable file "a.out" is created.
  Do as follows to execute it:
    $ ./a.out
*/

/*
  The following statement includes iostream header. This is for input/output.
*/
#include <iostream>

int main()
{
  /* Because cout is defined in "std" naespace, std:: is needed to access cout. */
  std::cout << "Hello, world!" << std::endl;

  /* You can skip std:: if you enable the next line. */
  // using namespace std;
  //  cout << "Hello, world!" << endl;
  
  /* This is sometimes not recommended because if something else is named "cout" in other namespace, a confliction is made.
     In ROOT, "using namespace std;" is always done. This is why you don't need std:: in your ROOT macros.  */
  return 0;
}
