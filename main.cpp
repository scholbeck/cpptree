#include <stdio.h>
#include <stdlib.h>
#include <armadillo>
#include <getopt.h>
#include <iostream>
#include "class_arguments.h"
#include "class_data.h"
#include "class_tree.h"
#include "class_node.h"
#include "class_optimizer.h"


void processArguments(int argc, char** argv, Arguments *arguments)
{
    const char* const short_opts = "f:h";
    const option long_opts[] = {
            {"filename", required_argument, nullptr, 'f'},
            {"help", no_argument, nullptr, 'h'},
            {nullptr, no_argument, nullptr, 0}
    };
	
    while (true)
    {
        const int opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        if (-1 == opt)
            break;

        switch (opt)
        {
        case 'f':
            arguments->setFilename(std::string(optarg));
            break;
        case 'h': // -h or --help
        case '?': // Unrecognized option
        default:
            // PrintHelp();
            break;
        }
    }
}

int main(int argc, char *argv[]) {

	Arguments arguments;	
	processArguments(argc, argv, &arguments);
	arma::mat mat = arma::mat();
	Data data = Data(&mat);
	data.load(arguments.getFilename());
	data.print();
	
	Tree tree = Tree(&data);
	
	Node n1 = Node(0, &tree);
	Node n2 = Node(00, &tree);
	Node n3 = Node(01, &tree);
	
	n1.addChild(&n2);
	n1.addChild(&n3);
	n3.getData()->print();
  
	ExhaustiveSearch optim_exhaust = ExhaustiveSearch(&data);
	RandomSearch optim_rand = RandomSearch(&data);
	
	n3.split(1, 10, optim_rand);
	
	return EXIT_SUCCESS;
}


