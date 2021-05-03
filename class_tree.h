#ifndef TREE_H
#define TREE_H

#include <armadillo>
#include "class_node.h"

class Tree {
  
  public:
    Tree(arma::mat data);
    
    arma::mat data;
    Node* root;
    
    arma::mat getData();
    void setRoot(Node* root);
};


#endif 
