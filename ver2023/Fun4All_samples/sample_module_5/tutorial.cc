//____________________________________________________________________________..
//
// This is a template for a Fun4All SubsysReco module with all methods from the
// $OFFLINE_MAIN/include/fun4all/SubsysReco.h baseclass
// You do not have to implement all of them, you can just remove unused methods
// here and in tutorial.h.
//
// tutorial(const std::string &name = "tutorial")
// everything is keyed to tutorial, duplicate names do work but it makes
// e.g. finding culprits in logs difficult or getting a pointer to the module
// from the command line
//
// tutorial::~tutorial()
// this is called when the Fun4AllServer is deleted at the end of running. Be
// mindful what you delete - you do loose ownership of object you put on the node tree
//
// int tutorial::Init(PHCompositeNode *topNode)
// This method is called when the module is registered with the Fun4AllServer. You
// can create historgrams here or put objects on the node tree but be aware that
// modules which haven't been registered yet did not put antyhing on the node tree
//
// int tutorial::InitRun(PHCompositeNode *topNode)
// This method is called when the first event is read (or generated). At
// this point the run number is known (which is mainly interesting for raw data
// processing). Also all objects are on the node tree in case your module's action
// depends on what else is around. Last chance to put nodes under the DST Node
// We mix events during readback if branches are added after the first event
//
// int tutorial::process_event(PHCompositeNode *topNode)
// called for every event. Return codes trigger actions, you find them in
// $OFFLINE_MAIN/include/fun4all/Fun4AllReturnCodes.h
//   everything is good:
//     return Fun4AllReturnCodes::EVENT_OK
//   abort event reconstruction, clear everything and process next event:
//     return Fun4AllReturnCodes::ABORT_EVENT; 
//   proceed but do not save this event in output (needs output manager setting):
//     return Fun4AllReturnCodes::DISCARD_EVENT; 
//   abort processing:
//     return Fun4AllReturnCodes::ABORT_RUN
// all other integers will lead to an error and abort of processing
//
// int tutorial::ResetEvent(PHCompositeNode *topNode)
// If you have internal data structures (arrays, stl containers) which needs clearing
// after each event, this is the place to do that. The nodes under the DST node are cleared
// by the framework
//
// int tutorial::EndRun(const int runnumber)
// This method is called at the end of a run when an event from a new run is
// encountered. Useful when analyzing multiple runs (raw data). Also called at
// the end of processing (before the End() method)
//
// int tutorial::End(PHCompositeNode *topNode)
// This is called at the end of processing. It needs to be called by the macro
// by Fun4AllServer::End(), so do not forget this in your macro
//
// int tutorial::Reset(PHCompositeNode *topNode)
// not really used - it is called before the dtor is called
//
// void tutorial::Print(const std::string &what) const
// Called from the command line - useful to print information when you need it
//
//____________________________________________________________________________..

#include "tutorial.h"
using namespace std;

//____________________________________________________________________________..
tutorial::tutorial(const std::string &name):
 SubsysReco(name)
{
  output_name_ = "output.root";
}

//____________________________________________________________________________..
tutorial::~tutorial()
{

}

//____________________________________________________________________________..
int tutorial::Init(PHCompositeNode *topNode)
{

  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int tutorial::InitRun(PHCompositeNode *topNode)
{


  //////////////////////////////////////////////////////////////////////////////////////////
  // Initialization of file output                                                        //
  //////////////////////////////////////////////////////////////////////////////////////////
  output_ = new TFile( output_name_.c_str(), "RECREATE" ) ;

  //////////////////////////////////////////////////////////////////////////////////////////
  // Initialization of histgrams                                                          //
  //////////////////////////////////////////////////////////////////////////////////////////
  // the number of hits in an event
  hist_hit_num_ = new TH1D( "hist_hit_num_",
			    "#INTTRAWHIT;The number of INTTRAWHIT;Entries",
			    1000, 0.0, 1e6 );
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // Initialization of TTrees (ntuples also)                                              //
  //////////////////////////////////////////////////////////////////////////////////////////
  tree_ = new TTree( "tree_INTT_raw_hit", "TTree for INTTRAWHIT information" );
  tree_->Branch( "pid", &pid_, "pid/I" );
  tree_->Branch( "bco_full", &bco_full_, "bco_full/L" );
  tree_->Branch( "bco", &bco_, "bco/I" );
  tree_->Branch( "diff", &diff_, "diff/I" );
  
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int tutorial::process_event(PHCompositeNode *topNode)
{


  //////////////////////////////////////////////////////////////////////////////////////////
  // Getting Nodes                                                                        //
  //////////////////////////////////////////////////////////////////////////////////////////
  string node_name = "INTTRAWHIT";
  auto *node_inttrawhit_map = 
    findNode::getClass<InttRawHitContainer>(topNode, node_name );

  if (!node_inttrawhit_map)
    {
      cerr << PHWHERE << node_name << " node is missing." << endl;
      return Fun4AllReturnCodes::ABORTEVENT;
    }

  //////////////////////////////////////////////////////////////////////////////////////////
  // Analysis                                                                             //
  //////////////////////////////////////////////////////////////////////////////////////////
  int hit_num = node_inttrawhit_map->get_nhits();
  cout << "#INTTRAWHIT: " << hit_num << endl;
  for( int i=0; i<hit_num; i++ )
    {
      InttRawHit* hit = node_inttrawhit_map->get_hit( i );

      // Get and assign hit information
      pid_ = hit->get_packetid();
      bco_full_ = hit->get_bco();
      bco_ = hit->get_FPHX_BCO();
      diff_ = abs( (bco_full_ & 0x7f) - bco_ );

      // Filling TTree
      tree_->Fill();

      if( false ) // If you want
	{
	  cout
	    << std::setw(  1 ) << hit->get_packetid() << " " // FELIX server
	    << std::setw( 12 ) << hit->get_bco() << " "      // so_called bco_full
	    // << std::setw( 10 ) << hit->get_word() << " "  // the data from FELIX
	    << std::setw(  2 ) << hit->get_fee() << " "      // module or felix ch
	    << std::setw(  2 ) << hit->get_chip_id() << " "  // chip ID
	    << std::setw(  3 ) << hit->get_channel_id() << " " // channel ID
	    << std::setw(  1 ) << hit->get_adc() << " "      // ADC
	    << std::setw(  3 ) << hit->get_FPHX_BCO() << " " // bco (hit bco, from 0 - 127)
	    << std::setw(  3 ) << (hit->get_bco()&0x7f) - hit->get_FPHX_BCO() // the BCO difference
	    // << std::setw(  1 ) << hit->get_full_FPHX() << " " // ?
	    // << std::setw(  1 ) << hit->get_full_ROC() << " " // ?
	    // << std::setw(  2 ) << hit->get_amplitude() << " " // only for calibration
	    << endl;
	}
    }

  // Filling histogram
  hist_hit_num_->Fill( hit_num );


  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int tutorial::ResetEvent(PHCompositeNode *topNode)
{

  return Fun4AllReturnCodes::EVENT_OK;
}


//____________________________________________________________________________..
int tutorial::EndRun(const int runnumber)
{

  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int tutorial::End(PHCompositeNode *topNode)
{

  output_->WriteTObject( hist_hit_num_, hist_hit_num_->GetName() );
  output_->WriteTObject( tree_, tree_->GetName() );
  output_->Close();
  
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int tutorial::Reset(PHCompositeNode *topNode)
{

  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
void tutorial::Print(const std::string &what) const
{

}
