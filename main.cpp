#include <stdio.h>
#include <stdlib.h>
#include <armadillo>
#include <getopt.h>
#include <iostream>
#include "class_arguments.h"
#include "class_data.h"
#include "class_tree.h"
#include "class_node.h"
#include "class_model.h"
#include "class_objective.h"
#include "class_optimizer.h"


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

int main(int argc, char *argv[]) {

	Arguments arguments;
	int arg_status = 0;
	if ((arg_status = processArguments(argc, argv, &arguments)) == -1) {
		return EXIT_FAILURE;
	}
	arma::mat mat = arma::mat();
	Data data = Data(mat, 4);
	data.load(arguments.getFilename());
	// data.print();
	
	Tree tree = Tree(&data, arguments.getMaxSplits(), arguments.getMinNodeSize());
	
	Node n1 = Node(0, &data, &tree);
	Node n2 = Node(00, &data, &tree);
	Node n3 = Node(01, &data, &tree);
	
	n1.addChild(&n2);
	n1.addChild(&n3);
	//n3.getData()->print();
  
	OptimizerExhaustiveSearch optim_exhaust = OptimizerExhaustiveSearch();
	OptimizerRandomSearch optim_rand = OptimizerRandomSearch();
	
	std::vector<Node*> child_nodes;
	Split s = Split();
	s.setFeatureIndex(1);
	s.addSplitValue(1);
	s.addSplitValue(2);
	s.addSplitValue(3);
	
	std::vector<lluint> rows = {1, 50, 10, 3};
	std::vector<lluint> cols = {1, 2, 4};
	
	Data subset = data.subset(rows, cols);
	
	std::vector<Data> data_subsets = data.split(s);
	
	//printf("%f", data.elem(10, 1));
	ModelConstant mod = ModelConstant(data);
	mod.train();
	//data_subsets[2].print();
	ObjectiveSSE objective = ObjectiveSSE();
	std::vector<double> preds = mod.predict(data);
	double obj_value = objective.compute(data, mod);
	printf("preds size %ld", preds.size());
	// child_nodes = n3.split(optim_exhaust);
	
	return EXIT_SUCCESS;
}


