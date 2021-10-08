#ifndef MODEL_H
#define MODEL_H

#include <stdlib.h>
#include <vector>
#include "class_data.h"
#include "class_objective.h"

class Model {
	public:
		
		Model();
		
		virtual void update(Data* data, int row, char setdiff) = 0;
		virtual double predictSingle(Data* data, int row) = 0;
	
		void updateSet(Data* data, std::vector<int> rows, char setdiff);
		std::vector<double> predictMult(Data* data, std::vector<int> rows);
};


class ModelAverage: public Model {
	public:
		double mean_target;
		double cumsum_target;
		int n;

		ModelAverage();
		void update(Data* data, int row, char setdiff);
		double predictSingle(Data* data, int row);
};


class ModelSingleFeatureLinReg: public Model {
	public:
		ModelSingleFeatureLinReg(int feature);

		double cumsum_feature, cumsum_target, cumsum_product, cumsum_squared_feature;
		int n;
		int feature;
		double alpha, beta;

		void update(Data* data, int row, char setdiff);
		double predictSingle(Data* data, int row);
		void computeCoefficients();
};




#endif