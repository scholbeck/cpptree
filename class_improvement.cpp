#include "class_improvement.h"
#include "class_objective.h"

ObjImprovement::ObjImprovement() {
	//
}

ObjImprovementAdditive::ObjImprovementAdditive() : ObjImprovement() {
	//
}


double ObjImprovementAdditive::compute(std::vector<double> childnode_obj, double obj_prev) {
    double improv = 0;
    int n_children = childnode_obj.size();
    for (int i = 0; i < n_children; i++) {
        improv += childnode_obj[i];
    }
    improv -= obj_prev;
    return improv;
}