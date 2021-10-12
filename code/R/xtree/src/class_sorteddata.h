#ifndef SORTED_DATA_H
#define SORTED_DATA_H

#include <vector>
#include <map>
#include <string>
#include "class_data.h"

class Data;

class SortedFeatureSubset {

	public:
		SortedFeatureSubset(size_t size);
		std::string ID;
		std::vector<std::pair<double, int>> sorted_values;
		// first pair element: sorted asc feature values
		// second pair element: corresponding row IDs
};

class SortedFeature {

	public:
		SortedFeature();
		int index;
		std::map<std::string, SortedFeatureSubset*> subsets;
		// maps node ID, e.g., "0010101", to SortedFeatureSubset

		void splitSubset(std::string ID, std::vector<std::vector<int>> subset_obs);
		SortedFeatureSubset* getSubset(std::string ID);
};

class SortedData {
  
	public:
		SortedData();
		
		std::map<int, SortedFeature*> sorted_features;
		// maps feature index to SortedFeature objects

		void sort(Data* data);
		void split(std::string ID, std::vector<std::vector<int>> subset_obs);
		SortedFeatureSubset* getSortedFeatureSubset(std::string ID, int feature);
};


#endif 