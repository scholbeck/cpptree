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
    return levels;
}


std::vector<std::string> Reader::detectColTypes(std::vector<std::string> vec) {
	std::vector<std::string> types;
	char value;
	int n = vec.size();
	types.push_back(std::string("ID"));
	for (int i = 0; i < n; i++) {
		value = vec[i][0];
		if (isdigit(value)) {
			types.push_back(std::string("num"));
		} else {
			types.push_back(std::string("categ"));
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

Data* Reader::read(std::string filename, char sep) {
	
	std::vector<std::vector<std::string>> rows_strings;
	std::string line;
	std::ifstream file;
	
	file.open(filename);
	while (std::getline(file, line)) {
        rows_strings.push_back(parseLine(line, sep)); // read and parse remaining lines
    }
    std::vector<std::string> types = detectColTypes(rows_strings[0]);
    int n_rows = rows_strings.size();
    int n_cols = rows_strings[0].size();
	Data* data = new Data();
	data->addCol(initVectorSeqDouble(0, n_rows));
	data->setColTypes(types);
    std::vector<double> new_row;
    
    for (int i = 0; i < n_rows; i++) {
		new_row.push_back(i);
		for (int j = 0; j < n_cols; j++) {
			if (types[j + 1] == "num") {
				new_row.push_back(std::stod(rows_strings[i][j]));
			} else if (types[j + 1] == "categ") {
				// types[j] = "categ"
				std::set<std::string> levels = detectLevels(rows_strings, j);
				std::map<std::string, int> m;
				int l = 0;
				for (auto it = levels.begin(); it != levels.end(); ++it) {
					m.insert(std::pair<std::string, int> (*it, l));
					l++;
				}
				// create mapping for each categ feature j and for all its feature levels to integer 0, 1, 2, etc.
				data->addCategEncoding(j + 1, m);
				// add mapping to data object
				new_row.push_back(m.at(rows_strings[i][j]));
				// add mapped integer to data
			}
		}
		data->addRow(new_row);
		new_row.clear();
	}
	
	return data;
}

