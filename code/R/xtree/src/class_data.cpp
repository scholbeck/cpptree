#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include "helper_functions.h"
#include "class_data.h"
#include <vector>
#include <map>
#include <ctime>
#include <algorithm>

Data::Data() {
	
}

bool Data::selfCheck() {
	bool ret = true;
	if ((this->target_index < 0) | (this->target_index > (this->ncols() - 1))) {
		ret = false;
	}
	return ret;
}

int Data::getTargetIndex() {
	return this->target_index;
}
void Data::setTargetIndex(int target) {
	this->target_index = target;
}

void Data::addRow(std::vector<double> row) {
	// check dimensions before appending row
	rows.push_back(row);
}

void Data::setCategEncodings(std::map<int, std::map<std::string, int>> categ_encodings) {
	this->categ_encodings = categ_encodings;
}

void Data::addCategEncoding(int col, std::map<std::string, int> m) {
	this->categ_encodings.insert(std::pair<int, std::map<std::string, int>> (col, m));
}

std::map<int, std::map<std::string, int>> Data::getCategEncodings() {
	return this->categ_encodings;
}

void Data::replaceRow(int row_ix, std::vector<double> values) {
	this->rows[row_ix] = values;
}

void Data::replaceCol(int col_ix, std::vector<double> values) {
  int n_rows = this->nrows();
  for (int i = 0; i < n_rows; i++) {
    this->setElem(values[i], i, col_ix);
  }
}

void Data::addRows(std::vector<std::vector<double>> row_vec) {
	// check dimensions before appending rows
	int n = row_vec.size();
	for (int i = 0; i < n; ++i) {
		this->addRow(row_vec[i]);
	} 
}

void Data::addCol(std::vector<double> col) {
	// check dimensions before appending col
	int n_rows = this->nrows();
	for (int i = 0; i < n_rows; ++i) {
		this->rows[i].push_back(col[i]);
	}
}

void Data::addCols(std::vector<std::vector<double>> col_vec) {
	// check dimensions before appending cols
	int n = col_vec.size();
	for (int j = 0; j < n; j++) {
		this->addCol(col_vec[j]);
	}
}

double Data::elem(int row_index, int col_index) {
	return this->rows[row_index][col_index];
}

void Data::setElem(double value, int row_index, int col_index) {
  this->rows[row_index][col_index] = value;
}

int Data::nrows() {
	return this->rows.size();
}

int Data::ncols() {
	return this->row(0).size();
}

std::vector<double> Data::row(int i) {
	return this->rows[i];
}

std::vector<double> Data::selectObs(int id) {
	int r;
	int n = this->nrows();
	for (int i = 0; i < n; ++i) {
		if (this->rows[i][0] == id) {
			r = i;
			break;
		}
	}
	return this->rows[r];
}

std::vector<double> Data::col(int j) {
	int n_rows = this->nrows();
	std::vector<double> c(n_rows, 0);
	for (int i = 0; i < n_rows; ++i) {
		c[i] = this->rows[i][j];
	}
	return c;
}

void Data::init(int n_rows, int n_cols) {
	this->rows.reserve(n_rows);
	std::vector<std::vector<double>> vec(n_rows, std::vector<double>(n_cols, 0));
	this->rows = vec;
}

void Data::initRandom(int n_rows, int n_cols) {
	this->init(n_rows, n_cols);
	
	for (int i = 0; i < n_rows; ++i) {
		for (int j = 0; j < n_cols; j++) {
			(this->rows[i])[j] = std::rand();
		}
	}
}

void Data::print() {
	int n_rows = this->nrows();
	int n_cols = this->ncols();
	std::cout << "\n";
	for (int i = 0; i < n_rows; ++i) {
		for (int j = 0; j < n_cols; j++) {
			std::cout << this->rows[i][j];
		}
	std::cout << "\n";
	}
}

void Data::sizeSummary() {
	std::cout << "DATA SUMMARY\n";
	std::cout << "\tmatrix of dimension : " << this->nrows() << " x " << this->ncols() << "\n";
}

void Data::summary() {
	std::cout << "DATA SUMMARY\n";
	std::cout << "\tmatrix of dimension : " << this->nrows() << " x " << this->ncols() << "\n";
	std::cout << "\ttarget : column " << this->getTargetIndex() << " \n";
	std::cout << "\tcolumn types : ";
	std::vector<std::string> types = this->getColTypes();
	
	std::map<std::string, int> levels;
	std::cout << "| (0 : ID) | ";
	for (int j = 1; j < this->ncols(); j++) {
		std::cout << "(" << j << " : "<< types[j] << ") | ";
	}
	
	std::cout << "\n";
	std::cout << "\tcategorical encodings :\n";
	/*
	for (int j = 1; j < this->ncols(); j++) {
		if (types[j] == "categ") {
			std::cout << "\t\tcolumn " << j << " : \n";
			levels = this->categ_encodings.at(j);
			for (auto it = levels.begin(); it != levels.end(); ++it) {
				std::cout << "\t\t\t" << it->first << " = " << it->second << "\n";
			}
		}
	}
	 */
}

void Data::setColTypes(std::vector<std::string> types) {
	this->coltypes = types;
}
std::vector<std::string> Data::getColTypes() {
	return this->coltypes;
}
/*
Data* Data::subset(std::vector<int> rows, std::vector<int> cols) {

	std::sort(rows.begin(), rows.end());
	//printVectorInt(rows);
	std::sort(cols.begin(), cols.end());
	//printVectorInt(cols);
	int n_rows = this->nrows();
	int n_cols = this->ncols();
	int n_rows_subset = rows.size();
	int n_cols_subset = cols.size();
	Data* subset;
	subset.init(n_rows_subset, n_cols_subset);
	subset.setCategEncodings(this->categ_encodings);
	subset.setTargetIndex(this->target_index);
	subset.setColTypes(this->coltypes);

	for (int i = 0; i < n_rows_subset; ++i) {
		for (int l = 0; i < n_rows; l++) {
			if (rows[i] == this->rows[l][0]) {
				for (int j = 0; j < n_cols_subset; j++) {
					//std::cout << i << rows[i] << " " << this->rows[l][j] << std::endl << std::flush;
					subset.rows[i][j] = this->rows[l][j];
				}
				break;
			}
		}
	}
	
	std::vector<std::map<double, int>> ordered_features_new;
	ordered_features_new.reserve(n_cols_subset);
	std::map<double, int> feature;
	for (int j = 0; j < n_cols_subset; j++) {
		feature = this->ordered_features[cols[j]];
		for (int i = 0; i < n_rows_subset; ++i) {
			feature.erase(rows[i]);
		}
		ordered_features_new.push_back(feature);
	}
	
	return subset;
}
*/
Data* Data::subset(std::vector<int> rows, std::vector<int> cols) {

	std::sort(cols.begin(), cols.end());
	int n_rows_subset = rows.size();
	int n_cols_subset = cols.size();
	Data* subset = new Data();
	subset->init(n_rows_subset, n_cols_subset);
	subset->setCategEncodings(this->categ_encodings);
	subset->setTargetIndex(this->target_index);
	subset->setColTypes(this->coltypes);
	subset->sorted_features = this->updateSortedValues(rows);
	for (int i = 0; i < n_rows_subset; ++i) {
		for (int j = 0; j < n_cols_subset; j++) {
			(subset->rows[i])[j] = this->elem(rows[i], cols[j]);
		}
	}
	return subset;
}

Data* Data::subsetRows(std::vector<int> rows) {
	std::vector<int> cols = initVectorSeq(0, this->ncols() - 1);
	Data* subset_data = this->subset(rows, cols);
	return subset_data;
}

std::vector<std::vector<int>> Data::splitBinaryObs(double split_value, int col_index) {
	int n_elements = this->nrows();
	std::vector<int> rows_left;
	std::vector<int> rows_right;
	rows_left.reserve(n_elements);
	rows_right.reserve(n_elements);
	std::vector<std::vector<int>> split_obs;
	
	for (int i = 0; i < n_elements; ++i) {
		if (this->elem(i, col_index) <= split_value) {
			rows_left.push_back(i);
		} else {
			rows_right.push_back(i);
		}
	}
	split_obs.push_back(rows_left);
	split_obs.push_back(rows_right);
	return split_obs;
}




/*
std::vector<Data*> Data::splitCateg(int col_index) {
	std::vector<int> cols = initVectorSeq(0, (this->ncols()) - 1); // init vector from 0 to highest column index
	std::vector<int> level_rows;
	std::vector<Data*> data_partitioned;
	std::map<std::string, int> levels = this->categ_encodings.at(col_index);
	int n_levels = levels.size();
	int level;
	int n_elements = this->nrows();
	
	for (auto it = levels.begin(); it != levels.end(); ++it) {
		level = it->second;
		for (int i = 0; i < n_elements; ++i) {
			if (this->elem(i, col_index) == level) {
				level_rows.push_back(i);
			}
		}
		Data* subset = this->subset(level_rows, cols);
		data_partitioned.push_back(subset);
		level_rows.clear();
	}
	return data_partitioned;
}

std::vector<Data*> Data::splitBinary(double split_value, int col_index) {
	std::vector<int> rows_left, rows_right;
	std::vector<Data*> data_partitioned;
	data_partitioned.resize(2);
	int n_elements = this->nrows();
	for (int i = 0; i < n_elements; ++i) {
		if (this->elem(i, col_index) <= split_value) {
			rows_left.push_back(i);
		} else {
			rows_right.push_back(i);
		}
	}
	data_partitioned[0] = this->subsetRows(rows_left);
	data_partitioned[1] = this->subsetRows(rows_right);	
	return data_partitioned;
}

std::vector<std::vector<int>> Data::splitCategObs(int col_index, std::vector<std::vector<int>> level_permuts) {
	
	int n_nodes = level_permuts.size();
	int n_elements = this->nrows();
	std::vector<int> level_rows;
	std::vector<std::vector<int>> obs_partitioned;
	for (int l = 0; l < n_nodes; l++) {
		int n_levels = level_permuts[l].size();
		for (int j = 0; j < n_levels; j++) {
			int level = level_permuts[l][j];
			for (int i = 0; i < n_elements; ++i) {
				if (this->elem(i, col_index) == level) {
					level_rows.push_back(i);
				}
			}
		}
		obs_partitioned.push_back(level_rows);
		level_rows.clear();
	}
	return obs_partitioned;
}

std::vector<std::vector<int>> Data::splitCategObs(int col_index) {
	std::vector<int> cols = initVectorSeq(0, (this->ncols()) - 1); // init vector from 0 to highest column index
	std::vector<int> level_rows;
	std::vector<std::vector<int>> obs_partitioned;
	std::map<std::string, int> levels = this->categ_encodings.at(col_index);
	
	int n_levels = levels.size();
	int level;
	int n_elements = this->nrows();
	
	for (auto it = levels.begin(); it != levels.end(); ++it) {
		level = it->second;
		for (int i = 0; i < n_elements; ++i) {
			if (this->elem(i, col_index) == level) {
				level_rows.push_back(i);
			}
		}
		obs_partitioned.push_back(level_rows);
		level_rows.clear();
	}
	return obs_partitioned;
}

std::vector<std::vector<int>> Data::splitBinaryObs(double split_value, int col_index) {
	int n_elements = this->nrows();
	std::vector<int> rows_left;
	std::vector<int> rows_right;
	rows_left.reserve(n_elements);
	rows_right.reserve(n_elements);
	std::vector<std::vector<int>> split_obs;
	
	for (int i = 0; i < n_elements; ++i) {
		if (this->elem(i, col_index) <= split_value) {
			rows_left.push_back(i);
		} else {
			rows_right.push_back(i);
		}
	}
	split_obs.push_back(rows_left);
	split_obs.push_back(rows_right);
	return split_obs;
}

std::vector<std::vector<int>> Data::splitObs(Split* split) {
	std::vector<std::vector<int>> split_multiway;
	std::vector<double> split_values = split->getSplitValues();
	// split values have to be sorted and duplicates removed in split first!
	int n_splits = split_values.size();
	int feature = split->getSplitFeatureIndex();
	std::vector<std::vector<int>> split_binary;
	split_binary = this->splitBinaryObs(split_values[0], feature);
	split_multiway.push_back(split_binary[0]);
	split_multiway.push_back(split_binary[1]);
	
	for (int i = 1; i < n_splits; ++i) {
		Data* right_subset = this->subsetRows(split_binary[1]);
		split_binary = right_subset->splitBinaryObs(split_values[i], feature); // split last element in two
		split_multiway.pop_back(); // remove last element which was split in two
		split_multiway.push_back(split_binary[0]); // add resulting splits
		split_multiway.push_back(split_binary[1]);
	}
	return split_multiway;
}


std::vector<Data*>  Data::split(Split* split) {
	
	std::vector<Data*>  split_multiway;
	if (split->getSplitType() == "num") {
		std::vector<double> split_values = split->getSplitValues();
		// split values have to be sorted and duplicates removed in split first!
		int n_splits = split_values.size();
		int feature = split->getSplitFeatureIndex();
		std::vector<Data*>  split_binary;
		split_binary = this->splitBinary(split_values[0], feature);
		split_multiway.push_back(split_binary[0]);
		split_multiway.push_back(split_binary[1]);
		for (int i = 1; i < n_splits; ++i) {
			split_binary = split_binary[1]->splitBinary(split_values[i], feature); // split last element in two
			split_multiway.pop_back(); // remove last element which was split in two
			split_multiway.push_back(split_binary[0]); // add resulting splits
			split_multiway.push_back(split_binary[1]);
		}
	} else {
		split_multiway = this->splitCateg(split->getSplitFeatureIndex());
	}
	return split_multiway;
}


void Data::orderFeatures() {
	std::map<double, int> feat;
	std::vector<double> id, values;
	int n_rows = this->nrows();
	int n_features = this->ncols()-2;
	for (int j = 0; j < n_features; j++) {
		values = this->col(j);
		id = this->col(0);
		for (int i = 0; i < n_rows; ++i) {
    		feat[id[i]] = values[i];
		}
	}
}

std::vector<std::vector<std::vector<int>>> Data::computeCategPermuts(int col_index, int n_nodes) {
	std::map<std::string, int> levels = this->categ_encodings.at(col_index);
	int n_levels = levels.size();
	std::vector<std::vector<std::vector<int>>> levels_partitioned;
	std::vector<int> levels_left;
	std::vector<int> levels_right;
	std::vector<std::vector<int>> levels_combined;
	for (int i = 0; i < n_levels; i++) {
		levels_left.push_back(i);
		for (int j = 0; j < n_nodes; j++) {
			if (i != j) {
				levels_right.push_back(j);
			}
		}
		levels_combined.push_back(levels_left);
		levels_combined.push_back(levels_right);
		levels_partitioned.push_back(levels_combined);
		levels_left.clear();
		levels_right.clear();
		levels_combined.clear();
	}
	return levels_partitioned;
}
*/

std::vector<std::vector<std::vector<int>>> Data::computeCategPermuts(int col_index, int n_subsets) {
	std::map<std::string, int> levels = this->categ_encodings.at(col_index);
	int n_levels = levels.size();
	std::vector<std::vector<std::vector<int>>> levels_partitioned;
	std::vector<std::vector<int>> single_partition;
	single_partition.reserve(n_subsets);
	std::vector<std::vector<int>> split_perms = permuteIndices(n_levels - 1, n_subsets - 1);
	int n_perms = split_perms.size();
	// split perms contains split points to partition level index sequence into subsets
	for (int i = 0; i < n_perms; i++) {
		// loop through each possible index permutation into subsets
		for (int j = 0; j < (n_subsets - 1); j++) {
			// for each index permutation, loop through all split points
			// for k child nodes, there are k-1 split points
			if (j == 0) {
				single_partition.push_back(initVectorSeq(0, split_perms[i][j]));
				// first split point creates level set from 0 to split point
			} else {
				single_partition.push_back(initVectorSeq(split_perms[i][j-1] + 1, split_perms[i][j]));
				// subsequent split points create level sets from previous to next split point
			}
		}
		single_partition.push_back(initVectorSeq(split_perms[i][n_subsets-2] + 1, n_levels - 1));
		// last split point creates level set from split point at index (n_subsets-1)-1 to largest level index at n_levels - 1
		levels_partitioned.push_back(single_partition);
		single_partition.clear();
	}
	return levels_partitioned;
}

int Data::getNLevels(int col) {
	std::map<std::string, int> levels = this->categ_encodings.at(col);
	return levels.size();
}



void Data::sortFeatures() {
  int n_cols = this->ncols();
  int n_rows = this->nrows();
  for (int j = 1; j < n_cols; j++) {
    if (j == this->getTargetIndex()) {
      continue;
    }
    std::vector<std::pair<double, int>> single_feature;
    for (int i = 0; i < n_rows; i++) {
      single_feature.push_back(std::make_pair(this->elem(i, j), this->elem(i, 0)));
    }
    std::sort(single_feature.begin(), single_feature.end());
    this->sorted_features.insert(std::pair<int, std::vector<std::pair<double, int>>>(j, single_feature));
  }
}

std::map<int, std::vector<std::pair<double, int>>> Data::updateSortedValues(std::vector<int> rows) {
  std::map<int, std::vector<std::pair<double, int>>> sorted_features_update;
  int n_cols = this->ncols();
  int n_rows_subset = rows.size();
  for (int j = 1; j < n_cols; j++) {
    if (j == this->getTargetIndex()) {
      continue;
    }
    int r;
    std::vector<std::pair<double, int>> single_feature_update;
    for (int i = 0; i < n_rows_subset; i++) {
      r = rows[i];
      single_feature_update.push_back(this->sorted_features.at(j)[r]);
    }
    // sorted_features_update.insert(j, single_feature);
    sorted_features_update.insert(std::pair<int, std::vector<std::pair<double, int>>>(j, single_feature_update));
  }
  return sorted_features_update;
}


std::vector<double> Data::getSortedFeatureValues(int col) {
  std::vector<double> values;
  std::vector<std::pair<double, int>> pairs = this->sorted_features.at(col);
  values.reserve(pairs.size());
  for (size_t it = 0; it < pairs.size(); ++it) {
    values.push_back(pairs[it].first);
  }
  return values;
}