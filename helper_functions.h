
#ifndef HELPER_H
#define HELPER_H

#include <array>
#include <vector>

std::vector<int> initvectorLLUINT(int start, int end);
double mean(std::vector<double> vec);
void printVectorLLUInt(std::vector<int> vec);
void printVectorInt(std::vector<int> vec);
void printVectorDouble(std::vector<double> vec);
void printVectorString(std::vector<std::string>);
std::vector<int> initVectorSeq(int from, int to);
std::vector<double> initVectorSeqDouble(int from, int to);
std::array<std::vector<int>, 2> diffSet(std::vector<int> set_a, std::vector<int> set_b);

#endif 
