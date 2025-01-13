# Sample 4
This analysis module demonstrates analysis for INTT clusters.

## Input & Output

### Input
This analysis module requires
- TrkrClusterv4 node
- ActsGeometry node
- EventHeaderv1 node

### Output
2 Trees are written into a ROOT file.
The output name is "tutorial_sample4.root" by default.
Users can change the name with tutorial::SetOutPutPath( string path )

#### Event-base TTree
name: tree_event
Branches
- run: run number (int)
- event: event ID in this run (int)
- cluster_num: the number of INTT clusters in this event (int)
- cluster_num_layer: the number of clusters on each INTT layer (int [4], 0: inner of inner barrel, 1: outer of inner, 2: inner of outer, 3: outer of outer)

#### Cluster-base TTree
name: tree_cluster
Branches
- run: run number (int)
- event: event ID in this run (int)
- position: Position of this cluster in the sPHENIX lab-frame (cm) (float [3], 0: x, 1: y, 2: z)
- layer: INTT layer of this cluster (int, 0-3 )
- adc: ADC of this cluster (float, It's actually so-called DAC value)
- size_phi: cluster size in phi direction (float)
- phi: phi position of this cluster (radian) (float, homework)
- theta: theta position of this cluster (radian) (float, homework)
- eta: pseudorapidity of this cluster (float, homework)

