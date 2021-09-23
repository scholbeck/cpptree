setwd("xtree")
library(Rcpp)
library(devtools)

load_all()
df = read.csv("../../../data/bh.data", header = FALSE)
df$V4 = as.factor(df$V4)
df$V9 = as.factor(df$V9)
# df = iris
# df$iris = iris$Species

tree = xtree(df,
             n_children = 3,
             objective_type = "sse",
             model_type = "mean",
             search_algo_type = "random",
             min_node_size = 10,
             max_depth = 3,
             target = 14)

tree$grow()
tree$print()
tree$getTreeStructure()
partyobj = convertToParty(tree, df)

library(ggparty)
ggparty(partyobj, add_vars = list(ID = "$node$info$ID")) +
  geom_edge() +
  geom_edge_label() +
  geom_node_label(aes(label = splitvar),
                  ids = "inner") +
  geom_node_label(aes(label = ID),
                  ids = "terminal")
  
  