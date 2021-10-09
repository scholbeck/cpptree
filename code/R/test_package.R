setwd("xtree")
library(Rcpp)
library(devtools)

devtools::load_all()


# library(rpart)
df = read.csv("../../../data/seoulbike.csv", header = FALSE)
# df = iris
# df$iris = iris$Species
# df = mlr::getTaskData(mlr::bh.task)
tree = xtree(df,
             n_children = 2,
             objective_type = "sse",
             model_type = "linear",
             formula = "x6",
             search_algo_type = "exhaustive",
             min_node_size = 20,
             max_depth = 3,
             target = 1)

tree$print()
strct = tree$getTreeStructure()


partyobj = convertToParty(tree, df)

library(ggparty)
ggparty(partyobj, add_vars = list(ID = "$node$info$model")) +
  geom_edge() +
  geom_edge_label() +
  geom_node_label(aes(label = splitvar),
                  ids = "inner") +
  geom_node_label(aes(label = ID),
                  ids = "terminal")
  
