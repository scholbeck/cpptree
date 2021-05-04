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
    Node(int id, Tree* tree);
    
    Tree* tree;
    int id;
    Node* child_left;
    Node* child_right;
    
    int getId();
    void setChildLeft(Node* child_node);
    Node* getChildLeft();
    void setChildRight(Node* child_node);
    Node* getChildRight();
    Data* getData();
    void split(int target_index, int min_node_size, Optimizer optimizer);
    
};


#endif 
