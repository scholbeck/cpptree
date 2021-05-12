
#include <iostream>
#include <string>
#include "class_node.h"
#include "class_optimizer.h"
#include "class_model.h"
#include "class_data.h"




Node::Node(std::string id, Data* data) {
	
	this->data = data;
	this->id = id;
	this->child_cnt = 0;
	this->is_leaf = false;
}

std::string Node::getId() {
	return id;
}

Data* Node::getData() {
	return this->data;
}

void Node::addChild(Node* child) {
	children.push_back(child);
	this->child_cnt++;
}

void Node::summary() {	
	std::cout << "node summary:\n";
	std::cout << "node ID: " << this->id << "\n";
}

std::vector<Node*> Node::split(Model* mod, Objective* obj, Optimizer* optim, std::string algo) {

	Split s;
	s = optim->searchOptimum(*(this->getData()), obj, mod, algo);
	std::vector<Data> child_node_data = this->getData()->split(s);
	int n_child_nodes = child_node_data.size();
	std::vector<Node*> child_nodes;
	for (int i = 0; i < n_child_nodes; i++) {
		std::string child_id = this->id + std::to_string(i);
		Node* child = new Node(child_id, &child_node_data[i]);
		child_nodes.push_back(child);
	}
	return child_nodes;
}
