#ifndef TREE_H
#define TREE_H

#include "class_data.h"
#include "class_node.h"

class Tree {
  
  public:
    Tree(Data* data, ssize_t max_splits, ssize_t min_node_size);
    
    Data* data;
    Node* root;
    std::vector<Node*> nodes;
    ssize_t node_cnt;
    ssize_t leafnode_cnt;
    ssize_t max_splits;
    ssize_t min_node_size;
    
    Data* getData();
    ssize_t getMinNodeSize();
    ssize_t getMaxSplits();
    void setRoot(Node* root);
    void addNode(Node* node);
};


#endif 
