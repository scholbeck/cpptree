#ifndef TREE_H
#define TREE_H

#include "class_data.h"
#include "class_node.h"

class Tree {
  
  public:
    Tree(Data* data);
    
    Data* data;
    Node* root;
    std::vector<Node*> nodes;
    ssize_t node_cnt;
    ssize_t leafnode_cnt;
    
    Data* getData();
    void setRoot(Node* root);
    void addNode(Node* node);
};


#endif 
