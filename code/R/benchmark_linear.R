setwd("xtree")
library(Rcpp)
library(devtools)
library(partykit)
library(rpart)
library(microbenchmark)
library(ranger)
devtools::load_all()


unit = "ms"
minnodesize = 30
maxdepth = 1
n_eval = 10
bench_cpptree = list()
bench_mob = list()

model_node = function(y, x, start = NULL, weights = NULL, offset = NULL, ...) {
  lm(y ~ x)
}

n_vars = 5
nobsvec = c(10, 100, 1000, 10000)
bench_cpptree = list()
bench_mob = list()


model_node = function(y, x, start = NULL, weights = NULL, offset = NULL, ...) {
  lm(y ~ x)
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
  for (i in 2:n_vars) {
    df[[i]] = df[[1]] + rnorm(n_obs, 0, 1)
  }
  singlebench_cpp = microbenchmark(
    unit = unit,
    times = n_eval,
    cpptree(
      data = df,
      n_children = 2,
      objective_type = "sse",
      model_type = "linear",
      model_formula = "x2",
      search_algo_type = "exhaustive",
      min_node_size = minnodesize,
      max_depth = maxdepth,
      target = 1)
  )
  singlebench_mob = microbenchmark(
    unit = unit,
    times = n_eval,
    mob(y ~ V1 | . ,
        data = df,
        fit = model_node,
        control = mob_control(minbucket = minnodesize, minsplit = minnodesize, maxdepth = maxdepth + 1))
  )
  bench_cpptree = append(bench_cpptree, list(singlebench_cpp))
  bench_mob = append(bench_mob, list(singlebench_mob))
}

bench_cpptree
bench_mob
save(bench_cpptree, file = "bench_cpptree_fixedn_feature_linear.rdata")
save(bench_mob, file = "bench_mob_fixedn_feature_linear.rdata")

benchdf = data.frame("n_features" = nvarvec, "n_obs" = n_obs)

for (i in 1:length(nvarvec)) {
  benchdf[["cpptree"]][i] = summary(bench_cpptree[[i]])$median
  benchdf[["mob"]][i] = summary(bench_mob[[i]])$median
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
ggsave(p, file = "bench_fixedn_linear.png")


######### 

unit = "ms"
minnodesize = 30
maxdepth = 10
n_eval = 10
bench_cpptree = list()
bench_mob = list()

model_node = function(y, x, start = NULL, weights = NULL, offset = NULL, ...) {
  lm(y ~ x)
}

n_vars = 5
nobsvec = c(10, 100, 1000, 10000)
bench_cpptree = list()
bench_mob = list()

for (n_obs in nobsvec) {
  print(n_obs)
  df = data.frame("V1" = runif(n_obs, -10, 10))
  for (i in 1:n_vars) {
    df[[i]] = runif(n_obs, -10, 10)
  }
  y = vector()
  for (i in 1:n_obs) {
    if (df[i, 1] <= 0) {
      y[i] = -df[i, 1] 
    } else {
      y[i] = df[i, 1]
    }
  }
  df = data.frame("y" = y, df)
  singlebench_cpp = microbenchmark(
    unit = unit,
    times = n_eval,
    cpptree(
      data = df,
      n_children = 2,
      objective_type = "sse",
      model_type = "linear",
      model_formula = "x2",
      search_algo_type = "exhaustive",
      min_node_size = minnodesize,
      max_depth = maxdepth,
      target = 1)
  )
  singlebench_mob = microbenchmark(
    unit = unit,
    times = n_eval,
    mob(y ~ V1 | . ,
        data = df,
        fit = model_node,
        control = mob_control(minbucket = minnodesize, minsplit = minnodesize, maxdepth = maxdepth + 1))
  )
  bench_cpptree = append(bench_cpptree, list(singlebench_cpp))
  bench_mob = append(bench_mob, list(singlebench_mob))
}

bench_cpptree
bench_mob
save(bench_cpptree, file = "bench_cpptree_fixedfeature_linear.rdata")
save(bench_mob, file = "bench_mob_fixedfeature_linear.rdata")

benchdf = data.frame("n_features" = n_vars, "n_obs" = nobsvec)

for (i in 1:length(nobsvec)) {
  benchdf[["cpptree"]][i] = summary(bench_cpptree[[i]])$median
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
ggsave(p, file = "bench_fixedfeatures_linear.png")



######### 
