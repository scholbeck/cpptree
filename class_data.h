#ifndef DATA_H
#define DATA_H

#include <armadillo>
#include <vector>
#include "class_split.h"

class Split; // forward declaration due to circular dependency

using lluint = long long unsigned int;

class Data {
  
	public:
		Data();
    
		arma::mat data;
		lluint target_index;
		lluint n_rows;
		lluint n_cols;
		
		void load(std::string filename); // read data from disc
		arma::mat getData();
		void setData(arma::mat mat);
		void setTargetIndex(lluint target);
		void print();
		lluint getTargetIndex();
		double elem(lluint row_index, lluint col_index);
		lluint nrows();
		lluint ncols();
		std::vector<double> row(lluint i);
		std::vector<double> col(lluint j);
		double rowMean(lluint row_index);
		double colMean(lluint col_index);

		std::vector<Data> splitBinary(double split_value, lluint col_index);
		std::vector<Data> split(Split split);
		
		Data subset(std::vector<lluint> row_id_vec, std::vector<lluint> col_id_vec);
		
};


#endif 
