#include <stdio.h>
#include <stdlib.h>
#include <armadillo>
#include <getopt.h>
#include <iostream>
#include "class_tree.h"
#include "class_node.h"


Tree::Tree(arma::mat data) {
  
  this->data = data;
  this->root = NULL;
}

arma::mat Tree::getData() {
  return this->data;
}


void Tree::setRoot(Node* root) {
	this->root = root;
}

