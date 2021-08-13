#ifndef OBJECTIVE_H
#define OBJECTIVE_H

#include "class_arguments.h"
#include "class_model.h"
#include <vector>

class Objective {
	public:
    	Objective(Arguments args);
		std::vector<Model*> models;
		std::vector<double> values;
		Arguments args;
		// vectors contain value for each child node
    	virtual void init(Data data, int childnode) = 0;
		virtual double compute(Data data) = 0;
		virtual void update(Data data, int childnode, std::array<std::vector<int>, 2> diff) = 0;
};

class ObjectiveSSE: public Objective {
	public:
		ObjectiveSSE(Arguments args);
		void init(Data data, int childnode);
		double compute(Data data);
		void update(Data data, int childnode, std::array<std::vector<int>, 2> diff);
};

class ObjectiveGini: public Objective {
	public:
		ObjectiveGini();
		double compute(Data data);
		void update(Data data, int childnode, std::array<std::vector<int>, 2> diff);
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
