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

std::unique_ptr<Objective> Factory::createObjective() {
	std::unique_ptr<Objective> obj;
	if (this->args->getObjective() == "sse") {
		obj = std::unique_ptr<ObjectiveSSE>(new ObjectiveSSE(this->data, this->args));
	} else if (this->args->getObjective() == "gini") {
		obj = std::unique_ptr<ObjectiveGini>(new ObjectiveGini(this->data, this->args));
	}
	return obj;
}

std::unique_ptr<Model> Factory::createModel() {
	std::unique_ptr<Model> m;
	if ((this->args->getModel() == "mean") && (this->args->getObjective() == "sse")) {
		m = std::unique_ptr<ModelAverage>(new ModelAverage());
	} else if ((this->args->getModel() == "linear") && (this->args->getObjective() == "sse")) {
		m = std::unique_ptr<ModelSingleFeatureLinReg>(new ModelSingleFeatureLinReg(this->args->getFormula()->getFeatures()[0]));
	} else if ((this->args->getModel() == "") && (this->args->getObjective() == "gini")) {}
	return m;
}

std::unique_ptr<SplitGeneratorBatch> Factory::createSplitGeneratorBatch() {
	std::unique_ptr<SplitGeneratorBatch> g;
	if ((this->args->getAlgorithm() == "exhaustive") && (this->args->getMaxChildren() == 2)) {
		g = std::unique_ptr<SplitGeneratorBatchBinExh>(new SplitGeneratorBatchBinExh());
	} else if ((this->args->getAlgorithm() == "random") && (this->args->getMaxChildren() > 2)) {
		g = std::unique_ptr<SplitGeneratorBatchMultRand>(new SplitGeneratorBatchMultRand());
	}
	return g;
}

std::unique_ptr<SplitGeneratorStream> Factory::createSplitGeneratorStream() {
	std::unique_ptr<SplitGeneratorStream> g;
	if (this->args->getAlgorithm() == "bayesianoptim") {
		g = std::unique_ptr<SplitGeneratorStreamBayesianOptim>(new SplitGeneratorStreamBayesianOptim());
	}
	return g;
}

std::unique_ptr<Splitter> Factory::createSplitter() {
	std::unique_ptr<Splitter> s;
	if ((this->args->getAlgorithm() == "exhaustive") || (this->args->getAlgorithm() == "random")) {
		s = std::unique_ptr<SplitterNaive>(new SplitterNaive());
	} else if (this->args->getAlgorithm() == "bayesianoptim") {
		s = std::unique_ptr<SplitterAdaptive>(new SplitterAdaptive());
	}
	return s;
}