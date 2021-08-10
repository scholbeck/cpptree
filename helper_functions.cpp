

#include <iostream>
#include "helper_functions.h"



std::vector<int> initVectorLLUInt(int start, int end) {
	
	std::vector<int> vec;
	for (int i = start; i <= end; i++) {
		vec.push_back(i);
	}
	return vec;
}

void printVectorLLUInt(std::vector<int> vec) {
	
	int n = vec.size();
	printf("Printing vector:\n");
	for (int i = 0; i < n; i++) {
		std::cout << vec[i] << "\n";
	}
	printf("\n");
}

void printVectorInt(std::vector<int> vec) {
	
	int n = vec.size();
	printf("Printing vector:\n");
	for (int i = 0; i < n; i++) {
		std::cout << vec[i] << "\n";
	}
	printf("\n");
}

void printVectorString(std::vector<std::string> vec) {
	
	int n = vec.size();
	printf("Printing vector:\n");
	for (int i = 0; i < n; i++) {
		std::cout << vec[i] << "\n";
	}
	printf("\n");
}

void printVectorDouble(std::vector<double> vec) {
	
	int n = vec.size();
	printf("Printing vector:\n");
	for (int i = 0; i < n; i++) {
		std::cout << vec[i] << "\n";
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


std::vector<int> initVectorSeq(int from, int to) {
	std::vector<int> seq;
	for (int i = from; i <= to; i++) {
		seq.push_back(i);
	}
	return seq;

}

std::vector<double> initVectorSeqDouble(int from, int to) {
	std::vector<double> seq;
	for (int i = from; i <= to; i++) {
		seq.push_back((double) i);
	}
	return seq;

}

std::vector<double> initVectorDoubleValue(double value, int size) {
	std::vector<double> vec;
	for (int i = 0; i < size; i++) {
		vec.push_back(value);
	}
	return vec;
}
