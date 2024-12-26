#include <algorithm>
#include <array>
#include <iostream>
#include "helper_functions.h"
#include <cmath>


std::vector<int> initVectorLLUInt(int start, int end) {
	
	std::vector<int> vec;
	for (int i = start; i <= end; ++i) {
		vec.push_back(i);
	}
	return vec;
}

void printVectorLLUInt(std::vector<int> vec) {
	
	int n = vec.size();
	printf("Printing vector:\n");
	for (int i = 0; i < n; ++i) {
		std::cout << vec[i] << "\n";
	}
	printf("\n");
}

void printVectorInt(std::vector<int> vec) {
	
	int n = vec.size();
	printf("Printing vector:\n");
	for (int i = 0; i < n; ++i) {
		std::cout << vec[i] << "\n";
	}
	printf("\n");
}

void printVectorString(std::vector<std::string> vec) {
	
	int n = vec.size();
	printf("Printing vector:\n");
	for (int i = 0; i < n; ++i) {
		std::cout << vec[i] << "\n";
	}
	printf("\n");
}

void printVectorDouble(std::vector<double> vec) {
	
	int n = vec.size();
	printf("Printing vector:\n");
	for (int i = 0; i < n; ++i) {
		std::cout << vec[i] << "\n";
	}
	printf("\n");
}

double mean(std::vector<double> vec) {
	
	int n = vec.size();
	double cumsum = 0;
	for (int i = 0; i < n; ++i) {
		cumsum += vec[i];
	}
	return cumsum / n;
}


std::vector<int> initVectorSeq(int from, int to) {
	std::vector<int> seq;
	seq.reserve(to - from);
	for (int i = from; i <= to; ++i) {
		seq.push_back(i);
	}
	return seq;

}

std::vector<double> initVectorSeqDouble(int from, int to) {
	std::vector<double> seq;
	for (int i = from; i <= to; ++i) {
		seq.push_back((double) i);
	}
	return seq;

}

std::vector<double> initVectorDoubleValue(double value, int size) {
	std::vector<double> vec;
	for (int i = 0; i < size; ++i) {
		vec.push_back(value);
	}
	return vec;
}

std::array<std::vector<int>, 2> diffSet(std::vector<int> set_a, std::vector<int> set_b) {
	std::array<std::vector<int>, 2> diff;
	// first elem = additional obs in set a, second elem = removed obs in set a
	// compared to set b
	std::set_difference(set_a.begin(), set_a.end(), set_b.begin(), set_b.end(),
	std::inserter(diff[0], diff[0].begin()));
	std::set_difference(set_b.begin(), set_b.end(), set_a.begin(), set_a.end(),
	std::inserter(diff[1], diff[1].begin()));
	return diff;
}
/*
double recursiveMeanForward(double obs_additional, double mean_backward) {
	double mean_forward;
	mean_forward = ((n-1) / n) * mean_backward + (1 / n) * obs_additional; 
	mean_forward = (n / (n-1)) * (mean_backward - (1 / n) * obs_additional);
	return mean_forward;
}

double recursiveMeanBackward(double obs_removed, double mean_forward) {
	double mean_backward;
	mean_backward = (n / (n-1)) * (mean_forward - (1 / n) * obs_removed);
	return mean_backward;
}

std::vector<double> recursiveSumSquaresTotalForward(double obs_additional, double SST_backward, double M_backward) {
	std::vector<double> return_vec(2);
	double SST_forward, M_forward;
	M_forward = M_backward + (1 / n) * (obs_additional - M_backward);
	SST_forward = SST_backward + ((obs_additional - M_backward) * (obs_additional - M_forward));
	return_vec[0] = M_forward;
	return_vec[1] = SST_forward;
	return return_vec;
}

std::vector<double> recursiveSumSquaresTotalBackward(double obs_removed, double SST_forward, double M_forward) {
	std::vector<double> return_vec(2);
	double SST_backward, M_backward;
	M_backward = (1 / (1 - (1 / n))) * (M_forward - (1 / n) * obs_removed)
	SST_backward = SST_forward - ((obs_removed - M_backward) * (obs_removed - M_forward));
	return_vec[0] = M_backward;
	return_vec[1] = SST_backward;
	return return_vec;
}

double recursiveCovarianceForward(double cov_backward, double mean_x, double mean_y_backward, double x_additional, double y_additional) {
	double cov_forward = cov_backward + ((x_additional - mean_x) * (y_additional - mean_y_backward));
	return cov_forward;
}
*/
/*
std::array<std::vector<int>, 2> diffSet(std::vector<int> set_a, std::vector<int> set_b) {
	std::array<std::vector<int>, 2> diff;
	// first elem = additional obs in set a, second elem = removed obs in set a
	// compared to set b
	int n_a = set_a.size();
	int n_b = set_b.size();
	bool in_a_notin_b = true;
	bool in_b_notin_a = true;
	for (int i = 0; i < n_a; ++i) {
		for (int j = 0; j < n_b; j++) {
			if (set_a[i] == set_b[j]) {
				in_a_notin_b = false;
				break;
			}
		}
		if (in_a_notin_b == true) {
			diff[0].push_back(set_a[i]);
		}
	}
	for (int i = 0; i < n_b; ++i) {
		for (int j = 0; j < n_a; j++) {
			if (set_b[i] == set_a[j]) {
				in_b_notin_a = false;
				break;
			}
		}
		if (in_b_notin_a == true) {
			diff[1].push_back(set_b[i]);
		}
	}
	return diff;
} 
*/

bool checkObsSize(std::vector<std::vector<int>> split_obs, int min_node_size) {
	bool geq_min = true;
	if (split_obs.empty() == false) {
		int n_splits = split_obs.size();
		for (int i = 0; i < n_splits; ++i) {
			if (split_obs[i].empty() == false) {
				if ((int) split_obs[i].size() < min_node_size) {
					geq_min = false;
				}
			} else {
				geq_min = false;
			}
		}
	} else {
		geq_min = false;
	}
	return geq_min;
}

double cumsum(std::vector<double> vec) {
	int n = vec.size();
	double cumsum = 0;
	for (int i = 0; i < n; ++i) {
		cumsum += vec[i];
	}
	return cumsum;
}

double quantileSortedVec(double quantile, std::vector<double> vec) {
	double n = (double) vec.size();
	return vec[floor(quantile * n)];
}


long long int factorial(int n) {
	long long int fctrl = (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
	return fctrl;
}

std::vector<std::vector<int>> permuteIndices(int n, int k) {
	std::vector<std::vector<int>> split_ix;
	split_ix.reserve(factorial(n) / (factorial(n - k) * factorial(k)));

    std::string bitmask(k, 1); // k leading 1s
    bitmask.resize(n, 0); // n-k trailing 0s

    // permute bitmask
    do {
		std::vector<int> combination;
		combination.reserve(k);
        for (int i = 0; i < n; ++i) {
        // [0...n-1] integers
            if (bitmask[i]) {
				combination.push_back(i);
			}
        }
        split_ix.push_back(combination);
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));

    return split_ix;
}

std::vector<int> convertDoubleToIntVector(std::vector<double> double_vec) {
	std::vector<int> int_vec(double_vec.size());
	for (int i = 0; i < double_vec.size(); ++i) {
		int_vec[i] = (int) double_vec[i];
	}
	return int_vec;
}