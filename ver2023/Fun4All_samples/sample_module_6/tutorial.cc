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
  SubsysReco(name),
  output_( nullptr ),
  hist_cluster_( nullptr )
{
}

//____________________________________________________________________________..
tutorial::~tutorial()
{
}

//____________________________________________________________________________..
int tutorial::Init(PHCompositeNode *topNode)
{

  ////////////////////////////////////////////////////////
  // Initialization of the member                       //
  ////////////////////////////////////////////////////////
  output_ = new TFile( output_path_.c_str(), "RECREATE" );
  
  hist_cluster_ = new TH1D( "hist_cluster", "Number of cluster distribution;#Cluster;Entries", 100, 0, 100 );
  

  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int tutorial::InitRun(PHCompositeNode *topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int tutorial::process_event(PHCompositeNode *topNode)
{

  //////////////////////////////////////////////////////////////////////////////////////////
  // Getting Nodes                                                                        //
  //////////////////////////////////////////////////////////////////////////////////////////  
  string node_name_inttrawhit = "INTTRAWHIT";
  auto *node_inttrawhit_map = 
    findNode::getClass<InttRawHitContainer>(topNode, node_name_inttrawhit );

  if (!node_inttrawhit_map)
    {
      cerr << PHWHERE << node_name_inttrawhit << " node is missing." << endl;
      return Fun4AllReturnCodes::ABORTEVENT;
    }

  // TRKR_CLUSTER node: Information of TrkrCluster
  string node_name_trkr_cluster = "TRKR_CLUSTER";
  auto *node_cluster_map = 
    findNode::getClass<TrkrClusterContainerv4>(topNode, node_name_trkr_cluster);

  if (!node_cluster_map)
    {
      std::cerr << PHWHERE << node_name_trkr_cluster << " node is missing." << std::endl;
      return Fun4AllReturnCodes::ABORTEVENT;
    }

  // ActsGeometry node: for the global coordinate
  string node_name_acts = "ActsGeometry";
  ActsGeometry *node_acts = findNode::getClass<ActsGeometry>(topNode, node_name_acts);
  if ( !node_acts )
    {
      std::cout << PHWHERE << node_name_acts << " node is missging." << std::endl;
      return Fun4AllReturnCodes::ABORTEVENT;
    }

  cout << "#" << node_name_inttrawhit << " " << node_inttrawhit_map->get_nhits() << "\t"
       << "#" << node_name_trkr_cluster << " " << node_cluster_map->size()
       << endl;
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // Loop over TrkrClusters                                                               //
  //////////////////////////////////////////////////////////////////////////////////////////

  std::vector < TrkrCluster* > clusters;
  for (unsigned int inttlayer = 0; inttlayer < 4; inttlayer++)
    {
      //      cout << " INTT layer " << inttlayer << endl;
      //      int layer= ( inttlayer < 2 ? 0 : 1 );

      // loop over all hits
      for (const auto &hitsetkey : node_cluster_map->getHitSetKeys(TrkrDefs::TrkrId::inttId, inttlayer + 3) )
	{

	  // type: std::pair<ConstIterator, ConstIterator> ConstRange
	  // here, MMap::const_iterator ConstIterator;
	  auto range = node_cluster_map->getClusters(hitsetkey);
      
	  // loop over iterators of this cluster
	  for (auto clusIter = range.first; clusIter != range.second; ++clusIter)
	    {
	      const auto cluskey = clusIter->first;
	      const auto cluster = clusIter->second;
	      clusters.push_back( cluster );

	      const auto globalPos = node_acts->getGlobalPosition(cluskey, cluster);
	
	      // int ladder_z   = InttDefs::getLadderZId(cluskey);
	      // int ladder_phi = InttDefs::getLadderPhiId(cluskey);
	      int size       = cluster->getSize();

	      //	      if( nCluster < 5 )
	      if (Verbosity() > 5)
		{
		  std::cout
		    // << "xyz("
		    //    << std::setprecision(4) << std::setw(8) << globalPos.x() << ", "
		    //    << std::setprecision(4) << std::setw(8) << globalPos.y() << ", "
		    //    << std::setprecision(4) << std::setw(8) << globalPos.z()
		    //    << ") \t"
		       << "xyz("
		       << std::setprecision(4) << std::setw(8) << cluster->getPosition( 0 ) << ", "
		       << std::setprecision(4) << std::setw(8) << cluster->getPosition( 1 ) << ", "
		       << std::setprecision(4) << std::setw(8) << cluster->getPosition( 2 ) << ") "
		       << "local xy("
		       << std::setprecision(4) << std::setw(8) << cluster->getLocalX() << ", "
		       << std::setprecision(4) << std::setw(8) << cluster->getLocalY() << ")\t "
		    
		       << cluster->getAdc() << " "
		       << size << " "
		       << inttlayer << " "
		       // << ladder_z << " "
		       // << ladder_phi
		       << std::endl;
		}

		cluster->setPosition(0,  globalPos.x() );
		cluster->setPosition(1,  globalPos.y() );
		cluster->setPosition(2,  globalPos.z() );
	    }
	}
    }

  std::cout << clusters.size() << " clusters in this event" << std::endl;
  hist_cluster_->Fill( clusters.size() );
  
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
  ////////////////////////////////////////////////////////
  // Writing objects to the output file                 //
  ////////////////////////////////////////////////////////
  output_->WriteTObject( hist_cluster_, hist_cluster_->GetName() );
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
