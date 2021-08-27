#ifndef AGGREGATION_H
#define AGGREGATION_H

#include "class_node.h"
#include "class_objective.h"


class Aggregation {
  
  public:
    Aggregation();
    virtual double compute(std::vector<double> childnode_obj) = 0;
};


class AggregationAdditive: public Aggregation {
	public:
		AggregationAdditive();
    double compute(std::vector<double> childnode_obj);
};
#endif 
