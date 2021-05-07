#ifndef NODE_H
#define NODE_H

#include <armadillo>
#include "class_data.h"
#include "class_tree.h"
#include "class_optimizer.h"

class Tree;
// forward declaration for circular dependency

class Node {
	
  public:
    Node(int id, Data* data, Tree* tree);
    
    Tree* tree;
    Data* data;
    int id;
    ssize_t child_cnt;
    std::vector<Node*> children;
    bool is_leaf;
    
    int getId();
    Data* getData();
    
    std::vector<Node*> split(Optimizer optimizer);
    void addChild(Node* child);
    
};


#endif 
