#ifndef OBJECTIVE_H
#define OBJECTIVE_H


#include <vector>

class Objective {
  public:
    Objective();
    virtual double compute(std::vector<double> target, std::vector<double> predictions) = 0;
};

class ObjectiveSSE: public Objective {
	public:
		ObjectiveSSE();
		double compute(std::vector<double> target, std::vector<double> predictions);
};


#endif 
