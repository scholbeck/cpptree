#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "class_data.h"
#include "class_split.h"
#include "class_objective.h"
#include "class_model.h"
#include "class_arguments.h"
#include "class_factory.h"
#include "class_splitdifference.h"
#include "helper_functions.h"
#include <cmath>
#include <algorithm>

Objective::Objective(Data* data, Arguments* args) {	
	this->args = args;
	this->data = data;
	this->n_nodes = args->getMaxChildren();
	Factory factory = Factory(data, args);
	for (int i = 0; i < n_nodes; ++i) {
		this->node_obj_values.push_back(0);
		this->models.push_back(factory.createModel());
	}
}

std::vector<std::string> Objective::generateAggregateModelInfo() {
	std::vector<std::string> aggr_model_info(this->n_nodes);
	for (int i = 0; i < this->n_nodes; ++i) {
		aggr_model_info[i] = this->models[i]->generateModelInfo();
		// model info is only generated upon request (i.e., if a better split than the previous one is found)
	}
	return aggr_model_info;
}

// OBJECTIVE SSE
ObjectiveSSE::ObjectiveSSE(Data* data, Arguments* args) : Objective(data, args) {}

double ObjectiveSSE::compute(Model* mod, std::vector<int> observations) {
	int n = observations.size();
	double cumsum = 0;
	for (int i = 0; i < n; ++i) {
		cumsum += pow(((this->data->elem(observations[i], this->data->getTargetIndex())) - (mod->predictSingle(data, observations[i]))), 2);
	}
	return cumsum;
}

void ObjectiveSSE::update(Split* split_upd, Split* split_prev) {
	SplitDifference split_diff;
	split_diff.computeSplitDifference(split_upd, split_prev);
	int n_nodes = split_upd->split_obs.size();
	for (int j = 0; j < n_nodes; ++j) {
		if (!split_diff.additional_obs[j].empty()) {
			this->models[j]->update(data, split_diff.additional_obs[j], '+');
		}
		if (!split_diff.removed_obs[j].empty()) {
			this->models[j]->update(data, split_diff.removed_obs[j], '-');
		}
		double cumsum = 0;
		int n = split_upd->split_obs[j].size();
		for (int i = 0; i < n; ++i) {
			cumsum += pow(((this->data->elem(split_upd->split_obs[j][i], this->data->getTargetIndex())) - (this->models[j]->predictSingle(data, split_upd->split_obs[j][i]))), 2);
		}
		this->node_obj_values[j] = cumsum;
	}
}


// OBJECTIVE GINI
ObjectiveGini::ObjectiveGini(Data* data, Arguments* args) : Objective(data, args) {
	std::map<std::string, int> levels = this->data->getCategEncodings().at(this->data->getTargetIndex());
	std::map<int, int> empty_cnt;
	for (auto it = levels.begin(); it != levels.end(); ++it) {
		empty_cnt.insert(std::pair<int, int>(it->second, 0));
	}
	for (int i = 0; i < this->n_nodes; ++i) {
		this->class_counts.push_back(empty_cnt);
		this->n_obs.push_back(0);
	}
}


double ObjectiveGini::compute(Model* mod, std::vector<int> observations) {
	int n_obs = observations.size();
	std::map<std::string, int> levels = this->data->getCategEncodings().at(this->data->getTargetIndex());
	// create mapping from each level to count integer
	std::map<int, int> class_counts;
	for (auto it = levels.begin(); it != levels.end(); ++it) {
		class_counts.insert(std::pair<int, int>(it->second, 0));
	}
	// compute class counts
	for (int i = 0; i < n_obs; ++i) {
		for (auto it = levels.begin(); it != levels.end(); ++it) {
			if (this->data->elem(observations[i], this->data->getTargetIndex()) == it->second) {
				class_counts.at(it->second) += 1;
				break;
			}
		}
	}
	// compute Gini impurity
	double value = 0;
	for (auto it = levels.begin(); it != levels.end(); ++it) {
		value += pow(class_counts.at(it->second), 2);
	}
	value = 1 - (pow((1 / (double) n_obs), 2) * value);
	return value;
}

void ObjectiveGini::update(Split* split_upd, Split* split_prev) {
	
	SplitDifference split_diff;
	split_diff.computeSplitDifference(split_upd, split_prev);
	int n_nodes = split_upd->split_obs.size();

	std::map<std::string, int> levels = this->data->getCategEncodings().at(this->data->getTargetIndex());
	std::vector<double> observation;
	for (int j = 0; j < n_nodes; ++j) {
		// for each node, update the target class counts
		if (!split_diff.additional_obs[j].empty()) {
			// additional observations
			for (int i = 0; i < split_diff.additional_obs[j].size(); ++i) {
				this->n_obs[j] += 1;
				for (auto it = levels.begin(); it != levels.end(); ++it) {
					if (this->data->elem(split_diff.additional_obs[j][i], this->data->getTargetIndex()) == it->second) {
						this->class_counts[j].at(it->second) += 1;
						break;
					}
				}
			}
		}
		if (!split_diff.removed_obs[j].empty()) {
			// removed observations
			for (int i = 0; i < split_diff.removed_obs[j].size(); ++i) {		
				this->n_obs[j] -= 1;;
				for (auto it = levels.begin(); it != levels.end(); ++it) {
					if (this->data->elem(split_diff.removed_obs[j][i], this->data->getTargetIndex()) == it->second) {
						this->class_counts[j].at(it->second) -= 1;
						break;
					}
				}
			}
		}
		// recompute Gini impurity
		double value = 0;
		for (auto it = levels.begin(); it != levels.end(); ++it) {
			value += pow(this->class_counts[j].at(it->second), 2);
		}
		this->node_obj_values[j] = 1 - (pow((1 / (double) this->n_obs[j]), 2) * value);
	}
}

std::vector<std::string> ObjectiveGini::generateAggregateModelInfo() {
	// overriding the parent class method because objective does not require model objects
	int n_nodes = this->node_obj_values.size();
	std::vector<std::string> node_info;
	double value;
	for (int j = 0; j < n_nodes; ++j) {
		value = this->node_obj_values[j];
		if (value < 0) {
			value = 0;
			// rounding errors may result in negative gini close to 0
		}
		std::ostringstream sstream;
		sstream << std::setprecision(2) << std::fixed; // printout with 2 decimal places
		sstream << "gini = " << value;
		node_info.push_back(sstream.str());
	}
	return node_info;
}