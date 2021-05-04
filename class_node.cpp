#include <stdio.h>
#include <stdlib.h>
#include <armadillo>
#include <getopt.h>
#include <iostream>
#include "class_tree.h"
#include "class_node.h"



Node::Node(int id, Tree* tree) {
	
	this->tree = tree;
	this->id = id;
	this->child_cnt = 0;
	this->is_leaf = false;
}

int Node::getId() {
	return id;
}

Data* Node::getData() {
	return tree->getData();
}

void Node::addChild(Node* child) {
	children.push_back(child);
	this->child_cnt++;
}

void Node::split(int target_index, int min_node_size, Optimizer optimizer) {
	optimizer.optimize();
}
