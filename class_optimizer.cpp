#include <stdio.h>
#include <stdlib.h>
#include <armadillo>
#include <getopt.h>
#include <iostream>
#include "class_tree.h"
#include "class_node.h"
#include "class_optimizer.h"
#include "class_split.h"
#include "helper_functions.h"


Optimizer::Optimizer() {

}

Split Optimizer::optimize(Data* data, Objective* obj, Model* mod) {
	
	Split s;
	return s;
	
	// if (method == exhaustive search) {} ....
}

Split Optimizer::exhaustiveSearch(Data* data, Objective* obj, Model* mod) {
	// only implemented for binary splits
	lluint n_features = data->ncols();
	lluint n_obs = data->nrows();
	Split current_split = Split();
	Split best_split = Split();
	std::vector<Data> splitted_data;
	double best_obj_val = obj->compute(data, mod);
	double current_obj_val = best_obj_val;
	double obj_child_left, obj_child_right;
	
	for (lluint col; col < data->ncols(); col++) {
		current_split.setFeatureIndex(col);
		for (lluint row = 0; row < n_obs; row++) {
			current_split.addSplitValue(data->elem(row, col));
			splitted_data = data->split(current_split);
			
			obj_child_left = obj->compute(&splitted_data[0], mod);
			obj_child_right = obj->compute(&splitted_data[1], mod);
			current_obj_val = obj_child_left + obj_child_left; 
			
			if (current_obj_val < best_obj_val) {
				best_obj_val = current_obj_val;
				best_split = current_split;
			} else {
				current_split.clearObject();
			}
		}
	}
	return best_split;
}
