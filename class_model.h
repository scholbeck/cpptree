#ifndef MODEL_H
#define MODEL_H

#include "class_data.h"

class Model {
  
	public:
		Data training_data;
		bool is_trained;
		
		Model();
		void setTrainingData(Data data);
		void checkTrained();
		virtual void train() = 0;
		virtual void summary() = 0;
		virtual std::vector<double> predict(Data data) = 0;
};

class ModelAverage: public Model {
	public:
		double mean_prediction;
		
		ModelAverage();
		void train();
		void summary();
		std::vector<double> predict(Data data);
};

class ModelLinearRegression: public Model {
	public:
		//using Model::predict;
};


#endif 
