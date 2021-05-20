#ifndef TREE_H
#define TREE_H

#include "class_data.h"
#include "class_optimizer.h"
#include "class_node.h"

class Node;

class Tree {
  
  public:
    Tree(Data data, Arguments args);
    
    Arguments args;
    Data data;
    Node* root;
    std::vector<Node*> nodes;
    int node_cnt;
    int leafnode_cnt;
    
    void addNode(Node* node);
    void grow();
    void summary();
    void freeNodeMemory();
    void print();
    void sortNodesAsc();
};


#endif 
