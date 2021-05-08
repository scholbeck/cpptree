#include <stdio.h>
#include <stdlib.h>
#include <armadillo>
#include <getopt.h>
#include <iostream>
#include "class_tree.h"
#include "class_node.h"
#include "class_optimizer.h"
#include "class_model.h"


Node::Node(int id, Data* data, Tree* tree) {
	
	this->tree = tree;
	this->data = data;
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

std::vector<Node*> Node::split() {
	std::vector<Data*> data_splitted;
	std::vector<Node*> child_nodes;
	Split s;
	Optimizer opt;
	ModelAverage mod = ModelAverage(this->getData());
	
	s = opt.optimize((this->tree)->getData(), (this->tree)->getObjective(), &mod);
	
	/*
	data_splitted = (this->getData())->split(s);
	int n_children = data_splitted.size();
	
	for (int i = 0; i < n_children; i++) {
		Node* n = new Node(001, data_splitted[i], this->tree);
		child_nodes.push_back(n);
	}
	*/
	return child_nodes;
}
