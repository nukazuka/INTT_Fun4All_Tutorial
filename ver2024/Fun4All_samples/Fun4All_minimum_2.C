#include <fun4all/Fun4AllServer.h>
#include <fun4all/SubsysReco.h>

R__LOAD_LIBRARY(libfun4all.so)

// It should be tutorial.h of sample_moudle_2
#include <tutorial.h>
R__LOAD_LIBRARY( libtutorial.so )

int Fun4All_minimum_2( int nEvents = 1 )
{

  Fun4AllServer *se = Fun4AllServer::instance();
  
  tutorial* analysis_module = new tutorial( "name" );
  se->registerSubsystem( analysis_module );

  se->run(nEvents);
  se->End();
  delete se;

  gSystem->Exit(0);
  return 0;
}
