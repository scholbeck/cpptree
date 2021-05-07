
#include "class_model.h"
#include "class_data.h"


Model::Model(Data training_data):training_data(training_data) {
  this->training_data = training_data;
}

void Model::train() {
	// placeholder
	// method overloaded by inheritance
}

std::vector<double> Model::predict(Data data) {
	// placeholder
	// method overloaded by inheritance
	std::vector<double> predictions;
	return predictions;
}

void ModelConstant::train() {
	this->mean_prediction = (this->training_data).colMean((this->training_data).getTargetIndex());
}

std::vector<double> ModelConstant::predict(Data data) {
  
	std::vector<double> predictions;
	int n = data.nrows();
	
	for (int i = 0; i < n; i++) {
		predictions.push_back(this->mean_prediction);
	}
	
	return predictions;
  
}
