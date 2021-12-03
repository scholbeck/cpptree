setwd("xtree")
library(Rcpp)
library(devtools)
library(partykit)
library(rpart)
library(microbenchmark)
library(ranger)
devtools::load_all()
data("BostonHousing", package = "mlbench")
df = BostonHousing

unit = "ms"
minnodesize = 30
maxdepth = 30
n_eval = 10
bench_cpptree = list()
bench_rpart = list()
bench_ranger = list()
bench_mob = list()

model_node = function(y, x, start = NULL, weights = NULL, offset = NULL, ...) {
  lm(y ~ 1)
}

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
    target = 14)
)
singlebench_rpart = microbenchmark(
  unit = unit,
  times = n_eval,
  rpart(
    medv ~ ., df, control = rpart.control(maxdepth = maxdepth, cp = 0, minsplit = minnodesize, minbucket = minnodesize))
)
singlebench_ranger = microbenchmark(
  unit = unit,
  times = n_eval,
  ranger(
    formula = medv ~ .,
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
  mob(medv ~ .,
      data = df,
      fit = model_node,
      control = mob_control(minbucket = minnodesize, minsplit = minnodesize, maxdepth = maxdepth + 1))
)


benchdf = data.frame("software" = "cpptree", "milliseconds" = singlebench_cpp$time)
benchdf = rbind(benchdf, data.frame("software" = "rpart", "milliseconds" = singlebench_rpart$time))
benchdf = rbind(benchdf, data.frame("software" = "ranger", "milliseconds" = singlebench_ranger$time))
benchdf = rbind(benchdf, data.frame("software" = "mob", "milliseconds" = singlebench_mob$time))
benchdf$milliseconds = benchdf$milliseconds / 1000000

benchdf

p = ggplot(benchdf) +
  geom_boxplot(aes(milliseconds, group = software, color = software)) +
  theme_bw() +
  theme(axis.title.y=element_blank(),
        axis.text.y=element_blank(),
        axis.ticks.y=element_blank())
p
ggsave(p, file = "bench_boston_mean.png")


######### 


bench_cpptree = list()
bench_mob = list()

model_node = function(y, x, start = NULL, weights = NULL, offset = NULL, ...) {
  lm(y ~ x)
}

singlebench_cpp = microbenchmark(
  unit = unit,
  times = n_eval,
  cpptree(
    data = df,
    n_children = 2,
    objective_type = "sse",
    model_type = "linear",
    model_formula = "x6",
    search_algo_type = "exhaustive",
    min_node_size = minnodesize,
    max_depth = maxdepth,
    target = 14)
)

singlebench_mob = microbenchmark(
  unit = unit,
  times = n_eval,
  mob(medv ~ rm | .,
      data = df,
      fit = model_node,
      control = mob_control(minbucket = minnodesize, minsplit = minnodesize, maxdepth = maxdepth + 1))
)

benchdf = data.frame("software" = "cpptree", "milliseconds" = singlebench_cpp$time)
benchdf = rbind(benchdf, data.frame("software" = "mob", "milliseconds" = singlebench_mob$time))
benchdf$milliseconds = benchdf$milliseconds / 1000000

p = ggplot(benchdf) +
  geom_boxplot(aes(milliseconds, group = software, color = software)) +
  theme_bw() +
  theme(axis.title.y=element_blank(),
        axis.text.y=element_blank(),
        axis.ticks.y=element_blank())
p
ggsave(p, file = "bench_boston_linear.png")
