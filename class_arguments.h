#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string>

using lluint = long long unsigned int;

class Arguments {
  
  public:
    Arguments();
    
    std::string filename;
    lluint max_splits;
    lluint min_node_size;
    std::string algorithm;
    std::string objective;
    std::string model;
    
    std::string getFilename();
    void setFilename(std::string);
	
	lluint getMaxSplits();
	void setMaxSplits(lluint max_splits);
	
	lluint getMinNodeSize();
	void setMinNodeSize(lluint min_node_size);
	
	std::string getAlgorithm();
	void setAlgorithm(std::string algo);
	
	std::string getObjective();
	void setObjective(std::string obj);
	
	std::string getModel();
	void setModel(std::string mod);
	
};


#endif 
