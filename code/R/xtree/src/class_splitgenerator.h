#ifndef SPLITGENERATOR_H
#define SPLITGENERATOR_H

#include "class_arguments.h"
#include "class_split.h"
#include "class_data.h"

// ---------------
// PARENT CLASS (ABSTRACT)
// ---------------

class SplitGenerator {
  public:
    SplitGenerator();
    virtual ~SplitGenerator() {}
    
    bool checkMinNodeSize(Split* split, int min_node_size);
};

// ---------------
// FIRST INHERITANCE LEVEL (ABSTRACT)
// ---------------

class SplitGeneratorBatch : public SplitGenerator {
	public: SplitGeneratorBatch();
	virtual ~SplitGeneratorBatch() {}

	virtual std::vector<Split*> generate(Data* data, std::vector<int> observations, std::string ID, Arguments* args) = 0;
	// returns batch of split pointers
};

class SplitGeneratorStream : public SplitGenerator {
	public: SplitGeneratorStream();
	virtual ~SplitGeneratorStream() {}

	virtual Split* generate(double prev_obj_value, Data* data, std::vector<int> observations, std::string ID, Arguments* args) = 0;
	// returns single split pointer
};

// ---------------
// SECOND INHERITANCE LEVEL
// ---------------
// BATCH SPLIT GENERATORS

class SplitGeneratorBatchBinExh: public SplitGeneratorBatch {
	public:
		SplitGeneratorBatchBinExh();
		std::vector<Split*> generate(Data* data, std::vector<int> observations, std::string ID, Arguments* args);
};


class SplitGeneratorBatchMultRand: public SplitGeneratorBatch {
	public:
		SplitGeneratorBatchMultRand();
		std::vector<Split*> generate(Data* data, std::vector<int> observations, std::string ID, Arguments* args);
};

// ---------------
// SECOND INHERITANCE LEVEL
// ---------------
// STREAM SPLIT GENERATORS

class SplitGeneratorStreamBayesianOptim: public SplitGeneratorStream {
	public:
		SplitGeneratorStreamBayesianOptim();
		Split* generate(double prev_obj_value, Data* data, std::vector<int> observations, std::string ID, Arguments* args);
};


#endif 
