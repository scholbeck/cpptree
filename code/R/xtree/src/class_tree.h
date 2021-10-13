#ifndef TREE_H
#define TREE_H

#include "class_data.h"
#include "class_node.h"
#include "class_factory.h"
#include <memory>

class Node;

class Tree {
  
  public:
    Tree(Data* data, Arguments* args);

    Arguments* args;
    Factory* factory;
    Data* data;
    std::vector<std::unique_ptr<Node>> nodes; // Tree object is responsible for life time of Node objects
    int node_cnt;
    int leafnode_cnt;
    int depth;
    
    void addNode(std::unique_ptr<Node> node);
    Factory* getFactory();
    Arguments* getArgs();
    int grow();
    void summary();
    void print();
    void printSubTree(Node* node);
    void sortNodesAsc();
    void gatherTreeInformation();
};


#endif 
