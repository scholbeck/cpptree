#ifndef NODE_H
#define NODE_H

#include "class_tree.h"
#include "class_arguments.h"
#include "class_data.h"
#include "class_objective.h"
#include "string"

class Tree;

class Node {
	
  public:
    Node(std::string id, Data data, Tree* tree, std::string decision_rule);
    
    std::string id;
    Data data;
    std::string decision_rule; // rule that was used to to get from parent to this object
    Tree* tree;
    Model* mod;
    double obj_val;
    int child_cnt;
    std::vector<Node*> child_nodes;
    bool is_leaf;
    
    double getObjValue();
    std::string getDecisionRule();
    std::string getId();
    Model* getModel();
    Split getSplitData();
    void setModel(Model* mod);
    void summary();
    Data getData();
    bool isLeaf();
    std::vector<Node*> split();
    int recursiveSplit();
    std::vector<Node*> getChildNodes();
    void addChild(Node* child);
	void setSplit(Split s);
	std::string createDecisionRule(Split s, int child_ix);
    void buildModel();
    Optimizer* createOptimizer();

    
};
#endif 
