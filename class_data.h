#ifndef DATA_H
#define DATA_H

#include <string>
#include <stdlib.h>
#include <vector>
#include <map>
#include "class_split.h"


class Data {
  
	public:
		Data();
    
		std::vector<std::vector<double>> rows; // each row is a vector containg all col values
		int target_index;
		std::vector<std::map<std::string, int>> categ_encodings;
		
		void load(std::string filename); // read data from disc
		
		void setTargetIndex(int target);
		void addCategEncoding(std::map<std::string, int>);
		void addRow(std::vector<double> row);
		void addRows(std::vector<std::vector<double>> row_vec);
		void replaceRow(int row_ix, std::vector<double> values);	
		void addCol(std::vector<double> col);
		void addCols(std::vector<std::vector<double>> col_vec);
		
		void print();
		int getTargetIndex();
		double elem(int row_index, int col_index);
		int nrows();
		int ncols();
		std::vector<double> row(int i);
		std::vector<double> col(int j);
		void init(int n_rows, int n_cols);
		void initRandom(int n_rows, int n_cols);
		void summary();		
		Data subset(std::vector<int> rows, std::vector<int> cols);
		std::vector<Data> splitBinary(double split_value, int col_index);
		std::vector<Data> split(Split split);
};


#endif 
