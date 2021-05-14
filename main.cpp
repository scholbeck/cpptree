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
#include "class_factory.h"
#include "helper_functions.h"


void printHelp() {
	std::cout << "Required arguments: filename, children, minsize, algorithm, objective, model.\n";
	std::cout << "Example: ./tree --algorithm exhaustive --objective sse --model mean --minsize 30 --children 2.\n";
}

int processArguments(int argc, char** argv, Arguments *arguments)
{
	const option long_opts[] = {
            {"filename", required_argument, nullptr, 1000},
            {"children", required_argument, nullptr, 1100},
            {"minsize", required_argument, nullptr, 1200},
            {"algorithm", required_argument, nullptr, 1300},
            {"objective", required_argument, nullptr, 1400},
            {"model", required_argument, nullptr, 1500},
            {"help", no_argument, nullptr, 1600}
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
            arguments->setMaxChildren((lluint) atol(optarg));
            break;
        case 1200:
            arguments->setMinNodeSize((lluint) atol(optarg));
            break;
        case 1300:
            arguments->setAlgorithm(std::string(optarg));
            break;
        case 1400:
            arguments->setObjective(std::string(optarg));
            break;  
		case 1500:
            arguments->setModel(std::string(optarg));
            break; 
        case 1600:
            printHelp();
            break;
        case '?': // Unrecognized option
        default:
            printHelp();
            break;
        }
    }
    /*
    if (arguments->getFilename() == "") {
		printf("Filename not specified.\n");
		return -1;
    }
    * IMPLEMENT FILE READER
    */
    if (arguments->getMinNodeSize() == 0) {
		printf("Minimum node size not specified.\n");
		return -1;
    }
    if (arguments->getMaxChildren() == 0) {
		printf("Maximum child number not specified.\n");
		return -1;
    }
    if (arguments->getObjective() == "") {
		printf("Objective function not specified.\n");
		return -1;
    }
    if (arguments->getModel() == "") {
		printf("Model type not specified.\n");
		return -1;
    }
    if (arguments->getAlgorithm() == "") {
		printf("Search algorithm not specified.\n");
		return -1;
    }
    
    return 0;
    
}

int main(int argc, char *argv[]) {
	
	Arguments args;
	int arg_status = 0;
	if ((arg_status = processArguments(argc, argv, &args)) == -1) {
		return EXIT_FAILURE;
	}
	
	Data data;

	lluint n = 100;
	data.initRandom(n, 5);
	data.setTargetIndex(0);
	
	Factory factory = Factory(args);
	Model* mod = factory.createModel();
	Objective* obj = factory.createObjective();
	Optimizer* optim = factory.createOptimizer();
	
	optim->setObjective(obj);
	optim->setModel(mod);
	optim->setMinNodeSize(args.getMinNodeSize());
	
	std::vector<Node> child_nodes;
	Node node = Node("0", data);
	node.setOptimizer(optim);
	child_nodes = node.split();

	if (child_nodes.empty() == false) {
		int n_childs = child_nodes.size();
		for (int i = 0; i < n_childs; i++) {
			child_nodes[i].summary();
		}
	}
	
	free(mod);
	free(obj);
	free(optim);
	
	return EXIT_SUCCESS;
}


