#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "class_data.h"
#include "class_objective.h"
#include "class_model.h"
#include <cmath>
#include <algorithm>


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
