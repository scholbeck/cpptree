#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <vector>
#include <string>
#include <iostream>
#include "class_model.h"
#include "class_optimizer.h"
#include "class_objective.h"
#include "class_split.h"
#include "helper_functions.h"


Optimizer::Optimizer() {

}

Split Optimizer::searchOptimum(Data data, Objective* obj, Model* mod, std::string algo) {
	Split s;
	if (algo == "exhaustive") {
		s = this->exhaustiveSearch(data, obj, mod);	
	} else if (algo == "random") {
		//
	} // ...
	return s;
}

Split Optimizer::exhaustiveSearch(Data data, Objective* obj, Model* mod) {
	// only implemented for binary splits

	lluint n_rows = data.nrows();
	lluint n_cols = data.ncols();
	Split current_split, best_split;
	std::vector<Data> splitted_data;
	double best_obj_val = obj->compute(data, mod);
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
				current_obj_val += obj->compute(splitted_data[i], mod);
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
