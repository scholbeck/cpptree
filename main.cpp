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
#include "class_tree.h"
#include "class_reader.h"
#include "helper_functions.h"
#include <ctime>


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
            {"task", required_argument, nullptr, 1600},
            {"sep", required_argument, nullptr, 1700},
            {"target", required_argument, nullptr, 1800},
            {"help", no_argument, nullptr, 1900}
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
            arguments->setMaxChildren((int) atol(optarg));
            break;
        case 1200:
            arguments->setMinNodeSize((int) atol(optarg));
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
            arguments->setTask(std::string(optarg));
            break;
        case 1700:
            arguments->setSep(*optarg);
            break;
        case 1800:
            arguments->setTargetIndex((int) atol(optarg));
            break;
        case 1900:
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
	Reader reader;
	Data data;
	
	int arg_status = 0;
	if ((arg_status = processArguments(argc, argv, &args)) == -1) {
		return EXIT_FAILURE;
	}
	data = reader.read(args.getFilename(), args.getSep());
	data.setTargetIndex(args.getTargetIndex());
	
	std::clock_t start;
    double duration;
    start = std::clock();
	Tree tree = Tree(data, args);
	int ret = tree.grow();
	duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
	//tree.sortNodesAsc();
	tree.summary();
    std::cout << "Training complete. Duration: "<< duration << "sec\n";

	tree.freeNodeMemory();
	
	return EXIT_SUCCESS;
}


