#include "class_data.h"
#include "helper_functions.h"



std::vector<lluint> initVectorLLUINT(lluint start, lluint end) {
	
	std::vector<lluint> vec;
	for (lluint i = start; i <= end; i++) {
		vec.push_back(i);
	}
	return vec;
}

void printVectorLLUINT(std::vector<lluint> vec) {
	
	lluint n = vec.size();
	printf("Printing vector:\n");
	for (lluint i = 0; i < n; i++) {
		printf("%lld ", vec[i]);
	}
	printf("\n");
}

void printVectorInt(std::vector<int> vec) {
	
	int n = vec.size();
	printf("Printing vector:\n");
	for (int i = 0; i < n; i++) {
		printf("%d ", vec[i]);
	}
	printf("\n");
}

double mean(std::vector<double> vec) {
	
	int n = vec.size();
	double cumsum = 0;
	for (int i = 0; i < n; i++) {
		cumsum += vec[i];
	}
	return cumsum / n;
}
