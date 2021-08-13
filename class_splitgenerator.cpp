#include "class_splitgenerator.h"
#include "class_data.h"
#include "class_arguments.h"
#include "helper_functions.h"
#include <iostream>
#include <algorithm>

SplitGenerator::SplitGenerator(Data data, Arguments args) {
	this->data = data;
	this->args = args;
}

bool SplitGenerator::checkMinNodeSize(Split split) {
	
	bool geq_min_node_size = true;
	if (split.splitted_obs.empty()) {
		geq_min_node_size = false;
	} else {
		for (int i = 0; i < this->args.getMaxChildren(); i++) {
			if ((split.splitted_obs[i].empty()) |
				(split.splitted_obs[i].size() < this->args.getMinNodeSize())) {
					geq_min_node_size = false;
			}
		}
	}
	return geq_min_node_size;
}

SplitGeneratorBinExh::SplitGeneratorBinExh(Data data, Arguments args) : SplitGenerator(data, args) {
	//
}

std::vector<Split> SplitGeneratorBinExh::generate() {
	std::vector<Split> splits;
	int n_rows = data.nrows();
	int n_cols = data.ncols();
	
	std::vector<int> col_ix_num, col_ix_categ, categ;
	for (int j = 1; j < n_cols; j++) {
		// exclude first column with ID
		if (this->data.getColTypes()[j] == "num") {
			col_ix_num.push_back(j);
		} else {
			col_ix_categ.push_back(j);
		}	
	}
	int n_cols_num = col_ix_num.size();
	int n_cols_categ = col_ix_categ.size();
	
	Split current_split = Split(1);	
	int col;
	//categorical features
	for (int j = 0; j < n_cols_categ; j++) {
		col = col_ix_categ[j];
		if (col == this->data.getTargetIndex()) {
			continue;
		}
		current_split.setFeatureIndex(col);
		current_split.setSplitType("categ");
		current_split.splitted_obs = this->data.splitObs(current_split);
		if (this->checkMinNodeSize(current_split)) {
			splits.push_back(current_split);	
		}
		current_split.clear();
	}
	// numeric features
	std::vector<double> col_values;
	for (int j = 0; j < n_cols_num; j++) {
		col = col_ix_num[j];
		if (col == this->data.getTargetIndex()) {
			continue;
		}
		col_values = this->data.col(col);
		std::sort(col_values.begin(), col_values.end());
		col_values.erase(std::unique(col_values.begin(), col_values.end()), col_values.end());
		int n_unique_values = col_values.size();
		for (int i = 0; i < n_unique_values; i++) {
			double split_val = col_values[i];
			current_split.addSplitValue(split_val);
			current_split.setSplitType("num");
			current_split.setFeatureIndex(col);
			current_split.splitted_obs = this->data.splitObs(current_split);
			if (this->checkMinNodeSize(current_split)) {
				splits.push_back(current_split);	
			}
			current_split.clear();
		}
	}
	return splits;
}