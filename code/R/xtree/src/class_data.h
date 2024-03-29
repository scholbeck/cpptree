#ifndef DATA_H
#define DATA_H

#include <string>
#include <stdlib.h>
#include <vector>
#include <map>
#include <array>
#include <memory>
#include "class_split.h"
#include "class_sorteddata.h"

class SortedData;

class Data {
  
	public:
		Data();
		~Data() {}
    
		std::vector<std::vector<double>> rows; // each row is a vector containg all col values
		std::vector<int> features;
		std::vector<int> observations;
		int target_index;
		std::vector<std::string> coltypes;
		std::map<int, std::map<std::string, int>> categ_encodings;
		std::unique_ptr<SortedData> sorted_data; // Data object is responsible for life time of SortedData object

		void load(std::string filename); // read data from disc
		void setTargetIndex(int target);
		void addCategEncoding(int col, std::map<std::string, int>);
		std::map<int, std::map<std::string, int>> getCategEncodings();
		void setCategEncodings(std::map<int, std::map<std::string, int>> categ_encodings);
		void setColTypes(std::vector<std::string> types);
		std::vector<std::string> getColTypes();
		std::array<std::vector<int>, 2> getColTypesNumCateg();
		void addRow(std::vector<double> row);
		void addRows(std::vector<std::vector<double>> row_vec);
		void replaceRow(int row_ix, std::vector<double> values);
		void replaceCol(int col_ix, std::vector<double> values);	
		void addCol(std::vector<double> col);
		void addCols(std::vector<std::vector<double>> col_vec);
		
		void print();
		bool selfCheck();
		int getTargetIndex();
		double elem(int row_index, int col_index);
		void setElem(double value, int row_index, int col_index);
		int nrows();
		int ncols();
		std::vector<double> row(int i);
		std::vector<double> col(int j);
		std::vector<double> selectObs(int id);
		std::vector<double> columnSubset(std::vector<int> rows, int col);
		void init(int n_rows, int n_cols);
		void initRandom(int n_rows, int n_cols);
		void sizeSummary();
		void summary();
		Data* subset(std::vector<int> rows, std::vector<int> cols);
		Data* subsetRows(std::vector<int> rows);
		std::vector<std::vector<std::vector<int>>> computeCategPermuts(int col_index, int n_subsets);
		std::vector<std::vector<int>> splitBinaryObs(double split_value, int col_index);
		int getNLevels(int col);
		void sortFeatures();
		void createSortedData();
		void createObservationIDs();
		std::vector<int> getObservationIDs();
};


#endif 
