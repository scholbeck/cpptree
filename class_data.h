#ifndef DATA_H
#define DATA_H

#include <armadillo>

class Data {
  
  public:
    Data(arma::mat* mat);
    
    arma::mat* data;
    
    arma::mat* getData();
    void setData(arma::mat* data);
    void load(std::string filename);
    void print();
    
};


#endif 
