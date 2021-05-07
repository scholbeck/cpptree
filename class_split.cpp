#include <stdio.h>
#include <stdlib.h>
#include "class_split.h"

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

void Split::setFeatureIndex(lluint feature_index) {
	this->feature_index = feature_index;
}

lluint Split::getSplitFeatureIndex() {
	return this->feature_index;
}

void Split::clearObject() {
	this->split_cnt = 0;
	this->feature_index = 0;
	this->split_values.clear();
}
