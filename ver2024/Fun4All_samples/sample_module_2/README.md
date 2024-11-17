# Sample 2
This analysis module just prints words on your terminal like:
```
[nukazuka@sphnx05 09:49:10 Fun4All_samples] $ root -q -b Fun4All_minimum_2.C ; echo $?

Processing Fun4All_minimum_2.C...
tutorial::tutorial(const std::string &name) Calling ctor
tutorial::Init(PHCompositeNode *topNode) Initializing
Fun4AllServer::setRun(): run 0 uses CDB TIMESTAMP 0
tutorial::InitRun(PHCompositeNode *topNode) Initializing for Run XXX
--------------------------------------

List of Nodes in Fun4AllServer:
Node Tree under TopNode TOP
TOP (PHCompositeNode)/
   DST (PHCompositeNode)/
      RUN (PHCompositeNode)/
         PAR (PHCompositeNode)/


tutorial::process_event(PHCompositeNode *topNode) Processing Event
tutorial::ResetEvent(PHCompositeNode *topNode) Resetting internal structures, prepare for next event
tutorial::EndRun(const int runnumber) Ending Run for Run 0
tutorial::End(PHCompositeNode *topNode) This is the End...
tutorial::Reset(PHCompositeNode *topNode) being Reset
tutorial::~tutorial() Calling dtor
0[nukazuka@sphnx05 09:49:10 Fun4All_samples] $ root -q -b Fun4All_minimum_2.C ; echo $?
```
