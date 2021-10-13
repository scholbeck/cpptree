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


SplitGenerator::SplitGenerator() {}

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

// ---------------
// SECOND INHERITANCE LEVEL
// ---------------
// BATCH SPLIT GENERATORS

SplitGeneratorBatch::SplitGeneratorBatch() {}

SplitGeneratorBatchBinExh::SplitGeneratorBatchBinExh() : SplitGeneratorBatch() {}

std::vector<std::unique_ptr<Split>> SplitGeneratorBatchBinExh::generate(Data* data, std::vector<int> observations, std::string ID, Arguments* args) {
	
	int n_rows = observations.size();
	int n_min = args->getMinNodeSize();
	std::vector<std::unique_ptr<Split>> splits;
	int n_cols = data->ncols();
	splits.reserve(n_rows);
	std::array<std::vector<int>, 2> coltypes = data->getColTypesNumCateg();
	int col;
	// numeric features
	for (auto it_col = coltypes[0].begin(); it_col != coltypes[0].end(); ++it_col) {
		if (*it_col == data->getTargetIndex()) {
			continue;
		}
		SortedFeatureSubset* sorted_subset = data->sorted_data->getSortedFeatureSubset(ID, *it_col);
		for (int i = (n_min - 1); i < (n_rows - n_min); ++i) {
			if (sorted_subset->sorted_values[i].first == sorted_subset->sorted_values[i-1].first) {
				continue;
			}
			std::unique_ptr<Split> current_split = std::unique_ptr<SplitNum>(new SplitNum(args->getMaxChildren() - 1));
			current_split->addSplitValue(sorted_subset->sorted_values[i].first);
			current_split->setFeatureIndex(*it_col);
			current_split->computePartitionings(data, observations);
			if (this->checkMinNodeSize(current_split.get(), args->getMinNodeSize())) {
				splits.push_back(std::move(current_split));
			}
		}
	}
	// categorical features
	for (auto it_col = coltypes[1].begin(); it_col != coltypes[1].end(); ++it_col) {
		if (*it_col == data->getTargetIndex()) {
			continue;
		}
		if (data->getNLevels(*it_col) == 1) {
			continue;
		}
		std::vector<std::vector<std::vector<int>>> level_permuts = data->computeCategPermuts(*it_col, args->getMaxChildren());
		int n_permuts = level_permuts.size();
		for (int i = 0; i < n_permuts; i++) {
			std::map<std::string, int> levels = data->getCategEncodings().at(*it_col);
			for (int p = 0; p < n_permuts; p++) {
				std::unique_ptr<Split> current_split = std::unique_ptr<SplitCateg>(new SplitCateg(args->getMaxChildren() - 1, levels));
				current_split->setFeatureIndex(*it_col);
				current_split->setLevelPartitionings(level_permuts[p]);
				current_split->computePartitionings(data, observations);
				if (this->checkMinNodeSize(current_split.get(), args->getMinNodeSize())) {
					splits.push_back(std::move(current_split));
				}
			}
		}
	}
	return splits;
}


SplitGeneratorBatchMultRand::SplitGeneratorBatchMultRand() : SplitGeneratorBatch() {}

std::vector<std::unique_ptr<Split>> SplitGeneratorBatchMultRand::generate(Data* data, std::vector<int> observations, std::string ID, Arguments* args) {

	std::vector<std::unique_ptr<Split>> splits;
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
		if (data->getNLevels(col) < args->getMaxChildren()) {
			continue;
		}
		std::vector<std::vector<std::vector<int>>> level_permuts = data->computeCategPermuts(col, args->getMaxChildren());
		int n_permuts = level_permuts.size();
		for (int i = 0; i < n_permuts; i++) {
			std::map<std::string, int> levels = data->getCategEncodings().at(col);
			for (int p = 0; p < n_permuts; p++) {
				std::unique_ptr<Split> current_split = std::unique_ptr<SplitCateg>(new SplitCateg(args->getMaxChildren()-1, levels));
				current_split->setFeatureIndex(col);
				current_split->setLevelPartitionings(level_permuts[p]);
				current_split->computePartitionings(data, observations);
				if (this->checkMinNodeSize(current_split.get(), args->getMinNodeSize())) {
					splits.push_back(std::move(current_split));
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
			std::unique_ptr<Split> current_split = std::unique_ptr<SplitNum>(new SplitNum(args->getMaxChildren() - 1));
			current_split->setFeatureIndex(col);
			std::vector<int> rnd_obs_vec;
			for (int i = 0; i < args->getMaxChildren() - 1; i++) {
				rnd_obs_vec.push_back(distr_row(gen));
				std::sort(rnd_obs_vec.begin(), rnd_obs_vec.end());
				rnd_obs_vec.erase(std::unique(rnd_obs_vec.begin(), rnd_obs_vec.end()), rnd_obs_vec.end());
			}
			if ((int) rnd_obs_vec.size() == args->getMaxChildren() - 1) {
				for (int i = 0; i < args->getMaxChildren() - 1; i++) {
					current_split->addSplitValue(rnd_obs_vec[i]);
				}
			}
			current_split->computePartitionings(data, observations);
			if (this->checkMinNodeSize(current_split.get(), args->getMinNodeSize())) {
				splits.push_back(std::move(current_split));
			}
		}
	}
	return splits;
}

// ---------------
// SECOND INHERITANCE LEVEL
// ---------------
// STREAM SPLIT GENERATORS
SplitGeneratorStream::SplitGeneratorStream() {}

SplitGeneratorStreamBayesianOptim::SplitGeneratorStreamBayesianOptim() : SplitGeneratorStream() {}

std::unique_ptr<Split> SplitGeneratorStreamBayesianOptim::generate(double prev_obj_value, Data* data, std::vector<int> observations, std::string ID, Arguments* args) {
	std::unique_ptr<Split> s;
	return s;
}