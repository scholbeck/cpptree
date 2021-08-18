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

iris:
	./tree --filename data/iris.data --target 4 --sep , --task classif --algorithm exhaustive --objective gini --minsize 20 --children 2

iris3way:
	./tree --filename data/iris_10.data --target 4 --sep , --task classif --algorithm exhaustive --objective gini --model majorvote --minsize 1 --children 3

mushroom:
	./tree --filename data/mushroom.data --target 1 --sep , --task classif --algorithm exhaustive --objective gini --model majorvote --minsize 20 --children 2

gt:
	./tree --filename data/gt_data2.csv --target 1 --sep , --task regr --algorithm exhaustive --objective sse --model mean --minsize 100 --children 2

gt2:
	./tree --filename data/gt_data1feature.csv --target 1 --sep , --task regr --algorithm exhaustive --objective sse --model mean --minsize 100 --children 2

gt3:
	./tree --filename data/gt_2015.csv --target 1 --sep , --task regr --algorithm exhaustive --objective sse --model mean --minsize 100 --children 2
