#include "class_improvement.h"
#include "class_objective.h"

ObjImprovement::ObjImprovement() {
	//
}

ObjImprovementAdditive::ObjImprovementAdditive() : ObjImprovement() {
	//
}


double ObjImprovementAdditive::compute(std::vector<Node*> nodes, Objective* obj) {
    double improv_val = 0;
    return improv_val;
}