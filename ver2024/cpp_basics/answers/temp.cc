#include <TH1D.h>
int main()
{
  TH1D* hist = new TH1D( "name", "title", 100, 0, 100 ) ; 
  return 0;
}
