#include <sstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "class_data.h"
#include "class_split.h"
#include "class_splitdifference.h"
#include "iostream"
#include "helper_functions.h"
#include <algorithm>
#include <iomanip>

Split::Split(int max_splits) {
	this->feature_index = -1;
	this->max_splits = max_splits;
	for (int l = 0; l <= max_splits; l++) {
		std::vector<int> subset_obs;
		this->split_obs.push_back(subset_obs);
	}
}

int Split::getNumberChildNodes() {
	return (this->split_obs.size());
}

void Split::setObjValues(std::vector<double> obj_values) {
	this->obj_values = obj_values;
}
std::vector<double> Split::getObjValues() {
	return this->obj_values;
}

void Split::setModelInfo(std::vector<std::string> model_info) {
	this->model_info = model_info;
}
std::vector<std::string> Split::getModelInfo() {
	return this->model_info;
}


void Split::sortSplitValues() {
	std::sort(this->split_values.begin(), this->split_values.end());
	this->split_values.erase(unique(this->split_values.begin(), this->split_values.end()), this->split_values.end()); // remove duplicate split values
}

void Split::addSplitValue(double value) {
	(this->split_values).push_back(value);
	std::sort(this->split_values.begin(), this->split_values.end());
}

void Split::setFeatureIndex(int feature_index) {
	this->feature_index = feature_index;
}

int Split::getSplitFeatureIndex() {
	return this->feature_index;
}

void Split::clear() {
	this->feature_index = -1;
	this->split_values.clear();
	this->subset_level_sets.clear();
}

std::vector<double> Split::getSplitValues() {
  return this->split_values;
}

std::vector<std::vector<int>> Split::getLevelPartitionings() {
  return this->subset_level_sets;
}

void Split::setLevelPartitionings(std::vector<std::vector<int>> level_sets) {
  this->subset_level_sets = level_sets;
}

SplitNum::SplitNum(int max_splits) : Split(max_splits) {
	this->split_values.reserve(max_splits);
}

std::string SplitNum::getSplitType() {
  return(std::string("num"));
}

std::string SplitNum::createDecisionRule(int child_ix) {	
	int feature = this->getSplitFeatureIndex();
	std::string rule;
	std::ostringstream sstream;
	sstream << std::setprecision(2) << std::fixed;
	int n_splits = this->getSplitValues().size();
	// n split values = n+1 child nodes
	if (child_ix == 0) {
		sstream << this->getSplitValues()[0];
		rule = std::string("x") + std::to_string(feature) + std::string(" <= ") + sstream.str(); 
	} else if (child_ix == n_splits) {
		sstream << this->getSplitValues()[n_splits - 1];
		rule = std::string("x") + std::to_string(feature) + std::string(" > ") + sstream.str();
	} else {
		rule = std::string("x") + std::to_string(feature) + std::string(" ∈ ") + std::string(" ]");
		sstream << this->getSplitValues()[child_ix - 1];
		rule += sstream.str() + std::string(" , ");
		sstream.str(std::string());
		sstream.clear();
		// clear string stream
		sstream << this->getSplitValues()[child_ix];
		rule += sstream.str() + std::string("]"); 
	}
	return rule;
}

void SplitNum::computePartitionings(Data* data, std::vector<int> observations) {
	int n_splits = this->split_values.size();
	int n_obs = observations.size();
	
	// split values need to be sorted in ascending order
	bool rightmost_node = true;
	for (auto it = observations.begin(); it != observations.end(); ++it) {
		for (int j = 0; j < n_splits; ++j) {
			if (data->elem(*it, this->getSplitFeatureIndex()) <= this->split_values[j]) {
				this->split_obs[j].push_back(*it);
				rightmost_node = false;
				break;
			}
		}
		if (rightmost_node == true) {
			this->split_obs[n_splits].push_back(*it);
		}
		rightmost_node = true;
	}
}

SplitCateg::SplitCateg(int max_splits, std::map<std::string, int> levels) : Split(max_splits) {
	this->levels = levels;
	this->subset_level_sets.reserve(max_splits);
}

std::string SplitCateg::getSplitType() {
  return(std::string("categ"));
}

std::string SplitCateg::createDecisionRule(int child_ix) {	
	int feature = this->getSplitFeatureIndex();
	std::string rule = std::string("x") + std::to_string(feature) + std::string(" = ") ;
	std::ostringstream sstream;
	sstream << std::setprecision(2) << std::fixed;
	int n_subset_size = subset_level_sets[child_ix].size();
	rule += std::to_string(subset_level_sets[child_ix][0]);
	for (int j = 1; j < n_subset_size; j++) {
			rule += " | " + std::to_string(subset_level_sets[child_ix][j]);
	}
	return rule;
}

void SplitCateg::computePartitionings(Data* data, std::vector<int> observations) {
	int n_obs = observations.size();
	int n_subsets = this->subset_level_sets.size();
	int n_levels_per_subset;
	int row;
	for (int s = 0; s < n_subsets; s++) {
		n_levels_per_subset = subset_level_sets[s].size();
		for (int l = 0; l < n_levels_per_subset; l++) {
			for (int i = 0; i < n_obs; i++) {
				row = observations[i];
				if (data->elem(row, this->getSplitFeatureIndex()) == this->subset_level_sets[s][l]) {
					this->split_obs[s].push_back(row);
				}
			}
		}
	}
}
