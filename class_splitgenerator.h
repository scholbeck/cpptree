#ifndef SPLITGENERATOR_H
#define SPLITGENERATOR_H

#include "class_arguments.h"
#include "class_split.h"
#include "class_data.h"

class SplitGenerator {
  public:
    Data* data;
    Arguments args;
    SplitGenerator(Data* data, Arguments args);
    virtual std::vector<Split*> generate() = 0;
    bool checkMinNodeSize(Split* split);
};

class SplitGeneratorBinExh: public SplitGenerator {
	public:
		SplitGeneratorBinExh(Data* data, Arguments args);
		std::vector<Split*> generate();
};

class SplitGeneratorBinQuant: public SplitGenerator {
	public:
		SplitGeneratorBinQuant(Data* data, Arguments args);
		std::vector<Split*> generate();
};



#endif 
