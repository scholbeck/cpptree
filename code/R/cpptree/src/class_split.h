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
	  
		int feature_index;
		int split_cnt;
		int max_splits;
		std::vector<double> split_values; // empty for categorical splits
		std::vector<std::vector<int>> subset_level_sets; // empty for continuous splits
		std::vector<std::vector<int>> split_obs; // one vector for each child node
		std::vector<double> obj_values; // one value for each child node
		std::vector<std::string> model_info; // one string for each child node

		virtual std::string createDecisionRule(int child_ix) = 0;
		virtual std::string getSplitType() = 0;
		virtual void computePartitionings(Data* data, std::vector<int> observations) = 0;

		int	getNumberChildNodes();
		int getSplitFeatureIndex();
		std::vector<double> getSplitValues();
		std::vector<double> getObjValues();
		void setObjValues(std::vector<double> obj_values);
		std::vector<std::string> getModelInfo();
		void setModelInfo(std::vector<std::string> model_info);
		void setLevelPartitionings(std::vector<std::vector<int>> level_sets);
		std::vector<std::vector<int>> getLevelPartitionings();
		void setFeatureIndex(int feature_index);
		void addSplitValue(double splitpoint);
		void clear();
		void sortSplitValues();
};


class SplitNum : public Split {

	public:
		SplitNum(int max_splits);

	  	std::string createDecisionRule(int child_ix);
	  	std::string getSplitType();
		void computePartitionings(Data* data, std::vector<int> observations);
		
		std::vector<std::vector<int>> splitBinaryObs(Data* data, double split_value, int col_index);
};

class SplitCateg : public Split {

	public:
		std::map<std::string, int> levels;
		
		SplitCateg(int max_splits, std::map<std::string, int> levels);
		
		std::string createDecisionRule(int child_ix);
		std::string getSplitType();
		void computePartitionings(Data* data, std::vector<int> observations);
		
};




#endif 
