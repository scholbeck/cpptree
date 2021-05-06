#ifndef TREE_H
#define TREE_H

#include "class_data.h"
#include "class_node.h"

class Tree {
  
  public:
    Tree(Data* data, size_t max_children, size_t min_node_size);
    
    Data* data;
    Node* root;
    std::vector<Node*> nodes;
    size_t node_cnt;
    size_t leafnode_cnt;
    size_t max_children;
    size_t min_node_size;
    size_t target_index;
    Objective objective;
    
    Data* getData();
    Objective getObjective();
    size_t getMinNodeSize();
    size_t getMaxChildren();
    size_t getTargetIndex();
    void setRoot(Node* root);
    void addNode(Node* node);
};


#endif 
