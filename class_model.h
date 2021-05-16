#ifndef MODEL_H
#define MODEL_H

#include "class_data.h"
#include "class_objective.h"


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
		virtual double evaluate(Data data, Objective* obj);
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
