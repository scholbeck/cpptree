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
int Split::nsplits() {
	return this->max_splits;
}
void Split::sortSplitValues() {
	std::sort(this->split_values.begin(), this->split_values.end());
	this->split_values.erase(unique(this->split_values.begin(), this->split_values.end()), this->split_values.end()); // remove duplicate split values
}
void Split::addSplitValue(double value) {
	(this->split_values).push_back(value);
	std::sort(this->split_values.begin(), this->split_values.end());
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

void SplitNum::computePartitionings(Data* data) {
	std::vector<std::vector<int>> split_multiway;
	std::vector<double> split_values = this->getSplitValues();
	// split values have to be sorted and duplicates removed in split first!
	int n_splits = split_values.size();
	int feature = this->getSplitFeatureIndex();
	std::vector<std::vector<int>> split_binary;
	split_binary = data->splitBinaryObs(split_values[0], feature);
	split_multiway.push_back(split_binary[0]);
	split_multiway.push_back(split_binary[1]);
	for (int i = 1; i < n_splits; ++i) {
		Data* right_subset = data->subsetRows(split_binary[1]);
		split_binary = right_subset->splitBinaryObs(split_values[i], feature); // split last element in two
		split_multiway.pop_back(); // remove last element which was split in two
		split_multiway.push_back(split_binary[0]); // add resulting splits
		split_multiway.push_back(split_binary[1]);
	}
	this->splitted_obs = split_multiway;
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
	int n_subset_size = subset_level_sets[child_ix].size();
	rule += std::to_string(subset_level_sets[child_ix][0]);
	for (int j = 1; j < n_subset_size; j++) {
			rule += " | " + std::to_string(subset_level_sets[child_ix][j]);
	}
	return rule;
}

void SplitCateg::setLevelPartitionings(std::vector<std::vector<int>> level_sets) {
	this->subset_level_sets = level_sets;
}

void SplitCateg::computePartitionings(Data* data) {
	int n_obs = data->nrows();
	int n_subsets = this->subset_level_sets.size();
	int n_levels_per_subset;
	for (int s = 0; s < n_subsets; s++) {
		n_levels_per_subset = subset_level_sets[s].size();
		for (int l = 0; l < n_levels_per_subset; l++) {
			for (int i = 0; i < n_obs; i++) {
				if (data->elem(i, this->getSplitFeatureIndex()) == this->subset_level_sets[s][l]) {
					this->splitted_obs[s].push_back(i);
				}
			}
		}	
	}	
}