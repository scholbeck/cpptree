#include <stdio.h>
#include <stdlib.h>
#include "class_split.h"
#include "iostream"

Split::Split() {
	this->split_cnt = 0;
	this->feature_index = 0;
}

void Split::addSplitValue(double value) {
	(this->split_values).push_back(value);
	this->split_cnt++;
}

std::vector<double> Split::getSplitValues() {
	return this->split_values;
}

void Split::setFeatureIndex(int feature_index) {
	this->feature_index = feature_index;
}

int Split::getSplitFeatureIndex() {
	return this->feature_index;
}

void Split::clear() {
	this->split_cnt = 0;
	this->feature_index = 0;
	this->split_values.clear();
}

void Split::summary() {
	std::cout << "split point summary \n";
	std::cout << "split feature : " << this->getSplitFeatureIndex() << "\n";
	std::cout << "split values : ";
	int n_splits = this->split_cnt;
	for (int i = 0; i < n_splits; i++) {
		std::cout << this->getSplitValues()[i];
	}
	std::cout << "\n";
}
