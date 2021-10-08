#ifndef OBJECTIVE_H
#define OBJECTIVE_H

#include "class_arguments.h"
#include "class_model.h"
#include "class_factory.h"
#include <vector>

class Factory;

class Objective {
	// virtual class
	public:
    	Objective(Arguments* args);
		virtual ~Objective() {}

		Arguments* args;
		Factory* factory;
		std::vector<Model*> models;
		std::vector<double> node_obj_values;
		int n_nodes;
		std::array<std::vector<int>, 2> computeSplitDifference(Split* split_upd, Split* split_prev);
		virtual double compute(Data* data, Model* mod, std::vector<int> observations) = 0;
		virtual void update(Data* data, Split* split_upd, Split* split_prev) = 0;
};

class ObjectiveSSE: public Objective {
	public:
		ObjectiveSSE(Arguments* args);
		double compute(Data* data, Model* mod, std::vector<int> observations);
		void update(Data* data, Split* split_upd, Split* split_prev);
};

class ObjectiveGini: public Objective {
	public:
		std::map<int, int> class_counts;
		int n;

		ObjectiveGini(Arguments* args);
		double compute(Data* data, Model* mod, std::vector<int> observations);
		void update(Data* data, Split* split_upd, Split* split_prev);
};



#endif 
