CC = g++

main:
	$(CC) -O3 -o tree main.cpp class_arguments.cpp class_factory.cpp class_reader.cpp class_data.cpp class_node.cpp class_split.cpp class_model.cpp class_objective.cpp class_tree.cpp class_aggregation.cpp class_splitgenerator.cpp helper_functions.cpp
	
debug:
	$(CC) -O2 -g -o tree -pg main.cpp class_arguments.cpp class_factory.cpp class_reader.cpp class_data.cpp class_node.cpp class_split.cpp class_model.cpp class_objective.cpp class_tree.cpp class_aggregation.cpp class_splitgenerator.cpp helper_functions.cpp

gprof:
	$(CC) -O2 -o tree -pg main.cpp class_arguments.cpp class_factory.cpp class_reader.cpp class_data.cpp class_node.cpp class_split.cpp class_model.cpp class_objective.cpp class_tree.cpp class_aggregation.cpp class_splitgenerator.cpp helper_functions.cpp

val:
	valgrind --tool=callgrind ./tree --filename data/gt_data2.csv --target 1 --sep , --task regr --algorithm exhaustive --objective sse --model mean --minsize 100 --children 2

clean:
	rm -f main

boston:
	./tree --filename data/bh.data --target 13 --sep , --task regr --algorithm exhaustive --objective sse --model mean --minsize 10 --children 2 --maxdepth 10

bostonlinear:
	./tree --filename data/bh.data --target 13 --sep , --task regr --algorithm exhaustive --objective sse --model linear --minsize 10 --children 2 --maxdepth 10

bostonmult3:
	./tree --filename data/bh.data --target 13 --sep , --task regr --algorithm random --objective sse --minsize 10 --children 3 --maxdepth 10

bostonmult4:
	./tree --filename data/bh.data --target 13 --sep , --task regr --algorithm random --objective sse --minsize 10 --children 4 --maxdepth 10

bostonmult5:
	./tree --filename data/bh.data --target 13 --sep , --task regr --algorithm random --objective sse --minsize 10 --children 5 --maxdepth 10

iris:
	./tree --filename data/iris.data --target 4 --sep , --task classif --algorithm exhaustive --objective gini --minsize 10 --children 2 --maxdepth 10

irismult:
	./tree --filename data/iris.data --target 4 --sep , --task classif --algorithm random --objective gini --minsize 1 --children 3 --maxdepth 10

iris3way:
	./tree --filename data/iris_10.data --target 4 --sep , --task classif --algorithm exhaustive --objective gini --model majorvote --minsize 1 --children 3

mushroom:
	./tree --filename data/mushroom.data --target 22 --sep , --task classif --algorithm exhaustive --objective gini --minsize 10 --children 2

mushroommult3:
	./tree --filename data/mushroom.data --target 22 --sep , --task classif --algorithm random --objective gini --minsize 1 --children 3

gt:
	./tree --filename data/gt_data2.csv --target 1 --sep , --task regr --algorithm exhaustive --objective sse --model mean --minsize 10 --maxdepth 10 --children 2

gtmult3:
	./tree --filename data/gt_data2.csv --target 1 --sep , --task regr --algorithm random --objective sse --model mean --minsize 10 --maxdepth 10 --children 3

gtmult4:
	./tree --filename data/gt_data2.csv --target 1 --sep , --task regr --algorithm random --objective sse --model mean --minsize 10 --maxdepth 10 --children 4

gtmult5:
	./tree --filename data/gt_data2.csv --target 1 --sep , --task regr --algorithm random --objective sse --model mean --minsize 10 --maxdepth 10 --children 5

gt2:
	./tree --filename data/gt_data1feature.csv --target 1 --sep , --task regr --algorithm exhaustive --objective sse --model mean --minsize 100 --children 2

gt3:
	./tree --filename data/gt_2015.csv --target 1 --sep , --task regr --algorithm exhaustive --objective sse --model mean --minsize 10 --maxdepth 30 --children 2
