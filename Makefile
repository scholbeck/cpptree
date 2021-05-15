CC = g++
CFLAGS = -Wl,--start-group

main: main.cpp
	g++ -o tree main.cpp class_arguments.cpp class_data.cpp class_node.cpp class_split.cpp class_model.cpp class_objective.cpp class_optimizer.cpp class_factory.cpp class_tree.cpp helper_functions.cpp
	
clean:
	rm -f main

run:
	./tree --algorithm exhaustive --objective sse --model mean --minsize 30 --children 2
