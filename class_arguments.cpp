#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include "class_arguments.h"



Arguments::Arguments() {
  
  this->filename = "";
  this->min_node_size = 0;
  this->max_splits = 0;
}

std::string Arguments::getFilename() {
  return this->filename;
}
void Arguments::setFilename(std::string filename) {
  this->filename = filename;
}

ssize_t Arguments::getMaxSplits() {
	return this->max_splits;
}
void Arguments::setMaxSplits(ssize_t max_splits) {
	this->max_splits = max_splits;
}

ssize_t Arguments::getMinNodeSize() {
	return this->min_node_size;
}
void Arguments::setMinNodeSize(ssize_t min_node_size) {
	this->min_node_size = min_node_size;
}




