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

Tree::Tree(Data* data, Arguments* args) : factory(new Factory(args))  {
	this->node_cnt = 0;
	this->data = data;
	this->leafnode_cnt = 0;
	this->args = args;
	this->root = new Node("0", this, convertDoubleToIntVector(data->col(0)), -1, "root");
	this->root->split = nullptr;
}

void Tree::addNode(Node* node) {
	this->nodes.push_back(node);
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
	Objective* obj = this->factory->createObjective();
	Model* root_model = this->factory->createModel();
	if (root_model != nullptr) {
		root_model->updateSet(this->data, this->root->observations, '+');
	}
	this->root->obj_val = obj->compute(this->data, root_model, root->observations);
	int ret = this->root->recursiveSplit();
	this->gatherTreeInformation();
	
	return ret;
}

void Tree::freeNodeMemory() {
	while (this->nodes.empty() == false) {
		//delete(this->nodes.back()->getModel());
		free(this->nodes.back());
		this->nodes.pop_back();
	}
}

void Tree::summary() {
	//this->root->getData()->summary();
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
			// std::cout << std::string((level * 4) , ' ') << "├──" << repeatString((((this->depth) - level) * 4) + depth * 2, "─") << "<" << node->getDecisionRule() << ">──[*" << node->getId() << "]\n" ;//<< "] (" << node->getModel()->getShortSummary() << " | obj = " << sstream.str() << ")\n";
			std::cout << std::string((level * 4) , ' ') << "├──<" << node->getDecisionRule() << ">──[*" << node->getId() << "]\n" ;//<< "] (" << node->getModel()->getShortSummary() << " | obj = " << sstream.str() << ")\n";
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
	printSubTree(this->nodes[0]);
}

// ├──

// └──
