#include "class_data.h"
#include "class_model.h"
#include "helper_functions.h"



Model::Model(Data* train) {
	this->training_data = train;
}

ModelAverage::ModelAverage(Data* train) : Model(train) {
	//
}

void ModelAverage::train() {
	
	//this->mean_prediction = (this->training_data)->colMean((this->training_data)->getTargetIndex());
	lluint target_index = this->training_data->getTargetIndex();
	std::vector<double> target = this->training_data->col(target_index);
	double target_mean = mean(target);
	this->mean_prediction = target_mean;
}

void ModelAverage::print() {
	
	printf("Model summary:\n");
	printf("Constant prediction = mean target value %f\n", this->mean_prediction);
}

std::vector<double> ModelAverage::predict(Data* data) {
	std::vector<double> predictions;
	int n = data->nrows();
	
	for (int i = 0; i < n; i++) {
		predictions.push_back(this->mean_prediction);
	}
	return predictions;
}
