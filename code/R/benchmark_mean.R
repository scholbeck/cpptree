setwd("xtree")
library(Rcpp)
library(devtools)
library(partykit)
library(rpart)
library(microbenchmark)
library(ranger)
devtools::load_all()

######### 
# fixed n
######### 


unit = "ms"
minnodesize = 30
maxdepth = 1
n_eval = 10
bench_cpptree = list()
bench_rpart = list()
bench_ranger = list()
bench_mob = list()

model_node = function(y, x, start = NULL, weights = NULL, offset = NULL, ...) {
  lm(y ~ 1)
}

nvarvec = c(1, 10, 100, 1000)
set.seed(123)
n_obs = 100

for (n_vars in nvarvec) {
  print(n_vars)
  df = data.frame("V1" = runif(n_obs, -10, 10))
  for (i in 1:n_vars) {
    df[[i]] = runif(n_obs, -10, 10)
  }
  df = data.frame("y" = as.vector(ifelse(df[, 1] < 0, -5, 5)), df)
  singlebench_cpp = microbenchmark(
    unit = unit,
    times = n_eval,
    cpptree(
      data = df,
      n_children = 2,
      objective_type = "sse",
      model_type = "mean",
      search_algo_type = "exhaustive",
      min_node_size = minnodesize,
      max_depth = maxdepth,
      target = 1)
  )
  singlebench_rpart = microbenchmark(
    unit = unit,
    times = n_eval,
    rpart(
      y ~ ., df, control = rpart.control(maxdepth = maxdepth, cp = 0, minsplit = minnodesize, minbucket = minnodesize))
  )
  singlebench_ranger = microbenchmark(
    unit = unit,
    times = n_eval,
    ranger(
      formula = y ~ .,
      data = df,
      num.trees = 1,
      num.threads = 1,
      min.node.size = minnodesize,
      max.depth = maxdepth,
      mtry = ncol(df) - 1,
      replace = FALSE,
      sample.fraction = 1)
  )
  singlebench_mob = microbenchmark(
    unit = unit,
    times = n_eval,
    mob(y ~ .,
        data = df,
        fit = model_node,
        control = mob_control(minbucket = minnodesize, minsplit = minnodesize, maxdepth = maxdepth + 1))
  )
  bench_cpptree = append(bench_cpptree, list(singlebench_cpp))
  bench_rpart = append(bench_rpart, list(singlebench_rpart))
  bench_ranger = append(bench_ranger, list(singlebench_ranger))
  bench_mob = append(bench_mob, list(singlebench_mob))
}

bench_cpptree
bench_rpart
bench_ranger
bench_mob
save(bench_cpptree, file = "bench_cpptree_fixedn_singlesplit_mean.rdata")
save(bench_rpart, file = "bench_rpart_fixedn_singlesplit_mean.rdata")
save(bench_ranger, file = "bench_ranger_fixedn_singlesplit_mean.rdata")
save(bench_mob, file = "bench_mob_fixedn_singlesplit_mean.rdata")

benchdf = data.frame("n_features" = nvarvec, "n_obs" = n_obs)

for (i in 1:length(nvarvec)) {
  benchdf[["cpptree"]][i] = summary(bench_cpptree[[i]])$mean
  benchdf[["rpart"]][i] = summary(bench_rpart[[i]])$mean
  benchdf[["ranger"]][i] = summary(bench_ranger[[i]])$mean
  benchdf[["mob"]][i] = summary(bench_mob[[i]])$mean
}
benchdf

library(reshape2)
library(ggplot2)
benchdf = melt(benchdf, id.vars = c("n_features", "n_obs"), variable.name = "software", value.name = "milliseconds")

p = ggplot(benchdf, aes(x = n_features, y = milliseconds, group = software, color = software))  +
  geom_point() +
  geom_line() + 
  theme_bw()
p
ggsave(p, file = "bench_fixedn_mean.png")

####
# entire tree
####

unit = "ms"
minnodesize = 1
maxdepth = 30
n_eval = 10
bench_cpptree = list()
bench_rpart = list()
bench_ranger = list()

for (n_vars in nvarvec) {
  print(n_vars)
  df = data.frame("V1" = runif(n_obs, -10, 10))
  for (i in 1:n_vars) {
    df[[i]] = runif(n_obs, -10, 10)
  }
  df = data.frame("y" = apply(df, MARGIN = 1, sum), df)
  singlebench_cpp = microbenchmark(
    unit = unit,
    times = n_eval,
    cpptree(
      data = df,
      n_children = 2,
      objective_type = "sse",
      model_type = "mean",
      search_algo_type = "exhaustive",
      min_node_size = minnodesize,
      max_depth = maxdepth,
      target = 1)
  )
  singlebench_rpart = microbenchmark(
    unit = unit,
    times = n_eval,
    rpart(
      y ~ ., df, control = rpart.control(maxdepth = maxdepth, cp = 0, minsplit = minnodesize, minbucket = minnodesize))
  )
  singlebench_ranger = microbenchmark(
    unit = unit,
    times = n_eval,
    ranger(
      formula = y ~ .,
      data = df,
      num.trees = 1,
      num.threads = 1,
      min.node.size = minnodesize,
      max.depth = maxdepth,
      mtry = ncol(df) - 1,
      replace = FALSE,
      sample.fraction = 1)
  )
  bench_cpptree = append(bench_cpptree, list(singlebench_cpp))
  bench_rpart = append(bench_rpart, list(singlebench_rpart))
  bench_ranger = append(bench_ranger, list(singlebench_ranger))
}

benchdf = data.frame("n_features" = nvarvec, "n_obs" = n_obs)

for (i in 1:length(nvarvec)) {
  benchdf[["cpptree"]][i] = summary(bench_cpptree[[i]])$mean
  benchdf[["rpart"]][i] = summary(bench_rpart[[i]])$mean
  benchdf[["ranger"]][i] = summary(bench_ranger[[i]])$mean
}
benchdf

library(reshape2)
library(ggplot2)
benchdf = melt(benchdf, id.vars = c("n_features", "n_obs"), variable.name = "software", value.name = "milliseconds")

p = ggplot(benchdf, aes(x = n_features, y = milliseconds, group = software, color = software))  +
  geom_point() +
  geom_line() + 
  theme_bw()
p
ggsave(p, file = "bench_fixedn_mean_entiretree.png")

######### 
# fixed p 
######### 

unit = "ms"
minnodesize = 30
maxdepth = 1
bench_cpptree = list()
bench_rpart = list()
bench_ranger = list()
bench_mob = list()

model_node = function(y, x, start = NULL, weights = NULL, offset = NULL, ...) {
  lm(y ~ 1)
}

n_vars = 5
nobsvec = c(10, 100, 1000, 10000)
bench_cpptree = list()
bench_rpart = list()
bench_ranger = list()
bench_mob = list()

for (n_obs in nobsvec) {
  print(n_obs)
  df = data.frame("V1" = runif(n_obs, -10, 10))
  for (i in 1:n_vars) {
    df[[i]] = runif(n_obs, -10, 10)
  }
  df = data.frame("y" = as.vector(ifelse(df[, 1] < 0, -1000, 1000)), df)
  singlebench_cpp = microbenchmark(
    unit = unit,
    times = n_eval,
    cpptree(
      data = df,
      n_children = 2,
      objective_type = "sse",
      model_type = "mean",
      search_algo_type = "exhaustive",
      min_node_size = minnodesize,
      max_depth = maxdepth,
      target = 1)
  )
  singlebench_rpart = microbenchmark(
    unit = unit,
    times = n_eval,
    rpart(
      y ~ ., df, control = rpart.control(maxdepth = maxdepth, cp = 0, minsplit = minnodesize, minbucket = minnodesize))
  )
  singlebench_ranger = microbenchmark(
    unit = unit,
    times = n_eval,
    ranger(
      formula = y ~ .,
      data = df,
      num.trees = 1,
      num.threads = 1,
      min.node.size = minnodesize,
      max.depth = maxdepth,
      mtry = ncol(df)-1,
      replace = FALSE,
      sample.fraction = 1)
  )
  singlebench_mob = microbenchmark(
    unit = unit,
    times = n_eval,
    mob(y ~ .,
        data = df,
        fit = model_node,
        control = mob_control(minbucket = minnodesize, minsplit = minnodesize, maxdepth = maxdepth + 1))
  )
  bench_cpptree = append(bench_cpptree, list(singlebench_cpp))
  bench_rpart = append(bench_rpart, list(singlebench_rpart))
  bench_ranger = append(bench_ranger, list(singlebench_ranger))
  bench_mob = append(bench_mob, list(singlebench_mob))
}

bench_cpptree
bench_rpart
bench_ranger
bench_mob
save(bench_cpptree, file = "bench_cpptree_fixedfeature_mean.rdata")
save(bench_rpart, file = "bench_rpart_fixedfeature_mean.rdata")
save(bench_ranger, file = "bench_ranger_fixedfeature_mean.rdata")
save(bench_mob, file = "bench_mob_fixedfeature_mean.rdata")

benchdf = data.frame("n_features" = n_vars, "n_obs" = nobsvec)

for (i in 1:length(nobsvec)) {
  benchdf[["cpptree"]][i] = summary(bench_cpptree[[i]])$median
  benchdf[["rpart"]][i] = summary(bench_rpart[[i]])$median
  benchdf[["ranger"]][i] = summary(bench_ranger[[i]])$median
  benchdf[["mob"]][i] = summary(bench_mob[[i]])$median
}

benchdf

library(reshape2)
library(ggplot2)
benchdf = melt(benchdf, id.vars = c("n_features", "n_obs"), variable.name = "software", value.name = "milliseconds")

benchdf$milliseconds = round(benchdf$milliseconds)
p = ggplot(benchdf, aes(x = n_obs, y = milliseconds, group = software, color = software))  +
  geom_point() +
  geom_line() + 
  theme_bw()
p
ggsave(p, file = "bench_fixedfeatures_mean.png")

######### 

####
# entire tree
####

n_vars = 5
nobsvec = c(10, 100, 1000, 10000)
bench_cpptree = list()
bench_rpart = list()
bench_ranger = list()
bench_mob = list()

for (n_obs in nobsvec) {
  print(n_obs)
  df = data.frame("V1" = runif(n_obs, -10, 10))
  for (i in 1:n_vars) {
    df[[i]] = runif(n_obs, -10, 10)
  }
  df = data.frame("y" = apply(df, MARGIN = 1, sum), df)
  singlebench_cpp = microbenchmark(
    unit = unit,
    times = n_eval,
    cpptree(
      data = df,
      n_children = 2,
      objective_type = "sse",
      model_type = "mean",
      search_algo_type = "exhaustive",
      min_node_size = minnodesize,
      max_depth = maxdepth,
      target = 1)
  )
  singlebench_rpart = microbenchmark(
    unit = unit,
    times = n_eval,
    rpart(
      y ~ ., df, control = rpart.control(maxdepth = maxdepth, cp = 0, minsplit = minnodesize, minbucket = minnodesize))
  )
  singlebench_ranger = microbenchmark(
    unit = unit,
    times = n_eval,
    ranger(
      formula = y ~ .,
      data = df,
      num.trees = 1,
      num.threads = 1,
      min.node.size = minnodesize,
      max.depth = maxdepth,
      mtry = ncol(df) - 1,
      replace = FALSE,
      sample.fraction = 1)
  )
  bench_cpptree = append(bench_cpptree, list(singlebench_cpp))
  bench_rpart = append(bench_rpart, list(singlebench_rpart))
  bench_ranger = append(bench_ranger, list(singlebench_ranger))
}

benchdf = data.frame("n_features" = n_vars, "n_obs" = nobsvec)

for (i in 1:length(nvarvec)) {
  benchdf[["cpptree"]][i] = summary(bench_cpptree[[i]])$mean
  benchdf[["rpart"]][i] = summary(bench_rpart[[i]])$mean
  benchdf[["ranger"]][i] = summary(bench_ranger[[i]])$mean
}
benchdf

library(reshape2)
library(ggplot2)
benchdf = melt(benchdf, id.vars = c("n_features", "n_obs"), variable.name = "software", value.name = "milliseconds")

p = ggplot(benchdf, aes(x = n_obs, y = milliseconds, group = software, color = software))  +
  geom_point() +
  geom_line() + 
  theme_bw()
p
ggsave(p, file = "bench_fixedp_mean_entiretree.png")
