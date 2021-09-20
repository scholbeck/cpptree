
#include <Rcpp.h>
#include <string>
#include "r_interface.h"
#include "class_data.h"
#include "class_tree.h"
#include "class_arguments.h"
#include "helper_functions.h"


void printDataToR(Data* data) {
  int n_rows = data->nrows();
  int n_cols = data->ncols();
  Rcpp::Rcout << "\n";
  for (int i = 0; i < n_rows; ++i) {
    for (int j = 0; j < n_cols; j++) {
      Rcpp::Rcout << data->rows[i][j] << " ";
    }
    Rcpp::Rcout << "\n";
  }
}

void printDataSummaryToR(Data* data) {
  Rcpp::Rcout << "DATA SUMMARY\n";
  
  Rcpp::Rcout << "\tmatrix of dimension : " << data->nrows() << " x " << data->ncols() << "\n";
  Rcpp::Rcout << "\ttarget : column " << data->getTargetIndex() << " \n";
  Rcpp::Rcout << "\tcolumn types : ";
  std::vector<std::string> types = data->getColTypes();
  
  
  std::map<std::string, int> levels;
  Rcpp::Rcout << "| (0 : ID) | ";
  for (int j = 1; j < data->ncols(); j++) {
    Rcpp::Rcout << "(" << j << " : "<< types[j] << ") | ";
  }
  
  Rcpp::Rcout << "\n";
  Rcpp::Rcout << "\tcategorical encodings :\n";
  
  for (int j = 1; j < data->ncols(); j++) {
    if (types[j] == "categ") {
      Rcpp::Rcout << "\t\tcolumn " << j << " : \n";
      levels = data->categ_encodings.at(j);
      for (auto it = levels.begin(); it != levels.end(); ++it) {
        Rcpp::Rcout << "\t\t\t" << it->first << " = " << it->second << "\n";
      }
    }
  }
}

std::vector<std::string> convertRcppStringVector(Rcpp::StringVector vec) {
  int n = vec.size();
  std::vector<std::string> conv_vec(n);
  for (int i = 0; i < n; i++) {
    conv_vec[i] = Rcpp::as<std::string>(vec[i]);
  }
  return conv_vec;
}


Data* convertData(Rcpp::DataFrame r_data, int target_index, Rcpp::StringVector coltypes, Rcpp::List categ_encodings) {
  int n_rows = r_data.nrows();
  int n_cols = r_data.size();
  Data* data = new Data();
  data->init(n_rows, n_cols + 1);
  // + 1 for ID column
  data->replaceCol(0, initVectorSeqDouble(0, n_rows - 1));
  data->setTargetIndex(target_index);
  std::vector<std::string> types, r_types_conv;
  r_types_conv = convertRcppStringVector(coltypes);
  types.reserve(n_cols + 1);
  types.push_back(std::string("ID"));
  types.insert(
    types.end(),
    std::make_move_iterator(r_types_conv.begin()),
    std::make_move_iterator(r_types_conv.end())
  );
  data->setColTypes(types);
  Rcpp::NumericVector r_column;
  std::vector<double> cpp_column;
  
  // data columns
  for (int j = 0; j < n_cols; j++) {
    r_column = r_data[j];
    cpp_column = Rcpp::as<std::vector<double>>(r_column);
    for (int i = 0; i < n_rows; i++) {
      data->setElem(cpp_column[i], i , j + 1);
      // j + 1 to account for ID column
    }
  }
  return data;
}

void printTreeSummaryToR(Tree* tree) {
  Rcpp::Rcout << "------------------------------------------------------\n";
  Rcpp::Rcout << "TREE SUMMARY\n";
  Rcpp::Rcout << "\tnodes : " << tree->node_cnt << "\n";
  Rcpp::Rcout << "\tleaf nodes : " << tree->leafnode_cnt << "\n";
  Rcpp::Rcout << "\tdepth : " << tree->depth << "\n\n";
  tree->print();
  /*
   for (int i = 0; i < tree->node_cnt; ++i) {
   tree->nodes[i]->summary();
   }
   */
  Rcpp::Rcout << "------------------------------------------------------\n";
}

void printSubTreeToR(Node* node) {
  int level = node->getId().length() - 1;
  std::ostringstream sstream;
  sstream << std::setprecision(2) << std::fixed ; //<< node->getObjValue(); // obj printout with 2 decimal places
  if (level == 0) {
    Rcpp::Rcout << "└──[" << node->getId() << "]\n"; // << "] (" << node->getModel()->getShortSummary() << " | obj = " << sstream.str() << ")\n";
  } else {
    if (node->isLeaf()) {
      // Rcpp::Rcout << std::string((level * 4) , ' ') << "├──" << repeatString((((this->depth) - level) * 4) + depth * 2, "─") << "<" << node->getDecisionRule() << ">──[*" << node->getId() << "]\n" ;//<< "] (" << node->getModel()->getShortSummary() << " | obj = " << sstream.str() << ")\n";
      Rcpp::Rcout << std::string((level * 4) , ' ') << "├──<" << node->getDecisionRule() << ">──[*" << node->getId() << "]\n" ;//<< "] (" << node->getModel()->getShortSummary() << " | obj = " << sstream.str() << ")\n";
    } else {
      Rcpp::Rcout << std::string((level * 4) , ' ') << "├──<" << node->getDecisionRule() << ">──[" << node->getId() << "]\n" ;//<< "] (" << node->getModel()->getShortSummary() << " | obj = " << sstream.str() << ")\n";
    }
  }
  std::vector<Node*> child_nodes = node->getChildNodes();
  int n_children = child_nodes.size();
  for (int i = 0; i < n_children; ++i) {
    printSubTreeToR(child_nodes[i]);
  }
}

void printTreeStructureToR(Tree* tree) {
  printSubTreeToR(tree->nodes[0]);
}

XTree::XTree(Rcpp::DataFrame r_data, int target_index,
           Rcpp::StringVector coltypes, Rcpp::List categ_encodings,
           Rcpp::StringVector params) {
  
  Data* data = convertData(r_data, target_index, coltypes, categ_encodings);
  Arguments args;
  args.setMinNodeSize(20);
  args.setAlgorithm("exhaustive");
  args.setMaxChildren(2);
  args.setMaxDepth(30);
  args.setModel("mean");
  args.setObjective("sse");
  this->tree = new Tree(data, args);
}

void XTree::grow() {
  this->tree->grow();
}

void XTree::print() {
  printTreeStructureToR(this->tree);
  
}


