#ifndef NODE_H
#define NODE_H

#include "class_data.h"
#include "class_optimizer.h"
#include "string"

class Node {
	
  public:
    Node(std::string id, Data* data);
    
    std::string id;
    Data* data; 
    uint child_cnt;
    std::vector<Node*> children;
    bool is_leaf;
    
    std::string getId();
    void summary();
    Data* getData();
    std::vector<Node*> split(Model* mod, Objective* obj, Optimizer* optim, std::string algo);
    void addChild(Node* child);
    
    
    
};
#endif 
