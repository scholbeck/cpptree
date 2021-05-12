#ifndef SPLIT_H
#define SPLIT_H

#include <vector>
#include "class_data.h"

using lluint = long long unsigned int;


class Split {

	public:
		Split();
		
		lluint feature_index;
		lluint split_cnt;
		std::vector<double> split_values;
		
		lluint getSplitFeatureIndex();
		void setFeatureIndex(lluint feature_index);
		std::vector<double> getSplitValues();
		void addSplitValue(double splitpoint);
		void clear();
		void summary();
};





#endif 
