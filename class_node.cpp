#include <sstream>
#include <iostream>
#include <string>
#include "class_arguments.h"
#include "class_node.h"
#include "class_optimizer.h"
#include "class_model.h"
#include "class_data.h"
#include "class_aggregation.h"
#include "class_splitgenerator.h"
#include "helper_functions.h"
#include <iomanip>

Node::Node(std::string id, Data data, Tree* tree, std::string decision_rule) {
	this->tree = tree;
	this->data = data;
	this->id = id;
	this->decision_rule = decision_rule;
	this->child_cnt = 0;
	this->is_leaf = false;
	
	this->mod = tree->getFactory().createModel();
	this->mod->setTrainingData(data);
	this->mod->train();
	this->obj_val = this->mod->evaluate(data, tree->getFactory().createObjective());
	
	this->tree->addNode(this);
}

double Node::getObjValue() {
	return this->obj_val;
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

std::string Node::createDecisionRule(Split s, int child_ix) {
	int feature = s.getSplitFeatureIndex();
	std::string rule;
	
	std::ostringstream sstream;
	sstream << std::setprecision(2) << std::fixed;
	
	if (s.getSplitType() == "num") {
		int n_splits = s.getSplitValues().size();
		// n split values = n+1 child nodes
		if (child_ix == 0) {
			sstream << s.getSplitValues()[0];
			rule = std::string("x") + std::to_string(feature) + std::string(" <= ") + sstream.str(); 
		} else if (child_ix == n_splits) {
			sstream << s.getSplitValues()[n_splits - 1];
			rule = std::string("x") + std::to_string(feature) + std::string(" > ") + sstream.str();
		} else {
			rule = std::string("x") + std::to_string(feature) + std::string(" ∈ ") + std::string(" ]");
			sstream << s.getSplitValues()[child_ix - 1];
			rule += sstream.str() + std::string(" , ");
			sstream.str(std::string());
			sstream.clear();
			// clear string stream
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
	std::vector<Node*> child_nodes;
	SplitGenerator* split_generator = new SplitGeneratorBinExh();
	std::vector<Split> splits = split_generator->generate(this->data);
	int n_splits = splits.size();
	std::vector<std::vector<int>> split_obs_current, split_obs_prev;
	std::array<std::vector<int>, 2> diff;
	std::vector<double> childnode_obj(this->tree->getArgs().getMaxChildren(), 0);
	std::vector<Data> childnode_data;
	Data subset_data;
	ObjectiveSSE obj;
	AggregationAdditive aggreg;
	double child_obj_val, opt_obj_val;
	int optsplit_ix = -1;
	opt_obj_val = obj.compute(this->data);
	bool geq_min_node_size = true;
	if (!splits.empty()) {
		for (int i = 0; i < n_splits; i++) {
			// loop over every split
			if (i == 0) {
				// for the first split, the objective cannot be updated
				split_obs_prev = this->data.splitObs(splits[i]);
				geq_min_node_size = checkObsSize(split_obs_prev, this->tree->getArgs().getMinNodeSize());
				if (geq_min_node_size == false) {
					geq_min_node_size = true;
					continue;
				}
				// check node size for all subsets created by splitting
				for (int j = 0; j < split_obs_prev.size(); j++) {
					subset_data = this->data.subsetRows(split_obs_prev[j]);
					childnode_data.push_back(subset_data);
					childnode_obj[j] = obj.compute(subset_data);
					// compute objective for child node
				}
			} else {
				split_obs_current = this->data.splitObs(splits[i]);
				geq_min_node_size = checkObsSize(split_obs_current, this->tree->getArgs().getMinNodeSize());
				if (geq_min_node_size == false) {
					geq_min_node_size = true;
					continue;
				}
				// check node size for all subsets created by splitting
				for (int j = 0; j < split_obs_current.size(); j++) {
					// for each subset, do:
					diff = diffSet(split_obs_current[j], split_obs_prev[j]);
					// diff[0] contains additional observations in the subset versus the previous split
					// diff[1] contains removed observations in the subset versus the previous split 
					childnode_obj[j] = obj.update(this->data, childnode_obj[j], diff);
					// update objective for subset (child node)
				}
				split_obs_prev = split_obs_current;
			}
			child_obj_val = aggreg.compute(childnode_obj);
			// aggregate objective of all child nodes
			if (child_obj_val < opt_obj_val) {
				opt_obj_val = child_obj_val;
				optsplit_ix = i;
			}
			// if aggregate objective better than parent objective, record best split 
		}
	}
	if (optsplit_ix != -1) {
		// if a split has been found, do:
		Split optsplit = splits[optsplit_ix];
		if (optsplit.getSplitFeatureIndex() != -1) {
			std::vector<Data> child_node_data = this->data.split(optsplit);
			int n_child_nodes = child_node_data.size();
			for (int i = 0; i < n_child_nodes; i++) {
				std::string child_id = this->id + std::to_string(i);
				std::string rule = this->createDecisionRule(optsplit, i);
				Node* child = new Node(child_id, child_node_data[i], this->tree, rule);
				child_nodes.push_back(child);
			}
		}
	}
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
	
	return ret;
}

