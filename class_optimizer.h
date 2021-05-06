#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "class_data.h"
#include "class_objective.h"
#include "class_split.h"


class Optimizer {
	
  public:
    Optimizer();
    
	Split optimize(Data* data, size_t max_children, Objective objective);
    
};

class OptimizerExhaustiveSearch: public Optimizer {
	public:
		using Optimizer::Optimizer;
		using Optimizer::optimize;
		virtual Split optimize(Data* data, size_t max_children, Objective objective);
};

class OptimizerRandomSearch: public Optimizer {
	public:
		using Optimizer::Optimizer;
		using Optimizer::optimize;
		virtual Split optimize(Data* data, size_t max_children, Objective objective);
};

#endif 
