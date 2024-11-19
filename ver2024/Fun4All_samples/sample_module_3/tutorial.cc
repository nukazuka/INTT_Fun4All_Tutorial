#include "tutorial.h"

tutorial::tutorial(const std::string &name):
  SubsysReco(name),
  output_( nullptr ),
  hist_hit_num_( nullptr )
{
}

tutorial::~tutorial()
{
}

int tutorial::Init(PHCompositeNode *topNode)
{

  ////////////////////////////////////////////////////////
  // Initialization of the member                       //
  ////////////////////////////////////////////////////////
  output_ = new TFile( output_path_.c_str(), "RECREATE" );
  
  hist_hit_num_ = new TH1D( "hit_num", "Number of raw hit distribution;#Cluster;Entries",
			    10000, 0, 10000 );
  
  return Fun4AllReturnCodes::EVENT_OK;
}

int tutorial::InitRun(PHCompositeNode *topNode)
{

  return Fun4AllReturnCodes::EVENT_OK;
}

int tutorial::process_event(PHCompositeNode *topNode)
{

  //////////////////////////////////////////////////////////////////////////////////////////
  // Getting INTTRAWHIT node                                                              //
  //////////////////////////////////////////////////////////////////////////////////////////
  std::string node_name_inttrawhit = "INTTRAWHIT";
  InttRawHitContainer* node_inttrawhit_map_ =
    findNode::getClass<InttRawHitContainer>(topNode, node_name_inttrawhit);
  
  if (!node_inttrawhit_map_)
    {
      std::cerr << PHWHERE << node_name_inttrawhit << " node is missing." << std::endl;
      return Fun4AllReturnCodes::ABORTEVENT;
    }

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // Loop over raw hits                                                                  //
  //////////////////////////////////////////////////////////////////////////////////////////

  int raw_hit_num = node_inttrawhit_map_->get_nhits();
  hist_hit_num_->Fill( raw_hit_num );

  /// If an user wants to see detailed informaiton, print it out
  
  if( this->Verbosity() > 0 )
    {
      if( this->Verbosity() > 1 )
	{
	  std::cout << std::string( 100, '-' ) << std::endl;
	}
      
      std::cout << "#raw hit: " << std::setw(5) << raw_hit_num << std::endl;
    }
  
  for (unsigned int i = 0; i < raw_hit_num; i++)
    {
      InttRawHit* hit = node_inttrawhit_map_->get_hit(i);

      if( this->Verbosity() > 1 )
	{

	  if( i % 30 == 0 )
	    this->PrintHitParameterHeader();
	  
	  std::cout
	    << std::setw( 10 ) << hit->get_event_counter() << " " // uint32_t 
	    // << std::setw() << hit->get_word() << " " // uint32_t 
	    << std::setw(  5 ) << hit->get_packetid() << " " // int32_t 
	    << std::setw( 14 ) << hit->get_fee() << " " // uint16_t 
	    << std::setw(  4 ) << hit->get_chip_id() << " " // uint16_t 
	    << std::setw(  4 ) << hit->get_channel_id() << " " // uint16_t 
	    << std::setw(  3 ) << hit->get_adc() << " " // uint16_t 
	    << std::setw( 20 ) << hit->get_bco() << " " // uint64_t 
	    << std::setw(  8 ) << hit->get_FPHX_BCO() << " " // uint16_t 
	    // hit->get_full_FPHX(); // uint16_t, for low level debugging
	    // hit->get_full_ROC(); // uint16_t, for low level debugging
	    // hit->get_amplitude(); // uint16_t, for calibration data
	    << std::endl;
	}
    }
  return Fun4AllReturnCodes::EVENT_OK;
}

int tutorial::ResetEvent(PHCompositeNode *topNode)
{

  return Fun4AllReturnCodes::EVENT_OK;
}

int tutorial::EndRun(const int runnumber)
{

  return Fun4AllReturnCodes::EVENT_OK;
}

int tutorial::End(PHCompositeNode *topNode)
{

  ////////////////////////////////////////////////////////
  // Writing objects to the output file                 //
  ////////////////////////////////////////////////////////
  output_->WriteTObject( hist_hit_num_, hist_hit_num_->GetName() );
  output_->Close();

  return Fun4AllReturnCodes::EVENT_OK;
}

int tutorial::Reset(PHCompositeNode *topNode)
{

  return Fun4AllReturnCodes::EVENT_OK;
}

void tutorial::Print(const std::string &what) const
{
  std::cout << "tutorial::Print(const std::string &what) const Printing info for " << what << std::endl;
}

void tutorial::PrintHitParameterHeader()
{
  if( this->Verbosity() > 1)
    {
      std::cout
	<< std::setw( 10 ) << "Event cntr" << " " 	    
	<< std::setw(  5 ) << "PktID"<< " " 
	<< std::setw( 14 ) << "Half Ladder ID" << " " 
	<< std::setw(  4 ) << "Chip" << " " 
	<< std::setw(  4 ) << "Chan" << " " 
	<< std::setw(  3 ) << "ADC" << " " 
	<< std::setw( 20 ) << "GTM BCO" << " " 
	<< std::setw(  8 ) << "FPHX BCO" << " "
	<< std::endl;
    }
}
