#ifndef TREE_H
#define TREE_H

#include "class_data.h"
#include "class_node.h"

class Tree {
  
  public:
    Tree(Data* data);
    
    Data* data;
    Node* root;
    
    Data* getData();
    void setRoot(Node* root);
};


#endif 
