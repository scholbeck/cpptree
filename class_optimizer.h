#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "string"
#include "class_data.h"
#include "class_objective.h"
#include "class_split.h"


class Optimizer {
	
  public:
    Optimizer();
    
    Split searchOptimum(Data data, Objective* obj, Model* mod, std::string algo);
	Split exhaustiveSearch(Data data, Objective* obj, Model* mod);
};

#endif 
