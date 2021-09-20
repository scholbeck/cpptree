#include <Rcpp.h>
#include "r_interface.h"

RCPP_MODULE(xtree) {
  
  Rcpp::class_<XTree>("XTree")
  
  .constructor<Rcpp::DataFrame, int, Rcpp::StringVector, Rcpp::List, Rcpp::StringVector>("Construct a tree object.")
  .method("grow", &XTree::grow)
  .method("print", &XTree::print)
  ;
}

RCPP_EXPOSED_CLASS(XTree)