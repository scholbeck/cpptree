#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stdlib.h>
#include <string>

class Reader {
  
  public:
    Reader();
    Data read(std::string filename, char sep);
    std::vector<std::string> detectColTypes(std::vector<std::string> vec);
};

#endif 
