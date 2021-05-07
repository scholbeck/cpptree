
#include "class_model.h"
#include "class_data.h"


Model::Model(Data* training_data) {
  
}

std::vector<double> Model::predict(Data* data) {
	// placeholder
	// method overloaded by inheritance
	std::vector<double> predictions;
	return predictions;
}

std::vector<double> ModelConstant::predict(Data* data) {
  
	double mean = data->colMean(data->getTargetIndex());
	int n = data->nrows();
	std::vector<double> predictions;
	
	for (int i = 0; i < n; i++) {
		predictions.push_back(mean);
	}
	
	return predictions;
  
}
