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

Node::Node(std::string id, Data* data, Tree* tree, std::string decision_rule) {
	this->tree = tree;
	this->data = data;
	this->id = id;
	this->decision_rule = decision_rule;
	this->child_cnt = 0;
	this->child_nodes.reserve(tree->getArgs().getMaxChildren());
	this->is_leaf = false;
	this->tree->addNode(this);
}

double Node::getObjValue() {
	return this->obj_val;
}

std::string Node::getId() {
	return id;
}
Data* Node::getData() {
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
	int feature = s->getSplitFeatureIndex();
	std::string rule = s->createDecisionRule(child_ix);

	return rule;
	/*
	if (s->getSplitType() == "num") {
		int n_splits = s->getSplitValues().size();
		// n split values = n+1 child nodes
		if (child_ix == 0) {
			sstream << s->getSplitValues()[0];
			rule = std::string("x") + std::to_string(feature) + std::string(" <= ") + sstream.str(); 
		} else if (child_ix == n_splits) {
			sstream << s->getSplitValues()[n_splits - 1];
			rule = std::string("x") + std::to_string(feature) + std::string(" > ") + sstream.str();
		} else {
			rule = std::string("x") + std::to_string(feature) + std::string(" ∈ ") + std::string(" ]");
			sstream << s->getSplitValues()[child_ix - 1];
			rule += sstream.str() + std::string(" , ");
			sstream.str(std::string());
			sstream.clear();
			// clear string stream
			sstream << s->getSplitValues()[child_ix];
			rule += sstream.str() + std::string("]"); 
		}
	} else {
		// categ split feature
		std::map<std::string, int> levels = this->data->getCategEncodings().at(feature);
		for (auto it = levels.begin(); it != levels.end(); ++it) {
			if (child_ix == it->second) {
				rule = std::string("x") + std::to_string(feature) + std::string(" = ") + std::to_string(child_ix);
			}
		}
	}
	return rule;
	*/
}


std::vector<Node*> Node::split() {
	SplitGenerator* split_generator = this->tree->getFactory().createSplitGenerator(this->data, this->tree->getArgs()); 
	std::vector<Split*> splits = split_generator->generate();
	free(split_generator);
	int n_splits = splits.size();
	std::array<std::vector<int>, 2> diff;
	AggregationAdditive aggreg;
	int n_children = this->tree->getArgs().getMaxChildren();
	Objective* obj = this->tree->getFactory().createObjective();
	double child_obj_val, opt_obj_val;
	int optsplit_ix = -1;
	this->obj_val = obj->compute(this->data);
	opt_obj_val = this->obj_val;
	Data* subset;
	if (!splits.empty()) {
		for (int i = 0; i < n_splits; ++i) {
			//std::cout << i << std::flush;
			// loop over every split
			if (i == 0) {
				// for the first split, the objective cannot be updated
				for (int j = 0; j < n_children; j++) {
					subset = this->data->subsetRows(splits[i]->splitted_obs[j]);
					//subset->summary();
					obj->init(subset, j);
					// objective is initialized with all initial observations
				}
			} else {
				// check node size for all subsets created by splitting
				for (int j = 0; j < n_children; j++) {
					//std::cout << j << std::flush;
					// for each subset, do:
					diff = diffSet(splits[i]->splitted_obs[j], splits[i-1]->splitted_obs[j]);
					// diff[0] contains additional observations in the subset versus the previous split
					// diff[1] contains removed observations in the subset versus the previous split 
					obj->update(this->data, j, diff);
					// update objective for subset (child node)
				}
			}
			child_obj_val = aggreg.compute(obj->values);
			// aggregate objective of all child nodes
			if (child_obj_val < opt_obj_val) {
				opt_obj_val = child_obj_val;
				optsplit_ix = i;
			}
			// if aggregate objective better than parent objective, record best split 
		}
	}
	std::vector<Node*> child_nodes;
	child_nodes.reserve(n_children);
	if (optsplit_ix != -1) {
		// if a split has been found, do:
		for (int i = 0; i < n_children; ++i) {
			Node* child = new Node(
				this->id + std::to_string(i),
				this->data->subsetRows(splits[optsplit_ix]->splitted_obs[i]),
				this->tree,
				this->createDecisionRule(splits[optsplit_ix], i));
			child_nodes.push_back(child);
		}
	}
	//free(subset);
	for (int i = 0; i < n_splits; ++i) {
		free(splits[i]);
	}
	free(obj);
	return child_nodes;
}

int Node::recursiveSplit() {
	if ((this->getId().length() -1) == this->tree->getArgs().getMaxDepth()) {
		this->is_leaf = true;
		return 0;
	}
	this->child_nodes = this->split();
	int n_child_nodes = 0; 
	int ret = 0;
	if (!child_nodes.empty()) {
		n_child_nodes = child_nodes.size();
		for (int i = 0; i < n_child_nodes; ++i) {
			//child_nodes[i]->summary();
			ret += child_nodes[i]->recursiveSplit();	
		}
	} else {
		this->is_leaf = true;
	}
	return ret;
}

