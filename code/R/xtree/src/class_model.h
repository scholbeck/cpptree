#ifndef MODEL_H
#define MODEL_H

#include <stdlib.h>
#include <vector>
#include "class_data.h"
#include "class_objective.h"

class Model {
	public:
		
		Model();
		
		// methods to be overwritten by inheriting classes
		virtual void update(Data* data, int row, char setdiff) = 0;
		virtual double predictSingle(Data* data, int row) = 0;
		virtual std::string generateModelInfo() = 0;

		// common methods for all models
		void updateSet(Data* data, std::vector<int> rows, char setdiff);
		std::vector<double> predictMult(Data* data, std::vector<int> rows);
};


class ModelAverage: public Model {
	public:
		double mean_target;
		double cumsum_target;
		int n;

		ModelAverage();
		// overwritten methods
		void update(Data* data, int row, char setdiff);
		double predictSingle(Data* data, int row);
		std::string generateModelInfo();

};


class ModelSingleFeatureLinReg: public Model {
	public:
		ModelSingleFeatureLinReg(int feature);

		double cumsum_feature, cumsum_target, cumsum_product, cumsum_squared_feature;
		int n;
		int feature;
		double alpha, beta;

		// overwritten methods
		void update(Data* data, int row, char setdiff);
		double predictSingle(Data* data, int row);
		std::string generateModelInfo();
		// additional model-specific methods
		void computeCoefficients();
};




#endif