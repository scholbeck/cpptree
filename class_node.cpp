#include <stdio.h>
#include <stdlib.h>
#include <armadillo>
#include <getopt.h>
#include <iostream>
#include "class_tree.h"
#include "class_node.h"



Node::Node(int id, arma::mat* data) {
  
	this->data = data;
	this->id = id;
	this->child_left = NULL;
	this->child_right = NULL;
}

int Node::getId() {
	return id;
}

Node* Node::getChildLeft() {
	return child_left;
}

Node* Node::getChildRight() {
	return child_right;
}

void Node::setChildLeft(Node* child_node) {
	child_left = child_node;
}

void Node::setChildRight(Node* child_node) {
	child_right = child_node;
}

arma::mat* Node::getData() {
	return data;
}


void Node::split(int target_index, int min_node_size, Optimizer optimizer) {
	optimizer.optimize();
}
