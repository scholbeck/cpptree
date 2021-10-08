#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "class_arguments.h"

class Reader {
  
  public:
    Reader();
    Data* read(std::string filename, Arguments* args);
    std::vector<std::string> detectColTypes(std::vector<std::string> vec);
};

#endif 
