#include <G4_Input.C>

#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>
#include <ffamodules/CDBInterface.h>

#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <phool/PHRandomSeed.h>
#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libfun4all.so)

#include <tutorial.h>
R__LOAD_LIBRARY( libtutorial.so )

int Fun4All_minimum_2(
		     int nEvents = 1, //5,
		     const string &inputFile = "/sphenix/tg/tg01/commissioning/INTT/work/hachiya/InttReco/DST_InttCluster_run20869.root",
		     //"https://www.phenix.bnl.gov/WWW/publish/phnxbld/sPHENIX/files/sPHENIX_G4Hits_sHijing_9-11fm_00000_00010.root",
		     const int skip = 0
		     )
{

  Fun4AllServer *se = Fun4AllServer::instance();

  INPUTREADHITS::filename[0] = inputFile;
  InputInit();
  InputRegister();
  
  tutorial* analysis_module = new tutorial( "name" );
  se->registerSubsystem( analysis_module );

  se->skip(skip);
  se->run(nEvents);
  se->End();
  delete se;

  gSystem->Exit(0);
  return 0;
}
