#ifndef TREE_H
#define TREE_H

#include "class_data.h"
#include "class_node.h"
#include "class_factory.h"

class Node;

class Tree {
  
  public:
    Tree(Data* data, Arguments* args);
    ~Tree() {
      while (this->nodes.empty() == false) {
		    delete(this->nodes.back());
		    this->nodes.pop_back();
	    }
    }
    Arguments* args;
    Factory* factory;
    Data* data;
    Node* root;
    std::vector<Node*> nodes;
    int node_cnt;
    int leafnode_cnt;
    int depth;
    
    void addNode(Node* node);
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
