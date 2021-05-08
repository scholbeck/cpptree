#include <stdio.h>
#include <stdlib.h>
#include <armadillo>
#include <getopt.h>
#include <iostream>
#include "class_data.h"
#include "class_split.h"
#include "helper_functions.h"
#include <vector>

Data::Data() {
	
}

void Data::setData(arma::mat mat) {
	this->data = mat;
}

void Data::setTargetIndex(lluint target) {
	this->target_index = target;
}

arma::mat Data::getData() {
	return this->data;
}

double Data::elem(lluint row_index, lluint col_index) {
	return (this->data)(row_index, col_index);
}

lluint Data::nrows() {
	return this->getData().n_rows;
}

lluint Data::ncols() {
	return this->getData().n_cols;
}

std::vector<double> Data::row(lluint i) {
	std::vector<double> stdvec = arma::conv_to<std::vector<double>>::from((this->data).row(i));
	return stdvec;
}

std::vector<double> Data::col(lluint j) {
	std::vector<double> stdvec = arma::conv_to<std::vector<double>>::from((this->data).col(j));
	return stdvec;
}

lluint Data::getTargetIndex() {
	return this->target_index;
}

void Data::load(std::string filename) {
	(this->data).load(filename);
}

void Data::print() {
	(this->data).print();
}

double Data::rowMean(lluint row_index) {
	return arma::mean((this->data).row(row_index));
}

double Data::colMean(lluint col_index) {
	return arma::mean((this->data).col(col_index));
}

Data Data::subset(std::vector<lluint> rows, std::vector<lluint> cols) {
	arma::uvec arma_rows = arma::uvec(rows);
	arma::uvec arma_cols = arma::uvec(cols);	
	
	arma::mat subset_mat = (this->getData()).submat(arma_rows, arma_cols);
	Data subset_data;
	subset_data.setData(subset_mat);
	subset_data.setTargetIndex(this->getTargetIndex());
	
	return subset_data;
}

std::vector<Data> Data::splitBinary(lluint row_index, lluint col_index) {
	std::vector<lluint> rows_left;
	std::vector<lluint> rows_right;
	std::vector<Data> data_partitioned;
	
	double element;
	double split_point = this->elem(row_index, col_index);
	lluint n_elements = this->nrows();
	
	for (lluint i = 0; i < n_elements; i++) {
		element = this->elem(i, col_index);
		if (element <= split_point) {
			rows_left.push_back(i);
		} else {
			rows_right.push_back(i);
		}
	}
	
	std::vector<lluint> cols = initVectorLLUINT(0, (this->ncols()) - 1); // init vector from 0 to highest column index
	Data subset_left = this->subset(rows_left, cols);
	Data subset_right = this->subset(rows_right, cols);
	data_partitioned.push_back(subset_left);
	data_partitioned.push_back(subset_right);
	
	return data_partitioned;
}


std::vector<Data> Data::split(Split split) {
	
	std::vector<double> split_values = split.getSplitValues();
	int n_splits = split_values.size();
	lluint feature = split.getSplitFeatureIndex();
	std::vector<Data> split_multiway;
	std::vector<Data> split_binary;
	
	split_binary = this->splitBinary(split_values[0], feature);
	split_multiway.insert(split_multiway.end(), split_binary.begin(), split_binary.end());
	for (int i = 1; i < n_splits; i++) {
		split_binary = (split_binary[1]).splitBinary(split_values[i], feature);
		split_multiway.insert(split_multiway.end(), split_binary.begin(), split_binary.end());
	}
	
	return split_multiway;
}

