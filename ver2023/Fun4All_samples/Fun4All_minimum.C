#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libfun4all.so)

int Fun4All_minimum()
{

  Fun4AllServer *se = Fun4AllServer::instance();

  se->run( 1 );
  se->End();
  delete se;

  gSystem->Exit(0);
  return 0;
}

