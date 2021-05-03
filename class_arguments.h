#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <stdio.h>
#include <stdlib.h>

class Arguments {
  
  public:
    Arguments();
    
    std::string filename;
    
    void setFilename(std::string);
    std::string getFilename();
};


#endif 
