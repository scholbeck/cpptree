setwd("xtree")
library(Rcpp)
library(devtools)
library(xtree)

load_all()
df = read.csv("../../../data/bh.data", header = FALSE)
params = c("lol")

loadModule("xtree", TRUE)
tree = xtree(df, 0, params)
tree$grow()
tree$print()
tree$getTreeStructure()
partyobj = xtree::as_party(tree, df)

library(ggparty)
ggparty(partyobj, add_vars = list(ID = "$node$info$ID")) +
  geom_edge() +
  geom_edge_label() +
  geom_node_label(aes(label = splitvar),
                  ids = "inner") +
  geom_node_label(aes(label = ID),
                  ids = "terminal")
  
  