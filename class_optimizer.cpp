#include <stdio.h>
#include <stdlib.h>
#include <armadillo>
#include <getopt.h>
#include <iostream>
#include "class_tree.h"
#include "class_node.h"
#include "class_optimizer.h"



Optimizer::Optimizer(arma::mat* data) {
  
	this->data = data;
}

void Optimizer::optimize() {\
	// placeholder
	// function overloaded by inheritance
}

void ExhaustiveSearch::optimize() {
}
