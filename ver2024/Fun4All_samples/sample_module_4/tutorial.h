// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef TUTORIAL_H
#define TUTORIAL_H

// Fun4All libraries
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllReturnCodes.h>

//#include <ffaobjects/FlagSavev1.h>
#include <ffaobjects/EventHeaderv1.h>

#include <phool/PHCompositeNode.h>
#include <phool/getClass.h>
#include <phool/recoConsts.h>

#include <trackbase/ActsGeometry.h>
#include <trackbase/InttDefs.h>
#include <trackbase/TrkrDefs.h>
#include <trackbase/TrkrClusterv4.h>
#include <trackbase/TrkrClusterContainerv4.h>

// std libraries
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>

// ROOT libraries
#include "TFile.h"
#include "TTree.h"


class PHCompositeNode;

class tutorial : public SubsysReco
{
 public:

  tutorial(const std::string &name = "tutorial");

  ~tutorial() override;

  /** Called during initialization.
      Typically this is where you can book histograms, and e.g.
      register them to Fun4AllServer (so they can be output to file
      using Fun4AllServer::dumpHistos() method).
   */
  int Init(PHCompositeNode *topNode) override;

  /** Called for first event when run number is known.
      Typically this is where you may want to fetch data from
      database, because you know the run number. A place
      to book histograms which have to know the run number.
   */
  int InitRun(PHCompositeNode *topNode) override;

  /** Called for each event.
      This is where you do the real work.
   */
  int process_event(PHCompositeNode *topNode) override;

  /// Clean up internals after each event.
  int ResetEvent(PHCompositeNode *topNode) override;

  /// Called at the end of each run.
  int EndRun(const int runnumber) override;

  /// Called at the end of all processing.
  int End(PHCompositeNode *topNode) override;

  /// Reset
  int Reset(PHCompositeNode * /*topNode*/) override;

  void Print(const std::string &what = "ALL") const override;

  //! You can set the name of the output file, otherwise it's tutorial_sample3.root
  void SetOutputPath( std::string path ){ output_path_ = path; };

 private:
  //! A function for the analysis of INTT clusters
  int cluster_analysis(PHCompositeNode *topNode, TrkrClusterContainerv4* node_cluster_map, ActsGeometry* node_acts );

  //! Reset function for cluster parameters
  int ResetClusterLoop();

  std::string output_path_ = "tutorial_sample4.root";
  TFile* output_;	//! I/O of output ROOT file
  TTree* tree_event_;	//! Tree for event informaiton
  TTree* tree_cluster_; //! Tree for cluster information

  //variables for tree_event_
  int run_num_ = 0;			//! run number
  int event_id_ = 0;			//! event number in this run
  int cluster_num_ = 0;			//! the number of clusters on INTT
  int cluster_num_layer_[4] = { 0 };	//! the number of clusters on each INTT layer (0-3)
  
  // variables for tree_cluster_
  float position_[3];			//! cluster position in the lab-frame in cm
  int layer_ = 0;			//! INTT layer ID for this cluster
  float adc_ = 0;			//! ADC of this cluster ( not 0, 1, .., 7 but DAC value)
  float size_phi_ = 0;			//! cluster size in phi direction
  float phi_ = 0;			//! phi position of this cluster (radian)
  float theta_ = 0;			//! theta position of this cluster (radian)
  float eta_ = 0;			//! pseudorapidity of this cluster;

};

#endif // TUTORIAL_H
