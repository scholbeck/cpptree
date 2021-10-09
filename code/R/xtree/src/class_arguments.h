#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "class_formula.h"

class Arguments {
  
  public:
    Arguments();
    
    std::string filename;
    int target;
    int max_children;
    int min_node_size;
	int max_depth;
    std::string algorithm;
    std::string objective;
    std::string model;
    std::string task;
	std::string print;
	std::vector<std::string> coltypes;
    char sep;
    Formula* formula;

    std::string getFilename();
    void setFilename(std::string);
	
	int getTargetIndex();
	void setTargetIndex(int target);
	
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
	
	std::string getTask();
	void setTask(std::string task);
	
	char getSep();
	void setSep(char sep);

	int getMaxDepth();
	void setMaxDepth(int max_depth);

	std::string getPrint();
	void setPrint(std::string print);

	std::vector<std::string> getColTypes();
	void setColTypes(std::string coltypes);

	Formula* getFormula();
	void setFormula(Formula* formula);

	int processArguments(int argc, char** argv);
	void checkArgs();
};


#endif 
