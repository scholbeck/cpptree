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
#include <random>


SplitGenerator::SplitGenerator(Data* data, std::vector<int> obs, Arguments args) {
	this->data = data;
	this->obs = obs;
	this->args = args;
}

bool SplitGenerator::checkMinNodeSize(Split* split) {
	
	bool geq_min_node_size = true;
	if (split->splitted_obs.empty()) {
		geq_min_node_size = false;
	} else {
		for (int i = 0; i < this->args.getMaxChildren(); i++) {
			if ((split->splitted_obs[i].empty()) ||
				((int) split->splitted_obs[i].size() < this->args.getMinNodeSize())) {
					geq_min_node_size = false;
			}
		}
	}
	return geq_min_node_size;
}

SplitGeneratorBinExh::SplitGeneratorBinExh(Data* data, std::vector<int> obs, Arguments args) : SplitGenerator(data, obs, args) {
	//
}

std::vector<Split*> SplitGeneratorBinExh::generate() {
	
	int n_min = args.getMinNodeSize();
	std::vector<Split*> splits;
	int n_rows = this->obs.size();
	int n_cols = this->data->ncols();
	splits.reserve(n_rows * n_cols);
	std::vector<int> col_ix_num, col_ix_categ, categ;
	col_ix_num.reserve(n_cols);
	col_ix_categ.reserve(n_cols);
	for (int j = 1; j < n_cols; j++) {
		// exclude first column with ID
		if (this->data->getColTypes()[j] == "num") {
			col_ix_num.push_back(j);
		} else if (this->data->getColTypes()[j] == "categ") {
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
		if (this->data->getNLevels(col) == 1) {
			continue;
		}
		std::vector<std::vector<std::vector<int>>> level_permuts = this->data->computeCategPermuts(col, this->args.getMaxChildren());
		int n_permuts = level_permuts.size();
		for (int i = 0; i < n_permuts; i++) {
			std::map<std::string, int> levels = this->data->getCategEncodings().at(col);
			for (int p = 0; p < n_permuts; p++) {
				SplitCateg* current_split = new SplitCateg(this->args.getMaxChildren()-1, levels);
				current_split->setFeatureIndex(col);
				current_split->setLevelPartitionings(level_permuts[p]);
				current_split->computePartitionings(this->data, obs);
				if (this->checkMinNodeSize(current_split)) {
					splits.push_back(current_split);	
				}
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
		//col_values = data->getSortedFeatureValues(col);
		
		col_values = data->subsetRowsCol(obs, col);
		std::sort(col_values.begin(), col_values.end());
		// col_values.erase(std::unique(col_values.begin(), col_values.end()), col_values.end());
		//int n_unique_values = col_values.size();
		for (int i = (n_min - 1); i < (n_rows - n_min); i++) {
			if (col_values[i] == col_values[i-1]) {
				continue;
			}
			SplitNum* current_split = new SplitNum(1);
			current_split->addSplitValue(col_values[i]);
			current_split->setFeatureIndex(col);
			current_split->computePartitionings(this->data, obs);
			if (this->checkMinNodeSize(current_split)) {
				splits.push_back(current_split);
			}
		}
		col_values.clear();
	}
	return splits;
}


SplitGeneratorMultRand::SplitGeneratorMultRand(Data* data, std::vector<int> obs, Arguments args) : SplitGenerator(data, obs, args) {
	//
}

std::vector<Split*> SplitGeneratorMultRand::generate() {

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

	for (int j = 0; j < n_cols_categ; j++) {
		col = col_ix_categ[j];
		if (col == this->data->getTargetIndex()) {
			continue;
		}
		if (this->data->getNLevels(col) < this->args.getMaxChildren()) {
			continue;
		}
		std::vector<std::vector<std::vector<int>>> level_permuts = this->data->computeCategPermuts(col, this->args.getMaxChildren());
		int n_permuts = level_permuts.size();
		for (int i = 0; i < n_permuts; i++) {
			std::map<std::string, int> levels = this->data->getCategEncodings().at(col);
			for (int p = 0; p < n_permuts; p++) {
				SplitCateg* current_split = new SplitCateg(this->args.getMaxChildren()-1, levels);
				current_split->setFeatureIndex(col);
				current_split->setLevelPartitionings(level_permuts[p]);
				current_split->computePartitionings(this->data, obs);
				if (this->checkMinNodeSize(current_split)) {
					splits.push_back(current_split);	
				}
			}
		}
	}


	
	std::vector<double> split_values;
	std::vector<double> col_values;
	for (int j = 0; j < n_cols_num; j++) {
		std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
		int col = col_ix_num[j];
		col_values = data->col(col);
		std::sort(col_values.begin(), col_values.end());
		col_values.erase(std::unique(col_values.begin(), col_values.end()), col_values.end());
		int n_unique_values = col_values.size();
		std::uniform_int_distribution<> distr_row(0, n_unique_values - 1); // define the range
		int R = 100;
		for (int r = 0; r < R; r++) {
			SplitNum* current_split = new SplitNum(args.getMaxChildren() - 1);
			current_split->setFeatureIndex(col);
			std::vector<int> rnd_obs_vec;
			for (int i = 0; i < args.getMaxChildren() - 1; i++) {
				rnd_obs_vec.push_back(distr_row(gen));
				std::sort(rnd_obs_vec.begin(), rnd_obs_vec.end());
				rnd_obs_vec.erase(std::unique(rnd_obs_vec.begin(), rnd_obs_vec.end()), rnd_obs_vec.end());
			}
			if ((int) rnd_obs_vec.size() == args.getMaxChildren() - 1) {
				for (int i = 0; i < args.getMaxChildren() - 1; i++) {
					current_split->addSplitValue(rnd_obs_vec[i]);
				}
			}
			current_split->computePartitionings(this->data, obs);
			if (this->checkMinNodeSize(current_split)) {
				splits.push_back(current_split);
			}
		}
	}
	return splits;
}
