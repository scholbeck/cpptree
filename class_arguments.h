#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string>

class Arguments {
  
  public:
    Arguments();
    
    std::string filename;
    int max_children;
    int min_node_size;
    std::string algorithm;
    std::string objective;
    std::string model;
    
    std::string getFilename();
    void setFilename(std::string);
	
	int getMaxChildren();
	void setMaxChildren(int max_children);
	
	int getMinNodeSize();
	void setMinNodeSize(int min_node_size);
	
	std::string getAlgorithm();
	void setAlgorithm(std::string algo);
	
	std::string getObjective();
	void setObjective(std::string obj);
	
	std::string getModel();
	void setModel(std::string mod);
	
};


#endif 
