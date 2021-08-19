#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include "class_arguments.h"

Arguments::Arguments() {
  this->min_node_size = 0;
  this->max_children = 0;
}

std::string Arguments::getFilename() {
  return this->filename;
}
void Arguments::setFilename(std::string filename) {
  this->filename = filename;
}

int Arguments::getTargetIndex() {
	return this->target;
}
void Arguments::setTargetIndex(int target) {
	this->target = target;
}

int Arguments::getMaxChildren() {
	return this->max_children;
}
void Arguments::setMaxChildren(int max_children) {
	this->max_children = max_children;
}

int Arguments::getMinNodeSize() {
	return this->min_node_size;
}
void Arguments::setMinNodeSize(int min_node_size) {
	this->min_node_size = min_node_size;
}

std::string Arguments::getAlgorithm() {
	return this->algorithm;
}
void Arguments::setAlgorithm(std::string algo) {
	this->algorithm = algo;
}

std::string Arguments::getObjective() {
	return this->objective;
}
void Arguments::setObjective(std::string obj) {
	this->objective = obj;
}

std::string Arguments::getModel() {
	return this->model;
}
void Arguments::setModel(std::string mod) {
	this->model = mod;
}

std::string Arguments::getTask() {
	return this->task;
}
void Arguments::setTask(std::string task) {
	this->task = task;
}

char Arguments::getSep() {
	return this->sep;
}
void Arguments::setSep(char sep) {
	this->sep = sep;
}


int Arguments::getMaxDepth() {
	return this->max_depth;
}
void Arguments::setMaxDepth(int max_depth) {
	this->max_depth = max_depth;
}

void Arguments::checkArgs() {
	if (this->max_depth == 0) {
		this->max_depth = 30;
	}
}


