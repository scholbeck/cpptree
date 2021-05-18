#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "helper_functions.h"
#include "class_data.h"
#include "class_reader.h"
#include <cctype>
#include <map>
#include <set>

Reader::Reader() {
	
}

std::set<std::string> detectLevels(std::vector<std::vector<std::string>> vec, int col_ix) {
	std::set<std::string> levels;
	int n_rows = vec.size();
	for (int i = 0; i < n_rows; i++) {
		levels.insert(vec[i][col_ix]);
	}
	
	for (auto it = levels.begin(); it != levels.end(); it++)
    {
        std::cout << *it << std::endl;
    }
	
    return levels;
}


std::vector<std::string> Reader::detectColTypes(std::vector<std::string> vec) {
	std::vector<std::string> types;
	char value;
	int n = vec.size();
	for (int i = 0; i < n; i++) {
		value = vec[i][0];
		if (isdigit(value)) {
			types.push_back("num");
		} else {
			types.push_back("categ");
		}
	}
	return types;
}

std::vector<std::string> parseLine(std::string line, char sep) {
	std::vector<std::string> word_vec;
	std::string word;
	int length = line.size();
	for (int i = 0; i < length; i++) {
		if (!isspace(line[i]) && (line[i] != sep)) {
			word.push_back(line[i]);
		} else {
			word_vec.push_back(word);
			word.clear();
		}
	}
	word_vec.push_back(word);
	return word_vec;
}

Data Reader::read(std::string filename, char sep) {
	
	std::vector<std::vector<std::string>> rows_strings;
	std::string line;
	std::ifstream file;
	
	file.open(filename);
	while (std::getline(file, line)) {
        rows_strings.push_back(parseLine(line, sep)); // read and parse remaining lines
    }
    
    int n_rows = rows_strings.size();
    int n_cols = rows_strings[0].size();
    std::vector<std::string> types = detectColTypes(rows_strings[0]);
	Data data;
    std::vector<double> new_row;
    
    for (int i = 0; i < n_rows; i++) {
		for (int j = 0; j < n_cols; j++) {	
			if (types[j] == "num") {
				new_row.push_back(std::stod(rows_strings[i][j]));
			} else {
				// types[j] = "categ"
				std::set<std::string> levels = detectLevels(rows_strings, j);
				std::map<std::string, int> m;
				int l = 0;
				for (auto it = levels.begin(); it != levels.end(); it++) {
					std::cout << *it;
					//m.insert({*it, l});	
					l++;
				}
				// create mapping for all feature levels to integer 0, 1, 2, etc.
				data.addCategEncoding(m);
				// add mapping to data object
				new_row.push_back(m.at(rows_strings[i][j]));
				// add mapped integer to data
			}
		}
		printVectorDouble(new_row);
		//data.addRow(new_row);
		new_row.clear();
		
	}
	
	return data;
}

