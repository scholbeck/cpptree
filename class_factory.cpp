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
	Objective* obj;
	if (this->args.getObjective() == "sse") {
		obj = new ObjectiveSSE();
	}
	/*
	} else if (this->args.getObjective() == "gini") {
		obj = new ObjectiveGini();
	}
	*/
	return obj;
}

Model* Factory::createModel() {
	Model* m;
	if (this->args.getModel() == "mean") {
		m = new ModelAverage();
	} else if (this->args.getModel() == "majorvote") {
		m = new ModelMajorityVote();
	}
	return m;
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

