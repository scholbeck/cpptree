#ifndef DATA_H
#define DATA_H

#include <string>
#include <stdlib.h>
#include <vector>
#include <vector>
#include "class_split.h"

using lluint = long long unsigned int;
class Split;

class Data {
  
	public:
		Data();
    
		std::vector<std::vector<double>> rows; // each row is a vector containg all col values
		lluint target_index;
		
		void load(std::string filename); // read data from disc
		
		void setTargetIndex(lluint target);
		void addRow(std::vector<double> row);
		void addRows(std::vector<std::vector<double>> row_vec);
		void addCol(std::vector<double> col);
		void addCols(std::vector<std::vector<double>> col_vec);
		
		void print();
		lluint getTargetIndex();
		double elem(lluint row_index, lluint col_index);
		lluint nrows();
		lluint ncols();
		std::vector<double> row(lluint i);
		std::vector<double> col(lluint j);
		void init(lluint n_rows, lluint n_cols);
		void initRandom(lluint n_rows, lluint n_cols);
		void summary();		
		Data subset(std::vector<lluint> rows, std::vector<lluint> cols);
		std::vector<Data> splitBinary(double split_value, lluint col_index);
		std::vector<Data> split(Split split);
};


#endif 
