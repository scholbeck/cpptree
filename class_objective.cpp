#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "class_data.h"
#include "class_objective.h"
#include "class_model.h"
#include "class_arguments.h"
#include "helper_functions.h"
#include <cmath>
#include <algorithm>

Objective::Objective(Arguments args) {	
	for (int i = 0; i < args.getMaxChildren(); i++) {
		Model* m = new ModelAverage();
		this->models.push_back(m);
		this->values.push_back(0);
	}
	this->args = args;
}

// ObjectiveSSE
ObjectiveSSE::ObjectiveSSE(Arguments args) : Objective(args) {

}

void ObjectiveSSE::init(Data data, int childnode) {
	this->models[childnode]->setTrainingData(data);
	this->models[childnode]->train();
}

double ObjectiveSSE::compute(Data data) {
	std::vector<double> target_obs = data.col(data.getTargetIndex());
	int n = data.nrows();
	double mean_target = mean(target_obs);
	double cumsum = 0;
	for (int i = 0; i < n; i++) {
		cumsum += pow((data.elem(i, data.getTargetIndex()) - mean_target), 2);
	}
	return cumsum;
}

void ObjectiveSSE::update(Data data, int childnode, std::array<std::vector<int>, 2> diff) {
	if (!(diff[0].empty() && diff[1].empty())) {
		int n_setplus = diff[0].size();
		int n_setminus = diff[1].size();
		std::vector<double> observation;
		for (int i = 0; i < n_setplus; i++) {
			observation = data.selectObs(diff[0][i]);
			this->models[childnode]->update(observation, '+');
			this->values[childnode] += pow((observation[data.getTargetIndex()] - this->models[childnode]->predictSingle(observation)), 2);
		}
		for (int i = 0; i < n_setminus; i++) {
			observation = data.selectObs(diff[1][i]);
			this->models[childnode]->update(observation, '-');
			this->values[childnode] -= pow((observation[data.getTargetIndex()] - this->models[childnode]->predictSingle(observation)), 2);
		}
	}
}


/*
Objective::Objective() {
	
}

// ObjectiveSSE

ObjectiveSSE::ObjectiveSSE() {
	
}

double ObjectiveSSE::compute(Data data, std::vector<double> target_preds) {
	std::vector<double> target_obs = data.col(data.getTargetIndex());
	int n = data.nrows();
	double cumsum = 0;
	for (int i = 0; i < n; i++) {
		cumsum += pow((target_preds[i] - target_obs[i]), 2);
	}
	return cumsum;
}

void ObjectiveSSE::summary() {
	std::cout << "Objective: Sum of Squared Errors>\n" << "class <SSE>\n";
}

// ObjectiveGini

ObjectiveGini::ObjectiveGini() {
	
}

double ObjectiveGini::compute(Data data, std::vector<double> target_preds) {
	std::vector<double> target_obs = data.col(data.getTargetIndex());
	int n_obs = data.nrows();
	std::map<std::string, int> levels = data.getCategEncodings().at(data.getTargetIndex());
	double gini;
	int l, cnt;
	for (auto it = levels.begin(); it != levels.end(); ++it) {
		l = it->second;
		cnt = std::count(target_obs.begin(), target_obs.end(), l);
		gini += pow(((double) cnt / n_obs), 2);
	}
	gini = 1 - gini;
	return gini;
}

void ObjectiveGini::summary() {
	std::cout << "Objective: Gini Impurity>\n";
}
*/