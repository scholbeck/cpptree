
#ifndef HELPER_H
#define HELPER_H

#include <vector>

using lluint = long long unsigned int;

std::vector<lluint> initvectorLLUINT(lluint start, lluint end);
double mean(std::vector<double> vec);
void printVectorLLUInt(std::vector<lluint> vec);
void printVectorInt(std::vector<int> vec);
void printVectorDouble(std::vector<double> vec);
std::vector<lluint> initVectorSeq(lluint from, lluint to);

#endif 
