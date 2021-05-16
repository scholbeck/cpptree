#include <stdio.h>
#include <stdlib.h>
#include "class_data.h"
#include "class_objective.h"
#include "class_model.h"
#include <cmath>


Objective::Objective() {
	
}

ObjectiveSSE::ObjectiveSSE() {
	
}

double ObjectiveSSE::compute(std::vector<double> target_obs, std::vector<double> target_preds) {
	int n = target_obs.size();
	double cumsum = 0;
	for (int i = 0; i < n; i++) {
		cumsum += pow((target_preds[i] - target_obs[i]), 2);
	}
	return cumsum;
}
