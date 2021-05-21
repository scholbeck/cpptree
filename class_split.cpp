#include <stdio.h>
#include <stdlib.h>
#include "class_split.h"
#include "iostream"

Split::Split() {
	this->feature_index = -1;
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

std::string Split::getSplitType() {
	return this->split_type;
}
void Split::setSplitType(std::string split_type) {
	this->split_type = split_type;
}

void Split::clear() {
	this->feature_index = -1;
	this->split_values.clear();
}

void Split::summary() {
	std::cout << "SPLIT SUMMARY\n";
	std::cout << "\tsplit feature : " << this->getSplitFeatureIndex() << "\n";
	std::cout << "\tsplit type " << split_type << "\n";
	if (this->split_type == "num") {
		std::cout << "\tsplit values : ";
		int n_splits = this->split_cnt;
		std::cout << "\t\t";
		for (int i = 0; i < n_splits; i++) {
			std::cout << this->getSplitValues()[i];
		}
	}
	std::cout << "\n";
}
