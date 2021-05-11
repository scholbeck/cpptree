#ifndef MODEL_H
#define MODEL_H

#include "class_data.h"

class Model {
  
	public:
		Data* training_data;
    
		Model(Data* train);
		virtual void train() = 0;
		virtual std::vector<double> predict(Data* data) = 0;
};

class ModelAverage: public Model {
	public:
		double mean_prediction;
		
		ModelAverage(Data* train);
		void train();
		std::vector<double> predict(Data* data);
		void print();
};

class ModelLinearRegression: public Model {
	public:
		//using Model::predict;
};


#endif 
