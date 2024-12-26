#ifndef MODEL_H
#define MODEL_H

#include <stdlib.h>
#include <vector>
#include "class_data.h"

class Model {
	public:
		
		Model();
		~Model() {};
		
		// methods to be overwritten by inheriting classes
		virtual void update(Data* data, std::vector<int> rows, char setdiff) = 0;
		virtual double predictSingle(Data* data, int row) = 0;
		virtual std::string generateModelInfo() = 0;

		// common methods for all models
		std::vector<double> predictMult(Data* data, std::vector<int> rows);
};


class ModelAverage: public Model {
	public:
		double mean_target;
		double cumsum_target;
		int n;

		ModelAverage();
		// overwritten methods
		void update(Data* data, std::vector<int> rows, char setdiff);
		double predictSingle(Data* data, int row);
		std::string generateModelInfo();

};


class ModelSingleFeatureLinReg: public Model {
	public:
		ModelSingleFeatureLinReg(int feature);

		double mean_x, mean_y, ss_cov, ss_var;
		int n;
		int feature;
		double alpha, beta;

		// overwritten methods
		void update(Data* data, std::vector<int> rows, char setdiff);
		double predictSingle(Data* data, int row);
		std::string generateModelInfo();

};




#endif