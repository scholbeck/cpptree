#include <stdio.h>
#include <stdlib.h>
#include <armadillo>
#include <getopt.h>
#include <iostream>
#include "class_data.h"
#include "class_tree.h"
#include "class_node.h"
#include "class_optimizer.h"
#include "class_arguments.h"


Tree::Tree(Data data, Arguments args) {
	this->root = new Node("0", data, this);
	this->addNode(root);
	this->args = args,
	this->node_cnt = 0;
	this->leafnode_cnt = 0;
}

void Tree::addNode(Node* node) {
	this->nodes.push_back(node);
	this->node_cnt++;
	if (node->isLeaf()) {
		this->leafnode_cnt++;
	}
}

void Tree::sortNodesAsc() {
	std::string left_id, current_id;
	Node* node_pntr_tmp = NULL;
	for (int i = 0; i < this->node_cnt; i++) {
		left_id = this->nodes[i]->getId();
		for (int j = i + 1; j < this->node_cnt; j++) {
			current_id = this->nodes[j]->getId();
			if ((std::stoi(current_id) < std::stoi(left_id)) && (current_id.length() <= left_id.length())) {
				node_pntr_tmp = this->nodes[i];
				this->nodes[i] = this->nodes[j];
				this->nodes[j] = node_pntr_tmp;
			}
		}
	}
	
}

void Tree::grow() {
	Optimizer* optim = this->root->createOptimizer(args);
	this->root->setModel(optim->buildModel(this->root->data, args));
	this->root->recursiveSplit();
}

void Tree::freeNodeMemory() {
	for (int i = 0; i < node_cnt; i++) {
		free(this->nodes.back()->getModel());
		free(this->nodes.back());
		this->nodes.pop_back();
	}
}

void Tree::summary() {
	std::cout << "------------------------------------------------------\n";
	std::cout << "TREE SUMMARY\n";
	std::cout << "\t" << this->node_cnt << " nodes\n";
	std::cout << "\t" << this->leafnode_cnt << " leaf nodes\n";
	this->print();
	for (int i = 0; i < this->node_cnt; i++) {
		this->nodes[i]->summary();
	}
	std::cout << "------------------------------------------------------\n";
}

void printSubTree(Node* node) {
	int level = node->getId().length() - 1;
	if (node->isLeaf()) {
		std::cout << std::string((level * 3) , ' ') << "├──<" << node->getId() << ">\n";
	} else {
		std::cout << std::string((level * 3) , ' ') << "├──" << node->getId() << "\n";
	}
	std::vector<Node*> child_nodes = node->getChildNodes();
	int n_children = child_nodes.size();
	for (int i = 0; i < n_children; i++) {
		printSubTree(child_nodes[i]);
	}
	
}

void Tree::print() {
	printSubTree(this->nodes[0]);
}

// ├──

// └──

