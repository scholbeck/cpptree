#ifndef MODEL_H
#define MODEL_H

#include "class_data.h"
#include "class_objective.h"


class Model {
  
	public:
		Data* training_data;
		bool is_trained;
		
		Model();
		void setTrainingData(Data* data);
		void checkTrained();
		virtual void train() = 0;
		virtual void update(std::vector<double> observation, char setdiff) = 0;
		virtual void summary() = 0;
		virtual std::string getShortSummary() = 0;
		virtual std::vector<double> predictMult(Data* data) = 0;
		virtual double predictSingle(std::vector<double> observation) = 0;
};

// Regression

class ModelAverage: public Model {
	public:
		double mean_target;
		double cumsum_target;
		int n;

		ModelAverage();
		void train();
		void summary();
		virtual void update(std::vector<double> observation, char setdiff);
		std::string getShortSummary();
		std::vector<double> predictMult(Data* data);
		virtual double predictSingle(std::vector<double> observation);
};

class ModelLinearRegression: public Model {
	public:
		std::map<int, double> params;
		double learning_rate;
		int n;

		ModelLinearRegression();
		void train();
		virtual void update(std::vector<double> observation, char setdiff);
		std::vector<double> predictMult(Data* data);
		virtual double predictSingle(std::vector<double> observation);
};

// Classification

/*
class ModelMajorityVote: public Model {
	public:
		int majority_class;
		std::map<int, double> probs;
		
		ModelMajorityVote();
		void train();
		void summary();
		std::string getShortSummary();
		std::vector<double> predict(Data* data);
};

*/
#endif 
