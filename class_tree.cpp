#include <stdio.h>
#include <stdlib.h>
#include <armadillo>
#include <getopt.h>
#include <iostream>
#include "class_data.h"
#include "class_tree.h"
#include "class_node.h"

Tree::Tree(Data* data) {
  
  this->data = data;
  this->root = NULL;
}

Data* Tree::getData() {
  return this->data;
}

void Tree::setRoot(Node* root) {
	this->root = root;
}

