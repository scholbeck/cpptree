#include <stdio.h>
#include <stdlib.h>
#include <armadillo>
#include <getopt.h>
#include <iostream>
#include "class_data.h"


Data::Data(arma::mat* mat) {
  this->data = mat;
}

arma::mat* Data::getData() {
  return this->data;
}

void Data::setData(arma::mat* data) {
	this->data = data;
}

void Data::load(std::string filename) {
	(this->data)->load(filename);
}

void Data::print() {
	(this->data)->print();
}
