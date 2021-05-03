#include <stdio.h>
#include <stdlib.h>
#include <armadillo>
#include <getopt.h>
#include <iostream>
#include "class_arguments.h"



Arguments::Arguments() {
  
  this->filename = "";
}

void Arguments::setFilename(std::string filename) {
  this->filename = filename;
}

std::string Arguments::getFilename() {
  return this->filename;
}
