#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "string"
#include "class_arguments.h"
#include "class_data.h"
#include "class_model.h"
#include "class_objective.h"
#include "class_split.h"


class Optimizer {
	
  public:
	Objective* obj;
	Data data;
	Model* mod;
	lluint min_node_size;
  
    Optimizer(); 
	virtual Split searchOptimum(Data data) = 0;
	
	Objective* getObjective();
	void setObjective(Objective* obj);
	
	Model* getModel();
	void setModel(Model* mod);
	
	lluint getMinNodeSize();
	void setMinNodeSize(lluint min_node_size);
	
	double evaluateObjective(Data data);
	
	bool checkNodeSize(std::vector<Data> split_data);
};

class ExhaustiveSearch: public Optimizer {
	public:
		ExhaustiveSearch();
		Split searchOptimum(Data data);
};


#endif 
