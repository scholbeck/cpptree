#include <stdio.h>
#include <stdlib.h>
#include <armadillo>
#include <getopt.h>
#include <iostream>
#include "class_data.h"
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
