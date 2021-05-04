main: main.cpp
	g++ -o main main.cpp class_arguments.cpp class_data.cpp class_node.cpp class_tree.cpp class_optimizer.cpp class_objective.cpp
	
clean:
	rm -f main
