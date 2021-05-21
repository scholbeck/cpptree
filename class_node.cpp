
#include <iostream>
#include <string>
#include "class_arguments.h"
#include "class_node.h"
#include "class_optimizer.h"
#include "class_model.h"
#include "class_data.h"

Node::Node(std::string id, Data data, Tree* tree, std::string decision_rule) {
	this->tree = tree;
	this->data = data;
	this->id = id;
	this->decision_rule = decision_rule;
	this->child_cnt = 0;
	this->is_leaf = false;
}


void Node::setSplit(Split s) {
	this->split_data = s;
}

std::string Node::getId() {
	return id;
}

Data Node::getData() {
	return this->data;
}

Model* Node::getModel() {
	return this->mod;
}
void Node::setModel(Model* mod) {
	this->mod = mod;
}

std::vector<Node*> Node::getChildNodes() {
	return this->child_nodes;
}
void Node::addChild(Node* child) {
	child_nodes.push_back(child);
	this->child_cnt++;
}

Split Node::getSplitData() {
	return this->split_data;
}

std::string Node::getDecisionRule() {
	return this->decision_rule;
}
void Node::summary() {
	std::cout << "------------------------------------------------------\n";
	std::cout << "NODE SUMMARY\n";
	std::cout << "\tnode ID: " << this->id << "\n";
	if (this->is_leaf == true) {
		std::cout << "\tis leaf: yes\n";
	} else {
		std::cout << "\tis leaf: no\n";
	}
	if (this->is_leaf == false) {
		std::cout << "\tsplit feature: " << this->split_data.getSplitFeatureIndex() << "\n";
		std::cout << "\tfeature type: " << this->split_data.getSplitType() << "\n";
		if (this->split_data.getSplitType() == "num") {
			std::cout << "split values: ";
		}
		int n_splits = this->split_data.getSplitValues().size();
		for (int i = 0; i < n_splits; i++) {
			std::cout << this->split_data.getSplitValues()[i] << " , ";
		}
		std::cout << "\n";
		this->data.summary();
	}
	this->mod->summary();
	
	std::cout << "------------------------------------------------------\n";
}

bool Node::isLeaf() {
	return this->is_leaf;
}

Optimizer* Node::createOptimizer(Arguments args) {
	Optimizer* optim = NULL;
	Objective* obj = NULL;
	if (args.getAlgorithm() == "exhaustive") {
		if (args.getTask() == "regr") {
			optim = new OptimExhaustSearchRegr();
		} else if (args.getTask() == "classif") {
			optim = new OptimExhaustSearchClassif();
		}
	}
	if (args.getObjective() == "sse") {
		obj = new ObjectiveSSE();
	} else if (args.getObjective() == "gini") {
		obj = new ObjectiveGini();
	}
	optim->setObjective(obj);
	optim->setMinNodeSize(args.getMinNodeSize());
	optim->setMaxChildren(args.getMaxChildren());
	return optim;
}

std::string Node::createDecisionRule(Split s, int child_ix) {
	int feature = s.getSplitFeatureIndex();
	std::string rule;
	if (s.getSplitType() == "num") {
		int n_splits = s.getSplitValues().size();
		if (child_ix == 0) {
			rule = std::string("col ") + std::to_string(feature) + std::string(" <= ") + std::to_string(s.getSplitValues()[0]); 
		} else if (child_ix == n_splits) {
			rule = std::string("col ") + std::to_string(feature) + std::string(" > ") + std::to_string(s.getSplitValues()[n_splits]); 	
		} else {
			rule = std::string("col ") + std::to_string(feature) + std::string(" ∈ ") + std::string("[") + std::to_string(s.getSplitValues()[child_ix - 1]) + std::string(" , ") + std::to_string(s.getSplitValues()[child_ix]) + std::string("]"); 
		}
	} else {
		std::map<std::string, int> levels = this->data.getCategEncodings().at(feature);
		for (auto it = levels.begin(); it != levels.end(); ++it) {
			if (child_ix == it->second) {
				rule = std::string("col ") + std::to_string(feature) + std::string(" == ") + std::to_string(child_ix);
			}
		}
	}
	return rule;
}

std::vector<Node*> Node::split() {
	Optimizer* optim = createOptimizer(this->tree->args);
	std::vector<Node*> child_nodes;
	Split s;
	s = optim->searchOptimum(this->data, this->tree->args);
	this->setSplit(s);
	if (s.getSplitFeatureIndex() != -1) {
		std::vector<Data> child_node_data = this->data.split(s);
		int n_child_nodes = child_node_data.size();
		for (int i = 0; i < n_child_nodes; i++) {
			std::string child_id = this->id + std::to_string(i);
			std::string rule = this->createDecisionRule(s, i);
			Node* child = new Node(child_id, child_node_data[i], this->tree, rule);
			child->setModel(s.getChildNodeModels()[i]);
			child_nodes.push_back(child);
		}
	}
	free(optim);
	return child_nodes;
}

void Node::recursiveSplit() {
	std::vector<Node*> child_nodes = this->split();
	this->child_nodes = child_nodes;
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

