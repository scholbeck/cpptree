#ifndef SPLITGENERATOR_H
#define SPLITGENERATOR_H

#include "class_split.h"
#include "class_data.h"

class SplitGenerator {
  public:
    SplitGenerator();
    virtual std::vector<Split> generate(Data data) = 0;
};

class SplitGeneratorBinExh: public SplitGenerator {
	public:
		SplitGeneratorBinExh();
		std::vector<Split> generate(Data data);
};


#endif 
