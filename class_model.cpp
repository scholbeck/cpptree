#include "class_data.h"
#include "class_model.h"
#include "helper_functions.h"
#include "iostream"
#include <algorithm>

Model::Model() {
	this->is_trained = false;
}

void Model::setTrainingData(Data data) {
	this->training_data = data;
}

void Model::checkTrained() {
	if (this->is_trained == false) {
		perror("Model is not trained.");
	}
}

double Model::evaluate(Data data, Objective* obj) {
	std::vector<double> target_pred = this->predict(data);
	return obj->compute(data);
}

// ModelAverage

ModelAverage::ModelAverage() : Model() {
	//
}

void ModelAverage::train() {
	int target_index = this->training_data.getTargetIndex();
	std::vector<double> target_values = this->training_data.col(target_index);
	this->cumsum_target = cumsum(target_values);
	this->n = target_values.size();
	this->mean_prediction = this->cumsum_target / this->n;
	this->is_trained = true;
}

void ModelAverage::update(std::array<std::vector<int>, 2> diff) {
	int n_setplus = diff[0].size();
	int n_setminus = diff[1].size();
	double element;
	for (int i = 0; i < n_setplus; i++) {
		this->n += n_setplus;
		element = this->training_data.elem(diff[0][i], this->training_data.getTargetIndex());
		this->cumsum_target += element;
		this->mean_prediction = cumsum_target / this->n;
	}
	for (int i = 0; i < n_setminus; i++) {
		this->n -= n_setplus;
		element = this->training_data.elem(diff[0][i], this->training_data.getTargetIndex());
		this->cumsum_target -= element;
		this->mean_prediction = cumsum_target / this->n;
	}
	this->is_trained = true;
}

void ModelAverage::summary() {
	std::cout << "model summary:\nclass <Average>\n";	
	std::cout << "mean target value = " << this->mean_prediction << "\n"; 
}

std::string ModelAverage::getShortSummary() {	
	std::string s;
	s = std::string("y = ") + std::to_string(this->mean_prediction);
	return s;
}

std::vector<double> ModelAverage::predict(Data data) {
	this->checkTrained();
	int n = data.nrows();
	std::vector<double> predictions(n, mean_prediction);
	return predictions;
}

// ModelMajorityVote

ModelMajorityVote::ModelMajorityVote() : Model() {
	//
}

void ModelMajorityVote::train() {
	int target_index = this->training_data.getTargetIndex();
	int n_obs = this->training_data.nrows();
	std::vector<double> target_values = this->training_data.col(target_index);
	std::map<std::string, int> levels = this->training_data.getCategEncodings().at(target_index);
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

std::vector<double> ModelMajorityVote::predict(Data data) {
	this->checkTrained();
	int n_obs = data.nrows();
	std::vector<double> predictions(n_obs, this->majority_class);
	return predictions;
}

