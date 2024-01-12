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

//____________________________________________________________________________..
tutorial::tutorial(const std::string &name):
 SubsysReco(name)
{
  std::cout << "tutorial::tutorial(const std::string &name) Calling ctor" << std::endl;
}

//____________________________________________________________________________..
tutorial::~tutorial()
{
  std::cout << "tutorial::~tutorial() Calling dtor" << std::endl;
}

//____________________________________________________________________________..
int tutorial::Init(PHCompositeNode *topNode)
{
  std::cout << "tutorial::Init(PHCompositeNode *topNode) Initializing" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int tutorial::InitRun(PHCompositeNode *topNode)
{
  std::cout << "tutorial::InitRun(PHCompositeNode *topNode) Initializing for Run XXX" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int tutorial::process_event(PHCompositeNode *topNode)
{
  std::cout << "tutorial::process_event(PHCompositeNode *topNode) Processing Event" << std::endl;

  //////////////////////////////////////////////////////////////////////////////////////////
  // Getting Nodes                                                                        //
  //////////////////////////////////////////////////////////////////////////////////////////
  // event header node: Information of events, e.g. event seqence
  auto *node_event_header = 
    findNode::getClass<EventHeaderv1>(topNode, "EventHeader");

  if (!node_event_header )
    {
      std::cerr << PHWHERE << "Eventheader node is missging." << std::endl;
      return Fun4AllReturnCodes::ABORTEVENT;

    }
  
  // TRKR_CLUSTER node: Information of TrkrCluster
  auto *node_cluster_map = 
    findNode::getClass<TrkrClusterContainerv4>(topNode, "TRKR_CLUSTER");

  if (!node_cluster_map)
    {
      std::cerr << PHWHERE << "TrkrClusterContainer node is missing." << std::endl;
      return Fun4AllReturnCodes::ABORTEVENT;
    }

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

	  // getHitSetKeys -> using HitSetKeyList = std::vector<TrkrDefs::hitsetkey>;
	  // TrkrDefs::hitsetkey -> typedefuint32_t	  
	    //cout << "\tHit set key: " << hitsetkey << endl;
	  
	  auto range = node_cluster_map->getClusters(hitsetkey);
	  // type: std::pair<ConstIterator, ConstIterator> ConstRange
	  // here, MMap::const_iterator ConstIterator;
      
	  // loop over iterators of this cluster
	  for (auto clusIter = range.first; clusIter != range.second; ++clusIter)
	    {
	      const auto cluskey = clusIter->first;
	      const auto cluster = clusIter->second;
	      clusters.push_back( cluster );

	      /*
	      const auto globalPos = m_tGeometry->getGlobalPosition(cluskey, cluster);
	
	      int ladder_z   = InttDefs::getLadderZId(cluskey);
	      int ladder_phi = InttDefs::getLadderPhiId(cluskey);
	      int size       = cluster->getSize();

	      //	      if( nCluster < 5 )
	      if (Verbosity() > 5)
		{
		  cout << "xyz("
		       << setprecision(4) << setw(8) << globalPos.x() << ", "
		       << setprecision(4) << setw(8) << globalPos.y() << ", "
		       << setprecision(4) << setw(8) << globalPos.z()
		       << ") \t"
		       << "xyz("
		       << setprecision(4) << setw(8) << cluster->getPosition( 0 ) << ", "
		       << setprecision(4) << setw(8) << cluster->getPosition( 1 ) << ", "
		       << setprecision(4) << setw(8) << cluster->getPosition( 2 ) << ") "
		       << "local xy("
		       << setprecision(4) << setw(8) << cluster->getLocalX() << ", "
		       << setprecision(4) << setw(8) << cluster->getLocalY() << ")\t "
		    
		       << cluster->getAdc() << " "
		       << size << " "
		       << inttlayer << " "
		       << ladder_z << " "
		       << ladder_phi
		       << endl;
		}

		cluster->setPosition(0,  globalPos.x() );
		cluster->setPosition(1,  globalPos.y() );
		cluster->setPosition(2,  globalPos.z() );
	      */
	    }
	}
    }
  
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int tutorial::ResetEvent(PHCompositeNode *topNode)
{
  std::cout << "tutorial::ResetEvent(PHCompositeNode *topNode) Resetting internal structures, prepare for next event" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int tutorial::EndRun(const int runnumber)
{
  std::cout << "tutorial::EndRun(const int runnumber) Ending Run for Run " << runnumber << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int tutorial::End(PHCompositeNode *topNode)
{
  std::cout << "tutorial::End(PHCompositeNode *topNode) This is the End..." << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int tutorial::Reset(PHCompositeNode *topNode)
{
 std::cout << "tutorial::Reset(PHCompositeNode *topNode) being Reset" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
void tutorial::Print(const std::string &what) const
{
  std::cout << "tutorial::Print(const std::string &what) const Printing info for " << what << std::endl;
}
