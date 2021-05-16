#ifndef SPLIT_H
#define SPLIT_H

#include <vector>

class Model;

class Split {

	public:
		Split();
		
		int feature_index;
		int split_cnt;
		std::vector<double> split_values;
		std::vector<Model*> child_node_models;
		
		int getSplitFeatureIndex();
		void setFeatureIndex(int feature_index);
		std::vector<double> getSplitValues();
		void addSplitValue(double splitpoint);
		void addChildNodeModel(Model* mod);
		std::vector<Model*> getChildNodeModels();
		void clear();
		void summary();
};





#endif 
