#include <iostream>
#include <vector>
#include <string>
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

lluint Optimizer::getMinNodeSize() {
	return this->min_node_size;
}

void Optimizer::setMinNodeSize(lluint min_node_size) {
	this->min_node_size = min_node_size;
}

bool Optimizer::checkNodeSize(std::vector<Data> split_data) {
	bool geq_min = true;
	if (split_data.empty() == false) {
		int n_splits = split_data.size();
		for (int i = 0; i < n_splits; i++) {
			if (split_data[i].nrows() < this->min_node_size) {
				geq_min = false;
			}
		}
	} else {
		geq_min = false;
	}
	return geq_min;
}

ExhaustiveSearch::ExhaustiveSearch() {
	
}

Split ExhaustiveSearch::searchOptimum(Data data) {
	// only implemented for binary splits
	// IMPLEMENT FOR ANY NUMBER OF SPLITS (RECURSIVELY?)

	lluint n_rows = data.nrows();
	lluint n_cols = data.ncols();
	Split current_split, best_split;
	std::vector<Data> split_data;
	this->mod->setTrainingData(data);
	this->mod->train();
	double best_obj_val = this->evaluateObjective(data);
	double current_obj_val = best_obj_val;
	bool geq_min_node_size = true;
	for (lluint col = 0; col < n_cols; col++) {
		if (col == data.getTargetIndex()) {
			continue;
		}
		for (lluint row = 0; row < n_rows; row++) {
			current_split.setFeatureIndex(col);
			current_split.addSplitValue(data.elem(row, col));
			split_data = data.split(current_split);
			geq_min_node_size = checkNodeSize(split_data);
			if (geq_min_node_size == false) {
				geq_min_node_size = true;
				current_split.clear();
				continue;
			}
			int n_splits = split_data.size();
			for (int i = 0; i < n_splits; i++) {
				this->mod->setTrainingData(split_data[i]);
				this->mod->train();
				current_obj_val += this->evaluateObjective(split_data[i]);
			}
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
