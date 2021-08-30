df_500 = read.csv("../data/seoulbike_500.csv", header = FALSE)
df_1000 = read.csv("../data/seoulbike_1000.csv", header = FALSE)
df_4000 = read.csv("../data/seoulbike_4000.csv", header = FALSE)
df_8000 = read.csv("../data/seoulbike_8000.csv", header = FALSE)
df_vec = list(df_500, df_1000, df_4000, df_8000)

library(rpart)
library(microbenchmark)
library(ranger)
bench_xtree = list()
bench_rpart = list()
bench_ranger = list()

xtree_msg1 = "../code/tree --filename ../data/seoulbike_500.csv --coltypes nnnnnnnnn --target 0 --sep , --task regr --algorithm exhaustive --model mean --objective sse --minsize 10 --children 2 --print false"
xtree_msg2 = "../code/tree --filename ../data/seoulbike_1000.csv --coltypes nnnnnnnnn --target 0 --sep , --task regr --algorithm exhaustive --model mean --objective sse --minsize 10 --children 2 --print false"
xtree_msg3 = "../code/tree --filename ../data/seoulbike_4000.csv --coltypes nnnnnnnnn --target 0 --sep , --task regr --algorithm exhaustive --model mean --objective sse --minsize 10 --children 2 --print false"
xtree_msg4 = "../code/tree --filename ../data/seoulbike_8000.csv --coltypes nnnnnnnnn --target 0 --sep , --task regr --algorithm exhaustive --model mean --objective sse --minsize 10 --children 2 --print false"
xtree_msgs = list(xtree_msg1, xtree_msg2, xtree_msg3, xtree_msg4)

for (i in 1:4) {
  bench_rpart = append(
    bench_rpart,
    mean(
      microbenchmark(
        rpart(V1 ~ .,
              df_vec[[i]], 
              control = rpart.control(
                minsplit = 10, 
                minbucket = 10, 
                cp = 0, 
                maxcompete = 0, 
                maxsurrogate = 0, 
                xval = 0)))$time) / 1000000)
  bench_ranger = append(
    bench_ranger, 
    mean(
      microbenchmark(
        ranger(V1 ~ .,
               df_vec[[i]],
               num.trees = 1, 
               num.threads = 1, 
               min.node.size = 10, 
               mtry = colnames(data),
               replace = FALSE, 
               sample.fraction = 1))$time) / 1000000)
  bench_xtree = append(
    bench_xtree, 
    mean(
      microbenchmark(
        system(
          xtree_msgs[[i]]))$time) / 1000000)
}

df_bench = data.frame(n = c(500, 1000, 4000, 8000))
df_bench$rpart = unlist(bench_rpart)
df_bench$ranger = unlist(bench_ranger)
df_bench$xtree = unlist(bench_xtree)

library(tidyr)
df_bench = tibble(df_bench)
df_bench = gather(df_bench, "package", "milliseconds", 2:4)

library(ggplot2)
ggplot(df_bench, aes(n, milliseconds, group = package, color = package)) +
  geom_line() +
  geom_point() +
  theme_bw() +
  scale_y_continuous(breaks = seq(0, 2000, 100)) +
  ggtitle("Korean bike sharing data (9 continuous features) ")
ggsave("benchmark_xtree.jpg")

# boston housing

df_boston = read.csv("../data/bh.data", header = FALSE)
bench_rpart = microbenchmark(rpart(V14 ~ ., df_boston, control = rpart.control(minsplit = 10, minbucket = 10, cp = 0, maxcompete = 0, maxsurrogate = 0, xval = 0)))$time / 1000000
bench_ranger = microbenchmark(ranger(V14 ~ ., df_boston, num.trees = 1, num.threads = 1, min.node.size = 10, mtry = colnames(data), replace = FALSE, sample.fraction = 1)) $time / 1000000
bench_xtree = microbenchmark(
  system("../code/tree --filename ../data/bh.data --target 13 --sep , --task regr --algorithm exhaustive --objective sse --model mean --minsize 10 --children 2 --print false"))$time / 1000000


df_bench = data.frame(rpart = bench_rpart, ranger = bench_ranger, xtree = bench_xtree)
df_bench = gather(df_bench, "package", "milliseconds")
ggplot(df_bench, aes(milliseconds, group = package, color = package)) +
  geom_boxplot() +
  ggtitle("boston housing data (506 obs, 13 features)") +
  theme_bw()
ggsave("boston.jpg")
