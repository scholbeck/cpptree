#ifndef SPLITGENERATOR_H
#define SPLITGENERATOR_H

#include "class_arguments.h"
#include "class_split.h"
#include "class_data.h"

class SplitGenerator {
  public:
    Data* data;
    std::vector<int> obs;
    Arguments args;
    
    SplitGenerator(Data* data, std::vector<int> obs, Arguments args);
    virtual ~SplitGenerator() {}
    
    virtual std::vector<Split*> generate() = 0;
    bool checkMinNodeSize(Split* split);
};

class SplitGeneratorBinExh: public SplitGenerator {
	public:
		SplitGeneratorBinExh(Data* data, std::vector<int> obs, Arguments args);
		std::vector<Split*> generate();
};


class SplitGeneratorMultRand: public SplitGenerator {
	public:
		SplitGeneratorMultRand(Data* data, std::vector<int> obs, Arguments args);
		std::vector<Split*> generate();
};



#endif 
