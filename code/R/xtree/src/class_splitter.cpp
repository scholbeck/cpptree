#include "class_splitter.h"
#include "class_splitgenerator.h"
#include "class_factory.h"
#include "class_aggregation.h"
#include "helper_functions.h"

Splitter::Splitter() {}

SplitterNaive::SplitterNaive() {}

Split* SplitterNaive::findBestSplit(Data* data, const std::vector<int> &observations, std::string ID, Arguments* args, double prev_obj) {
    Factory factory = Factory(data, args);
    SplitGeneratorBatch* split_generator = factory.createSplitGeneratorBatch();
	std::vector<Split*> splits = split_generator->generate(data, observations, ID, args);
	delete(split_generator);
	
	AggregationAdditive aggreg;
	Objective* obj = factory.createObjective();

	double child_obj_val, opt_obj_val_aggreg;
	opt_obj_val_aggreg = prev_obj;
	
	std::vector<double> opt_obj_values;
	std::vector<std::string> opt_model_info;

	int n_splits = splits.size();
	int optsplit_ix = -1;
			
	if (!splits.empty()) {
		obj->update(splits[0], nullptr);
		for (int i = 1; i < n_splits; ++i) {
			obj->update(splits[i], splits[i-1]);
			child_obj_val = aggreg.compute(obj->node_obj_values);
			if (child_obj_val < opt_obj_val_aggreg) {
				optsplit_ix = i;
				opt_obj_val_aggreg = child_obj_val;
				opt_obj_values = obj->node_obj_values;
				opt_model_info = obj->generateAggregateModelInfo();
			}
		}
	}
    for (int i = 0; i < n_splits; i++) {
	  if (i != optsplit_ix) {
	    delete(splits[i]);
	  }
	}
	obj->freeInternalMemory();
	delete(obj);
	
	Split* return_split;
	if (optsplit_ix != -1) {
		return_split = splits[optsplit_ix];
		return_split->setObjValues(opt_obj_values);
		return_split->setModelInfo(opt_model_info);
	}
    return return_split;
}



SplitterAdaptive::SplitterAdaptive() {}

Split* SplitterAdaptive::findBestSplit(Data* data, const std::vector<int> &observations, std::string ID, Arguments* args, double prev_obj) {
	// multi-threaded implementation of adaptive split search

	// producer consumer pattern:
	// thread 1: generate split, make available, sleep
	// thread 2: sleep until split available, get and evaluate objective, return result to thread 1, sleep
	Split* s;
	return s;
}


