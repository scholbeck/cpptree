#ifndef OBJECTIVE_H
#define OBJECTIVE_H

#include "class_arguments.h"
#include "class_model.h"
#include <vector>

class Objective {
	// virtual class
	public:
    	Objective(Arguments args);
		std::vector<Model*> models;
		std::vector<double> values;
		// vectors contain value / object for each child node
		Arguments args;
    	virtual void init(Data* data, int childnode) = 0;
		virtual double compute(Data* data, Model* mod) = 0;
		virtual void update(Data* data, int childnode, std::array<std::vector<int>, 2> diff) = 0;
};

class ObjectiveSSE: public Objective {
	public:
		ObjectiveSSE(Arguments args);
		void init(Data* data, int childnode);
		double compute(Data* data, Model* mod);
		void update(Data* data, int childnode, std::array<std::vector<int>, 2> diff);
};

class ObjectiveGini: public Objective {
	public:
		std::vector<std::map<int, int>> class_counts; 	
		std::vector<int> n;
		ObjectiveGini(Arguments args);
		void init(Data* data, int childnode);
		double compute(Data* data, Model* mod);
		void update(Data* data, int childnode, std::array<std::vector<int>, 2> diff);
};

#endif 
