// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef TUTORIAL_H
#define TUTORIAL_H

#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/PHCompositeNode.h>
#include <phool/getClass.h> // for findNode::getClass
#include <ffarawobjects/InttRawHit.h>
#include <ffarawobjects/InttRawHitContainer.h>

#include <string>
#include <iostream> // for std::cout, cerr, endl
#include <iomanip> // for std::setw

#include <Rtypes.h> // for Long64_t
#include <TFile.h>
#include <TH1D.h> 
#include <TTree.h>

class PHCompositeNode;

class tutorial : public SubsysReco
{
private:
  std::string output_name_;
  TFile* output_;

  TH1D* hist_hit_num_;
  TTree* tree_;
  int pid_; // Packet ID, i.e. FELIX server ID
  Long64_t bco_full_; // Because bco_full is very large, int cannot keep all infomation. 
  int bco_;
  int diff_; // The well-used BCO difference: (The lower 7 bits of bco_full) - (hit BCO)
  
  
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

  /////////////////////////////////////////////////////////
  void SetOutputName( std::string name ){ output_name_ = name; };
  
};

#endif // TUTORIAL_H
