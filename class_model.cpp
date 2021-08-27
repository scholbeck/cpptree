#include "class_data.h"
#include "class_model.h"
#include "helper_functions.h"
#include "iostream"
#include <algorithm>

Model::Model() {
	this->is_trained = false;
}

void Model::setTrainingData(Data* data) {
	this->training_data = data;
}

void Model::checkTrained() {
	if (this->is_trained == false) {
		perror("Model is not trained.");
	}
}

// mean target

ModelAverage::ModelAverage() : Model() {
	this->cumsum_target = 0;
	this->n = 0;
	this->mean_target = 0;
}

void ModelAverage::train() {
	int target_index = this->training_data->getTargetIndex();
	std::vector<double> target_values = this->training_data->col(target_index);
	this->cumsum_target = cumsum(target_values);
	this->n = target_values.size();
	this->mean_target = this->cumsum_target / this->n;
	this->is_trained = true;
}

void ModelAverage::update(std::vector<double> observation, char setdiff) {
	if (setdiff == '+') {
		this->cumsum_target += observation[this->training_data->getTargetIndex()];
		this->n += 1;
		this->mean_target = cumsum_target / this->n;
	} else {
		this->cumsum_target -= observation[this->training_data->getTargetIndex()];
		this->n -= 1;
		this->mean_target = cumsum_target / this->n;
	}
	this->is_trained = true;
}

void ModelAverage::summary() {
	std::cout << "model summary:\nclass <Average>\n";	
	std::cout << "mean target value = " << this->mean_target << "\n"; 
}
/*
std::string ModelAverage::getShortSummary() {	
	std::string s;
	s = std::string("y = ") + std::to_string(this->mean_target);
	return s;
}
*/
std::vector<double> ModelAverage::predictMult(Data* data) {
	this->checkTrained();
	int n = data->nrows();
	std::vector<double> predictions(n, mean_target);
	return predictions;
}

double ModelAverage::predictSingle(std::vector<double> observation) {
	this->checkTrained();
	return this->mean_target;
}

// linear regression

ModelLinearRegression::ModelLinearRegression() : Model() {
	this->learning_rate = 0.1;
	this->n = 0;
}

void ModelLinearRegression::train() {
	int n_cols = this->training_data->ncols();
	for (int j = 0; j < n_cols; j++) {
		if (j == this->training_data->getTargetIndex()) {
			continue;
		}
		this->params.insert({j, 0});
			// create feature-specific empty param
	}
	int target_index = this->training_data->getTargetIndex();
	std::vector<double> target_values = this->training_data->col(target_index);
	this->n = this->training_data->nrows();
	this->params.at(0) = -(this->n) * this->learning_rate;
	// update constant param
	for (int i = 0; i < this->n; i++) {
		for (int j = 1; j < n_cols; j++) {
			if (j == this->training_data->getTargetIndex()) {
				continue;
			}
			this->params.at(j) = this->params.at(j) - (this->learning_rate * this->training_data->elem(i, j));
			// update feature-specific param: derivative w.r.t. param = learning rate * observation
		}
	}
	this->is_trained = true;
}

void ModelLinearRegression::update(std::vector<double> observation, char setdiff) {	
	int n_cols = observation.size();
	if (setdiff == '+') {
		this->params.at(0) = this->params.at(0) - this->learning_rate;
		for (int j = 1; j < n_cols; j++) {
			if (j == this->training_data->getTargetIndex()) {
				continue;
			}
			this->params.at(j) = this->params.at(j) - this->learning_rate * observation[j];
			// update feature-specific param: derivative w.r.t. param = observation
		}
	} else {
		this->params.at(0) = this->params.at(0) + this->learning_rate;
		for (int j = 1; j < n_cols; j++) {
			if (j == this->training_data->getTargetIndex()) {
				continue;
			}
			this->params.at(j) = this->params.at(j) + this->learning_rate * observation[j];
			// update feature-specific param: derivative w.r.t. param = observation
		}
	}
}


std::vector<double> ModelLinearRegression::predictMult(Data* data) {
	this->checkTrained();
	int n = data->nrows();
	std::vector<double> predictions(n, 0);
	for (int i = 0; i < n; i++) {
		predictions[i] = this->predictSingle(data->row(i));
	}
	return predictions;
}

double ModelLinearRegression::predictSingle(std::vector<double> observation) {
	this->checkTrained();
	int n_cols = this->training_data->ncols();
	double pred = this->params.at(0);
	for (int j = 1; j < n_cols; j++) {
		if (j == this->training_data->getTargetIndex()) {
			continue;
		}
		pred += this->params.at(j) * observation[j];
		// update feature-specific param: derivative w.r.t. param = observation
	}
	return pred;
}

void ModelLinearRegression::summary() {
}

// ModelMajorityVote
/*
ModelMajorityVote::ModelMajorityVote() : Model() {
	//
}

void ModelMajorityVote::train() {
	int target_index = this->training_data->getTargetIndex();
	int n_obs = this->training_data->nrows();
	std::vector<double> target_values = this->training_data->col(target_index);
	std::map<std::string, int> levels = this->training_data->getCategEncodings().at(target_index);
	std::map<int, double> probs;
	int max_ix, l;
	double max_cnt, level_cnt;
	for (auto it = levels.begin(); it != levels.end(); ++it) {
		l = it->second; // mapped integer value of target level
		level_cnt = std::count(target_values.begin(), target_values.end(), l);
		probs.insert(std::pair<int, double> (l, (level_cnt / n_obs))); // insert pair of (level, count)	
		if (level_cnt > max_cnt) {
			max_cnt = level_cnt;
			max_ix = l;
		}
	}
	this->majority_class = max_ix;
	this->probs = probs;
	this->is_trained = true;
}

void ModelMajorityVote::summary() {
	std::cout << "MODEL SUMMARY\n\tclass <Majority Vote>\n";	
	std::cout << "\tmajority class: " << this->majority_class << "\n";
}

std::string ModelMajorityVote::getShortSummary() {	
	std::string s;
	s = std::string("class ") + std::to_string(this->majority_class);
	return s;
}

std::vector<double> ModelMajorityVote::predict(Data* data) {
	this->checkTrained();
	int n_obs = data->nrows();
	std::vector<double> predictions(n_obs, this->majority_class);
	return predictions;
}

*/