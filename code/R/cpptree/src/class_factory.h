#ifndef FACTORY_H
#define FACTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "class_data.h"
#include "class_arguments.h"
#include "class_objective.h"
#include "class_model.h"
#include "class_splitgenerator.h"
#include "class_splitter.h"
#include <memory>

class Objective;

class Factory {
  
  public:
    Factory(Data* data, Arguments* args);
    ~Factory() {};
    Data* data;
    Arguments* args;

    std::unique_ptr<Objective> createObjective();
	  std::unique_ptr<Model> createModel();    
    std::unique_ptr<SplitGeneratorBatch> createSplitGeneratorBatch();
    std::unique_ptr<SplitGeneratorStream> createSplitGeneratorStream();
    std::unique_ptr<Splitter> createSplitter();

};


#endif 
