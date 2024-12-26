
#include "class_data.h"
#include "class_sorteddata.h"
#include <map>
#include <string>
#include <algorithm>

SortedData::SortedData() {}

SortedFeature::SortedFeature() {}

SortedFeatureSubset::SortedFeatureSubset(size_t size) {
	this->sorted_values.reserve(size);
}

void SortedData::sort(Data* data) {
	int n_rows = data->nrows();
	int n_cols = data->ncols();
	for (int j = 0; j < n_cols; j++) {
		if (j == data->getTargetIndex()) {
			continue;
		}

		SortedFeatureSubset* subset = new SortedFeatureSubset(n_rows);
		subset->ID = "0";
		for (int i = 0; i < n_rows; i++) {
			subset->sorted_values.push_back(std::pair<double, int>(data->elem(i, j), i));
		}
		std::sort(subset->sorted_values.begin(), subset->sorted_values.end());

		SortedFeature* single_feature = new SortedFeature();
		single_feature->index = j;
		single_feature->subsets.insert(std::pair<std::string, SortedFeatureSubset*>("0", subset));

		this->sorted_features.insert(std::pair<int, SortedFeature*>(j, single_feature));
	}
}

void SortedData::split(std::string ID, std::vector<std::vector<int>> subset_obs) {
	for (auto it = this->sorted_features.begin(); it != this->sorted_features.end(); ++it) {
		//std::cout << it->first << std::flush;
		sorted_features.at(it->first)->splitSubset(ID, subset_obs);
	}
}

void SortedFeature::splitSubset(std::string ID, std::vector<std::vector<int>> subset_obs) {
	for (int s = 0; s < subset_obs.size(); s++) {
		SortedFeatureSubset* new_subset = new SortedFeatureSubset(subset_obs[s].size());
		for (auto it_values = this->subsets.at(ID)->sorted_values.begin();
			it_values != this->subsets.at(ID)->sorted_values.end();
			++it_values) {
				auto it = std::lower_bound(subset_obs[s].begin(), subset_obs[s].end(), (*it_values).second);
				// subset_obs already sorted so we can use std::lower_bound
				if (*it == (*it_values).second) {
					new_subset->sorted_values.push_back(*it_values);
				}
		}
		std::string new_ID = ID + std::to_string(s);
		new_subset->ID = new_ID;
		this->subsets.insert(std::pair<std::string, SortedFeatureSubset*> (new_ID, new_subset));
	}
	delete(this->subsets.at(ID));
	this->subsets.erase(ID);
}

/*

void SortedFeature::splitSubset(std::string ID, std::vector<std::vector<int>> subset_obs) {
	for (int s = 0; s < subset_obs.size(); s++) {
		SortedFeatureSubset* new_subset = new SortedFeatureSubset(subset_obs[s].size());
		for (auto it_values = this->subsets.at(ID)->sorted_values.begin();
			it_values != this->subsets.at(ID)->sorted_values.end();
			++it_values) {	
				for (auto it_subset_obs = subset_obs[s].begin();
					it_subset_obs != subset_obs[s].end();
					++it_subset_obs) {
					if ((*it_values).second == *it_subset_obs) {
						new_subset->sorted_values.push_back(*it_values);
						break;
					}
				}
				
		}
		std::string new_ID = ID + std::to_string(s);
		new_subset->ID = new_ID;
		this->subsets.insert(std::pair<std::string, SortedFeatureSubset*> (new_ID, new_subset));
	}
	delete(this->subsets.at(ID));
	this->subsets.erase(ID);
}
*/

SortedFeatureSubset* SortedFeature::getSubset(std::string ID) {
	return this->subsets.at(ID);
}

SortedFeatureSubset* SortedData::getSortedFeatureSubset(std::string ID, int feature) {
	return this->sorted_features.at(feature)->getSubset(ID);
}
