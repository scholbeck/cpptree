#ifndef OBJECTIVE_H
#define OBJECTIVE_H

#include "class_objective.h"
#include "class_data.h"

class Objective {
  
  public:
    Objective();
    double compute(Data* data, ssize_t target_index);
    
};

class ObjectiveMSE: public Objective {
	public:
		using Objective::Objective;
		using Objective::compute;
		virtual double compute(Data* data, ssize_t target_index);
};


#endif 
