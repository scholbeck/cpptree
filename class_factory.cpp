#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include "class_arguments.h"
#include "class_factory.h"
#include "class_objective.h"
#include "class_model.h"
#include "class_splitgenerator.h"

Factory::Factory(Arguments args) {
	this->args = args;
}

Objective* Factory::createObjective() {
	Objective* obj;
	if (this->args.getObjective() == "sse") {
		obj = new ObjectiveSSE(this->args);
	} else if (this->args.getObjective() == "gini") {
		obj = new ObjectiveGini(this->args);
	}
	return obj;
}

Model* Factory::createModel() {
	Model* m;
	if (this->args.getModel() == "mean") {
		m = new ModelAverage();
	}
	return m;
}

SplitGenerator* Factory::createSplitGenerator(Data* data, Arguments args) {
	SplitGenerator* g;
	if (this->args.getAlgorithm() == "exhaustive") {
		g = new SplitGeneratorBinExh(data, args);
	}
	return g;
}

/*
Optimizer* Factory::createOptimizer() {
	Optimizer* optim;
	Objective* obj;
	if (this->args.getAlgorithm() == "exhaustive") {
		optim = new OptimExhaustSearch();
	}
	if (this->args.getObjective() == "sse") {
		obj = new ObjectiveSSE();
	} else if (this->args.getObjective() == "gini") {
		obj = new ObjectiveGini();
	}
	optim->setObjective(obj);
	optim->setMinNodeSize(this->args.getMinNodeSize());
	optim->setMaxChildren(this->args.getMaxChildren());
	return optim;
}
*/

