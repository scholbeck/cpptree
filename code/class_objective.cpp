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
	for (int i = 0; i < args.getMaxChildren(); ++i) {
		Model* m = new ModelAverage();
		this->models.push_back(m);
		this->values.push_back(0);
	}
	this->args = args;
}

// ObjectiveSSE
ObjectiveSSE::ObjectiveSSE(Arguments args) : Objective(args) {

}

void ObjectiveSSE::init(Data* data, int childnode) {
	this->models[childnode]->setTrainingData(data);
	this->models[childnode]->train();
	this->values[childnode] = this->compute(data, this->models[childnode]);
}

double ObjectiveSSE::compute(Data* data, Model* mod) {
	std::vector<double> target_obs = data->col(data->getTargetIndex());
	int n = data->nrows();
	double mean_target = mean(target_obs);
	double cumsum = 0;
	double target, pred;
	std::vector<double> observation;
	for (int i = 0; i < n; ++i) {
		observation = data->row(i);
		pred = mod->predictSingle(observation);
		target = observation[data->getTargetIndex()];
		cumsum += pow((target - pred), 2);
	}
	return cumsum;
}

void ObjectiveSSE::update(Data* data, int childnode, std::array<std::vector<int>, 2> diff) {
	if (!(diff[0].empty() && diff[1].empty())) {
		int n_setplus = diff[0].size();
		int n_setminus = diff[1].size();
		std::vector<double> observation;
		for (int i = 0; i < n_setplus; ++i) {
			observation = data->row(diff[0][i]);
			this->models[childnode]->update(observation, '+');
			this->values[childnode] += pow((observation[data->getTargetIndex()] - this->models[childnode]->predictSingle(observation)), 2);
		}
		for (int i = 0; i < n_setminus; ++i) {
			observation = data->row(diff[1][i]);
			this->models[childnode]->update(observation, '-');
			this->values[childnode] -= pow((observation[data->getTargetIndex()] - this->models[childnode]->predictSingle(observation)), 2);
		}
	}
}


/*
ObjectiveSSE::ObjectiveSSE(Arguments args) : Objective(args) {

}

void ObjectiveSSE::init(Data* data, int childnode) {
	this->models[childnode]->setTrainingData(data);
	this->models[childnode]->train();
	this->values[childnode] = this->compute(data);
}

double ObjectiveSSE::compute(Data* data) {
	std::vector<double> target_obs = data->col(data->getTargetIndex());
	int n = data->nrows();
	double mean_target = mean(target_obs);
	double cumsum = 0;
	for (int i = 0; i < n; ++i) {
		cumsum += pow((data->elem(i, data->getTargetIndex()) - mean_target), 2);
	}
	return cumsum;
}

void ObjectiveSSE::update(Data* data, int childnode, std::array<std::vector<int>, 2> diff) {
	if (!(diff[0].empty() && diff[1].empty())) {
		int n_setplus = diff[0].size();
		int n_setminus = diff[1].size();
		std::vector<double> observation;
		for (int i = 0; i < n_setplus; ++i) {
			observation = data->row(diff[0][i]);
			this->models[childnode]->update(observation, '+');
			this->values[childnode] += pow((observation[data->getTargetIndex()] - this->models[childnode]->predictSingle(observation)), 2);
		}
		for (int i = 0; i < n_setminus; ++i) {
			observation = data->row(diff[1][i]);
			this->models[childnode]->update(observation, '-');
			this->values[childnode] -= pow((observation[data->getTargetIndex()] - this->models[childnode]->predictSingle(observation)), 2);
		}
	}
}
*/

// ObjectiveGini

ObjectiveGini::ObjectiveGini(Arguments args) : Objective(args) {
	this->values.resize(args.getMaxChildren());
	this->class_counts.resize(args.getMaxChildren());
	this->n.resize(args.getMaxChildren());
}

void ObjectiveGini::init(Data* data, int childnode) {
	std::map<std::string, int> levels = data->getCategEncodings().at(data->getTargetIndex());
	std::vector<double> target_obs = data->col(data->getTargetIndex());
	int cnt;
	double gini = 0;
	for (auto it = levels.begin(); it != levels.end(); ++it) {
		cnt = std::count(target_obs.begin(), target_obs.end(), it->second);
		this->class_counts[childnode].insert(std::pair<int, int>(it->second, cnt));
		gini += pow(((double) cnt), 2);
	}
	this->n[childnode] = target_obs.size();
	gini = 1 - (pow((1 / (double) this->n[childnode]), 2) * gini);
	this->values[childnode] = gini;
}

double ObjectiveGini::compute(Data* data, Model* mod) {
	std::vector<double> target_obs = data->col(data->getTargetIndex());
	int n_obs = data->nrows();
	std::map<std::string, int> levels = data->getCategEncodings().at(data->getTargetIndex());
	double gini;
	int l, cnt;
	for (auto it = levels.begin(); it != levels.end(); ++it) {
		l = it->second;
		cnt = std::count(target_obs.begin(), target_obs.end(), l);
		gini += pow(((double) cnt), 2);
	}
	gini = 1 - pow((1 / n_obs), 2) * gini;
	return gini;
}

void ObjectiveGini::update(Data* data, int childnode, std::array<std::vector<int>, 2> diff) {
	if (!(diff[0].empty() && diff[1].empty())) {
		std::map<std::string, int> levels = data->getCategEncodings().at(data->getTargetIndex());
		std::vector<double> observation;
		int n_setplus = diff[0].size();
		int n_setminus = diff[1].size();
		for (int i = 0; i < n_setplus; ++i) {
			observation = data->row(diff[0][i]);
			this->n[childnode] += 1;
			for (auto it = levels.begin(); it != levels.end(); ++it) {
				if (observation[data->getTargetIndex()] == it->second) {
					this->class_counts[childnode].at(it->second) += 1;
					break;
				}
			}
		}
		for (int i = 0; i < n_setminus; ++i) {						
			observation = data->row(diff[1][i]);
			this->n[childnode] -= 1;
			for (auto it = levels.begin(); it != levels.end(); ++it) {
				if (observation[data->getTargetIndex()] == it->second) {
					this->class_counts[childnode].at(it->second) -= 1;
					break;
				}
			}
		}
		int cnt;
		this->values[childnode] = 0;
		double gini = 0;
		for (auto it = levels.begin(); it != levels.end(); ++it) {
			cnt = this->class_counts[childnode].at(it->second);
			gini += pow(((double) cnt), 2);
		}
		gini = 1 - (pow((1 / (double) this->n[childnode]), 2) * gini);
		this->values[childnode] = gini;
	}
}
