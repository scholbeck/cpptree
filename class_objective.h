#ifndef OBJECTIVE_H
#define OBJECTIVE_H

#include "class_data.h"
#include "class_model.h"

class Objective {
  public:
    Objective();
    virtual double evaluateModel(Data data, Model* model) = 0;
};

class ObjectiveSSE: public Objective {
	public:
		ObjectiveSSE();
		double evaluateModel(Data data, Model* model);
};


#endif 
