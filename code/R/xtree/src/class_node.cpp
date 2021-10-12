#include <sstream>
#include <iostream>
#include <string>
#include "class_split.h"
#include "class_arguments.h"
#include "class_node.h"
#include "class_objective.h"
#include "class_model.h"
#include "class_data.h"
#include "class_aggregation.h"
#include "class_splitgenerator.h"
#include "class_splitter.h"
#include "helper_functions.h"
#include <iomanip>

Node::Node(std::string id, Tree* tree, std::vector<int> observations, double obj_val, std::string decision_rule, std::string model_info) {
	this->tree = tree;
	this->observations = observations;
	this->id = id;
	this->obj_val = obj_val;
	this->decision_rule = decision_rule;
	this->model_info = model_info;
	this->child_cnt = 0;
	this->child_nodes.reserve(tree->getArgs()->getMaxChildren());
	this->is_leaf = false;
	this->tree->addNode(this);
	this->split= nullptr;
}

double Node::getObjValue() {
	return this->obj_val;
}

std::string Node::getId() {
	return id;
}


std::vector<Node*> Node::getChildNodes() {
	return this->child_nodes;
}

void Node::addChild(Node* child) {
	child_nodes.push_back(child);
	this->child_cnt++;
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
	std::cout << "------------------------------------------------------\n";
}

bool Node::isLeaf() {
	return this->is_leaf;
}

std::string Node::createDecisionRule(Split* s, int child_ix) {
	std::string rule = s->createDecisionRule(child_ix);
	return rule;
}

Split* Node::getSplitData() {
  return this->split;
}
  
std::string Node::getModelInfo() {
	return this->model_info;
}
std::string Node::generateNodeInfo() {
	std::ostringstream sstream;
	sstream << std::setprecision(2) << std::fixed; // printout with 2 decimal places
	sstream << this->tree->getArgs()->getObjective() << " = " << this->obj_val << " | " << this->model_info;
	return sstream.str();
}


std::vector<Node*> Node::splitNode() {
	Splitter* splitter = this->tree->getFactory()->createSplitter();
	this->split = splitter->findBestSplit(this->tree->data, this->observations, this->id, this->tree->getArgs(), this->obj_val);
	delete(splitter);
	std::vector<Node*> child_nodes;
	if (this->split != nullptr) {
		// if a split has been found, do:
	  	this->tree->data->sorted_data->split(this->id, this->split->split_obs);
		// partition sorted features
		for (int i = 0; i < (this->split->getNumberChildNodes()); ++i) {
			Node* child = new Node(
				this->id + std::to_string(i),
				this->tree,
				this->split->split_obs[i],
				this->split->obj_values[i],
				this->split->createDecisionRule(i),
				this->split->getModelInfo()[i]);
			child_nodes.push_back(child);
		}
	}
	return child_nodes;
}

int Node::recursiveSplit() {
	if ((int) (this->getId().length() -1) == this->tree->getArgs()->getMaxDepth()) {
		this->is_leaf = true;
		return 0;
	}
	this->child_nodes = this->splitNode();
	int n_child_nodes = 0; 
	int ret = 0;
	if (!child_nodes.empty()) {
		n_child_nodes = child_nodes.size();
		for (int i = 0; i < n_child_nodes; ++i) {
			ret += child_nodes[i]->recursiveSplit();	
		}
	} else {
		this->is_leaf = true;
	}
	return ret;
}

