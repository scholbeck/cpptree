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
	this->args = args;
}

ObjectiveSSE::ObjectiveSSE(Arguments args) : Objective(args) {
}

double ObjectiveSSE::compute(Data* data, Model* mod, std::vector<int> observations) {
	int n = observations.size();
	double cumsum = 0;
	for (int i = 0; i < n; ++i) {
		cumsum += pow((data->elem(observations[i], data->getTargetIndex()) - mod->predictSingle(data, observations[i])), 2);
	}
	return cumsum;
}

double ObjectiveSSE::update(double prev_value, Data* data, Model* mod, std::array<std::vector<int>, 2> diff) {
	double upd_value = prev_value;
	if (!(diff[0].empty() && diff[1].empty())) {
		for (int i = 0; i < diff[0].size(); ++i) {
			upd_value += pow((data->elem(diff[0][i], data->getTargetIndex())) - (mod->predictSingle(data, diff[0][i])), 2);
		}
		for (int i = 0; i < diff[1].size(); ++i) {
			upd_value -= pow((data->elem(diff[0][i], data->getTargetIndex())) - (mod->predictSingle(data, diff[1][i])), 2);
		}
	}
	return upd_value;
}


// ObjectiveGini

ObjectiveGini::ObjectiveGini(Arguments args) : Objective(args) {
}

double ObjectiveGini::compute(Data* data, Model* mod, std::vector<int> observations) {
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

double ObjectiveGini::update(double prev_value, Data* data, Model* mod, std::array<std::vector<int>, 2> diff) {
	std::map<std::string, int> levels = data->getCategEncodings().at(data->getTargetIndex());
	std::vector<double> observation;
	int n_setplus = diff[0].size();
	int n_setminus = diff[1].size();
	
	if (!diff[0].empty()) {
		for (int i = 0; i < n_setplus; ++i) {
			observation = data->row(diff[0][i]);
			this->n++;
			for (auto it = levels.begin(); it != levels.end(); ++it) {
				if (observation[data->getTargetIndex()] == it->second) {
					this->class_counts.at(it->second) += 1;
					break;
				}
			}
		}
	}
	if (!diff[1].empty()) {
		for (int i = 0; i < n_setminus; ++i) {						
			observation = data->row(diff[1][i]);
			this->n--;
			for (auto it = levels.begin(); it != levels.end(); ++it) {
				if (observation[data->getTargetIndex()] == it->second) {
					this->class_counts.at(it->second) -= 1;
					break;
				}
			}
		}
	}
		
	double upd_value = prev_value;
	int cnt;
	for (auto it = levels.begin(); it != levels.end(); ++it) {
		cnt = this->class_counts.at(it->second);
		upd_value += pow(((double) cnt), 2);
	}
	upd_value = 1 - (pow((1 / (double) this->n), 2) * upd_value);
	return upd_value;
}
