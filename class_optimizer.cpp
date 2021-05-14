

#include <vector>
#include <string>
#include <iostream>
#include "class_arguments.h"
#include "class_model.h"
#include "class_optimizer.h"
#include "class_objective.h"
#include "class_split.h"
#include "helper_functions.h"


Optimizer::Optimizer() {

}

Model* Optimizer::getModel() {
	return this->mod;
}

void Optimizer::setModel(Model* mod) {
	this->mod = mod;
}

Objective* Optimizer::getObjective() {
	return this->obj;
}

void Optimizer::setObjective(Objective* obj) {
	this->obj = obj;
}

double Optimizer::evaluateObjective(Data data) {
	return this->obj->evaluateModel(data, this->mod);
}

ExhaustiveSearch::ExhaustiveSearch() {
	
}

Split ExhaustiveSearch::searchOptimum(Data data) {
	// only implemented for binary splits

	lluint n_rows = data.nrows();
	lluint n_cols = data.ncols();
	Split current_split, best_split;
	std::vector<Data> splitted_data;
	this->mod->setTrainingData(data);
	this->mod->train();
	double best_obj_val = this->evaluateObjective(data);
	double current_obj_val = best_obj_val;
	
	for (lluint col = 0; col < n_cols; col++) {
		if (col == data.getTargetIndex()) {
			continue;
		}
		for (lluint row = 0; row < n_rows; row++) {
			current_split.setFeatureIndex(col);
			current_split.addSplitValue(data.elem(row, col));
			splitted_data = data.split(current_split);
			int n_splits = splitted_data.size();
			for (int i = 0; i < n_splits; i++) {
				this->mod->setTrainingData(splitted_data[i]);
				this->mod->train();
				current_obj_val += this->evaluateObjective(data);
			}
			// std::cout << "current_obj_val " << current_obj_val << "\n"; 
			if (current_obj_val < best_obj_val) {
				best_obj_val = current_obj_val;
				best_split = current_split;
			}
			current_obj_val = 0;
			current_split.clear();
		}
	}
	return best_split;
}
