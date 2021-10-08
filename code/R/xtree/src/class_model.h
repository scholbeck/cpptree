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
		virtual void updateSet(Data* data, std::vector<int> rows, char setdiff) = 0;
		virtual double predictSingle(Data* data, int row) = 0;
		virtual std::vector<double> predictMult(Data* data, std::vector<int> rows) = 0;
};


class ModelAverage: public Model {
	public:
		double mean_target;
		double cumsum_target;
		int n;

		ModelAverage();
		void update(Data* data, int row, char setdiff);
		void updateSet(Data* data, std::vector<int> rows, char setdiff);
		double predictSingle(Data* data, int row);
		std::vector<double> predictMult(Data* data, std::vector<int> rows);
};




#endif