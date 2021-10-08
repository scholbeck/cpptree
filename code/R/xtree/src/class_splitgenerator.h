#ifndef SPLITGENERATOR_H
#define SPLITGENERATOR_H

#include "class_arguments.h"
#include "class_split.h"
#include "class_data.h"

class SplitGenerator {
  public:
    SplitGenerator();
    virtual ~SplitGenerator() {}
    
    virtual std::vector<Split*> generate(Data* data, std::vector<int> observations, std::string ID, Arguments* args) = 0;
    bool checkMinNodeSize(Split* split, int min_node_size);
};

class SplitGeneratorBinExh: public SplitGenerator {
	public:
		SplitGeneratorBinExh();
		std::vector<Split*> generate(Data* data, std::vector<int> observations, std::string ID, Arguments* args);
};


class SplitGeneratorMultRand: public SplitGenerator {
	public:
		SplitGeneratorMultRand();
		std::vector<Split*> generate(Data* data, std::vector<int> observations, std::string ID, Arguments* args);
};



#endif 
