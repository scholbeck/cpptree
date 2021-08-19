#include <sstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "class_data.h"
#include "class_split.h"
#include "iostream"
#include <algorithm>
#include <iomanip>

Split::Split(int max_splits) {
	this->feature_index = -1;
	this->max_splits = max_splits;
	this->split_values.reserve(max_splits);
	for (int i = 0; i <= max_splits; ++i) {
		std::vector<int> v(0, 1);
		this->splitted_obs.push_back(v);
	}
}

void Split::sortSplitValues() {
	std::sort(this->split_values.begin(), this->split_values.end());
	this->split_values.erase(unique(this->split_values.begin(), this->split_values.end()), this->split_values.end()); // remove duplicate split values
}
void Split::addSplitValue(double value) {
	(this->split_values).push_back(value);
}

std::vector<double> Split::getSplitValues() {
	return this->split_values;
}

void Split::addChildNodeModel(Model* mod) {
	this->child_node_models.push_back(mod);
}

std::vector<Model*> Split::getChildNodeModels() {
	return this->child_node_models;
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
	this->split_values.reserve(this->max_splits);
}

/*
void Split::summary() {
	std::cout << "SPLIT SUMMARY\n";
	std::cout << "\tsplit feature : " << this->getSplitFeatureIndex() << "\n";
	std::cout << "\tsplit type " << split_type << "\n";
	if (this->split_type == "num") {
		std::cout << "\tsplit values : ";
		int n_splits = this->split_values.size();
		std::cout << "\t";
		for (int i = 0; i < n_splits; ++i) {
			std::cout << this->split_values[i] << " ";
		}
	}
	std::cout << "\n";
}
*/

SplitNum::SplitNum(int max_splits) : Split(max_splits) {

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


SplitCateg::SplitCateg(int max_splits, std::map<std::string, int> levels) : Split(max_splits) {
	this->levels = levels;
}

std::string SplitCateg::createDecisionRule(int child_ix) {	
	int feature = this->getSplitFeatureIndex();
	std::string rule = std::string("x") + std::to_string(feature) + std::string(" = ") ;
	std::ostringstream sstream;
	sstream << std::setprecision(2) << std::fixed;
	int n_splits = this->getSplitValues().size();
	//for (int i = 0; i < this->subset_level_sets.size(); i++) {
	int n_subset_size = subset_level_sets[child_ix].size();
	for (int j = 0; j < (n_subset_size - 1); j++) {
			rule += std::to_string(subset_level_sets[child_ix][j]) + " | ";
	}
	rule += std::to_string(subset_level_sets[child_ix][n_subset_size]);
	//}
	return rule;
}

