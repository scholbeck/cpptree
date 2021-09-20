#ifndef RINTERFACE_H
#define RINTERFACE_H

#include <Rcpp.h>
#include "class_tree.h"

class XTree {
    
  public:
    Tree* tree;
    
    XTree(Rcpp::DataFrame r_data, int target_index, Rcpp::StringVector coltypes,
          Rcpp::List categ_encodings, Rcpp::StringVector params);
    
    void grow();
    void print();
};



#endif