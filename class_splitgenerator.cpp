#include "class_splitgenerator.h"
#include "class_data.h"
#include <iostream>
#include <algorithm>

SplitGenerator::SplitGenerator() {
	//
}

SplitGeneratorBinExh::SplitGeneratorBinExh() : SplitGenerator() {
	//
}

std::vector<Split> SplitGeneratorBinExh::generate(Data data) {
	std::vector<Split> splits;
	int n_rows = data.nrows();
	int n_cols = data.ncols();
	
	std::vector<int> col_ix_num, col_ix_categ, categ;
	for (int j = 1; j < n_cols; j++) {
		// exclude first column with ID
		if (data.getColTypes()[j] == "num") {
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
		if (col == data.getTargetIndex()) {
			continue;
		}
		current_split.setFeatureIndex(col);
		current_split.setSplitType("categ");
		splits.push_back(current_split);
		current_split.clear();
	}
	// numeric features
	std::vector<double> col_values;
	for (int j = 0; j < n_cols_num; j++) {
		col = col_ix_num[j];
		if (col == data.getTargetIndex()) {
			continue;
		}
		col_values = data.col(col);
		std::sort(col_values.begin(), col_values.end());
		col_values.erase(std::unique(col_values.begin(), col_values.end()), col_values.end());
		int n_unique_values = col_values.size();
		for (int i = 0; i < n_unique_values; i++) {
			double split_val = col_values[i];
			current_split.addSplitValue(split_val);
			current_split.setSplitType("num");
			current_split.setFeatureIndex(col);
			splits.push_back(current_split);
			current_split.clear();
		}
	}
	return splits;
}