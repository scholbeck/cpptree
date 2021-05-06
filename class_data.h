#ifndef DATA_H
#define DATA_H

#include <armadillo>
#include <vector>
#include "class_split.h"

class Split; // forward declaration due to circular dependency

class Data {
  
  public:
    Data(arma::mat* mat, size_t target_index);
    
    arma::mat* data;
    size_t target_index;
    size_t nrows;
    size_t ncols;
    
    arma::mat* getData();
    void setData(arma::mat* data);
    size_t getTargetIndex();
    
    double getElement(size_t i, size_t j);
    double getNRows();
    double getNCols();
    std::vector<double> getRow(size_t i);
    std::vector<double> getColumn(size_t j);
    double rowMean(size_t i);
    double colMean(size_t j);
    void load(std::string filename);
    void print();
    std::vector<Data*> splitBinary(size_t feature_index);
    std::vector<Data*> split(Split split);
    
};


#endif 
