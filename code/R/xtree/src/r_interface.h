#ifndef RINTERFACE_H
#define RINTERFACE_H

#include <Rcpp.h>
#include "class_tree.h"

class RInterface {
    
  public:
    Tree* tree;
    int depth, node_cnt, leafnode_cnt;
    
    RInterface(Rcpp::DataFrame r_data, Rcpp::StringVector coltypes, Rcpp::List params);
      
    void grow();
    void print();
    Rcpp::DataFrame getTreeStructure();
};



#endif