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

Objective* Optimizer::getObjective() {
	return this->obj;
}

void Optimizer::setObjective(Objective* obj) {
	this->obj = obj;
}

int Optimizer::getMaxChildren() {
	return this->max_children;
}

void Optimizer::setMaxChildren(int max_children) {
	this->max_children = max_children;
}

int Optimizer::getMinNodeSize() {
	return this->min_node_size;
}

void Optimizer::setMinNodeSize(int min_node_size) {
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

Model* Optimizer::buildModel(Data data, Arguments args) {
	Model* m;
	
	if (args.getModel() == "mean") {
		m = new ModelAverage();
	} else if (args.getModel() == "majorvote") {
		m = new ModelMajorityVote();
	}
	m->setTrainingData(data);
	m->train();
	return m;
}


// OptimExhaustSearchRegr

OptimExhaustSearchRegr::OptimExhaustSearchRegr() {
	
}

void OptimExhaustSearchRegr::summary() {
	std::cout << "Optimizer of class <OptimExhaustSearchRegr>\n";
}


Split OptimExhaustSearchRegr::searchOptimum(Data data, Arguments args) {
	// only implemented for binary splits and model average
	// IMPLEMENT FOR ANY NUMBER OF SPLITS (RECURSIVELY?) AND LINEAR MODELS

	std::vector<Model*> child_node_models;
	
	Model* mod = this->buildModel(data, args);
	double best_obj_val = mod->evaluate(data, obj);
	double current_obj_val = best_obj_val;
	
	int n_rows = data.nrows();
	int n_cols = data.ncols();
	Split current_split, best_split;
	current_split.child_node_models = child_node_models;
	std::vector<Data> split_data;
	bool geq_min_node_size = true;
	
	for (int col = 0; col < n_cols; col++) {
		if (col == data.getTargetIndex()) {
			continue;
		}
		for (int row = 0; row < n_rows; row++) {
			current_split.setFeatureIndex(col);
			current_split.addSplitValue(data.elem(row, col));
			split_data = data.split(current_split);
			geq_min_node_size = checkNodeSize(split_data);
			if (geq_min_node_size == false) {
				geq_min_node_size = true;
				current_split.clear();
				continue;
			}
			int n_children = split_data.size();
			for (int i = 0; i < n_children; i++) {
				Model* m = this->buildModel(split_data[i], args);
				child_node_models.push_back(m);
				current_split.child_node_models = child_node_models;
				current_obj_val += m->evaluate(split_data[i], this->obj);
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

// OptimExhaustSearchClassif

OptimExhaustSearchClassif::OptimExhaustSearchClassif() {
	
}

void OptimExhaustSearchClassif::summary() {
	std::cout << "Optimizer of class <OptimExhaustSearchClassif>\n";
}

Split OptimExhaustSearchClassif::searchOptimum(Data data, Arguments args) {
	// only implemented for binary splits and model average
	// IMPLEMENT FOR ANY NUMBER OF SPLITS (RECURSIVELY?) AND LINEAR MODELS
	

	int n_rows = data.nrows();
	int n_cols = data.ncols();
	std::vector<int> col_ix_num, col_ix_categ, categ;
	for (int j = 0; j < n_cols; j++) {
		if (data.getColTypes()[j] == "num") {
			col_ix_num.push_back(j);
		} else {
			col_ix_categ.push_back(j);
		}	
	}
	int n_cols_num = col_ix_num.size();
	int n_cols_categ = col_ix_categ.size();
	
	std::vector<Model*> child_node_models;	
	Model* mod = this->buildModel(data, args);
	
	double best_obj_val = mod->evaluate(data, obj);
	double current_obj_val = best_obj_val;
	
	Split current_split, best_split;
	current_split.child_node_models = child_node_models;
	std::vector<Data> split_data;
	bool geq_min_node_size = true;
	
	int col;
	for (int j = 0; j < n_cols_categ; j++) {
		col = col_ix_categ[j];
		current_split.setFeatureIndex(col);
		current_split.setSplitType("categ");
		split_data = data.split(current_split);
		geq_min_node_size = checkNodeSize(split_data);
		if (geq_min_node_size == false) {
			geq_min_node_size = true;
			current_split.clear();
			continue;
		}
		int n_children = split_data.size();
		for (int i = 0; i < n_children; i++) {
			Model* m = this->buildModel(split_data[i], args);
			child_node_models.push_back(m);
			current_split.child_node_models = child_node_models;
			current_obj_val += m->evaluate(split_data[i], this->obj);
		}
		if (current_obj_val < best_obj_val) {
			best_obj_val = current_obj_val;
			best_split = current_split;
		}
		current_obj_val = 0;
		current_split.clear();
	}
	
	for (int j = 0; j < n_cols_num; j++) {
		col = col_ix_num[j];
		if (col == data.getTargetIndex()) {
			continue;
		}
		for (int row = 0; row < n_rows; row++) {
			current_split.setSplitType("num");
			current_split.setFeatureIndex(col);
			current_split.addSplitValue(data.elem(row, col));
			split_data = data.split(current_split);
			geq_min_node_size = checkNodeSize(split_data);
			if (geq_min_node_size == false) {
				geq_min_node_size = true;
				current_split.clear();
				continue;
			}
			int n_children = split_data.size();
			for (int i = 0; i < n_children; i++) {
				Model* m = this->buildModel(split_data[i], args);
				child_node_models.push_back(m);
				current_split.child_node_models = child_node_models;
				current_obj_val += m->evaluate(split_data[i], this->obj);
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
