#include <sstream>
#include <iostream>
#include <string>
#include "class_arguments.h"
#include "class_node.h"
#include "class_optimizer.h"
#include "class_model.h"
#include "class_data.h"
#include <iomanip>

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
	std::cout << "\tdecision rule: " << this->decision_rule << "\n";
	this->data.sizeSummary();
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
	
	std::ostringstream sstream;
	sstream << std::setprecision(2) << std::fixed;
	
	if (s.getSplitType() == "num") {
		int n_splits = s.getSplitValues().size();
		if (child_ix == 0) {
			sstream << s.getSplitValues()[0];
			rule = std::string("x") + std::to_string(feature) + std::string(" <= ") + sstream.str(); 
		} else if (child_ix == n_splits) {
			sstream << s.getSplitValues()[n_splits];
			rule = std::string("x") + std::to_string(feature) + std::string(" > ") + sstream.str();
		} else {
			rule = std::string("x") + std::to_string(feature) + std::string(" ∈ ") + std::string("[");
			sstream << s.getSplitValues()[child_ix - 1];
			rule += sstream.str() + std::string(" , ");
			sstream << s.getSplitValues()[child_ix];
			rule += sstream.str() + std::string("]"); 
		}
	} else {
		std::map<std::string, int> levels = this->data.getCategEncodings().at(feature);
		for (auto it = levels.begin(); it != levels.end(); ++it) {
			if (child_ix == it->second) {
				rule = std::string("x") + std::to_string(feature) + std::string(" = ") + std::to_string(child_ix);
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

int Node::recursiveSplit() {
	std::vector<Node*> child_nodes = this->split();
	this->child_nodes = child_nodes;
	int n_child_nodes = 0; 
	int ret = 0;
	if (child_nodes.empty() == false) {
		n_child_nodes = child_nodes.size();
		for (int i = 0; i < n_child_nodes; i++) {
			ret += child_nodes[i]->recursiveSplit();	
		}
	} else {
		this->is_leaf = true;
	}
	this->tree->addNode(this);
	return 1;
}

