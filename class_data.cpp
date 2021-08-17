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
		rows[i].push_back(col[i]);
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
	printf("\n");
	for (int i = 0; i < n_rows; ++i) {
		for (int j = 0; j < n_cols; j++) {
			printf("%f ", (this->rows[i])[j]);
		}
	printf("\n");
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
	for (int j = 1; j < this->ncols(); j++) {
		if (types[j] == "categ") {
			std::cout << "\t\tcolumn " << j << " : \n";
			levels = this->categ_encodings.at(j);
			for (auto it = levels.begin(); it != levels.end(); ++it) {
				std::cout << "\t\t\t" << it->first << " = " << it->second << "\n";
			}
		}
	}
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
	subset-> setColTypes(this->coltypes);
	for (int i = 0; i < n_rows_subset; ++i) {
		for (int j = 0; j < n_cols_subset; j++) {
			(subset->rows[i])[j] = this->elem(rows[i], cols[j]);
		}
	}
	return subset;
}



Data* Data::subsetRows(std::vector<int> rows) {
	std::vector<int> cols = initVectorSeq(0, this->ncols()-1);
	Data* subset_data = this->subset(rows, cols);
	return subset_data;
}

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

std::vector<std::vector<int>> Data::splitObs(Split split) {
	std::vector<std::vector<int>> split_multiway;
	if (split.getSplitType() == "num") {
		std::vector<double> split_values = split.getSplitValues();
		// split values have to be sorted and duplicates removed in split first!
		int n_splits = split_values.size();
		int feature = split.getSplitFeatureIndex();
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
	} else {
		split_multiway = this->splitCategObs(split.getSplitFeatureIndex());
	}
	return split_multiway;
}

std::vector<Data*>  Data::split(Split split) {
	
	std::vector<Data*>  split_multiway;
	if (split.getSplitType() == "num") {
		std::vector<double> split_values = split.getSplitValues();
		// split values have to be sorted and duplicates removed in split first!
		int n_splits = split_values.size();
		int feature = split.getSplitFeatureIndex();
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
		split_multiway = this->splitCateg(split.getSplitFeatureIndex());
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