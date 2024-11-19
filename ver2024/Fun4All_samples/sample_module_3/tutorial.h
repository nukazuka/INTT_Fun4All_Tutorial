// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef TUTORIAL_H
#define TUTORIAL_H

// Fun4All libraries
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/PHCompositeNode.h>
#include <phool/getClass.h>

#include <ffarawobjects/InttRawHit.h>
#include <ffarawobjects/InttRawHitv2.h>
#include <ffarawobjects/InttRawHitContainer.h>
#include <ffarawobjects/InttRawHitContainerv2.h>

#include <string>
#include <iostream>
#include <iomanip>

// ROOT libraries
#include <TFile.h>
#include <TH1D.h>
#include <TFile.h>

class PHCompositeNode;

class tutorial : public SubsysReco
{
 public:

  tutorial(const std::string &name = "tutorial");

  ~tutorial() override;

  //! Init function of this analysis module. ROOT file is opened. A histgram object is created.
  int Init(PHCompositeNode *topNode) override;

  int InitRun(PHCompositeNode *topNode) override;

  //! This function is executed in each event
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

  // private member variables and objects for output
  std::string output_path_ = "tutorial_sample3.root";
  TFile* output_; //! I/O of output ROOT file
  TH1D* hist_hit_num_; //! a histogram for the number of raw hit per event
  void PrintHitParameterHeader(); //! Printing a header line for raw hit parameters
};

#endif // TUTORIAL_H
