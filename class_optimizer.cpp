#include <stdio.h>
#include <stdlib.h>
#include <armadillo>
#include <getopt.h>
#include <iostream>
#include "class_tree.h"
#include "class_node.h"
#include "class_optimizer.h"
#include "class_split.h"


Optimizer::Optimizer() {

}

Split Optimizer::optimize(Data* data, Objective objective) {
	// placeholder
	// method overloaded by inheritance
	Split split = Split();
	return split;
}

Split OptimizerExhaustiveSearch::optimize(Data* data, Objective objective) {
	
	Split split = Split();
	
	return split;
}

Split OptimizerRandomSearch::optimize(Data* data, Objective objective) {
	Split split = Split();
	return split;
}
