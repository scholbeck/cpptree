#include <stdio.h>
#include <stdlib.h>
#include "class_split.h"

Split::Split() {
	this->split_cnt = -1;
	this->feature_index = -1;
}

void Split::addSplitpoint(size_t split) {
	
	(this->splitpoints).push_back(split);
	this->split_cnt++;
}

std::vector<size_t> Split::getSplitpoints() {
	
	return this->splitpoints;
}

void Split::setFeatureIndex(ssize_t feature_index) {
	this->feature_index = feature_index;
}

ssize_t Split::getSplitFeatureIndex() {
	return this->feature_index;
}


void Split::clearObject() {
	this->split_cnt = -1;
	this->feature_index = -1;
	this->splitpoints.clear();
}
