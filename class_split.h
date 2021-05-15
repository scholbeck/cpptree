#ifndef SPLIT_H
#define SPLIT_H

#include <vector>
#include "class_data.h"

class Split {

	public:
		Split();
		
		int feature_index;
		int split_cnt;
		std::vector<double> split_values;
		
		int getSplitFeatureIndex();
		void setFeatureIndex(int feature_index);
		std::vector<double> getSplitValues();
		void addSplitValue(double splitpoint);
		void clear();
		void summary();
};





#endif 
