#include <fun4all/Fun4AllServer.h>

#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
R__LOAD_LIBRARY(libfun4all.so)

// It should be tutorial.h of sample_moudle_3
#include <tutorial.h>
R__LOAD_LIBRARY( libtutorial.so )

int Fun4All_minimum_3( int nEvents = 1,
		       const string &data = "/sphenix/lustre01/sphnxpro/physics/slurp/streaming/physics/inttonlyrun_00051100_00051200/DST_INTT_EVENT_run2pp_new_2024p002-00051171-00000.root" )
{

  Fun4AllServer *se = Fun4AllServer::instance();

  Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin");
  in->fileopen( data ); // this is an old method but supports skip function
  //in->AddFile( data ); // another way to read a DST
  se->registerInputManager(in);

  tutorial* analysis_module = new tutorial( "name" );
  analysis_module->Verbosity( 1 ); // 0: minimum(default), 1: event by event info, 2: hit by hit info
  se->registerSubsystem( analysis_module );

  // se->skip( 0 ); // First some events (given through argument) are skipped. fileopen needs to be used. AddFile doesn't support it.
  se->run(nEvents);
  se->End();
  delete se;

  gSystem->Exit(0);
  return 0;
}
