#include "class_data.h"
#include "helper_functions.h"



std::vector<lluint> initVectorLLUInt(lluint start, lluint end) {
	
	std::vector<lluint> vec;
	for (lluint i = start; i <= end; i++) {
		vec.push_back(i);
	}
	return vec;
}

void printvectorLLUInt(std::vector<lluint> vec) {
	
	lluint n = vec.size();
	printf("Printing vector:\n");
	for (lluint i = 0; i < n; i++) {
		printf("%lld\n ", vec[i]);
	}
	printf("\n");
}

void printvectorInt(std::vector<int> vec) {
	
	int n = vec.size();
	printf("Printing vector:\n");
	for (int i = 0; i < n; i++) {
		printf("%d\n", vec[i]);
	}
	printf("\n");
}

void printVectorDouble(std::vector<double> vec) {
	
	int n = vec.size();
	printf("Printing vector:\n");
	for (int i = 0; i < n; i++) {
		printf("%f\n", vec[i]);
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



std::vector<lluint> initVectorSeq(lluint from, lluint to) {
	std::vector<lluint> seq;
	for (lluint i = from; i < to; i++) {
		seq.push_back(i);
	}
	return seq;
}
