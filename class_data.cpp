#include <stdio.h>
#include <stdlib.h>
#include <armadillo>
#include <getopt.h>
#include <iostream>
#include "class_data.h"
#include "class_split.h"
#include <vector>

Data::Data(arma::mat* mat, size_t target_index) {
  this->data = mat;
  this->nrows = mat->n_rows;
  this->ncols = mat->n_cols;
  this->target_index = target_index;
}

arma::mat* Data::getData() {
  return this->data;
}

void Data::setData(arma::mat* data) {
	this->data = data;
}

double Data::getElement(size_t i, size_t j) {
	return (this->data)->at(i, j);
}

double Data::getNRows() {
	return this->nrows;
}

double Data::getNCols() {
	return this->ncols;
}

std::vector<double> Data::getColumn(size_t j) {
	std::vector<double> stdvec = arma::conv_to<std::vector<double>>::from((this->data)->col(j));
	return stdvec;
}

std::vector<double> Data::getRow(size_t i) {
	std::vector<double> stdvec = arma::conv_to<std::vector<double>>::from((this->data)->row(i));
	return stdvec;
}

size_t Data::getTargetIndex() {
	return this->target_index;
}

void Data::load(std::string filename) {
	(this->data)->load(filename);
}

void Data::print() {
	(this->data)->print();
}

double Data::rowMean(size_t i) {
	return arma::mean((this->data)->row(i));
}

double Data::colMean(size_t j) {
	return arma::mean((this->data)->col(j));
}

std::vector<Data*> Data::splitBinary(size_t feature_index) {
	std::vector<Data*> data_partitioned;
	
	arma::mat* matrix_left = new arma::mat((this->data)->cols(0, feature_index));
	arma::mat* matrix_right = new arma::mat((this->data)->cols(feature_index + 1, this->getNCols()));
	
	Data* data_left = new Data(matrix_left, this->getTargetIndex());
	Data* data_right = new Data(matrix_right, this->getTargetIndex());
	
	data_partitioned.push_back(data_left);
	data_partitioned.push_back(data_right);
	
	return data_partitioned;
}

std::vector<Data*> Data::split(Split split) {
	
	std::vector<size_t> splitpoints = split.getSplitpoints();
	int n_splits = splitpoints.size();
	ssize_t feature = split.getSplitFeatureIndex();
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
