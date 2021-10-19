setwd("xtree")
library(Rcpp)
library(devtools)
library(partykit)
library(microbenchmark)
devtools::load_all()

data("BostonHousing", package = "mlbench")
df = BostonHousing
# library(rpart)
# df = read.csv("../../../data/seoulbike.csv", header = FALSE)
# df = iris
# df$iris = iris$Species
tree = cpptree(
  df,
  n_children = 2,
  objective_type = "sse",
  model_type = "linear",
  model_formula = "x2",
  search_algo_type = "exhaustive",
  min_node_size = 1,
  max_depth = 100,
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
  




# 
# 
# library(ranger)
# library(partykit)
# library(mgcv)
# mod = ranger(medv ~ ., data = BostonHousing, importance = "permutation")
# sort(mod$variable.importance, decreasing = TRUE)
# 
# BostonHousing$prediction = predict(mod, data = BostonHousing)$predictions
# 
# model_node = function(y, x, start = NULL, weights = NULL, offset = NULL, ...) {
#   # gam(y ~ 0 + s(x), start = start, ...)
#   lm(y ~ x)
# }
# 
# library(microbenchmark)
# pid_tree = microbenchmark(mob(prediction ~ . | crim, data = BostonHousing, fit = model_node))
