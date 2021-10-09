#ifndef NODE_H
#define NODE_H

#include "class_tree.h"
#include "class_arguments.h"
#include "class_data.h"
#include "class_objective.h"
#include "class_split.h"
#include "string"

class Tree;

class Node {
	
  public:
    Node(std::string id, Tree* tree, std::vector<int> observations, double obj_val, std::string decision_rule);
    
    std::string id;
    std::vector<int> observations;
    Split* split;
    std::string decision_rule; // rule that was used to to get from parent to this object
    std::string model_info; // info on model that resides in this node
    Tree* tree;
    Model* mod;
    double obj_val;
    int child_cnt;
    std::vector<Node*> child_nodes;
    bool is_leaf;
  
    
    double getObjValue();
    std::string getDecisionRule();
    std::string getId();
    Split* getSplitData();
    Model* getModel();
    void setModel(Model* mod);
    void summary();
    bool isLeaf();
    std::vector<Node*> splitNode();
    int recursiveSplit();
    std::vector<Node*> getChildNodes();
    void addChild(Node* child);
	  void setSplit(Split* s);
	  std::string createDecisionRule(Split* s, int child_ix);
    std::string getModelInfo();
    void setModelInfo(std::string);
    
};
#endif 
