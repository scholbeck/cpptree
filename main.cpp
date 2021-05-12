#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include "class_data.h"
#include "class_arguments.h"
#include "class_split.h"
#include "class_model.h"
#include "class_optimizer.h"
#include "class_node.h"
#include "helper_functions.h"


/*
void printHelp() {
	printf("Required arguments: filename, maxsplit, minsize.\n");
}

int processArguments(int argc, char** argv, Arguments *arguments)
{
	const option long_opts[] = {
            {"filename", required_argument, nullptr, 1000},
            {"maxsplit", required_argument, nullptr, 1100},
            {"minsize", required_argument, nullptr, 1200},
            {"help", no_argument, nullptr, 1300}
    };
	
    while (true)
    {
        const int opt = getopt_long(argc, argv, "", long_opts, nullptr);

        if (-1 == opt)
            break;

        switch (opt)
        {
        case 1000:
            arguments->setFilename(std::string(optarg));
            break;
        case 1100:
            arguments->setMaxSplits((ssize_t) atol(optarg));
            break;
        case 1200:
            arguments->setMinNodeSize((ssize_t) atol(optarg));
            break;
        case 1300:
        case '?': // Unrecognized option
        default:
            printHelp();
            break;
        }
    }
    
    if (arguments->getFilename() == "") {
		printf("Filename not specified.\n");
		return -1;
    }
    if (arguments->getMinNodeSize() == 0) {
		printf("Minimum node size not specified.\n");
		return -1;
    }
    if (arguments->getMaxSplits() == 0) {
		printf("Maximum split number not specified.\n");
		return -1;
    }
    
    return 0;
    
}

*/
int main(int argc, char *argv[]) {

	/*
	Arguments arguments;
	int arg_status = 0;
	if ((arg_status = processArguments(argc, argv, &arguments)) == -1) {
		return EXIT_FAILURE;
	}
	*/
	Data data;
	data.initRandom(1000, 5);
	data.setTargetIndex(0);
	// data.summary();
	std::vector<lluint> r = {0, 1, 5};
	std::vector<lluint> c = {0, 1, 4};
	
	Data subset;
	subset = data.subset(r, c);
	
	/*	
	Split s = Split();
	s.addSplitValue(data.elem(0, 3));
	s.addSplitValue(data.elem(10, 3));
	s.addSplitValue(data.elem(20, 3));
	s.addSplitValue(data.elem(30, 3));

	s.setFeatureIndex(1);
	std::vector<Data> part2 = data.split(s);
	
	int n_splits = part2.size();
	for (int i = 0; i < n_splits; i++) {
		part2[i].summary();
	}
	*/
	ModelAverage mod = ModelAverage(data);
	mod.train();

	Optimizer optim = Optimizer();
	ObjectiveSSE obj = ObjectiveSSE();
	Split sp;
	mod.summary();
	/*
	double obj_val = obj.compute(data, &mod);
	sp = optim.exhaustiveSearch(data, &obj, &mod);
	sp.summary();
	*/
	
	std::vector<Node*> child_nodes;
	Node n = Node("0", &data);
	child_nodes = n.split(&mod, &obj, &optim, "exhaustive");
	int n_childs = child_nodes.size();
	for (int i = 0; i < n_childs; i++) {
		child_nodes[i]->summary();
	}
	
	
	
	return EXIT_SUCCESS;
}


