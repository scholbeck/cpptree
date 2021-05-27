#ifndef FACTORY_H
#define FACTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "class_data.h"
#include "class_arguments.h"
#include "class_objective.h"
#include "class_model.h"
#include "class_optimizer.h"


class Factory {
  
  public:
    Factory(Arguments args);
    
    Arguments args;

    Objective* createObjective();
	Model* createModel();    
    Optimizer* createOptimizer();

};


#endif 
