#include <stdio.h>
#include <stdlib.h>
#include <armadillo>
#include <getopt.h>
#include <iostream>
#include "class_data.h"
#include "class_tree.h"
#include "class_node.h"
#include "class_arguments.h"
#include "class_factory.h"
#include "helper_functions.h"
#include <iomanip>

Tree::Tree(Data* data, Arguments* args) : factory(new Factory(data, args))  {
	this->node_cnt = 0;
	this->data = data;
	this->leafnode_cnt = 0;
	this->args = args;
}

void Tree::addNode(std::unique_ptr<Node> node) {
	this->nodes.push_back(std::move(node)); // smart pointer has to be moved instead of copied
	this->node_cnt++;
}

void Tree::gatherTreeInformation() {
	int max_length = 1;
	int current_length;
	for (int i = 0; i < this->node_cnt; ++i) {
		if (this->nodes[i]->isLeaf()) {
			this->leafnode_cnt++;
		}
		// gather leaf node count
		current_length = this->nodes[i]->getId().length()-1;
		if (current_length > max_length) {
			max_length = current_length;
		}
		// gather tree depth
	}
	this->depth = max_length;
}

Arguments* Tree::getArgs() {
	return this->args;
}

int Tree::grow() {
	std::unique_ptr<Objective> obj = this->factory->createObjective();
	std::unique_ptr<Model> root_model = this->factory->createModel();
	std::vector<int> root_obs = this->data->getObservationIDs();
	if (root_model != nullptr) {
		root_model->update(this->data, root_obs, '+');
	}
	std::unique_ptr<Node> r = std::make_unique<Node>("0", this, root_obs, -1, "root", "");
	r->split = nullptr;
	r->obj_val = obj->compute(root_model.get(), root_obs);
	this->addNode(std::move(r));
	int ret = this->nodes[0]->recursiveSplit();
	this->gatherTreeInformation();
	
	return ret;
}

void Tree::summary() {
	std::cout << "------------------------------------------------------\n";
	std::cout << "TREE SUMMARY\n";
	std::cout << "\tnodes : " << this->node_cnt << "\n";
	std::cout << "\tleaf nodes : " << this->leafnode_cnt << "\n";
	std::cout << "\tdepth : " << this->depth << "\n\n";
	this->print();
	std::cout << "------------------------------------------------------\n";
}


Factory* Tree::getFactory() {
	return this->factory;
}

std::string repeatString(int n, std::string s) {
    std::ostringstream os;
    for(int i = 0; i < n; ++i)
        os << s;
    return os.str();
}

void Tree::printSubTree(Node* node) {
	int level = node->getId().length() - 1;
		std::ostringstream sstream;
		sstream << std::setprecision(2) << std::fixed ; //<< node->getObjValue(); // obj printout with 2 decimal places
	if (level == 0) {
		std::cout << "└──[" << node->getId() << "]\n"; // << "] (" << node->getModel()->getShortSummary() << " | obj = " << sstream.str() << ")\n";
	} else {
		if (node->isLeaf()) {
			std::cout << std::string((level * 4) , ' ') << "├──<" << node->getDecisionRule() << ">──[*" << node->getId() << "] (" << node->generateNodeInfo() << ")\n" ;//<< "] (" << node->getModel()->getShortSummary() << " | obj = " << sstream.str() << ")\n";
		} else {
			std::cout << std::string((level * 4) , ' ') << "├──<" << node->getDecisionRule() << ">──[" << node->getId() << "]\n" ;//<< "] (" << node->getModel()->getShortSummary() << " | obj = " << sstream.str() << ")\n";
		}
	}
	std::vector<Node*> child_nodes = node->getChildNodes();
	int n_children = child_nodes.size();
	for (int i = 0; i < n_children; ++i) {
		printSubTree(child_nodes[i]);
	}
}

void Tree::print() {
	printSubTree(this->nodes[0].get());
}

// ├──

// └──
