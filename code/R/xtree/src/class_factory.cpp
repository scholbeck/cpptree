#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include "class_arguments.h"
#include "class_factory.h"
#include "class_objective.h"
#include "class_model.h"
#include "class_splitgenerator.h"
#include "class_splitter.h"

Factory::Factory(Data* data, Arguments* args) {
	this->data = data;
	this->args = args;
}

Objective* Factory::createObjective() {
	Objective* obj;
	if (this->args->getObjective() == "sse") {
		obj = new ObjectiveSSE(this->data, this->args);
	} else if (this->args->getObjective() == "gini") {
		obj = new ObjectiveGini(this->data, this->args);
	}
	return obj;
}

Model* Factory::createModel() {
	Model* m = nullptr;
	if ((this->args->getModel() == "mean") && (this->args->getObjective() == "sse")) {
		m = new ModelAverage();
	} else if ((this->args->getModel() == "linear") && (this->args->getObjective() == "sse")) {
		m = new ModelSingleFeatureLinReg(this->args->getFormula()->getFeatures()[0]);
	} else if ((this->args->getModel() == "") && (this->args->getObjective() == "gini")) {}
	return m;
}

SplitGeneratorBatch* Factory::createSplitGeneratorBatch() {
	SplitGeneratorBatch* g;
	if ((this->args->getAlgorithm() == "exhaustive") && (this->args->getMaxChildren() == 2)) {
		g = new SplitGeneratorBatchBinExh();
	} else if ((this->args->getAlgorithm() == "random") && (this->args->getMaxChildren() > 2)) {
		g = new SplitGeneratorBatchMultRand();
	}
	return g;
}

SplitGeneratorStream* Factory::createSplitGeneratorStream() {
	SplitGeneratorStream* g;
	if (this->args->getAlgorithm() == "bayesianoptim") {
		g = new SplitGeneratorStreamBayesianOptim();
	}
	return g;
}

Splitter* Factory::createSplitter() {
	Splitter* s;
	if ((this->args->getAlgorithm() == "exhaustive") || (this->args->getAlgorithm() == "random")) {
		s = new SplitterNaive();
	} else if (this->args->getAlgorithm() == "bayesianoptim") {
		s = new SplitterAdaptive();
	}
	return s;
}