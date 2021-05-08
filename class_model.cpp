
#include "class_data.h"
#include "class_model.h"



Model::Model(Data* train) {
	
	this->training_data = train;
}

ModelAverage::ModelAverage(Data* train) : Model(train) {
	//
}

void ModelAverage::train() {
	this->mean_prediction = (this->training_data)->colMean((this->training_data)->getTargetIndex());
}

std::vector<double> ModelAverage::predict(Data* data) {
  
	std::vector<double> predictions;
	int n = data->nrows();
	
	for (int i = 0; i < n; i++) {
		predictions.push_back(this->mean_prediction);
	}
	
	return predictions;
  
}

