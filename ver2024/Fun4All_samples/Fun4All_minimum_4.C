// Fun4All headers
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllDstInputManager.h>

// Some general header macros
#include <GlobalVariables.C>
#include <G4Setup_sPHENIX.C>
#include <G4_Input.C>

// Trkr headers
#include <Trkr_RecoInit.C>
#include <Trkr_Clustering.C>
#include <G4_ActsGeom.C>

// something else
#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>
#include <ffamodules/CDBInterface.h>

#include <phool/PHRandomSeed.h>
#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libfun4all.so)

#include <tutorial.h>
R__LOAD_LIBRARY( libtutorial.so )

int Fun4All_minimum_4(
		      int nEvents = 10,
		      const string &data = "/sphenix/lustre01/sphnxpro/physics/slurp/tracking/new_2024p007/run_00051100_00051200/DST_TRKR_CLUSTER_run2pp_new_2024p007-00051100-00000.root" 
		      )
{
  
  Fun4AllServer *se = Fun4AllServer::instance();
  //se->Verbosity(0);
  
  // Read DST
  Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin");
  in->fileopen( data );
  // in->AddListFile(inputfile); // to read a list of files, use it. A path to the text file needs to be given.
  se->registerInputManager( in );

  // Flag Handler is always needed to read flags from input (if used)
  // and update our rc flags with them. At the end it saves all flags
  // again on the DST in the Flags node under the RUN node
  FlagHandler *flag = new FlagHandler();
  se->registerSubsystem(flag);
  Enable::CDB = true;
  // global tag

  recoConsts *rc = recoConsts::instance();
  rc->set_StringFlag("CDB_GLOBALTAG",CDB::global_tag);
  // 64 bit timestamp
  rc->set_uint64Flag("TIMESTAMP",CDB::timestamp);
  rc->set_IntFlag("RUNNUMBER", 0 );

  ///////////////////////////////////////////////////////////////////////////////////
  // Something depends on Acts should be below.... 
  ///////////////////////////////////////////////////////////////////////////////////
  // central tracking
  Enable::MVTX					= true;
  Enable::TPC					= true;
  Enable::MICROMEGAS				= true;
  Enable::INTT					= true;
  Enable::BLACKHOLE				= true;
  G4MAGNET::magfield_rescale = 1.4;

  // Initialize the selected subsystems
  // G4Init();

  // GEANT4 Detector description
  // if (!Input::READHITS)
  //   {
  //     G4Setup();
  //   }

  TrackingInit(); // necessary for ActsGeometry

  //////////////////////////
  // Your analysis module // 
  //////////////////////////
  tutorial* analysis_module = new tutorial( "name" );
  analysis_module->Verbosity( 1 ); // 0: minimum(default), 1: event by event info, 2: hit by hit info
  se->registerSubsystem( analysis_module );
  
  //se->skip(skip);
  se->run(nEvents);
  se->End();
  
  delete se;
  
  gSystem->Exit(0);
  return 0;
}
