#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "class_data.h"
#include "class_objective.h"
#include "class_model.h"
#include "class_arguments.h"
#include "class_factory.h"
#include "helper_functions.h"
#include <cmath>
#include <algorithm>

Objective::Objective(Arguments* args) : factory(new Factory(args)) {	
	this->args = args;
	this->n_nodes = args->getMaxChildren();
	for (int i = 0; i < n_nodes; ++i) {
		this->node_obj_values.push_back(0);
		this->models.push_back(this->factory->createModel());
	}
}

ObjectiveSSE::ObjectiveSSE(Arguments* args) : Objective(args) {
}

double ObjectiveSSE::compute(Data* data, Model* mod, std::vector<int> observations) {
	int n = observations.size();
	double cumsum = 0;
	for (int i = 0; i < n; ++i) {
		cumsum += pow((data->elem(observations[i], data->getTargetIndex()) - mod->predictSingle(data, observations[i])), 2);
	}
	return cumsum;
}

void ObjectiveSSE::update(Data* data, Split* split_upd, Split* split_prev) {

	SplitDifference split_diff;
	split_diff.computeSplitDifference(split_upd, split_prev);
	int n_nodes = split_upd->split_obs.size();

	for (int j = 0; j < n_nodes; j++) {
		if (!split_diff.additional_obs[j].empty()) {
			double value = 0;
			for (int i = 0; i < split_diff.additional_obs[j].size(); ++i) {
				this->models[j]->update(data, split_diff.additional_obs[j][i], '+');
				value = pow(
					((data->elem(
						split_diff.additional_obs[j][i], data->getTargetIndex())) - (this->models[j]->predictSingle(data, split_diff.additional_obs[j][i]))), 2);
				if (std::isnan(value)) {
					value = 0;
				}
				this->node_obj_values[j] += value;
			}
		}
		if (!split_diff.removed_obs[j].empty()) {
			double value = 0;
			for (int i = 0; i < split_diff.removed_obs[j].size(); ++i) {
				this->models[j]->update(data, split_diff.removed_obs[j][i], '-');
				value = pow(
					((data->elem(
						split_diff.removed_obs[j][i], data->getTargetIndex())) - (this->models[j]->predictSingle(data, split_diff.removed_obs[j][i]))), 2);
				if (std::isnan(value)) {
					value = 0;
				}
				this->node_obj_values[j] -= value;
			}
		}
	}
}


// ObjectiveGini

ObjectiveGini::ObjectiveGini(Arguments* args) : Objective(args) {
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

void ObjectiveGini::update(Data* data, Split* split_upd, Split* split_prev) {
	/*
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
	*/
}
