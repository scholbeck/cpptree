
#include <iostream>
#include <string>
#include "class_arguments.h"
#include "class_node.h"
#include "class_optimizer.h"
#include "class_model.h"
#include "class_data.h"

Node::Node(std::string id, Data data, Optimizer* optim, Tree* tree) {
	
	this->tree = tree;
	this->data = data;
	this->id = id;
	this->optim = optim;
	this->child_cnt = 0;
	this->is_leaf = false;
}

std::string Node::getId() {
	return id;
}

Data Node::getData() {
	return this->data;
}

Optimizer* Node::getOptimizer() {
	return this->optim;
}

void Node::setOptimizer(Optimizer* optim) {
	this->optim = optim;
}

void Node::addChild(Node* child) {
	children.push_back(child);
	this->child_cnt++;
}

void Node::summary() {	
	std::cout << "node summary:\n";
	std::cout << "node ID: " << this->id << "\n";
	this->data.summary();
	
}

bool Node::isLeaf() {
	return this->is_leaf;
}

std::vector<Node*> Node::split() {
	std::vector<Node*> child_nodes;
	Split s;
	s = this->optim->searchOptimum(this->data);
	if (s.getSplitValues().empty() == false) {
		std::vector<Data> child_node_data = this->data.split(s);
		int n_child_nodes = child_node_data.size();
		for (int i = 0; i < n_child_nodes; i++) {
			std::string child_id = this->id + std::to_string(i);
			Node* child = new Node(child_id, child_node_data[i], this->optim, this->tree);
			child_nodes.push_back(child);
		}
	}
	return child_nodes;
}

void Node::recursiveSplit() {
	
	std::vector<Node*> child_nodes = this->split();
	if (child_nodes.empty() == false) {
		int n_splits = child_nodes.size();
		for (int i = 0; i < n_splits; i++) {
			child_nodes[i]->recursiveSplit();	
		}
	} else {
		this->is_leaf = true;
	}
	this->tree->addNode(this);
}
