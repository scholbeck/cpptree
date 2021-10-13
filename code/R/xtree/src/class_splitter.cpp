#include <iostream>
#include "class_splitter.h"
#include "class_splitgenerator.h"
#include "class_factory.h"
#include "class_aggregation.h"
#include "helper_functions.h"

Splitter::Splitter() {}

SplitterNaive::SplitterNaive() {}

std::unique_ptr<Split> SplitterNaive::findBestSplit(Data* data, std::vector<int> observations, std::string ID, Arguments* args, double prev_obj) {
    
	Factory factory = Factory(data, args);
   	std::unique_ptr<SplitGeneratorBatch> split_generator = factory.createSplitGeneratorBatch();
	std::vector<std::unique_ptr<Split>> splits = split_generator->generate(data, observations, ID, args);
	// all splits inside this vector of smart pointers go out of scope (and are deleted) when the function returns
	int n_splits = splits.size();
	
	AggregationAdditive aggreg;
	std::unique_ptr<Objective> obj = factory.createObjective();

	double child_obj_val, opt_obj_val_aggreg;
	opt_obj_val_aggreg = prev_obj;
	
	std::vector<double> opt_obj_values;
	std::vector<std::string> opt_model_info;

	int optsplit_ix = -1;
	if (!splits.empty()) {
		obj->update(splits[0].get(), nullptr);
		for (int i = 1; i < n_splits; ++i) {
			obj->update(splits[i].get(), splits[i-1].get());
			child_obj_val = aggreg.compute(obj->node_obj_values);
			if (child_obj_val < opt_obj_val_aggreg) {
				optsplit_ix = i;
				opt_obj_val_aggreg = child_obj_val;
				opt_obj_values = obj->node_obj_values;
				opt_model_info = obj->generateAggregateModelInfo();
			}
		}
	}

	std::unique_ptr<Split> return_split;
	if (optsplit_ix != -1) {
		return_split = std::move(splits[optsplit_ix]);
		// transfer ownership of smart pointer from above vector to this new smart pointer which is returned
		return_split->setObjValues(opt_obj_values);
		return_split->setModelInfo(opt_model_info);
	}
    return return_split;
}



SplitterAdaptive::SplitterAdaptive() {}

std::unique_ptr<Split> SplitterAdaptive::findBestSplit(Data* data, std::vector<int> observations, std::string ID, Arguments* args, double prev_obj) {
	// implementation of adaptive split search
	std::unique_ptr<Split> s;
	return s;
}


