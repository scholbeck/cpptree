#include <stdio.h>
#include <stdlib.h>
#include <armadillo>
#include <getopt.h>
#include <iostream>
#include "class_data.h"
#include "class_tree.h"
#include "class_node.h"
#include "class_optimizer.h"

Tree::Tree(Data data, Optimizer* optim) {
	this->root = new Node("0", data, optim, this);
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

void Tree::grow() {
	this->root->recursiveSplit();
}

void Tree::freeNodeMemory() {
	for (int i = 0; i < node_cnt; i++) {
		free(this->nodes.back());
		this->nodes.pop_back();
	}
}

void Tree::summary() {
	std::cout << "tree summary:\n";
	std::cout << this->node_cnt << " nodes\n";
	std::cout << this->leafnode_cnt << " leaf nodes\n";
	for (int i = 0; i < this->node_cnt; i++) {
		this->nodes[i]->summary();
	}
}

