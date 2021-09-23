#include "class_aggregation.h"

Aggregation::Aggregation() {
	//
}

AggregationAdditive::AggregationAdditive() : Aggregation() {
	//
}


double AggregationAdditive::compute(std::vector<double> childnode_obj) {
    double obj_val = 0;
    int n_children = childnode_obj.size();
    for (int i = 0; i < n_children; i++) {
        obj_val += childnode_obj[i];
    }
    return obj_val;
}