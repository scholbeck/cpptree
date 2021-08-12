#ifndef OBJECTIVE_H
#define OBJECTIVE_H

#include "class_model.h"
#include <vector>

class Objective {
	public:
    	Objective();
		std::vector<Model*> models;
		std::vector<double> values;
		// vectors contain value for each child node
    	virtual double compute(Data data) = 0;
		virtual double update(Data data, int childnode, std::array<std::vector<int>, 2> diff) = 0;
};

class ObjectiveSSE: public Objective {
	public:
		ObjectiveSSE();
		double compute(Data data);
		double update(Data data, int childnode, std::array<std::vector<int>, 2> diff);
};

class ObjectiveGini: public Objective {
	public:
		ObjectiveGini();
		double compute(Data data);
		double update(Data data, int childnode, std::array<std::vector<int>, 2> diff);
};

/*
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
*/

#endif 
