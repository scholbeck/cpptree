#include "class_data.h"
#include "class_model.h"
#include "helper_functions.h"
#include "iostream"


Model::Model(Data train) {
	this->training_data = train;
	this->is_trained = false;
}

void Model::checkTrained() {
	if (this->is_trained == false) {
		perror("Model is not trained.");
	}
}

ModelAverage::ModelAverage(Data train) : Model(train) {
	//
}

void ModelAverage::train() {
	std::cout << "training model...\n";
	lluint target_index = this->training_data.getTargetIndex();
	std::vector<double> target_values = this->training_data.col(target_index);
	this->mean_prediction = mean(target_values);
	
	this->is_trained = true;
	std::cout << "training model complete.\n";
}

void ModelAverage::summary() {
	std::cout << "model summary:\nclass <Average>\n";	
	std::cout << "mean target value = " << this->mean_prediction << "\n"; 
}

std::vector<double> ModelAverage::predict(Data data) {
	
	this->checkTrained();
	
	std::vector<double> predictions;
	int n = data.nrows();
	
	for (int i = 0; i < n; i++) {
		predictions.push_back(this->mean_prediction);
	}
	return predictions;
}
