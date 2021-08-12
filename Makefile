CC = g++
CFLAGS = -g -o -pg

main:
	$(CC) $(CFLAGS) tree main.cpp class_arguments.cpp class_factory.cpp class_reader.cpp class_data.cpp class_node.cpp class_split.cpp class_model.cpp class_objective.cpp class_optimizer.cpp class_tree.cpp class_aggregation.cpp class_splitgenerator.cpp helper_functions.cpp
	
clean:
	rm -f main

iris:
	./tree --filename data/iris.data --target 4 --sep , --task classif --algorithm exhaustive --objective gini --model majorvote --minsize 20 --children 2

iris3way:
	./tree --filename data/iris_10.data --target 4 --sep , --task classif --algorithm exhaustive --objective gini --model majorvote --minsize 1 --children 3

mushroom:
	./tree --filename data/mushroom.data --target 1 --sep , --task classif --algorithm exhaustive --objective gini --model majorvote --minsize 20 --children 2

gt:
	./tree --filename data/gt_data.csv --target 0 --sep , --task regr --algorithm exhaustive --objective sse --model mean --minsize 100 --children 2
