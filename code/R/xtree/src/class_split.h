#ifndef SPLIT_H
#define SPLIT_H

#include "class_data.h"
#include <string>
#include <vector>
#include <map>

class Model;
class Data;

class Split {

	public:
		Split(int max_splits);
		
		double node_obj_val;
		int feature_index;
		int split_cnt;
		int max_splits;
		std::vector<double> split_values;
		std::vector<Model*> child_node_models;
		std::vector<std::vector<int>> splitted_obs;
		
		virtual std::string createDecisionRule(int child_ix) = 0;
		virtual void computePartitionings(Data* data) = 0;
		int getSplitFeatureIndex();
		int nsplits();
		void setFeatureIndex(int feature_index);
		std::vector<double> getSplitValues();
		void addSplitValue(double splitpoint);
		void addChildNodeModel(Model* mod);
		std::vector<Model*> getChildNodeModels();
		void clear();
		void sortSplitValues();
		virtual void summary() = 0;
};

class SplitNum : public Split {

	public:
		SplitNum(int max_splits);
		std::string createDecisionRule(int child_ix);
		void computePartitionings(Data* data);
		std::vector<std::vector<int>> splitBinaryObs(Data* data, double split_value, int col_index);
		void summary();
};

class SplitCateg : public Split {

	public:
		std::map<std::string, int> levels;
		std::vector<std::vector<int>> subset_level_sets;
		
		SplitCateg(int max_splits, std::map<std::string, int> levels);
		std::string createDecisionRule(int child_ix);
		void setLevelPartitionings(std::vector<std::vector<int>> level_sets);
		void computePartitionings(Data* data);
		void summary();
};


#endif 
