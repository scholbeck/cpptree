#ifndef SPLIT_H
#define SPLIT_H

#include <vector>
#include "class_data.h"

class Split {
  
  public:
    Split();
    
    ssize_t feature_index;
    ssize_t split_cnt;
    std::vector<size_t> splitpoints;
    
    ssize_t getSplitFeatureIndex();
	void setFeatureIndex(ssize_t feature_index);
    std::vector<size_t> getSplitpoints();
    void addSplitpoint(size_t splitpoint);
    void clearObject();
};



#endif 
