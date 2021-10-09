#include <sstream>
#include <iostream>
#include <string>
#include "class_arguments.h"
#include "class_node.h"
#include "class_objective.h"
#include "class_model.h"
#include "class_data.h"
#include "class_aggregation.h"
#include "class_splitgenerator.h"
#include "helper_functions.h"
#include <iomanip>

Node::Node(std::string id, Tree* tree, std::vector<int> observations, double obj_val, std::string decision_rule) {
	this->tree = tree;
	this->observations = observations;
	this->id = id;
	this->obj_val = obj_val;
	this->decision_rule = decision_rule;
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
void Node::setModelInfo(std::string model_info) {
	this->model_info = model_info;
}


std::vector<Node*> Node::splitNode() {

	SplitGenerator* split_generator = this->tree->getFactory()->createSplitGenerator();
	std::vector<Split*> splits = split_generator->generate(this->tree->data, this->observations, this->id, this->tree->getArgs());
	delete(split_generator);
	
	AggregationAdditive aggreg;
	Objective* obj = this->tree->getFactory()->createObjective();

	double child_obj_val, opt_obj_val;
	opt_obj_val = this->obj_val;

	std::vector<double> opt_obj_values;
	std::vector<std::string> opt_model_info;

	int n_splits = splits.size();
	int n_children = this->tree->getArgs()->getMaxChildren();
	int optsplit_ix = -1;
			
	if (!splits.empty()) {
		for (int i = 0; i < n_splits; ++i) {
			if (i == 0) {
				obj->update(this->tree->data, splits[0], nullptr);
			} else {
				obj->update(this->tree->data, splits[i], splits[i-1]);
			}
			child_obj_val = aggreg.compute(obj->node_obj_values);
			//std::cout << "child objective value " << child_obj_val << std::endl;
			if (child_obj_val < opt_obj_val) {
				opt_obj_val = child_obj_val;
				opt_obj_values = obj->node_obj_values;
				optsplit_ix = i;
				opt_model_info = obj->generateAggregateModelInfo();
			}
		}
	}
	std::vector<Node*> child_nodes;
	child_nodes.reserve(n_children);
	if (optsplit_ix != -1) {
	  	// if a split has been found, do:
	  	this->split = splits[optsplit_ix];
		this->tree->data->sorted_data->split(this->id, this->split->split_obs);
		// partition sorted features
		for (int i = 0; i < n_children; ++i) {
			Node* child = new Node(
				this->id + std::to_string(i),
				this->tree,
				this->split->split_obs[i],
				opt_obj_values[i],
				this->createDecisionRule(this->split, i));
			child->setModelInfo(opt_model_info[i]);	
			child_nodes.push_back(child);
		}
	}
	for (int i = 0; i < n_splits; i++) {
	  if (i != optsplit_ix) {
	    delete(splits[i]);
	  }
	}
	delete(obj);
	
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

