#ifndef DATA_H
#define DATA_H

#include <string>
#include <stdlib.h>
#include <deque>
#include <vector>
#include "class_split.h"

using lluint = long long unsigned int;
class Split;

class Data {
  
	public:
		Data();
    
		std::deque<std::deque<double>> rows; // each row is a vector containg all col values
		lluint target_index;
		
		void load(std::string filename); // read data from disc
		
		void setTargetIndex(lluint target);
		void addRow(std::deque<double> row);
		void addCol(std::deque<double> col);
		void print();
		lluint getTargetIndex();
		double elem(lluint row_index, lluint col_index);
		lluint nrows();
		lluint ncols();
		std::deque<double> row(lluint i);
		std::deque<double> col(lluint j);
		void init(lluint n_rows, lluint n_cols);
		void initRandom(lluint n_rows, lluint n_cols);
		void summary();		
		Data subset(std::deque<lluint> rows, std::deque<lluint> cols);
		std::vector<Data> splitBinary(double split_value, lluint col_index);
		std::vector<Data> split(Split split);

		/*
		std::deque<Data> splitBinary(double split_value, lluint col_index);
		std::deque<Data> split(Split split);
		
		Data subset(std::deque<lluint> row_id_vec, std::deque<lluint> col_id_vec);
		*/
};


#endif 
