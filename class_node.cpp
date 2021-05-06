#include <stdio.h>
#include <stdlib.h>
#include <armadillo>
#include <getopt.h>
#include <iostream>
#include "class_tree.h"
#include "class_node.h"
#include "class_optimizer.h"



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

void Node::split(Optimizer optimizer) {
	optimizer.optimize((this->tree)->getData(), (this->tree)->max_children, (this->tree)->getObjective());
}
