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

double ObjectiveSSE::compute(Data* data, Model* model) {
	
	std::vector<double> predictions = model->predict(data);
	std::vector<double> target_observed = data->col(data->getTargetIndex());
	
	int n = predictions.size();
	double cumsum = 0;
	
	for (int i = 0; i < n; i++) {
		cumsum += pow((predictions[i] - target_observed[i]), 2);
	}
	
	return cumsum;
}
