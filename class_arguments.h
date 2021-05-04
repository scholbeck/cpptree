#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <stdio.h>
#include <stdlib.h>

class Arguments {
  
  public:
    Arguments();
    
    std::string filename;
    ssize_t max_splits;
    ssize_t min_node_size;
    
    std::string getFilename();
    void setFilename(std::string);
	
	ssize_t getMaxSplits();
	void setMaxSplits(ssize_t max_splits);
	
	ssize_t getMinNodeSize();
	void setMinNodeSize(ssize_t min_node_size);
};


#endif 
