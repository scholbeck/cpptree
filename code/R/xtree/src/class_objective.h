#ifndef OBJECTIVE_H
#define OBJECTIVE_H

#include "class_arguments.h"
#include "class_model.h"
#include <vector>

class Objective {
	// virtual class
	public:
    Objective(Arguments args);
	  virtual ~Objective() {}

		Arguments args;
		virtual double compute(Data* data, Model* mod, std::vector<int> observations) = 0;
		virtual double update(double prev_value, Data* data, Model* mod, std::array<std::vector<int>, 2> diff) = 0;
};

class ObjectiveSSE: public Objective {
	public:
		ObjectiveSSE(Arguments args);
		double compute(Data* data, Model* mod, std::vector<int> observations);
		double update(double prev_value, Data* data, Model* mod, std::array<std::vector<int>, 2> diff);
};

class ObjectiveGini: public Objective {
	public:
		std::map<int, int> class_counts;
		int n;

		ObjectiveGini(Arguments args);
		double compute(Data* data, Model* mod, std::vector<int> observations);
		double update(double prev_value, Data* data, Model* mod, std::array<std::vector<int>, 2> diff);
};

#endif 
