#ifndef SPLITTER_H
#define SPLITTER_H

#include <stdio.h>
#include <stdlib.h>
#include "class_split.h"
#include "class_arguments.h"

class Splitter {
  
  public:
    Splitter();
    virtual Split* findBestSplit(Data* data, std::vector<int> observations, std::string ID, Arguments* args, double prev_obj) = 0;
};

class SplitterNaive : public Splitter {
    // finds best split based on upfront generated splits
    public:
    
    SplitterNaive();
    Split* findBestSplit(Data* data, std::vector<int> observations, std::string ID, Arguments* args, double prev_obj);
};

class SplitterAdaptive : public Splitter {
    // finds best split through a feedback loop between split point generator and objective evaluation
    public:

    SplitterAdaptive();
    Split* findBestSplit(Data* data, std::vector<int> observations, std::string ID, Arguments* args, double prev_obj);
};



#endif 
