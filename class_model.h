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
		virtual void update(std::array<std::vector<int>, 2> diff) = 0;
		virtual void summary() = 0;
		virtual std::string getShortSummary() = 0;
		virtual std::vector<double> predict(Data data) = 0;
		virtual double evaluate(Data data, Objective* obj);
};

// Regression

class ModelAverage: public Model {
	public:
		double mean_prediction;
		double cumsum_target;
		int n;

		ModelAverage();
		void train();
		void summary();
		virtual void update(std::array<std::vector<int>, 2> diff);
		std::string getShortSummary();
		std::vector<double> predict(Data data);
};

class ModelLinearRegression: public Model {
	public:
		//using Model::predict;
};

// Classification


class ModelMajorityVote: public Model {
	public:
		int majority_class;
		std::map<int, double> probs;
		
		ModelMajorityVote();
		void train();
		void summary();
		std::string getShortSummary();
		std::vector<double> predict(Data data);
};


#endif 
