#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
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

// OptimExhaustSearch

OptimExhaustSearch::OptimExhaustSearch() {
	
}

void OptimExhaustSearch::summary() {
	std::cout << "Optimizer of class <OptimExhaustSearch>\n";
}
 
long long int factorial(int n) {
	long long int fctrl = (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
	return fctrl;
}

std::vector<std::vector<int>> permuteIndices(int n, int k) {
	std::vector<std::vector<int>> split_ix;
	split_ix.reserve(factorial(n) / (factorial(n - k) * factorial(k)));
	
    std::string bitmask(k, 1); // k leading 1s
    bitmask.resize(n, 0); // n-k trailing 0s
 
    // print integers and permute bitmask
    do {
		std::vector<int> combination;
		combination.reserve(k);
        for (int i = 0; i < n; ++i) {
        // [0...n-1] integers
            if (bitmask[i]) {
				combination.push_back(i);
			}
        }
        split_ix.push_back(combination);
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
    
    return split_ix;
}

Split OptimExhaustSearch::searchOptimum(Data data, Arguments args, double node_obj_val) {

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
	
	double best_obj_val = node_obj_val;
	double current_obj_val = best_obj_val;
	
	Split current_split = Split(args.getMaxChildren() - 1);
	Split best_split = Split(args.getMaxChildren() - 1);
	std::vector<Data> split_data;
	bool geq_min_node_size = true;
	
	int col;
	//categorical features
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
			current_obj_val += (split_data[i].nrows() / n_rows) * m->evaluate(split_data[i], this->obj);
		}
		if (current_obj_val < best_obj_val) {
			best_obj_val = current_obj_val;
			best_split = current_split;
		}
		current_obj_val = 0;
		current_split.clear();
	}
	
	// numeric features
	for (int j = 0; j < n_cols_num; j++) {
		col = col_ix_num[j];
		if (col == data.getTargetIndex()) {
			continue;
		}
		for (int i = 0; i < n_rows; i++) {
			double split_val = data.elem(i, col);
			current_split.addSplitValue(split_val);
			current_split.setSplitType("num");
			current_split.setFeatureIndex(col);
			
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
				current_obj_val += (split_data[i].nrows() / n_rows) * m->evaluate(split_data[i], this->obj);
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





//Split OptimExhaustSearch::searchOptimum(Data data, Arguments args, double node_obj_val) {

	//int n_rows = data.nrows();
	//int n_cols = data.ncols();
	//std::vector<int> col_ix_num, col_ix_categ, categ;
	//for (int j = 0; j < n_cols; j++) {
		//if (data.getColTypes()[j] == "num") {
			//col_ix_num.push_back(j);
		//} else {
			//col_ix_categ.push_back(j);
		//}	
	//}
	//int n_cols_num = col_ix_num.size();
	//int n_cols_categ = col_ix_categ.size();
	
	//double best_obj_val = node_obj_val;
	//double current_obj_val = best_obj_val;
	
	//Split current_split = Split(args.getMaxChildren() - 1);
	//Split best_split = Split(args.getMaxChildren() - 1);
	//std::vector<Data> split_data;
	//bool geq_min_node_size = true;
	
	//int col;
	////categorical features
	//for (int j = 0; j < n_cols_categ; j++) {
		//col = col_ix_categ[j];
		//current_split.setFeatureIndex(col);
		//current_split.setSplitType("categ");
		//split_data = data.split(current_split);
		//geq_min_node_size = checkNodeSize(split_data);
		//if (geq_min_node_size == false) {
			//geq_min_node_size = true;
			//current_split.clear();
			//continue;
		//}
		//int n_children = split_data.size();
		//for (int i = 0; i < n_children; i++) {
			//Model* m = this->buildModel(split_data[i], args);
			//current_obj_val += (split_data[i].nrows() / n_rows) * m->evaluate(split_data[i], this->obj);
		//}
		//if (current_obj_val < best_obj_val) {
			//best_obj_val = current_obj_val;
			//best_split = current_split;
		//}
		//current_obj_val = 0;
		//current_split.clear();
	//}
	
	//// numeric features
	//for (int s = 1; s < args.getMaxChildren(); s++) {

		//// test each number of splits from 1 to specified maximum
		//std::vector<std::vector<int>> indices_perms = permuteIndices(n_rows, s);
		//int perms_size = indices_perms.size();
		
		//for (int j = 0; j < n_cols_num; j++) {
			//col = col_ix_num[j];
			//if (col == data.getTargetIndex()) {
				//continue;
			//}

			//for (int i = 0; i < perms_size; i++) {
				//std::vector<int> split_ix = indices_perms[i];
				//int n_splits = split_ix.size();
				//for (int l = 0; l < n_splits; l++) {
					//double split_val = data.elem(split_ix[l], col);
					//current_split.addSplitValue(split_val);
				//}
				//current_split.sortSplitValues();
				//current_split.setSplitType("num");
				//current_split.setFeatureIndex(col);
				
				//split_data = data.split(current_split);
				//geq_min_node_size = checkNodeSize(split_data);
				//if (geq_min_node_size == false) {
					//geq_min_node_size = true;
					//current_split.clear();
					//continue;
				//}
				//int n_children = split_data.size();
				//for (int i = 0; i < n_children; i++) {
					//Model* m = this->buildModel(split_data[i], args);
					//current_obj_val += (split_data[i].nrows() / n_rows) * m->evaluate(split_data[i], this->obj);
				//}
				//if (current_obj_val < best_obj_val) {
					//best_obj_val = current_obj_val;
					//best_split = current_split;
				//}
				//current_obj_val = 0;
				//current_split.clear();
			//}
		//}
	//}
	//return best_split;
//}



/*
 * 
 * 
Split OptimExhaustSearch::searchOptimum(Data data, Arguments args, double node_obj_val) {
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
	
	double best_obj_val = node_obj_val;
	double current_obj_val = best_obj_val;
	
	Split current_split, best_split;
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
			current_obj_val += (split_data[i].nrows() / n_rows) * m->evaluate(split_data[i], this->obj);
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
				current_obj_val += (split_data[i].nrows() / n_rows) * m->evaluate(split_data[i], this->obj);
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

*/
