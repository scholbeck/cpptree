#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include "class_arguments.h"
#include "class_factory.h"
#include "class_objective.h"
#include "class_model.h"
#include "class_splitgenerator.h"

Factory::Factory(Arguments* args) {
	this->args = args;
}

Objective* Factory::createObjective() {
	Objective* obj;
	if (this->args->getObjective() == "sse") {
		obj = new ObjectiveSSE(this->args);
	} else if (this->args->getObjective() == "gini") {
		obj = new ObjectiveGini(this->args);
	}
	return obj;
}

Model* Factory::createModel() {
	Model* m = nullptr;
	if ((this->args->getModel() == "mean") && (this->args->getObjective() == "sse")) {
		m = new ModelAverage();
	} else if ((this->args->getModel() == "linear") && (this->args->getObjective() == "sse")) {
		//m = new ModelLinearRegression();
	} else if ((this->args->getModel() == "") && (this->args->getObjective() == "gini")) {}
	return m;
}

SplitGenerator* Factory::createSplitGenerator() {
	SplitGenerator* g;
	if ((this->args->getAlgorithm() == "exhaustive") && (this->args->getMaxChildren() == 2)) {
		g = new SplitGeneratorBinExh();
	} else if ((this->args->getAlgorithm() == "random") && (this->args->getMaxChildren() > 2)) {
		g = new SplitGeneratorMultRand();
	}
	return g;
}
