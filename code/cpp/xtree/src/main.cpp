#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <algorithm>
#include "class_data.h"
#include "class_arguments.h"
#include "class_split.h"
#include "class_model.h"
#include "class_node.h"
#include "class_tree.h"
#include "class_reader.h"
#include "helper_functions.h"
#include <ctime>


int main(int argc, char *argv[]) {
	
	std::unique_ptr<Arguments> args = std::unique_ptr<Arguments>(new Arguments());
	std::unique_ptr<Reader> reader = std::unique_ptr<Reader>(new Reader());
	
	int arg_status = 0;
	if ((arg_status = args->processArguments(argc, argv)) == -1) {
		return EXIT_FAILURE;
	}
    args->checkArgs();
	std::unique_ptr<Data> data = reader->read(args->getFilename(), args.get());
	data->setTargetIndex(args->getTargetIndex() + 1);
    if (data->selfCheck() == false) {
        std::cout << "Specified wrong target index. Aborting..\n";
        return EXIT_FAILURE;
    }
    std::clock_t start;
    double duration;
    Tree tree = Tree(data.get(), args.get());
    start = std::clock();
    int ret = tree.grow();
	duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    if (args->getPrint() == "true") {
		tree.summary();
		std::cout << "Training complete. Duration: " << duration << "sec\n";
	}
	
	return EXIT_SUCCESS;
}


