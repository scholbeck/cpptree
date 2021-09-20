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

p = as_party(tree, df)
plot(p)
library(ggparty)
ggparty(p)
