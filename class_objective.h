#ifndef OBJECTIVE_H
#define OBJECTIVE_H


#include <vector>

class Objective {
  public:
    Objective();
    virtual double compute(Data data, std::vector<double> predictions) = 0;
    virtual void summary() = 0;
};

class ObjectiveSSE: public Objective {
	public:
		ObjectiveSSE();
		double compute(Data data, std::vector<double> predictions);
		void summary();
};

class ObjectiveGini: public Objective {
	public:
		ObjectiveGini();
		double compute(Data data, std::vector<double> predictions);
		void summary();
};


#endif 
