#ifndef OBJECTIVE_H
#define OBJECTIVE_H

#include "class_objective.h"
#include "class_data.h"
#include "class_model.h"

class Objective {
  
  public:
    Objective();
    double compute(Data* data, Model model);
    
};

class ObjectiveSSE: public Objective {
	public:
		using Objective::Objective;
		using Objective::compute;
		virtual double compute(Data* data, Model model);
};


#endif 
