#include "class_data.h"
#include "class_model.h"
#include "helper_functions.h"
#include "iostream"
#include <algorithm>

Model::Model() {
}

ModelAverage::ModelAverage() : Model() {
	this->cumsum_target = 0;
	this->n = 0;
	this->mean_target = 0;
}

void ModelAverage::update(Data* data, std::vector<int> rows, char setdiff) {
	for (int i = 0; i < rows.size(); ++i) {
		if (setdiff == '+') {
			this->cumsum_target += data->row(rows[i])[data->getTargetIndex()];
			this->n += 1;
			this->mean_target = cumsum_target / this->n;
		} else {
			this->cumsum_target -= data->row(rows[i])[data->getTargetIndex()];
			this->n -= 1;
			this->mean_target = cumsum_target / this->n;
		}
	}
}

double ModelAverage::predictSingle(Data* data, int row) {
	return this->mean_target;
}

std::vector<double> ModelAverage::predictMult(Data* data, std::vector<int> rows) {
	std::vector<double> predictions(rows.size(), this->mean_target);
	return predictions;
}