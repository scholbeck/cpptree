CC = g++
CFLAGS = -Wl,--start-group

main: main.cpp
	g++ -o tree main.cpp class_arguments.cpp class_reader.cpp class_data.cpp class_node.cpp class_split.cpp class_model.cpp class_objective.cpp class_optimizer.cpp class_tree.cpp helper_functions.cpp
	
clean:
	rm -f main

iris:
	./tree --filename data/iris.data --target 4 --sep , --task classif --algorithm exhaustive --objective gini --model majorvote --minsize 20 --children 2

mushroom:
	./tree --filename data/mushroom.data --target 1 --sep , --task classif --algorithm exhaustive --objective gini --model majorvote --minsize 20 --children 2

gt:
	./tree --filename data/gt_data.csv --target 0 --sep , --task regr --algorithm exhaustive --objective sse --model mean --minsize 100 --children 2
