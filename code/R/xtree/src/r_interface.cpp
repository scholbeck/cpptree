
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

std::map<std::string, int> createCategEncodingMap(Rcpp::StringVector levels, Rcpp::IntegerVector mappings) {
  int n = levels.size();
  std::string l;
  int encoding;
  std::map<std::string, int> m;
  for (int i = 0; i < n; i++) {
    l = Rcpp::as<std::string>(levels[i]);
    encoding = mappings[i];
  }
  m.insert(std::pair<std::string, int> (l, encoding));
  return m;
}

std::set<std::string> detectLevelsFromColumn(std::vector<std::string> col) {
  std::set<std::string> levels;
  int n = col.size();
  for (int i = 0; i < n; i++) {
    levels.insert(col[i]);
  }
  return levels;
}

Data* convertData(Rcpp::DataFrame r_data, int target_index, Rcpp::StringVector coltypes) {
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
  Rcpp::NumericVector r_column_num;
  std::vector<std::string> column_categ;
  std::vector<double> cpp_column;

  // data columns
  for (int j = 0; j < n_cols; j++) {
    if (coltypes[j] == "num") {
      r_column_num = r_data[j];
      cpp_column = Rcpp::as<std::vector<double>>(r_column_num);
      for (int i = 0; i < n_rows; i++) {
        data->setElem(cpp_column[i], i , j + 1);
        // insert into (j + 1)th column to account for ID column
      }
    } else if (coltypes[j] == "categ") {
      column_categ = convertRcppStringVector(r_data[j]);
      std::set<std::string> levels = detectLevelsFromColumn(column_categ);
      std::map<std::string, int> m;
      int l = 0;
      for (auto it = levels.begin(); it != levels.end(); ++it) {
        m.insert(std::pair<std::string, int> (*it, l));
        l++;
      }
      // create mapping for each categ feature j and for all its feature levels to integer 0, 1, 2, etc.
      data->addCategEncoding(j + 1, m);
      int elem;
      for (int i = 0; i < n_rows; i++) {
        elem = m.at(column_categ[i]);
        data->setElem(elem, i , j + 1);
        // insert into (j + 1)th column to account for ID column
      }
      
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

RInterface::RInterface(Rcpp::DataFrame r_data, Rcpp::StringVector coltypes,
                       Rcpp::List params) {
  
  Data* data = convertData(r_data, params["target"], coltypes);
  Arguments args;
  args.setTargetIndex(params["target"]);
  args.setMinNodeSize(params["min_node_size"]);
  args.setAlgorithm(params["search_algo_type"]);
  args.setMaxChildren(params["n_children"]);
  args.setMaxDepth(params["max_depth"]);
  args.setModel(params["model_type"]);
  args.setObjective(params["objective_type"]);
  
  this->tree = new Tree(data, args);
  this->tree->grow();
  this->depth = this->tree->depth;
  this->node_cnt = this->tree->node_cnt;
  this->leafnode_cnt = this->tree->leafnode_cnt;
}

// 
// void RInterface::grow() {
//   this->tree->grow();
//   this->depth = this->tree->depth;
//   this->node_cnt = this->tree->node_cnt;
//   this->leafnode_cnt = this->tree->leafnode_cnt;
// }

void RInterface::print() {
  printTreeStructureToR(this->tree);
}

Rcpp::DataFrame RInterface::getTreeStructure() {
  Rcpp::StringVector id_vec, parent_split_vec, split_value_vec, split_type_vec, level_partitioning_vec;
  Rcpp::IntegerVector is_leaf_vec, split_feature_vec;
  Split* split;
  for (int i = 0; i < this->tree->node_cnt; i++) {
    id_vec.push_back(this->tree->nodes[i]->getId());
    parent_split_vec.push_back(this->tree->nodes[i]->getDecisionRule());
    is_leaf_vec.push_back(this->tree->nodes[i]->isLeaf());
    split = this->tree->nodes[i]->getSplitData();
    
    if (split == nullptr) {
      split_feature_vec.push_back(-1);
      split_type_vec.push_back(std::string("NA"));
      split_value_vec.push_back(std::string("NA"));
      level_partitioning_vec.push_back(std::string("NA"));
      continue;
    }
    
    // Rcpp::Rcout << "feature" << split->getSplitFeatureIndex();
    split_feature_vec.push_back(split->getSplitFeatureIndex());
    std::string split_type = split->getSplitType();
    split_type_vec.push_back(split_type);
    
    std::vector<double> split_values;
    std::vector<std::vector<int>> level_partitionings;
    
    if (split_type == "num") {
      level_partitioning_vec.push_back(std::string("NA"));
      // push back placeholder vector for equal size
      
      split_values = split->getSplitValues();
      int n_splits = split_values.size();
      std::string s = std::to_string(split_values[0]);
      for (int i = 1; i < n_splits; i++) {
        s += "," + std::to_string(split_values[i]);
      }
      
      split_value_vec.push_back(s);
    } else if (split_type == "categ") {
      split_value_vec.push_back(std::string("NA"));
      // push back placeholder value for equal size
      level_partitionings = split->getLevelPartitionings();
      int n_children = level_partitionings.size();
      std::string s;
      for (int i = 0; i < (n_children - 1); i++) {
        s += "{" + std::to_string(level_partitionings[i][0]);
        int n_levels_per_subset = level_partitionings[i].size();
        for (int l = 1; l < n_levels_per_subset; l++) {
          s += "," + std::to_string(level_partitionings[i][l]);
        }
        s += "}|";
      }
      s += "{" + std::to_string(level_partitionings[n_children - 1][0]);
      int n_levels_per_subset = level_partitionings[n_children - 1].size();
      for (int l = 1; l < n_levels_per_subset; l++) {
        s += "," + std::to_string(level_partitionings[n_children - 1][l]);
      }
      s += "}";
      level_partitioning_vec.push_back(s);
    }
  }
  Rcpp::DataFrame tree_structure = Rcpp::DataFrame::create(
    Rcpp::Named("ID") = id_vec,
    Rcpp::Named("parent_split") = parent_split_vec,
    Rcpp::Named("is_leaf") = is_leaf_vec,
    Rcpp::Named("feature") = split_feature_vec,
    Rcpp::Named("values") = split_value_vec,
    Rcpp::Named("type") = split_type_vec,
    Rcpp::Named("levels") = level_partitioning_vec);
  
  return tree_structure;
}


RCPP_MODULE(xtree) {
  
  Rcpp::class_<RInterface>("ExtensibleTree")
  
  .constructor<Rcpp::DataFrame, Rcpp::StringVector, Rcpp::List>("Construct a tree object.")
  .field("depth", &RInterface::depth)
  .field("node_cnt", &RInterface::node_cnt)
  .field("leafnode_cnt", &RInterface::leafnode_cnt)
  .method("print", &RInterface::print)
  .method("getTreeStructure", &RInterface::getTreeStructure)
  ;
}

RCPP_EXPOSED_CLASS(ExtensibleTree)