#include <Rcpp.h>
#include "r_interface.h"

RCPP_MODULE(xtree) {
  
  Rcpp::class_<XTree>("XTree")
  
  .constructor<Rcpp::DataFrame, int, Rcpp::StringVector, Rcpp::List, Rcpp::StringVector>("Construct a tree object.")
  .field("depth", &XTree::depth)
  .field("node_cnt", &XTree::node_cnt)
  .field("leafnode_cnt", &XTree::leafnode_cnt)
  .method("grow", &XTree::grow)
  .method("print", &XTree::print)
  .method("getTreeStructure", &XTree::getTreeStructure)
  ;
}

RCPP_EXPOSED_CLASS(XTree)