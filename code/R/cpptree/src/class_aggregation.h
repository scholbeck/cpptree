#ifndef AGGREGATION_H
#define AGGREGATION_H

#include <vector>

class Aggregation {
  
  public:
    Aggregation();
    ~Aggregation() {}
    virtual double compute(std::vector<double> childnode_obj) = 0;
};


class AggregationAdditive: public Aggregation {
	public:
		AggregationAdditive();
    double compute(std::vector<double> childnode_obj);
};
#endif 
