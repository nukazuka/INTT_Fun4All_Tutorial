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
#include <trackbase/TrkrDefs.h>
#include <trackbase/TrkrClusterv4.h>
#include <trackbase/TrkrClusterContainerv4.h>

// MBD library
#include <mbd/MbdOutV2.h>

// std libraries
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>

// ROOT libraries
#include <TFile.h>
#include <TTree.h>
#include <TVector3.h>
#include <TMath.h>

class PHCompositeNode;

class tutorial : public SubsysReco
{
 public:

  tutorial(const std::string &name = "tutorial");

  ~tutorial() override;

  int Init(PHCompositeNode *topNode) override;

  int InitRun(PHCompositeNode *topNode) override;

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

  //! You can set the name of the output file, otherwise it's tutorial_sample4.root
  void SetOutputPath( std::string path ){ output_path_ = path; };

 private:
  //! A function for the analysis of INTT clusters
  int cluster_analysis(PHCompositeNode *topNode, TrkrClusterContainerv4* node_cluster_map, ActsGeometry* node_acts );
  int mbd_analysis(PHCompositeNode *topNode, MbdOut* mbdout_node );
  
  //! Reset function for cluster parameters
  int ResetClusterLoop();

  std::string output_path_ = "tutorial_sample5.root";
  TFile* output_;	//! I/O of output ROOT file
  TTree* tree_event_;	//! Tree for event informaiton
  TTree* tree_cluster_; //! Tree for cluster information

  //variables for tree_event_
  int run_num_ = 0;			//! run number
  int event_id_ = 0;			//! event number in this run
  int cluster_num_ = 0;			//! the number of clusters on INTT
  int cluster_num_layer_[4] = { 0 };	//! the number of clusters on each INTT layer (0-3)
  
  // variables for tree_cluster_
  TVector3 vec_cluster_;                //! position vector of a TrkrCluster
  float position_[3];			//! cluster position in the lab-frame in cm
  int layer_ = 0;			//! INTT layer ID for this cluster
  float adc_ = 0;			//! ADC of this cluster ( not 0, 1, .., 7 but DAC value)
  float size_phi_ = 0;			//! cluster size in phi direction
  float phi_ = 0;			//! phi position of this cluster (radian)
  float theta_ = 0;			//! theta position of this cluster (radian)
  float eta_ = 0;			//! pseudorapidity of this cluster;
  float eta_diff_ = 0;                  //! Difference of eta and corrected eta by MBD vertex

  // variables for MBDout
  TVector3 vec_mbd_;                    //! Position vector of a vertex reconstructed by MbdOut
  bool is_valid_mbd_out_ = false;       //! A flag to judge whether the vertex reconstruction was successful or not
  float position_mbd_[3] = { 0.0 };               //! x, y, z of MBD vertex for TTree
  float position_error_mbd_[3] = { 0.0 };         //! Error of x, y, z of MBD vertex for TTree 
  float mbd_q_part_[2] = { 0.0 };                  //! MBD charge, 0: South, 1: North
  float mbd_q_ = { 0.0 };                         //! Total MBD charg

};

#endif // TUTORIAL_H
