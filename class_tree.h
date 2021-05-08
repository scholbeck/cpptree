#ifndef TREE_H
#define TREE_H

#include "class_data.h"
#include "class_node.h"

class Tree {
  
  public:
    Tree(Data* data, uint max_children, uint min_node_size);
    
    Data* data;
    Node* root;
    std::vector<Node*> nodes;
    uint node_cnt;
    uint leafnode_cnt;
    uint max_children;
    uint min_node_size;
    uint target_index;
    Objective* obj;
    
    Data* getData();
    Objective* getObjective();
    uint getMinNodeSize();
    uint getMaxChildren();
    uint getTargetIndex();
    void setRoot(Node* root);
    void addNode(Node* node);
};


#endif 
