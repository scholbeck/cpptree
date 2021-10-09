#include "class_data.h"
#include "class_model.h"
#include "helper_functions.h"
#include "iostream"
#include <algorithm>
#include <cmath>

Model::Model() {
}

void Model::updateSet(Data* data, std::vector<int> rows, char setdiff) {
	for (int i = 0; i < rows.size(); ++i) {
		this->update(data, rows[i], setdiff);
	}
}

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

void ModelAverage::update(Data* data, int row, char setdiff) {
	if (setdiff == '+') {
		this->cumsum_target += data->row(row)[data->getTargetIndex()];
		this->n += 1;
		this->mean_target = cumsum_target / this->n;
	} else {
		this->cumsum_target -= data->row(row)[data->getTargetIndex()];
		this->n -= 1;
		this->mean_target = cumsum_target / this->n;
	}
}

double ModelAverage::predictSingle(Data* data, int row) {
	return this->mean_target;
}

std::string ModelAverage::generateModelInfo() {
	std::string str = std::string("mean y = ") + std::to_string(this->mean_target);
	return str;
}

/*
SINGLE FEATURE LINEAR REGRESSION MODEL
*/
ModelSingleFeatureLinReg::ModelSingleFeatureLinReg(int feature) : Model() {
	this->feature = feature;
	this->alpha = 0;
	this->beta = 0;
	this->n = 0;
	this->cumsum_feature = 0;
	this->cumsum_target = 0;
	this->cumsum_product = 0;
	this->cumsum_squared_feature = 0;
}

void ModelSingleFeatureLinReg::update(Data* data, int row, char setdiff) {
	if (setdiff == '+') {
		this->n += 1;
		this->cumsum_feature += data->elem(row, this->feature);
		this->cumsum_target += data->elem(row, data->getTargetIndex());
		this->cumsum_product += data->elem(row, this->feature) * data->elem(row, data->getTargetIndex()); 
		this->cumsum_squared_feature += pow(data->elem(row, this->feature), 2);

		this->computeCoefficients();
	} else {
		this->n -= 1;
		this->cumsum_feature -= data->elem(row, this->feature);
		this->cumsum_target -= data->elem(row, data->getTargetIndex());
		this->cumsum_product -= data->elem(row, this->feature) * data->elem(row, data->getTargetIndex()); 
		this->cumsum_squared_feature -= pow(data->elem(row, this->feature), 2);

		this->computeCoefficients();
	}
}

void ModelSingleFeatureLinReg::computeCoefficients() {
	this->beta = ((this->n * this->cumsum_product) - cumsum_feature - cumsum_target) / ((this->n * this->cumsum_squared_feature) - pow(this->cumsum_feature, 2));
	this->alpha = (this->cumsum_target / this->n) - (this->beta * (this->cumsum_feature / this->n));	
}

double ModelSingleFeatureLinReg::predictSingle(Data* data, int row) {
	return this->alpha + (data->elem(row, this->feature) * this->beta);
}

std::string ModelSingleFeatureLinReg::generateModelInfo() {
	std::string str = std::string("alpha = ") + std::to_string(this->alpha) + " | ";
	str += std::string("beta = ") + std::to_string(this->beta);
	return str;
}