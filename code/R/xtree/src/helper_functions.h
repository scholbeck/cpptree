
#ifndef HELPER_H
#define HELPER_H

#include <array>
#include <vector>

std::vector<int> initvectorLLUINT(int start, int end);
double mean(std::vector<double> vec);
double cumsum(std::vector<double> vec);
void printVectorLLUInt(std::vector<int> vec);
void printVectorInt(std::vector<int> vec);
void printVectorDouble(std::vector<double> vec);
void printVectorString(std::vector<std::string>);
std::vector<int> initVectorSeq(int from, int to);
std::vector<double> initVectorSeqDouble(int from, int to);
std::array<std::vector<int>, 2> diffSet(std::vector<int> set_a, std::vector<int> set_b);
bool checkObsSize(std::vector<std::vector<int>> split_obs, int min_node_size);
double quantileSortedVec(double quantile, std::vector<double> vec);
long long int factorial(int n);
std::vector<std::vector<int>> permuteIndices(int n, int k);
std::vector<int> convertDoubleToIntVector(std::vector<double> vec);

#endif 
