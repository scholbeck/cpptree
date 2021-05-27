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
	int min_node_size;
	int max_children;
  
    Optimizer(); 
    virtual void summary() = 0;
	virtual Split searchOptimum(Data data, Arguments args, double node_obj_val) = 0;
	int getMinNodeSize();
	void setMinNodeSize(int min_node_size);
	int getMaxChildren();
	void setMaxChildren(int max_children);
	Objective* getObjective();
	void setObjective(Objective* obj);
	double evaluateObjective(Data data);
	bool checkNodeSize(std::vector<Data> split_data);
	Model* buildModel(Data data, Arguments args);
};

class OptimExhaustSearch: public Optimizer {
	public:
		OptimExhaustSearch();
		void summary();
		Split searchOptimum(Data data, Arguments args, double node_obj_val);
};


#endif 
