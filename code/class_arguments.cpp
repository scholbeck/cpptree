#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include "class_arguments.h"

Arguments::Arguments() {
  this->min_node_size = 0;
  this->max_children = 0;
}

std::string Arguments::getFilename() {
  return this->filename;
}
void Arguments::setFilename(std::string filename) {
  this->filename = filename;
}

int Arguments::getTargetIndex() {
	return this->target;
}
void Arguments::setTargetIndex(int target) {
	this->target = target;
}

int Arguments::getMaxChildren() {
	return this->max_children;
}
void Arguments::setMaxChildren(int max_children) {
	this->max_children = max_children;
}

int Arguments::getMinNodeSize() {
	return this->min_node_size;
}
void Arguments::setMinNodeSize(int min_node_size) {
	this->min_node_size = min_node_size;
}

std::string Arguments::getAlgorithm() {
	return this->algorithm;
}
void Arguments::setAlgorithm(std::string algo) {
	this->algorithm = algo;
}

std::string Arguments::getObjective() {
	return this->objective;
}
void Arguments::setObjective(std::string obj) {
	this->objective = obj;
}

std::string Arguments::getModel() {
	return this->model;
}
void Arguments::setModel(std::string mod) {
	this->model = mod;
}

std::string Arguments::getTask() {
	return this->task;
}
void Arguments::setTask(std::string task) {
	this->task = task;
}

char Arguments::getSep() {
	return this->sep;
}
void Arguments::setSep(char sep) {
	this->sep = sep;
}


int Arguments::getMaxDepth() {
	return this->max_depth;
}
void Arguments::setMaxDepth(int max_depth) {
	this->max_depth = max_depth;
}

std::string Arguments::getPrint() {
	return this->print;
}
void Arguments::setPrint(std::string print) {
	this->print = print;
}

void Arguments::checkArgs() {
	if (this->max_depth == 0) {
		this->max_depth = 30;
	}
}

void printHelp() {
	std::cout << "Required arguments: filename, children, minsize, algorithm, objective, model.\n";
	std::cout << "Example: ./tree --algorithm exhaustive --objective sse --model mean --minsize 30 --children 2.\n";
}

int Arguments::processArguments(int argc, char** argv)
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
            {"maxdepth", required_argument, nullptr, 1900},
            {"print", required_argument, nullptr, 2000},
            {"help", no_argument, nullptr, 2100}
    };
	
    while (true)
    {
        const int opt = getopt_long(argc, argv, "", long_opts, nullptr);

        if (-1 == opt)
            break;

        switch (opt)
        {
        case 1000:
            this->setFilename(std::string(optarg));
            break;
        case 1100:
            this->setMaxChildren((int) atol(optarg));
            break;
        case 1200:
            if (optarg == "") {
                 this->setMinNodeSize(1);
            } else {
                 this->setMinNodeSize((int) atol(optarg));
            }
            break;
        case 1300:
            this->setAlgorithm(std::string(optarg));
            break;
        case 1400:
            this->setObjective(std::string(optarg));
            break;  
	    	case 1500:
            this->setModel(std::string(optarg));
            break; 
	    	case 1600:
            this->setTask(std::string(optarg));
            break;
        case 1700:
            this->setSep(*optarg);
            break;
        case 1800:
            this->setTargetIndex((int) atol(optarg));
            break;
        case 1900:
            this->setMaxDepth((int) atol(optarg));
            break;
        case 2000:
            this->setPrint(std::string(optarg));
            break;
        case 2100:
            printHelp();
            break;
        case '?':
        default:
            printHelp();
            break;
        }
    }
    
    if (this->getFilename() == "") {
      printf("Filename not specified.\n");
      return -1;
    }
    if (this->getMinNodeSize() == 0) {
      printf("Minimum node size not specified.\n");
      return -1;
    }
    if (this->getMaxChildren() == 0) {
      printf("Maximum child number not specified.\n");
      return -1;
    }
    if (this->getObjective() == "") {
      printf("Objective function not specified.\n");
      return -1;
    }
    if (this->getPrint() == "") {
      this->setPrint("true");
    }
    /*
    if (this->getModel() == "") {
		printf("Model type not specified.\n");
		return -1;
    }
    */
    if (this->getAlgorithm() == "") {
		printf("Search algorithm not specified.\n");
		return -1;
    }
    
    return 0;
    
}


