#ifndef IMPROVEMENT_H
#define IMPROVEMENT_H

#include "class_node.h"
#include "class_objective.h"


class ObjImprovement {
  
  public:
    ObjImprovement();
    virtual double compute(std::vector<Node*> nodes, Objective* obj) = 0;
};


class ObjImprovementAdditive: public ObjImprovement {
	public:
		ObjImprovementAdditive();
		double compute(std::vector<Node*> nodes, Objective* obj);
};
#endif 
