#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "class_data.h"
#include "class_objective.h"
#include "class_split.h"


class Optimizer {
	
  public:
    Optimizer();
    
    Split optimize(Data* data, Objective* obj, Model* mod);
	Split exhaustiveSearch(Data* data, Objective* obj, Model* mod);
};

#endif 
