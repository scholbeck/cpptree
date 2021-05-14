#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include "class_arguments.h"
#include "class_factory.h"
#include "class_objective.h"
#include "class_model.h"
#include "class_optimizer.h"

Factory::Factory(Arguments args) {
	this->args = args;
}

Objective* Factory::createObjective() {
	Objective* obj = nullptr;
	std::string arg = this->args.getObjective();
	if (arg == "sse") {
		obj = new ObjectiveSSE();
	}
	return obj;
}

Model* Factory::createModel() {
	Model* mod = nullptr;
	std::string arg = this->args.getModel();
	if (arg == "mean") {
		mod = new ModelAverage();
	}
	return mod;
}

Optimizer* Factory::createOptimizer() {

	Optimizer* optim = nullptr;
	std::string arg = this->args.getAlgorithm();
	if (arg == "exhaustive") {
		optim = new ExhaustiveSearch();
	}
	
	return optim;
}


