#include <stdio.h>
#include <stdlib.h>
#include <armadillo>
#include <getopt.h>
#include <iostream>
#include "class_data.h"
#include "class_tree.h"
#include "class_node.h"

Tree::Tree(Data* data, size_t max_children, size_t min_node_size) {
	this->data = data;
	this->root = NULL;
	this->max_children = max_children;
	this->min_node_size = min_node_size;
}

Data* Tree::getData() {
	return this->data;
}

size_t Tree::getMinNodeSize() {
	return this->min_node_size;
}

size_t Tree::getMaxChildren() {
	return this->max_children;
}

Objective Tree::getObjective() {
	return this->objective;
}

size_t Tree::getTargetIndex() {
	return this->target_index;
}

void Tree::setRoot(Node* root) {
	this->root = root;
}

void Tree::addNode(Node* node) {
	(this->nodes).push_back(node);
	this->node_cnt++;
}

