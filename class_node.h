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
    Node(std::string id, Data data, Tree* tree);
    
    std::string id;
    Data data; 
    Split split_data;
    Tree* tree;
    Model* mod;
    int child_cnt;
    std::vector<Node*> child_nodes;
    bool is_leaf;
    double obj_val;
    
    std::string getId();
    Model* getModel();
    void setModel(Model* mod);
    void summary();
    Data getData();
    bool isLeaf();
    std::vector<Node*> split();
    void recursiveSplit();
    std::vector<Node*> getChildNodes();
    void addChild(Node* child);
	void setSplit(Split s);
    Optimizer* getOptimizer();
    Optimizer* createOptimizer(Arguments args);

    
};
#endif 
