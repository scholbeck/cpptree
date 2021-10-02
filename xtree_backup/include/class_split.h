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
	  	virtual ~Split() {}
	  
		double node_obj_val;
		int feature_index;
		int split_cnt;
		int max_splits;
		std::vector<double> split_values;
		std::vector<std::vector<int>> subset_level_sets;
		std::vector<Model*> child_node_models;
		std::vector<std::vector<int>> splitted_obs;
		
		virtual std::string createDecisionRule(int child_ix) = 0;
		virtual std::string getSplitType() = 0;
		virtual void computePartitionings(Data* data, std::vector<int> observations) = 0;
		virtual void summary() = 0;
		
		int getSplitFeatureIndex();
		std::vector<double> getSplitValues();
		void setLevelPartitionings(std::vector<std::vector<int>> level_sets);
		std::vector<std::vector<int>> getLevelPartitionings();
		int nsplits();
		void setFeatureIndex(int feature_index);
		void addSplitValue(double splitpoint);
		void addChildNodeModel(Model* mod);
		std::vector<Model*> getChildNodeModels();
		void clear();
		void sortSplitValues();
};

class SplitNum : public Split {

	public:
		SplitNum(int max_splits);

	  	  
	  std::string createDecisionRule(int child_ix);
	  std::string getSplitType();
		void computePartitionings(Data* data, std::vector<int> observations);
		void summary();
		
		std::vector<std::vector<int>> splitBinaryObs(Data* data, double split_value, int col_index);
};

class SplitCateg : public Split {

	public:
		std::map<std::string, int> levels;
		
		SplitCateg(int max_splits, std::map<std::string, int> levels);
		
		
		std::string createDecisionRule(int child_ix);
		std::string getSplitType();
		void computePartitionings(Data* data, std::vector<int> observations);
		void summary();
		
};


#endif 
