setwd("xtree")
library(Rcpp)
library(devtools)
library(simstudy)

devtools::load_all()


# library(rpart)
df = read.csv("../../../data/seoulbike.csv", header = FALSE)
# df = iris
# df$iris = iris$Species
# df = mlr::getTaskData(mlr::bh.task)
tree = xtree(df,
             n_children = 2,
             objective_type = "sse",
             model_type = "mean",
             search_algo_type = "exhaustive",
             min_node_size = 20,
             max_depth = 5,
             target = 1)

tree$print()
strct = tree$getTreeStructure()


partyobj = convertToParty(tree, df)

library(ggparty)
ggparty(partyobj, add_vars = list(ID = "$node$info$ID")) +
  geom_edge() +
  geom_edge_label() +
  geom_node_label(aes(label = splitvar),
                  ids = "inner") +
  geom_node_label(aes(label = ID),
                  ids = "terminal")
  
