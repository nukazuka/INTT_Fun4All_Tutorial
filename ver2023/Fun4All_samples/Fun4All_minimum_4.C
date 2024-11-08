#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libfun4all.so)

#include <tutorial.h>
R__LOAD_LIBRARY( libtutorial.so )

int Fun4All_minimum_4(
		      int nEvents = 1, //5,
		      const string &inputFile = "/sphenix/tg/tg01/commissioning/INTT/data/dst_files/2023/test/test_DST_beam_intt_00020869_1000events.root",
		      const int skip = 0
		      )
{

  Fun4AllServer *se = Fun4AllServer::instance();

  // This is needed to read a DST file(s).
  Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin");
  in->fileopen( inputFile );
  // in->AddListFile(inputfile); // to read a list of files, use it. A path to the text file needs to be given.
  se->registerInputManager( in );
  
  tutorial* analysis_module = new tutorial( "name" );
  se->registerSubsystem( analysis_module );

  se->skip( skip );
  se->run( nEvents );
  se->End();
  delete se;

  gSystem->Exit(0);
  return 0;
}
