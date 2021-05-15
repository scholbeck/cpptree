#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include "class_arguments.h"

Arguments::Arguments() {
  
  this->filename = "";
  this->algorithm = "";
  this->objective = "";
  this->model = "";
  this->min_node_size = 0;
  this->max_children = 0;
}

std::string Arguments::getFilename() {
  return this->filename;
}
void Arguments::setFilename(std::string filename) {
  this->filename = filename;
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


