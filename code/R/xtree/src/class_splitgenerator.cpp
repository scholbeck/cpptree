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


SplitGenerator::SplitGenerator() {
}

bool SplitGenerator::checkMinNodeSize(Split* split, int min_node_size) {
	bool geq_min_node_size = true;
	if (split->split_obs.empty()) {
		geq_min_node_size = false;
	} else {
		for (int i = 0; i < split->split_obs.size(); i++) {
			if ((split->split_obs[i].empty()) ||
				((int) split->split_obs[i].size() < min_node_size)) {
					geq_min_node_size = false;
			}
		}
	}
	return geq_min_node_size;
}

SplitGeneratorBinExh::SplitGeneratorBinExh() : SplitGenerator() {
	//
}

std::vector<Split*> SplitGeneratorBinExh::generate(Data* data, std::vector<int> observations, Arguments args) {
	
	int n_rows = observations.size();
	int n_min = args.getMinNodeSize();
	std::vector<Split*> splits;
	int n_cols = data->ncols();
	splits.reserve(n_rows * n_cols);
	std::vector<int> col_ix_num, col_ix_categ, categ;
	col_ix_num.reserve(n_cols);
	col_ix_categ.reserve(n_cols);
	for (int j = 1; j < n_cols; j++) {
		// exclude first column with ID
		if (data->getColTypes()[j] == "num") {
			col_ix_num.push_back(j);
		} else if (data->getColTypes()[j] == "categ") {
			col_ix_categ.push_back(j);
		}	
	}
	int n_cols_num = col_ix_num.size();
	int n_cols_categ = col_ix_categ.size();
	int col;
	//categorical features
	for (int j = 0; j < n_cols_categ; j++) {
		col = col_ix_categ[j];
		if (col == data->getTargetIndex()) {
			continue;
		}
		if (data->getNLevels(col) == 1) {
			continue;
		}
		std::vector<std::vector<std::vector<int>>> level_permuts = data->computeCategPermuts(col, args.getMaxChildren());
		int n_permuts = level_permuts.size();
		for (int i = 0; i < n_permuts; i++) {
			std::map<std::string, int> levels = data->getCategEncodings().at(col);
			for (int p = 0; p < n_permuts; p++) {
				SplitCateg* current_split = new SplitCateg(args.getMaxChildren() - 1, levels);
				current_split->setFeatureIndex(col);
				current_split->setLevelPartitionings(level_permuts[p]);
				current_split->computePartitionings(data, observations);
				if (this->checkMinNodeSize(current_split, args.getMinNodeSize())) {
					splits.push_back(current_split);	
				}
			}
		}
	}
	// numeric features
	std::vector<double> col_values;
	for (auto it = col_ix_num.begin(); it != col_ix_num.end(); ++it) {
		if (*it == data->getTargetIndex()) {
			continue;
		}
		col_values = data->columnSubset(observations, *it);
		std::sort(col_values.begin(), col_values.end());

		for (int i = (n_min - 1); i < (n_rows - n_min); i++) {
			if (col_values[i] == col_values[i-1]) {
				continue;
			}
			SplitNum* current_split = new SplitNum(args.getMaxChildren() - 1);
			current_split->addSplitValue(col_values[i]);
			current_split->setFeatureIndex(*it);
			current_split->computePartitionings(data, observations);
			if (this->checkMinNodeSize(current_split, args.getMinNodeSize())) {
				splits.push_back(current_split);
			}
		}
		col_values.clear();
	}
	return splits;
}


SplitGeneratorMultRand::SplitGeneratorMultRand() : SplitGenerator() {
	//
}

std::vector<Split*> SplitGeneratorMultRand::generate(Data* data, std::vector<int> observations, Arguments args) {

	std::vector<Split*> splits;
	int n_rows = data->nrows();
	int n_cols = data->ncols();
	splits.reserve(n_rows * n_cols);
	std::vector<int> col_ix_num, col_ix_categ, categ;
	col_ix_num.reserve(n_cols);
	col_ix_categ.reserve(n_cols);

	for (int j = 1; j < n_cols; j++) {
		// exclude first column with ID
		if (data->getColTypes()[j] == "num") {
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
		if (col == data->getTargetIndex()) {
			continue;
		}
		if (data->getNLevels(col) < args.getMaxChildren()) {
			continue;
		}
		std::vector<std::vector<std::vector<int>>> level_permuts = data->computeCategPermuts(col, args.getMaxChildren());
		int n_permuts = level_permuts.size();
		for (int i = 0; i < n_permuts; i++) {
			std::map<std::string, int> levels = data->getCategEncodings().at(col);
			for (int p = 0; p < n_permuts; p++) {
				SplitCateg* current_split = new SplitCateg(args.getMaxChildren()-1, levels);
				current_split->setFeatureIndex(col);
				current_split->setLevelPartitionings(level_permuts[p]);
				current_split->computePartitionings(data, observations);
				if (this->checkMinNodeSize(current_split, args.getMinNodeSize())) {
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
			current_split->computePartitionings(data, observations);
			if (this->checkMinNodeSize(current_split, args.getMinNodeSize())) {
				splits.push_back(current_split);
			}
		}
	}
	return splits;
}
