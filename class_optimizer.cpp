#include <stdio.h>
#include <stdlib.h>
#include <armadillo>
#include <getopt.h>
#include <iostream>
#include "class_tree.h"
#include "class_node.h"
#include "class_optimizer.h"
#include "class_split.h"


Optimizer::Optimizer() {

}

Split Optimizer::optimize(Data* data, int max_splits, Objective objective) {
	// placeholder
	// method overloaded by inheritance
	Split split = Split();
	return split;
}

Split OptimizerExhaustiveSearch::optimize(Data* data, int max_children, Objective objective) {
	
	int n_features = data->ncols();
	int n_obs = data->nrows();
	Split split = Split();
	std::vector<Data*> splitted_data;
	
	for (int j = 0; j < n_features; j++) {
		split.setFeatureIndex(j);
		for (int k = 0; k < (max_children - 1); k++) {
			int i;
			for (i = (split.getSplitpoints().back() + 1); i < n_obs; i++) {
				split.addSplitpoint(i);
				break;
			}
		}
		split.clearObject();
	}	
	return split;
}

Split OptimizerRandomSearch::optimize(Data* data, int max_splits, Objective objective) {
	Split split = Split();
	return split;
}
