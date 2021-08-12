#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "class_data.h"
#include "class_objective.h"
#include "class_model.h"
#include "helper_functions.h"
#include <cmath>
#include <algorithm>

Objective::Objective() {	
}

// ObjectiveSSE
ObjectiveSSE::ObjectiveSSE() {
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

double ObjectiveSSE::update(Data data, int childnode, std::array<std::vector<int>, 2> diff) {
	this->models[childnode]->update(diff);
	int n_setplus = diff[0].size();
	int n_setminus = diff[1].size();
	double element;
	for (int i = 0; i < n_setplus; i++) {
		element = data.elem(diff[0][i], data.getTargetIndex());
		values[childnode] += pow((element - this->models[childnode]->), 2);
	}
	/*
	std::vector<double> target_obs = data.col(data.getTargetIndex());
	double cumsum_target = cumsum(target_obs);
	int n_target = target_obs.size();
	double mean_target = cumsum_target / n_target;

	double obj_upd = obj_prev;
	int n_setplus = diff[0].size();
	int n_setminus = diff[1].size();
	double element;
	for (int i = 0; i < n_setplus; i++) {
		n_target += n_setplus;
		element = data.elem(diff[0][i], data.getTargetIndex());
		cumsum_target += element;
		mean_target = cumsum_target / n_target;
		obj_upd += pow((element - mean_target), 2);
	}
	for (int i = 0; i < n_setminus; i++) {
		n_target -= n_setminus;
		element = data.elem(diff[1][i], data.getTargetIndex());
		cumsum_target -= element;
		mean_target = cumsum_target / n_target;
		obj_upd -= pow((element - mean_target), 2);
	}
	*/
	return obj_upd;
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