// #include <GlobalVariables.C>

//#include <G4Setup_sPHENIX_Bbc.C>
// #include <G4_Bbc.C>
// #include <G4_CaloTrigger.C>
// #include <G4_Centrality.C>
// #include <G4_DSTReader.C>
// #include <G4_Global.C>
// #include <G4_HIJetReco.C>
#include <G4_Input.C>
// #include <G4_Jets.C>
// #include <G4_KFParticle.C>
// #include <G4_ParticleFlow.C>
// #include <G4_Production.C>
// #include <G4_TopoClusterReco.C>

// #include <Trkr_RecoInit.C>
// #include <Trkr_Clustering.C>
// #include <Trkr_LaserClustering.C>
// #include <Trkr_Reco.C>
// #include <Trkr_Eval.C>
// #include <Trkr_QA.C>

// #include <Trkr_Diagnostics.C>
// #include <G4_User.C>
// #include <QA.C>

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
		     const string &inputFile = "https://www.phenix.bnl.gov/WWW/publish/phnxbld/sPHENIX/files/sPHENIX_G4Hits_sHijing_9-11fm_00000_00010.root",
		     const int skip = 0
		     )
{

  Fun4AllServer *se = Fun4AllServer::instance();
  //  se->Print("NODETREE"); // useless
  //se->Verbosity(0);

  //Opt to print all random seed used for debugging reproducibility. Comment out to reduce stdout prints.
  //PHRandomSeed::Verbosity(1);

  // just if we set some flags somewhere in this macro
  recoConsts *rc = recoConsts::instance();
  // By default every random number generator uses
  // PHRandomSeed() which reads /dev/urandom to get its seed
  // if the RANDOMSEED flag is set its value is taken as seed
  // You can either set this to a random value using PHRandomSeed()
  // which will make all seeds identical (not sure what the point of
  // this would be:
  //  rc->set_IntFlag("RANDOMSEED",PHRandomSeed());
  // or set it to a fixed value so you can debug your code
  //  rc->set_IntFlag("RANDOMSEED", 12345);

  INPUTREADHITS::filename[0] = inputFile;
  InputInit();

  InputRegister();

  tutorial* analysis_module = new tutorial( "name" );
  se->registerSubsystem( analysis_module );
  
  // Flag Handler is always needed to read flags from input (if used)
  // and update our rc flags with them. At the end it saves all flags
  // again on the DST in the Flags node under the RUN node
  // FlagHandler *flag = new FlagHandler();
  // se->registerSubsystem(flag);

  se->skip(skip);
  se->run(nEvents);
  se->End();
  delete se;

  gSystem->Exit(0);
  return 0;
}
