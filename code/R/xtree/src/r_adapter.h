#ifndef RADAPTER_H
#define RADAPTER_H

#include <Rcpp.h>
#include "class_tree.h"

class RAdapter {
    
  public:
    std::unique_ptr<Tree> tree;
    int depth, node_cnt, leafnode_cnt;
    
    RAdapter(Rcpp::DataFrame r_data, Rcpp::StringVector coltypes, Rcpp::List params);
    ~RAdapter() {}
      
    void print();
    Rcpp::DataFrame getTreeStructure();
};



#endif