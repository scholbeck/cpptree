#ifndef RINTERFACE_H
#define RINTERFACE_H

#include <Rcpp.h>
#include "class_tree.h"

class XTree {
    
  public:
    Tree* tree;
    int depth, node_cnt, leafnode_cnt;
    
    XTree(Rcpp::DataFrame r_data, int target_index, Rcpp::StringVector coltypes,
          Rcpp::List categ_encodings, Rcpp::StringVector params);
    
    void grow();
    void print();
    Rcpp::DataFrame getTreeStructure();
};



#endif