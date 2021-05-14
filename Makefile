CC = g++
CFLAGS = -Wl,--start-group

main: main.cpp
	g++ -o main main.cpp class_arguments.cpp class_data.cpp class_node.cpp class_split.cpp class_model.cpp class_objective.cpp class_optimizer.cpp class_factory.cpp helper_functions.cpp
	
	
clean:
	rm -f main
