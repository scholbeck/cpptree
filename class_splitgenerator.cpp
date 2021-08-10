#include "class_splitgenerator.h"
#include "class_data.h"
#include <iostream>

SplitGenerator::SplitGenerator() {
	//
}

SplitGeneratorBinExh::SplitGeneratorBinExh() : SplitGenerator() {
	//
}

std::vector<Split> SplitGeneratorBinExh::generate(Data data) {
	std::vector<Split> splits;
	int n_rows = data.nrows();
	int n_cols = data.ncols() - 1;
	
	std::vector<int> col_ix_num, col_ix_categ, categ;
	for (int j = 1; j < n_cols; j++) {
		std::cout << j;
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
		current_split.setFeatureIndex(col);
		current_split.setSplitType("categ");
		splits.push_back(current_split);
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
			splits.push_back(current_split);
			current_split.clear();
		}
	}
	
	return splits;
}