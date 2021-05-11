#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include "class_data.h"
#include <deque>
#include <ctime>
#include <algorithm>

Data::Data() {
	
}

lluint Data::getTargetIndex() {
	return this->target_index;
}
void Data::setTargetIndex(lluint target) {
	this->target_index = target;
}

void Data::addRow(std::deque<double> row) {
	// check dimensions before appending row
	rows.push_back(row);
}
void Data::addCol(std::deque<double> col) {
	// check dimensions before appending col
	lluint n_rows = this->nrows();
	for (lluint i = 0; i < n_rows; i++) {
		rows[i].push_back(col[i]);
	}
}

double Data::elem(lluint row_index, lluint col_index) {
	return (this->row(row_index))[col_index];
}

lluint Data::nrows() {
	return this->rows.size();
}

lluint Data::ncols() {
	return this->row(0).size();
}

std::deque<double> Data::row(lluint i) {
	return this->rows[i];
}

std::deque<double> Data::col(lluint j) {
	std::deque<double> c;
	lluint n_rows = this->nrows();
	for (lluint i = 0; i < n_rows; i++) {
		c.push_back(this->row(j)[i]);
	}
	return c;
}

void Data::init(lluint n_rows, lluint n_cols) {
	std::deque<std::deque<double>> vec(n_rows, std::deque<double>(n_cols));
	this->rows = vec;
}

void Data::initRandom(lluint n_rows, lluint n_cols) {
	this->init(n_rows, n_cols);
	
	for (lluint i = 0; i < n_rows; i++) {
		for (lluint j = 0; j < n_cols; j++) {
			(this->rows[i])[j] = std::rand();
		}
	}
}

void Data::print() {
	lluint n_rows = this->nrows();
	lluint n_cols = this->ncols();
	printf("\n");
	for (lluint i = 0; i < n_rows; i++) {
		for (lluint j = 0; j < n_cols; j++) {
			printf("%f ", (this->rows[i])[j]);
		}
	printf("\n");
	}
}

void Data::summary() {
	this->print();
	printf("\nObject summary:\nData frame of dimension %lld x %lld\n", this->nrows(), this->ncols());
}

Data Data::subset(std::deque<lluint> rows, std::deque<lluint> cols) {

	std::sort(cols.begin(), cols.end());
	Data subset;
	lluint n_rows = rows.size();
	lluint n_cols = cols.size();
	std::deque<double> subset_row;
	std::deque<double> subset_row_col;
	lluint row_temp, col_temp;
	std::deque<lluint> cols_cpy;
	for (lluint i = 0; i < n_rows; i++) {
		row_temp = rows.front();
		rows.pop_front();
		subset_row = this->row(row_temp);
		cols_cpy = cols;
		for (lluint j = 0; j < n_cols; j++) {
			col_temp = cols_cpy.front();
			cols_cpy.pop_front();
			subset_row_col.push_back(subset_row[col_temp]);
		}
		subset.addRow(subset_row_col);
		subset_row_col.clear();
	}
	
	return subset;
}


std::deque<lluint> initDequeSeq(lluint from, lluint to) {
	std::deque<lluint> seq;
	for (lluint i = from; i < to; i++) {
		seq.push_back(i);
	}
	return seq;
}

std::vector<Data> Data::splitBinary(double split_value, lluint col_index) {
	std::deque<lluint> rows_left;
	std::deque<lluint> rows_right;
	std::vector<Data> data_partitioned;
	
	double element;
	lluint n_elements = this->nrows();
	
	for (lluint i = 0; i < n_elements; i++) {
		element = this->elem(i, col_index);
		if (element <= split_value) {
			rows_left.push_back(i);
		} else {
			rows_right.push_back(i);
		}
	}
	
	std::deque<lluint> cols = initDequeSeq(0, this->ncols()); // init deque from 0 to highest column index
	Data subset_left = this->subset(rows_left, cols);
	Data subset_right = this->subset(rows_right, cols);
	data_partitioned.push_back(subset_left);
	data_partitioned.push_back(subset_right);
	
	return data_partitioned;
}


std::vector<Data> Data::split(Split split) {
	
	std::vector<double> split_values = split.getSplitValues();
	std::sort(split_values.begin(), split_values.end());
	int n_splits = split_values.size();
	lluint feature = split.getSplitFeatureIndex();
	std::vector<Data> split_multiway;
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
	
	return split_multiway;
}
