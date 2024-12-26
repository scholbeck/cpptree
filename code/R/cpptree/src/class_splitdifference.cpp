#include "class_splitdifference.h"
#include <algorithm>
#include <iostream>
#include "helper_functions.h"

SplitDifference::SplitDifference() {}

void SplitDifference::computeSplitDifference(Split* split_upd, Split* split_prev) {
	int n_splits = split_upd->split_obs.size();
	for (int i = 0; i < n_splits; ++i) {
		std::vector<int> emptyvec;
		this->additional_obs.push_back(emptyvec);
		this->removed_obs.push_back(emptyvec);
	}
	if (split_prev == nullptr) {
		// initial split
		for (int i = 0; i < n_splits; ++i) {
			// additional obs = all obs
			this->additional_obs[i] = split_upd->split_obs[i];
			// removed obs = none
		}
	} else {
		for (int i = 0; i < n_splits; ++i) {
			// values are (and need to be) sorted in ascending order
			// additional obs
			std::set_difference(
				split_upd->split_obs[i].begin(), split_upd->split_obs[i].end(), split_prev->split_obs[i].begin(), split_prev->split_obs[i].end(),
				std::inserter(
					this->additional_obs[i], this->additional_obs[i].begin()));
			// removed obs
			std::set_difference(
				split_prev->split_obs[i].begin(), split_prev->split_obs[i].end(), split_upd->split_obs[i].begin(), split_upd->split_obs[i].end(),
				std::inserter(
					this->removed_obs[i], this->removed_obs[i].begin()));
		}
	}
}	
