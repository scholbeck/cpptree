getwd()
setwd("code/R/cpptree")
library(Rcpp)
library(devtools)
library(partykit)
library(ggparty)
load_all()
data("BostonHousing", package = "mlbench")
df = BostonHousing

str(df)
tree = cpptree(
  data = df,
  n_children = 2,
  objective_type = "sse",
  model_type = "mean",
  search_algo_type = "exhaustive",
  min_node_size = 30,
  max_depth = 3,
  target = 4)

tree

tree$print()
tree$getTreeStructure()

partyobj = convertToParty(tree, df)
plot(partyobj)

p = ggparty(partyobj,
            add_vars = list(ID = "$node$info$model")) +
  geom_edge() +
  geom_edge_label(size = 10) +
  geom_node_label(aes(label = splitvar),
                  size = 10,
                  ids = "inner") +
  geom_node_plot(gglist = list(geom_point(aes(x = rm,
                                              y = medv),
                                          size = 3),
                               theme_bw(base_size = 25)),
                 ids = "terminal",
                 shared_axis_labels = TRUE) +
  geom_node_label(aes(label = ID),
                  size = 7,
                  ids = "terminal")

p 
ggsave(p, filename = "ggparty_linear.png", width = 20, height = 10)


tree = cpptree(
  data = df,
  n_children = 2,
  objective_type = "sse",
  model_type = "mean",
  search_algo_type = "exhaustive",
  min_node_size = 30,
  max_depth = 3,
  target = 14)

tree

tree$print()
tree$getTreeStructure()

partyobj = convertToParty(tree, BostonHousing)
plot(partyobj)

p = ggparty(partyobj,
            add_vars = list(ID = "$node$info$model")) +
  geom_edge() +
  geom_edge_label(size = 10) +
  geom_node_label(aes(label = splitvar),
                  size = 10,
                  ids = "inner") +
  geom_node_plot(gglist = list(geom_point(aes(x = rm,
                                              y = medv),
                                          size = 3),
                               theme_bw(base_size = 25)),
                 ids = "terminal",
                 shared_axis_labels = TRUE) +
  geom_node_label(aes(label = ID),
                  size = 7,
                  ids = "terminal")

p


ggsave(p, filename = "ggparty_mean.png", width = 20, height = 10)
library(rpart)
library(rpart.plot)
rpartmod = rpart(medv ~ ., df, control = rpart.control(maxdepth = 3, cp = 0, minsplit = 30, minbucket = 30))
png("sanity_check_rpart.png", width = 100, height = 100, units = 'mm', res = 500)

rpart.plot(rpartmod)
dev.off()
# 

tree = cpptree(
  data = df,
  n_children = 3,
  objective_type = "sse",
  model_type = "linear",
  model_formula = "x5",
  search_algo_type = "random",
  min_node_size = 10,
  max_depth = 2,
  target = 14)

tree$print()
tree$getTreeStructure()

partyobj = convertToParty(tree, BostonHousing)
plot(partyobj)

p = ggparty(partyobj,
            add_vars = list(ID = "$node$info$model")) +
  geom_edge() +
  geom_edge_label(size = 10) +
  geom_node_label(aes(label = splitvar),
                  size = 10,
                  ids = "inner") +
  geom_node_plot(gglist = list(geom_point(aes(x = rm,
                                              y = medv),
                                          size = 3),
                               theme_bw(base_size = 25)),
                               ids = "terminal",
                               shared_axis_labels = TRUE) +
  geom_node_label(aes(label = ID),
                  size = 5,
                  ids = "terminal")

p 
ggsave(p, filename = "ggpartymult.png", width = 20, height = 10)

