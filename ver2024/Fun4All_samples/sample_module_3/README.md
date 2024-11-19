# Sample 3
It analyzes INTT raw hits

## Input & Output
### Input
InttRawHitv2

### Output
A ROOT file. If you don't set the output name in the Fun4All macro using tutorial::SetOutputPath( std::string path ), the name is "tutorial_sample3.root".
The contents of the output file is just a TH1D histogram for the number of INTT raw hits in an event:

```
[nukazuka@sphnx06 03:29:41 work_now] $ root tutorial_sample3.root
root [0]
Attaching file tutorial_sample3.root as _file0...
(TFile *) 0x1c375a0
root [1] .ls
TFile**        tutorial_sample3.root
 TFile*        tutorial_sample3.root
   KEY: TH1D    hit_num;1    Number of raw hit distribution
   root [2]
```

