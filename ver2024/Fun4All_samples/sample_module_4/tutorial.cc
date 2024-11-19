#include "tutorial.h"

tutorial::tutorial(const std::string &name):
 SubsysReco(name)
{}

tutorial::~tutorial()
{}

int tutorial::Init(PHCompositeNode *topNode)
{

  ////////////////////////////////////////////////////////
  // Initialization of the member                       //
  ////////////////////////////////////////////////////////
  output_ = new TFile( output_path_.c_str(), "RECREATE" );

  // Preparation of event base tree
  tree_event_ = new TTree( "tree_event", "Event base TTree" );
  tree_event_->Branch( "run",         &run_num_,     "run_num/I" );
  tree_event_->Branch( "event",       &event_id_,    "event_id/I" );
  tree_event_->Branch( "cluster_num", &cluster_num_, "cluster_num/I" );

  // 0: inner layer of inner barrel
  // 1: outer layer of inner barrel
  // 2: inner layer of outer barrel
  // 3: outer layer of outer barrel
  tree_event_->Branch( "cluster_num_layer", &cluster_num_layer_, "cluster_num_layer_[4]/I" );

  // Preparation of cluster base tree
  tree_cluster_ = new TTree( "tree_cluster", "Cluster base TTree" );

  tree_cluster_->Branch( "run",      &run_num_,  "run_num/I" );
  tree_cluster_->Branch( "event",    &event_id_, "event_id/I" );
  tree_cluster_->Branch( "position", &position_, "position[3]/F" );
  tree_cluster_->Branch( "layer",    &layer_,    "layer/I" );
  tree_cluster_->Branch( "adc",      &adc_,      "adc/F" );
  tree_cluster_->Branch( "size_phi", &size_phi_, "size_phi_/F" );
  tree_cluster_->Branch( "phi",      &phi_,      "phi/F" );
  tree_cluster_->Branch( "theta",    &theta_,    "theta/F" );
  tree_cluster_->Branch( "eta",      &eta_,      "eta/F" );

  // Execute the reset function to assign initial value
  this->ResetEvent( topNode );
  return Fun4AllReturnCodes::EVENT_OK;
}

int tutorial::InitRun(PHCompositeNode *topNode)
{

  return Fun4AllReturnCodes::EVENT_OK;
}

int tutorial::process_event(PHCompositeNode *topNode)
{

  //////////////////////////////////////////////////////////////////////////////////////////
  // Get nodes                                                                            //
  //////////////////////////////////////////////////////////////////////////////////////////

  //------------------------------------------------//
  // Getting TrkrClusterContainer node              //
  // TRKR_CLUSTER node: Information of TrkrCluster  //
  //------------------------------------------------//
  std::string node_name_trkr_cluster = "TRKR_CLUSTER";
  TrkrClusterContainerv4* node_cluster_map =
    findNode::getClass<TrkrClusterContainerv4>(topNode, node_name_trkr_cluster);

  if(!node_cluster_map )
    {
      std::cerr << PHWHERE << node_name_trkr_cluster << " node is missing." << std::endl;
      return Fun4AllReturnCodes::ABORTEVENT;
    }

  //--------------------------------------------------------------//
  // Getting Acts node to assign (x, y, z) coordinate to clusters //
  // ActsGeometry node: for the global coordinate                 //
  //--------------------------------------------------------------//
  ActsGeometry *node_acts = findNode::getClass<ActsGeometry>(topNode, "ActsGeometry");
  if ( !node_acts )
    {
      std::cout << PHWHERE << "No ActsGeometry on node tree. Bailing." << std::endl;
      return Fun4AllReturnCodes::ABORTEVENT;
    }

  //---------------------------------------//
  // Getting EventHeader to get event info //
  //---------------------------------------//
  EventHeaderv1* node_event_header = findNode::getClass<EventHeaderv1>( topNode, "EventHeader" );
  if( !node_event_header )
    {
      std::cout << PHWHERE << "No EventHeader on node tree. Skip this event." << std::endl;
      return Fun4AllReturnCodes::ABORTEVENT;
    }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // Analysis                                                                             //
  //////////////////////////////////////////////////////////////////////////////////////////
  run_num_ = node_event_header->get_RunNumber();
  event_id_ = node_event_header->get_EvtSequence();

  // If user wants to see event infomation, do it
  if( this->Verbosity() > 0 )
    {
      std::cout << "Run " << run_num_ << "\t"
		<< "Event " << std::setw(10) << event_id_
		<< std::endl;
    }

  // analysis codes for INTT clusters are written in the function below  
  this->cluster_analysis( topNode, node_cluster_map, node_acts );

  // Fill event-base TTree at the end of event process
  tree_event_->Fill();
  return Fun4AllReturnCodes::EVENT_OK;
}

int tutorial::cluster_analysis(PHCompositeNode *topNode, TrkrClusterContainerv4* node_cluster_map, ActsGeometry* node_acts )
{


  // loop over all INTT layers (0: inner of inner barrel, 1: outer of inner, 2: inner of outer, 3: outer of outer)
  for (unsigned int inttlayer = 0; inttlayer < 4; inttlayer++)
    {

      // get clusters only on the INTT layer, and loop over them
      for (const auto &hitsetkey : node_cluster_map->getHitSetKeys(TrkrDefs::TrkrId::inttId, inttlayer + 3) )
	{

	  // #cluster counters
	  cluster_num_++; // all of them
	  cluster_num_layer_[ inttlayer ]++; // for each layer
	  
	  // type: std::pair<ConstIterator, ConstIterator> ConstRange
	  // here, MMap_::const_iterator ConstIterator;
	  auto range = node_cluster_map->getClusters(hitsetkey);

	  // loop over iterators of this cluster
	  for (auto clusIter = range.first; clusIter != range.second; ++clusIter)
	    {
	      
	      const auto cluskey = clusIter->first;
	      const auto cluster = clusIter->second;

	      // Get cluster position in lab-coordinate using Acts
	      const auto globalPos = node_acts->getGlobalPosition(cluskey, cluster);

	      // Set cluster position in lab-coordinate to this cluster object
	      cluster->setPosition(0,  globalPos.x() );
	      cluster->setPosition(1,  globalPos.y() );
	      cluster->setPosition(2,  globalPos.z() );

	      // Assign cluster parameters
	      position_[0] = cluster->getPosition( 0 ); // x
	      position_[1] = cluster->getPosition( 1 ); // y
	      position_[2] = cluster->getPosition( 2 ); // z
	      adc_ = cluster->getAdc();
	      size_phi_ = cluster->getPhiSize();

	      /** @TODO Calculate phi, theta, eta (pseudorapidity) by yourself		 
		phi_ = 0; // (radian)
		theta_ = 0; // (radian)
		eta = 0; // pseudorapidity
	       */
	      
	      // After getting all cluster parameters, let's fill them
	      tree_cluster_->Fill();

	      // Then, reset the parameters (it's not mandatory if all parameters are available all the time. It's just in case)
	      this->ResetClusterLoop();

	      // If user wants to see cluster information, do it
	      if( this->Verbosity() > 1 )
		{
		  // All Get functions of TrkrCluster are here though some are commented out
		  std::cout 
		    << std::setw(6) << std::setprecision(3) << cluster->getPosition(0) << " " // ; // float 
		    << std::setw(6) << std::setprecision(3) << cluster->getPosition(1) << " " // ; // float 
		    << std::setw(6) << std::setprecision(3) << cluster->getPosition(2) << " " // ; // float 
		    << std::setw(5) << std::setprecision(5) << cluster->getAdc() << " " // ; // unsigned int 
		    // << std::setw(5) << std::setprecision(5) << cluster->getMaxAdc() << " " // ; // unsigned int 

		    //<< std::setw(5) << std::setprecision(5) << cluster->getSize() << " " // ; // char, phi size * z size is returned
		    << std::setw(3) << std::setprecision(3) << cluster->getPhiSize() << " " // ; // float 
		    //<< std::setw(2) << std::setprecision(2) << cluster->getZSize() << " " // ; // float 
		    << std::endl;
		  // cluster->getPosition( 0 ) ; // float , argument can be 0 or 1, not so useful
		  // cluster->getLocalX() ; // float 
		  // cluster->getLocalY() ; // float 
		  // cluster->getSubSurfKey() ; // TrkrDefs::subsurfkey 
		  // cluster->getOverlap() ; // char 
		  // cluster->getEdge() ; // char
		  
		} // End of if( this->Verbosity() > 1 )
	    } // End of for (auto clusIter = range.first; clusIter != range.second; ++clusIter)
	} // End of for (const auto &hitsetkey : node_cluster_map->getHitSetKeys(TrkrDefs::TrkrId::inttId, inttlayer + 3) )
    } // End of for (unsigned int inttlayer = 0; inttlayer < 4; inttlayer++)

  if( this->Verbosity() > 0 )
    {
      std::cout << "Cluster num: " << std::setw(5) << cluster_num_ << std::endl;
    }
  
  return Fun4AllReturnCodes::EVENT_OK;
}

int tutorial::ResetEvent(PHCompositeNode *topNode)
{

  this->ResetClusterLoop(); // Resetting cluster parameters

  run_num_ = event_id_
    = cluster_num_ 
    = cluster_num_layer_[ 0 ] = cluster_num_layer_[ 1 ]
    = cluster_num_layer_[ 2 ] = cluster_num_layer_[ 3 ]
    = 0;
  return Fun4AllReturnCodes::EVENT_OK;
}

int tutorial::EndRun(const int runnumber)
{

  return Fun4AllReturnCodes::EVENT_OK;
}

int tutorial::End(PHCompositeNode *topNode)
{
  // Save Trrees into the output file
  output_->WriteTObject( tree_event_, tree_event_->GetName() );
  output_->WriteTObject( tree_cluster_, tree_cluster_->GetName() );
  output_->Close();
  
  return Fun4AllReturnCodes::EVENT_OK;
}

int tutorial::Reset(PHCompositeNode *topNode)
{
    
  return Fun4AllReturnCodes::EVENT_OK;
}

int tutorial::ResetClusterLoop()
{
  // Parameters to be resetted in the cluster loop are resettted

  // init/reset variables
  position_[0] = position_[1] = position_[2]
    = phi_ = theta_ = eta_
    = -9999.9;
  adc_ = size_phi_ = 0;
  
  return Fun4AllReturnCodes::EVENT_OK;
}

void tutorial::Print(const std::string &what) const
{

}
