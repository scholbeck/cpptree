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
	for (int i = 0; i < n; i++) {
		this->addRow(row_vec[i]);
	} 
}

void Data::addCol(std::vector<double> col) {
	// check dimensions before appending col
	int n_rows = this->nrows();
	for (int i = 0; i < n_rows; i++) {
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
	return (this->row(row_index))[col_index];
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

std::vector<double> Data::col(int j) {
	std::vector<double> c;
	int n_rows = this->nrows();
	for (int i = 0; i < n_rows; i++) {
		c.push_back(this->row(i)[j]);
	}
	return c;
}

void Data::init(int n_rows, int n_cols) {
	std::vector<std::vector<double>> vec(n_rows, std::vector<double>(n_cols, 0));
	this->rows = vec;
}

void Data::initRandom(int n_rows, int n_cols) {
	this->init(n_rows, n_cols);
	
	for (int i = 0; i < n_rows; i++) {
		for (int j = 0; j < n_cols; j++) {
			(this->rows[i])[j] = std::rand();
		}
	}
}

void Data::print() {
	int n_rows = this->nrows();
	int n_cols = this->ncols();
	printf("\n");
	for (int i = 0; i < n_rows; i++) {
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
	std::cout << "| ";
	for (int j = 0; j < this->ncols(); j++) {
		std::cout << "(" << j << " : "<< types[j] << ") | ";
	}
	std::cout << "\n";
	std::cout << "\tcategorical encodings :\n";
	for (int j = 0; j < this->ncols(); j++) {
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

Data Data::subset(std::vector<int> rows, std::vector<int> cols) {

	std::sort(cols.begin(), cols.end());
	int n_rows_subset = rows.size();
	int n_cols_subset = cols.size();
	Data subset;
	subset.init(n_rows_subset, n_cols_subset);
	subset.setCategEncodings(this->categ_encodings);
	subset.setTargetIndex(this->target_index);
	subset.setColTypes(this->coltypes);
	for (int i = 0; i < n_rows_subset; i++) {
		for (int j = 0; j < n_cols_subset; j++) {
			(subset.rows[i])[j] = this->elem(rows[i], cols[j]);
		}
	}
	return subset;
}


std::vector<Data> Data::splitCateg(int col_index) {
	std::vector<int> cols = initVectorSeq(0, (this->ncols()) - 1); // init vector from 0 to highest column index
	std::vector<int> level_rows;
	std::vector<Data> data_partitioned;
	std::map<std::string, int> levels = this->categ_encodings.at(col_index);
	double element;
	int n_levels = levels.size();
	int level;
	int n_elements = this->nrows();
	
	for (auto it = levels.begin(); it != levels.end(); ++it) {
		level = it->second;
		for (int i = 0; i < n_elements; i++) {
			element = this->elem(i, col_index);
			if (element == level) {
				level_rows.push_back(i);
			}
		}
		Data subset = this->subset(level_rows, cols);
		data_partitioned.push_back(subset);
		level_rows.clear();
	}
	return data_partitioned;
}

std::vector<Data> Data::splitBinary(double split_value, int col_index) {
	std::vector<int> rows_left;
	std::vector<int> rows_right;
	std::vector<Data> data_partitioned;
	
	double element;
	int n_elements = this->nrows();
	
	for (int i = 0; i < n_elements; i++) {
		element = this->elem(i, col_index);
		if (element <= split_value) {
			rows_left.push_back(i);
		} else {
			rows_right.push_back(i);
		}
	}
	std::vector<int> cols = initVectorSeq(0, (this->ncols()) - 1); // init vector from 0 to highest column index
	Data subset_left = this->subset(rows_left, cols);
	Data subset_right = this->subset(rows_right, cols);
	data_partitioned.push_back(subset_left);
	data_partitioned.push_back(subset_right);
	
	return data_partitioned;
}


std::vector<Data> Data::split(Split split) {
	
	std::vector<Data> split_multiway;
	if (split.getSplitType() == "num") {
		std::vector<double> split_values = split.getSplitValues();
		// split values have to be sorted and duplicates removed in split first!
		int n_splits = split_values.size();
		int feature = split.getSplitFeatureIndex();
		std::vector<Data> split_binary;
		split_binary = this->splitBinary(split_values[0], feature);
		split_multiway.push_back(split_binary[0]);
		split_multiway.push_back(split_binary[1]);
		
		for (int i = 1; i < n_splits; i++) {
			split_binary = (split_binary[1]).splitBinary(split_values[i], feature); // split last element in two
			split_multiway.pop_back(); // remove last element which was split in two
			split_multiway.push_back(split_binary[0]); // add resulting splits
			split_multiway.push_back(split_binary[1]);
		}
	} else {
		split_multiway = this->splitCateg(split.getSplitFeatureIndex());
	}
	return split_multiway;
}
