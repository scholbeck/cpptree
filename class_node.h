#ifndef NODE_H
#define NODE_H

#include "class_tree.h"
#include "class_arguments.h"
#include "class_data.h"
#include "class_optimizer.h"
#include "string"

class Tree;

class Node {
	
  public:
    Node(std::string id, Data data, Optimizer* optim, Tree* tree);
    
    std::string id;
    Data data; 
    Tree* tree;
    int child_cnt;
    std::vector<Node*> children;
    bool is_leaf;
    Optimizer* optim;
    
    std::string getId();
    void summary();
    Data getData();
    bool isLeaf();
    std::vector<Node*> split();
    void recursiveSplit();
    void addChild(Node* child);
    Optimizer* getOptimizer();
    void setOptimizer(Optimizer* optim);

    
};
#endif 
