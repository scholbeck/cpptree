#include "class_splitgenerator.h"
#include "class_data.h"
#include "class_arguments.h"
#include "helper_functions.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <array>
#include <vector>
#include <time.h>
#include <stdlib.h>



SplitGenerator::SplitGenerator(Data* data, Arguments args) {
	this->data = data;
	this->args = args;
}

bool SplitGenerator::checkMinNodeSize(Split* split) {
	
	bool geq_min_node_size = true;
	if (split->splitted_obs.empty()) {
		geq_min_node_size = false;
	} else {
		for (int i = 0; i < this->args.getMaxChildren(); i++) {
			if ((split->splitted_obs[i].empty()) |
				(split->splitted_obs[i].size() < this->args.getMinNodeSize())) {
					geq_min_node_size = false;
			}
		}
	}
	return geq_min_node_size;
}

SplitGeneratorBinExh::SplitGeneratorBinExh(Data* data, Arguments args) : SplitGenerator(data, args) {
	//
}

std::vector<Split*> SplitGeneratorBinExh::generate() {
	
	int n_min = args.getMinNodeSize();
	std::vector<Split*> splits;
	int n_rows = data->nrows();
	int n_cols = data->ncols();
	splits.reserve(n_rows * n_cols);
	std::vector<int> col_ix_num, col_ix_categ, categ;
	col_ix_num.reserve(n_cols);
	col_ix_categ.reserve(n_cols);
	for (int j = 1; j < n_cols; j++) {
		// exclude first column with ID
		if (this->data->getColTypes()[j] == "num") {
			col_ix_num.push_back(j);
		} else {
			col_ix_categ.push_back(j);
		}	
	}
	int n_cols_num = col_ix_num.size();
	int n_cols_categ = col_ix_categ.size();
	int col;
	//categorical features
	for (int j = 0; j < n_cols_categ; j++) {
		col = col_ix_categ[j];
		if (col == this->data->getTargetIndex()) {
			continue;
		}
		std::vector<std::vector<std::vector<int>>> level_permuts = this->data->computeCategPermuts(col, this->args.getMaxChildren());
		int n_permuts = level_permuts.size();
		for (int i = 0; i < n_permuts; i++) {
			std::map<std::string, int> levels = this->data->getCategEncodings().at(col);
			Split* current_split = new SplitCateg(1, levels);
			current_split->setFeatureIndex(col);;
			current_split->splitted_obs = this->data->splitCategObs(this->args.getMaxChildren(), level_permuts[i]);
			if (this->checkMinNodeSize(current_split)) {
				splits.push_back(current_split);	
			}
		}
	}
	// numeric features
	std::vector<double> col_values;
	for (int j = 0; j < n_cols_num; j++) {
		col = col_ix_num[j];
		if (col == this->data->getTargetIndex()) {
			continue;
		}
		col_values = data->col(col);
		std::sort(col_values.begin(), col_values.end());
		// col_values.erase(std::unique(col_values.begin(), col_values.end()), col_values.end());
		//int n_unique_values = col_values.size();
		for (int i = (n_min - 1); i < (n_rows - n_min); i++) {
			if (col_values[i] == col_values[i-1]) {
				continue;
			}
			Split* current_split = new SplitNum(1);
			current_split->addSplitValue(col_values[i]);
			current_split->setFeatureIndex(col);
			current_split->splitted_obs = this->data->splitObs(current_split);
			if (this->checkMinNodeSize(current_split)) {
				splits.push_back(current_split);
			}
		}
		col_values.clear();
	}
	return splits;
}
/*
SplitGeneratorMultRand::SplitGeneratorMultRand(Data* data, Arguments args) : SplitGenerator(data, args) {
	//
}

std::vector<Split*> SplitGeneratorMultRand::generate() {
	int n_min = args.getMinNodeSize();
	std::vector<Split*> splits;
	int n_rows = data->nrows();
	int n_cols = data->ncols();
	splits.reserve(n_rows * n_cols);
	std::vector<int> col_ix_num, col_ix_categ, categ;
	col_ix_num.reserve(n_cols);
	col_ix_categ.reserve(n_cols);
	for (int j = 1; j < n_cols; j++) {
		// exclude first column with ID
		if (this->data->getColTypes()[j] == "num") {
			col_ix_num.push_back(j);
		} else {
			col_ix_categ.push_back(j);
		}
	}
	srand(time(NULL));   // Initialization, should only be called once.
	int rnd_col = rand() % n_cols;      // Returns a pseudo-random integer between 0 and RAND_MAX.
	int rnd_row = rand() % n_rows;
	
}
*/
/*
std::vector<Split> SplitGeneratorBinExh::generate() {
	std::vector<Split> splits;
	int n_rows = data->nrows();
	int n_cols = data->ncols();
	splits.reserve(n_rows * n_cols);
	std::vector<int> col_ix_num, col_ix_categ, categ;
	col_ix_num.reserve(n_cols);
	col_ix_categ.reserve(n_cols);
	for (int j = 1; j < n_cols; j++) {
		// exclude first column with ID
		if (this->data->getColTypes()[j] == "num") {
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
		if (col == this->data->getTargetIndex()) {
			continue;
		}
		current_split.setFeatureIndex(col);
		current_split.setSplitType("categ");
		current_split.splitted_obs = this->data->splitObs(current_split);
		if (this->checkMinNodeSize(current_split)) {
			splits.push_back(current_split);	
		}
		current_split.clear();
	}
	// numeric features
	std::vector<std::pair<double, int>> col_values;
	col_values.reserve(n_rows);
	for (int j = 0; j < n_cols_num; j++) {
		col = col_ix_num[j];
		if (col == this->data->getTargetIndex()) {
			continue;
		}
		for (int i = 0; i < n_rows; i++) {
			col_values.push_back(std::pair<double, int> (data->col(col)[i], data->col(0)[i]));
    	}
		std::sort(col_values.begin(), col_values.end());
		
		// col_values.erase(std::unique(col_values.begin(), col_values.end()), col_values.end());
		//int n_unique_values = col_values.size();
		std::vector<int> left, right;
		left.reserve(n_rows);
		right.reserve(n_rows);
		for (int i = n_rows-1; i >= 0; i--) {
			right.push_back(col_values[i].second);
		}
		for (int i = 0; i < n_rows-1; i++) {
			left.push_back(col_values[i].second);
			right.pop_back();
			if ((i >= this->args.getMinNodeSize()-1) && (i < (n_rows - (this->args.getMinNodeSize())))) {
				if (col_values[i].first == col_values[i-1].first) {
					continue;
				}
				current_split.splitted_obs[0] = left;
				current_split.splitted_obs[1] = right;
				//printVectorInt(left);
				//printVectorInt(right);
				current_split.addSplitValue(col_values[i].first);
				current_split.setSplitType("num");
				current_split.setFeatureIndex(col);
				//current_split.splitted_obs = this->data->splitObs(current_split);
				splits.push_back(current_split);
				current_split.clear();
			}
		}
		col_values.clear();
	}
	return splits;
}
*/

/*
SplitGeneratorBinQuant::SplitGeneratorBinQuant(Data* data, Arguments args) : SplitGenerator(data, args) {
	//
}

std::vector<Split*> SplitGeneratorBinQuant::generate() {
	std::vector<Split> splits;
	int n_rows = data->nrows();
	int n_cols = data->ncols();
	splits.reserve(n_rows * n_cols);
	std::vector<int> col_ix_num, col_ix_categ, categ;
	col_ix_num.reserve(n_cols);
	col_ix_categ.reserve(n_cols);
	for (int j = 1; j < n_cols; j++) {
		// exclude first column with ID
		if (this->data->getColTypes()[j] == "num") {
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
		if (col == this->data->getTargetIndex()) {
			continue;
		}
		current_split.setFeatureIndex(col);
		current_split.setSplitType("categ");
		// current_split.splitted_obs = this->data->splitObs(current_split);
		if (this->checkMinNodeSize(current_split)) {
			splits.push_back(current_split);	
		}
		current_split.clear();
	}
	// numeric features
	std::vector<double> col_values;
	for (int j = 0; j < n_cols_num; j++) {
		col = col_ix_num[j];
		if (col == this->data->getTargetIndex()) {
			continue;
		}
		col_values = this->data->col(col);
		std::sort(col_values.begin(), col_values.end());
		col_values.erase(std::unique(col_values.begin(), col_values.end()), col_values.end());
		int n_unique_values = col_values.size();
		float start = this->args.getMinNodeSize() / n_unique_values;
		double val_prev, val_current;
		for (float i = start; i < 1-start; i+= 0.01) {
			val_current = col_values[floor(i * n_unique_values)];
			//std::cout << val_current << std::endl;
			if (val_current == val_prev) {
				continue;
			} else {
				val_prev = val_current;
			}
			current_split.addSplitValue(val_current);
			current_split.setSplitType("num");
			current_split.setFeatureIndex(col);
			// current_split.splitted_obs = this->data->splitObs(current_split);
			splits.push_back(current_split);	
			current_split.clear();
		}
	}
	return splits;
}

*/