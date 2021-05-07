#include <stdio.h>
#include <stdlib.h>
#include <armadillo>
#include <getopt.h>
#include <iostream>
#include "class_data.h"
#include "class_split.h"
#include <vector>



Data::Data(arma::mat* mat, lluint target_index) {
  this->data = mat;
  this->n_rows = mat->n_rows;
  this->n_cols = mat->n_cols;
  this->target_index = target_index;
}

arma::mat* Data::getData() {
  return this->data;
}

double Data::elem(lluint row_index, lluint col_index) {
	return (this->data)->at(row_index, col_index);
}

lluint Data::nrows() {
	return this->n_rows;
}

lluint Data::ncols() {
	return this->n_cols;
}

std::vector<double> Data::row(lluint i) {
	std::vector<double> stdvec = arma::conv_to<std::vector<double>>::from((this->data)->row(i));
	return stdvec;
}

std::vector<double> Data::col(lluint j) {
	std::vector<double> stdvec = arma::conv_to<std::vector<double>>::from((this->data)->col(j));
	return stdvec;
}

lluint Data::getTargetIndex() {
	return this->target_index;
}

void Data::load(std::string filename) {
	(this->data)->load(filename);
}

void Data::print() {
	(this->data)->print();
}

double Data::rowMean(lluint row_index) {
	return arma::mean((this->data)->row(row_index));
}

double Data::colMean(lluint col_index) {
	return arma::mean((this->data)->col(col_index));
}

Data* Data::subset(std::vector<lluint> rows, std::vector<lluint> cols) {
	
	arma::uvec arma_rows = arma::uvec(rows);
	arma::uvec arma_cols = arma::uvec(cols);	
	
	arma::mat subset_mat = (this->getData())->submat(arma_rows, arma_cols);
	
	Data* data_subset = new Data(&subset_mat, this->getTargetIndex());
	return data_subset;
}


/*
std::vector<Data*> Data::splitBinary(int feature_index, int row_index) {
	std::vector<int> indices_left;
	std::vector<int> indices_right;
	std::vector<Data*> data_partitioned;
	int n_rows = (this->getData())->getNRows();
	double element;
	double split_point = this->getElement(row_index, feature_index);
	
	
	for (int i = 0; i < n_rows; i++) {
		element = this->getElement(i, feature_index);
		if (element <= split_point) {
			indices_left.push_back(i);
		} else {
			indices_right.push_back(i);
		}
	}
	
	std::vector<int> col_vector = initSizeTVector(0, feature_index);
	Data* subset_left = this->subset(indices_left, col_vector);
	Data* subset_right = this->subset(indices_right, col_vector);
	
	data_partitioned.push_back(subset_left);
	data_partitioned.push_back(subset_right);
	
	return data_partitioned;
}

std::vector<Data*> Data::split(Split split) {
	
	std::vector<int> splitpoints = split.getSplitpoints();
	int n_splits = splitpoints.size();
	sint feature = split.getSplitFeatureIndex();
	std::vector<Data*> split_multiway;
	std::vector<Data*> split_binary;
	
	split_binary = this->splitBinary(splitpoints[0]);
	split_multiway.insert(split_multiway.end(), split_binary.begin(), split_binary.end());
	
	for (int i = 1; i < n_splits; i++) {
		split_binary = (split_binary[1])->splitBinary(splitpoints[i]);
		split_multiway.insert(split_multiway.end(), split_binary.begin(), split_binary.end());
	}
	
	return split_multiway;
}
* 
*
* */
