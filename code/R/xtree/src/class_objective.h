#ifndef OBJECTIVE_H
#define OBJECTIVE_H

#include "class_arguments.h"
#include "class_model.h"
#include "class_factory.h"
#include <vector>

class Factory;

class Objective {
	public:
    	Objective(Data* data, Arguments* args);
		virtual ~Objective() {}

		Arguments* args;
		Factory* factory;
		Data* data;
		std::vector<Model*> models;
		std::vector<double> node_obj_values;
		int n_nodes;

		// implemented by all inheriting classes
		virtual double compute(Model* mod, std::vector<int> observations) = 0;
		virtual void update(Split* split_upd, Split* split_prev) = 0;
		// overridden by some inheriting classes
		virtual std::vector<std::string> generateAggregateModelInfo();
		
		void freeInternalMemory();
};

class ObjectiveSSE: public Objective {
	public:
		ObjectiveSSE(Data* data, Arguments* args);
		double compute(Model* mod, std::vector<int> observations);
		void update(Split* split_upd, Split* split_prev);
};

class ObjectiveGini: public Objective {
	public:
		std::vector<std::map<int, int>> class_counts;
		std::vector<int> n_obs;

		ObjectiveGini(Data* data, Arguments* args);
		double compute(Model* mod, std::vector<int> observations);
		void update(Split* split_upd, Split* split_prev);

		std::vector<std::string> generateAggregateModelInfo() override;
		// overwrites parent class method
};



#endif 
