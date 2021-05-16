#include "class_data.h"
#include "class_model.h"
#include "helper_functions.h"
#include "iostream"


Model::Model() {
	this->is_trained = false;
}

void Model::setTrainingData(Data data) {
	this->training_data = data;
}

void Model::checkTrained() {
	if (this->is_trained == false) {
		perror("Model is not trained.");
	}
}


double Model::evaluate(Data data, Objective* obj) {
	std::vector<double> target_obs = data.col(data.getTargetIndex());
	std::vector<double> target_pred = this->predict(data);
	return obj->compute(target_obs, target_pred);
}

ModelAverage::ModelAverage() : Model() {
	//
}

void ModelAverage::train() {
	int target_index = this->training_data.getTargetIndex();
	std::vector<double> target_values = this->training_data.col(target_index);
	this->mean_prediction = mean(target_values);
	this->is_trained = true;
}

void ModelAverage::summary() {
	std::cout << "model summary:\nclass <Average>\n";	
	std::cout << "mean target value = " << this->mean_prediction << "\n"; 
}

std::vector<double> ModelAverage::predict(Data data) {
	this->checkTrained();
	int n = data.nrows();
	std::vector<double> predictions(n, mean_prediction);
	return predictions;
}

