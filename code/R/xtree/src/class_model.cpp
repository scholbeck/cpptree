#include "class_data.h"
#include "class_model.h"
#include "helper_functions.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iomanip>

Model::Model() {}

std::vector<double> Model::predictMult(Data* data, std::vector<int> rows) {
	std::vector<double> predictions(rows.size());
	for (int i = 0; i < rows.size(); ++i) {
		predictions[i] = this->predictSingle(data, rows[i]);
	}
	return predictions;
}

/*
MEAN TARGET VALUE
*/
ModelAverage::ModelAverage() : Model() {
	this->cumsum_target = 0;
	this->n = 0;
	this->mean_target = 0;
}

void ModelAverage::update(Data* data, std::vector<int> rows, char setdiff) {
	int n_update= rows.size();
	if (setdiff == '+') {
		this->n += n_update;
		for (int i = 0; i < n_update; ++i) {	
			this->cumsum_target += data->elem(rows[i], data->getTargetIndex());
		}
	}
	if (setdiff == '-') {
		this->n -= n_update;
		for (int i = 0; i < n_update; ++i) {	
			this->cumsum_target -= data->elem(rows[i], data->getTargetIndex());
		}
	}
	this->mean_target = cumsum_target / this->n;
}

double ModelAverage::predictSingle(Data* data, int row) {
	return this->mean_target;
}

std::string ModelAverage::generateModelInfo() {
	std::ostringstream sstream;
	sstream << std::setprecision(2) << std::fixed; // printout with 2 decimal places
	sstream << "mean y = " << this->mean_target;
	return sstream.str();
}

/*
SINGLE FEATURE LINEAR REGRESSION MODEL
*/
ModelSingleFeatureLinReg::ModelSingleFeatureLinReg(int feature) : Model() {
	this->feature = feature;
	this->alpha = 0;
	this->beta = 0;
	this->n = 0;
	this->mean_x = 0;
	this->mean_y = 0;
	this->ss_cov = 0;
	this->ss_var = 0;
}

void ModelSingleFeatureLinReg::update(Data* data, std::vector<int> rows, char setdiff) {
	int n_update = rows.size();
	if (setdiff == '+') {
		for (int i = 0; i < n_update; ++i) {
			this->n += 1;
			double mean_x_prev = this->mean_x;

			this->mean_x = this->mean_x + ((data->elem(rows[i], this->feature) - this->mean_x) / n);
			this->mean_y = this->mean_y + ((data->elem(rows[i], data->getTargetIndex()) - this->mean_y) / n);

			this->ss_var = this->ss_var + ((data->elem(rows[i], this->feature) - mean_x_prev) * ((data->elem(rows[i], this->feature) - this->mean_x)));
			this->ss_cov = this->ss_cov + ((data->elem(rows[i], this->feature) - mean_x_prev) * ((data->elem(rows[i], data->getTargetIndex()) - this->mean_y)));
		}
	}
	if (setdiff == '-') {
		for (int i = 0; i < n_update; ++i) {
			this->n -= 1;
			double mean_x_prev = this->mean_x;
			
			this->mean_x = (n / (n - 1)) * (this->mean_x - (data->elem(rows[i], this->feature)));
			this->mean_y = (n / (n - 1)) * (this->mean_y - (data->elem(rows[i], data->getTargetIndex())));

			this->ss_var = this->ss_var - ((data->elem(rows[i], this->feature) - mean_x_prev) * ((data->elem(rows[i], this->feature) - this->mean_x)));
			this->ss_cov = this->ss_cov - ((data->elem(rows[i], this->feature) - mean_x_prev) * ((data->elem(rows[i], data->getTargetIndex()) - this->mean_y)));
		}
	}
	this->beta = this->ss_cov / this->ss_var;
	this->alpha = this->mean_y - (this->beta * this->mean_x);
}

double ModelSingleFeatureLinReg::predictSingle(Data* data, int row) {
	return (this->alpha + (data->elem(row, this->feature) * this->beta));
}

std::string ModelSingleFeatureLinReg::generateModelInfo() {
	std::ostringstream sstream;
	sstream << std::setprecision(2) << std::fixed; // printout with 2 decimal places
	sstream << "alpha = " << this->alpha << " | beta = " << this->beta;
	return sstream.str();
}