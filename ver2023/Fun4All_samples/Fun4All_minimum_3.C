#include <GlobalVariables.C>

//#include <G4Setup_sPHENIX_Bbc.C>
#include <G4Setup_sPHENIX.C>
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

#include <Trkr_RecoInit.C>
#include <Trkr_Clustering.C>
#include <Trkr_LaserClustering.C>
#include <Trkr_Reco.C>
#include <Trkr_Eval.C>
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

int Fun4All_minimum_3(
		     int nEvents = 1, //5,
		     const string &inputFile = "https://www.phenix.bnl.gov/WWW/publish/phnxbld/sPHENIX/files/sPHENIX_G4Hits_sHijing_9-11fm_00000_00010.root",
		     const string &outputFile = "results.root",
		     const int skip = 0,
		     const bool is_pythia = true
		     
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

  // Force to use Pythia8 or GUN
  Input::PYTHIA8 = is_pythia;
  Input::GUN = !( is_pythia );
  if( Input::PYTHIA8 == false )
    {
      Input::GUN_NUMBER = 1; // if you need 3 of them
      Input::GUN_VERBOSITY = 1;
    }
  
  ///////////////////
  // Initialize the selected Input/Event generation
  ///////////////////
  InputInit();

  ///////////////
  // Set generator specific options
  ////////////////
  // can only be set after InputInit() is called
  
  // pythia8
  if (Input::PYTHIA8)
    {
      //! Nominal collision geometry is selected by Input::BEAM_CONFIGURATION
      Input::ApplysPHENIXBeamParameter(INPUTGENERATOR::Pythia8);
    }
  else // for GUN
    {
      // particle gun
      // if you run more than one of these Input::GUN_NUMBER > 1
      // add the settings for other with [1], next with [2]...
      INPUTGENERATOR::Gun[0]->AddParticle("pi-", 0, 1, 0.1 );
      INPUTGENERATOR::Gun[0]->set_vtx(0, 0, 0);
    }


  InputRegister();

  // Flag Handler is always needed to read flags from input (if used)
  // and update our rc flags with them. At the end it saves all flags
  // again on the DST in the Flags node under the RUN node
  FlagHandler *flag = new FlagHandler();
  se->registerSubsystem(flag);

  //======================
  // Write the DST
  //======================

  //Enable::DSTOUT = true;
  Enable::DSTOUT_COMPRESS = false;
  // DstOut::OutputDir = outdir;
  // DstOut::OutputFile = outputFile;

  //Option to convert DST to human command readable TTree for quick poke around the outputs
  //  Enable::DSTREADER = true;

  // turn the display on (default off)
  //Enable::DISPLAY = true;

  //======================
  // What to run
  //======================
  // QA, main switch
  Enable::QA					= false;

  // Global options (enabled for all enables subsystems - if implemented)
  //  Enable::ABSORBER				= true;
  //  Enable::OVERLAPCHECK			= true;
  //  Enable::VERBOSITY				= 1;

  // Enable::MBD				= true;
  // Enable::MBD_SUPPORT			= true; // save hist in mbd support structure
  // Enable::MBDRECO				= Enable::MBD && true
  //Enable::MBDFAKE				= true;  // Smeared vtx and t0, use if you don't want real MBD in simulation

  Enable::PIPE					= true;
  Enable::PIPE_ABSORBER				= true;

  // central tracking
  Enable::MVTX					= true;
  Enable::MVTX_CELL				= Enable::MVTX && true;
  Enable::MVTX_CLUSTER				= Enable::MVTX_CELL && true;
  Enable::MVTX_QA				= Enable::MVTX_CLUSTER && Enable::QA && true;

  Enable::INTT					= true;
  //  Enable::INTT_ABSORBER			= true; // enables layerwise support structure readout
  //  Enable::INTT_SUPPORT			= true; // enable global support structure readout
  Enable::INTT_CELL				= Enable::INTT && true;
  Enable::INTT_CLUSTER				= Enable::INTT_CELL && true;
  Enable::INTT_QA				= Enable::INTT_CLUSTER && Enable::QA && true;

  Enable::TPC					= true;
  Enable::TPC_ABSORBER				= true;
  Enable::TPC_CELL				= Enable::TPC && true;
  Enable::TPC_CLUSTER				= Enable::TPC_CELL && true;
  Enable::TPC_QA				= Enable::TPC_CLUSTER && Enable::QA && true;

  Enable::MICROMEGAS				= true;
  Enable::MICROMEGAS_CELL			= Enable::MICROMEGAS && true;
  Enable::MICROMEGAS_CLUSTER			= Enable::MICROMEGAS_CELL && true;
  Enable::MICROMEGAS_QA				= Enable::MICROMEGAS_CLUSTER && Enable::QA && true;

  Enable::TRACKING_TRACK			= (Enable::MICROMEGAS_CLUSTER
						   && Enable::TPC_CLUSTER
						   && Enable::INTT_CLUSTER
						   && Enable::MVTX_CLUSTER) && true;
  
  Enable::TRACKING_EVAL				= Enable::TRACKING_TRACK && false;
  Enable::TRACKING_QA				= Enable::TRACKING_TRACK && Enable::QA && true; // <-- this one

  //Additional tracking tools
  //Enable::TRACKING_DIAGNOSTICS		= Enable::TRACKING_TRACK && true;
  //G4TRACKING::filter_conversion_electrons	= true;      
  //  cemc electronics + thin layer of W-epoxy to get albedo from cemc
  //  into the tracking, cannot run together with CEMC
  //  Enable::CEMCALBEDO			= true;
  
  Enable::CEMC					= false;
  Enable::CEMC_ABSORBER				= true;
  Enable::CEMC_CELL				= Enable::CEMC && true;
  Enable::CEMC_TOWER				= Enable::CEMC_CELL && true;
  Enable::CEMC_CLUSTER				= Enable::CEMC_TOWER && true;
  Enable::CEMC_EVAL				= Enable::CEMC_CLUSTER && true;
  Enable::CEMC_QA				= Enable::CEMC_CLUSTER && Enable::QA && true;

  Enable::HCALIN				= false; //true;
  // Enable::HCALIN_ABSORBER			= true;
  // Enable::HCALIN_CELL			= Enable::HCALIN && true;
  // Enable::HCALIN_TOWER			= Enable::HCALIN_CELL && true;
  // Enable::HCALIN_CLUSTER			= Enable::HCALIN_TOWER && true;
  // Enable::HCALIN_EVAL			= Enable::HCALIN_CLUSTER && true;
  // Enable::HCALIN_QA				= Enable::HCALIN_CLUSTER && Enable::QA && true;

  Enable::MAGNET				= true;
  Enable::MAGNET_ABSORBER			= true;

  Enable::HCALOUT				= false; //true;
  // Enable::HCALOUT_ABSORBER			= true;
  // Enable::HCALOUT_CELL			= Enable::HCALOUT && true;
  // Enable::HCALOUT_TOWER			= Enable::HCALOUT_CELL && true;
  // Enable::HCALOUT_CLUSTER			= Enable::HCALOUT_TOWER && true;
  // Enable::HCALOUT_EVAL			= Enable::HCALOUT_CLUSTER && true;
  // Enable::HCALOUT_QA				= Enable::HCALOUT_CLUSTER && Enable::QA && true;

  //  Enable::EPD					= false; //true;
  // Enable::EPD_TILE				= Enable::EPD && true;

  //  Enable::BEAMLINE				= true;
  //  Enable::BEAMLINE_ABSORBER			= true;  // makes the beam line magnets sensitive volumes
  //  Enable::BEAMLINE_BLACKHOLE		= true; // turns the beamline magnets into black holes
  //  Enable::ZDC					= false; //true;
  //  Enable::ZDC_ABSORBER			= true;
  //  Enable::ZDC_SUPPORT			= true;
  // Enable::ZDC_TOWER				= Enable::ZDC && true;
  // Enable::ZDC_EVAL				= Enable::ZDC_TOWER && true;

  //! forward flux return plug door. Out of acceptance and off by default.
  //Enable::PLUGDOOR				= true;
  Enable::PLUGDOOR_ABSORBER			= true;

  //Enable::GLOBAL_RECO				= (Enable::MBDFAKE || Enable::TRACKING_TRACK) && true;
  //Enable::GLOBAL_FASTSIM			= true;

  //Enable::KFPARTICLE				= true;
  //Enable::KFPARTICLE_VERBOSITY		= 1;
  //Enable::KFPARTICLE_TRUTH_MATCH		= true;
  //Enable::KFPARTICLE_SAVE_NTUPLE		= true;

  // Enable::CALOTRIGGER				= Enable::CEMC_TOWER
  //   && Enable::HCALIN_TOWER
  //   && Enable::HCALOUT_TOWER
  //   && false;

  // Enable::JETS					= (Enable::GLOBAL_RECO || Enable::GLOBAL_FASTSIM) && false;
  // Enable::JETS_EVAL				= Enable::JETS && true;
  // Enable::JETS_QA				= Enable::JETS && Enable::QA && true;

  // HI Jet Reco for p+Au / Au+Au collisions (default is false for
  // single particle / p+p-only simulations, or for p+Au / Au+Au
  // simulations which don't particularly care about jets)
  // Enable::HIJETS				= Enable::JETS && Enable::CEMC_TOWER && Enable::HCALIN_TOWER && Enable::HCALOUT_TOWER && false;

  // // 3-D topoCluster reconstruction, potentially in all calorimeter layers
  // Enable::TOPOCLUSTER				= Enable::CEMC_TOWER
  //   && Enable::HCALIN_TOWER
  //   && Enable::HCALOUT_TOWER
  //   && false;
  
  // // particle flow jet reconstruction - needs topoClusters!
  // Enable::PARTICLEFLOW				= Enable::TOPOCLUSTER && false;
  // // centrality reconstruction
  // Enable::CENTRALITY				= false;

  // new settings using Enable namespace in GlobalVariables.C
  Enable::BLACKHOLE				= true;
  //Enable::BLACKHOLE_SAVEHITS			= false; // turn off saving of bh hits
  //Enable::BLACKHOLE_FORWARD_SAVEHITS		= false; // disable forward/backward hits
  //BlackHoleGeometry::visible			= true;

  //////////////////////////////
  // conditions DB flags      //
  //////////////////////////////
  Enable::CDB					= true;
  // global tag
  rc->set_StringFlag("CDB_GLOBALTAG",CDB::global_tag);
  // 64 bit timestamp
  rc->set_uint64Flag("TIMESTAMP",CDB::timestamp);
  
  /////////////////
  // Magnet Settings
  /////////////////
  G4MAGNET::magfield_rescale = 0.0;  // for zero field

  // Initialize the selected subsystems
  G4Init();

  ///////////////////////
  // GEANT4 Detector description
  ///////////////////////
  if (!Input::READHITS)
    {
      G4Setup();
    }

  ///////////////////////
  // Detector Division //
  ///////////////////////

  //  if ((Enable::MBD && Enable::MBDRECO) || Enable::MBDFAKE) Mbd_Reco();
  if (Enable::MVTX_CELL) Mvtx_Cells();
  if (Enable::INTT_CELL) Intt_Cells();
  if (Enable::TPC_CELL) TPC_Cells();
  if (Enable::MICROMEGAS_CELL) Micromegas_Cells();
  if (Enable::CEMC_CELL) CEMC_Cells();
  if (Enable::HCALIN_CELL) HCALInner_Cells();
  if (Enable::HCALOUT_CELL) HCALOuter_Cells();

  //////////////////////////////////
  // CEMC towering and clustering //
  //////////////////////////////////  
  if (Enable::CEMC_TOWER) CEMC_Towers();
  if (Enable::CEMC_CLUSTER) CEMC_Clusters();
  
  /////////////////
  // SVTX tracking
  //////////////--
  if(Enable::TRACKING_TRACK)
    {
      TrackingInit();
    }
  if (Enable::MVTX_CLUSTER) Mvtx_Clustering();
  if (Enable::INTT_CLUSTER) Intt_Clustering();
  if (Enable::TPC_CLUSTER)
    {
      if(G4TPC::ENABLE_DIRECT_LASER_HITS || G4TPC::ENABLE_CENTRAL_MEMBRANE_HITS)
	{
	  TPC_LaserClustering();
	}
      else
	{
	  TPC_Clustering();
	}
    }
  if (Enable::MICROMEGAS_CLUSTER) Micromegas_Clustering();

  if (Enable::TRACKING_TRACK)
    {
      Tracking_Reco();
    }

  //////////////////////
  // Simulation evaluation
  //////////////////////
  string outputroot = outputFile;
  string remove_this = ".root";
  size_t pos = outputroot.find(remove_this);
  if (pos != string::npos)
    {
      outputroot.erase(pos, remove_this.length());
    }
  
  if (Enable::TRACKING_EVAL) Tracking_Eval(outputroot + "_g4svtx_eval.root");

  tutorial* analysis_module = new tutorial( "name" );
  string output_path = "tutorial_results_";
  if( Input::PYTHIA8 )
    output_path += "pythia8_MC.root";
  else 
    output_path += "GUN_MC.root";

  analysis_module->SetOutputPath( output_path );
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
