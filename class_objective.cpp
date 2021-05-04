#include <stdio.h>
#include <stdlib.h>
#include "class_objective.h"

Objective::Objective() {
	
}

double Objective::compute(Data* data, ssize_t target_index) {
	// placeholder
	// function overloaded by inheritance
	return 0;
}

double ObjectiveMSE::compute(Data* data, ssize_t target_index) {
	return 0;
}
