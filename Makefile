main: main.cpp
	g++ -o main main.cpp class_arguments.cpp class_node.cpp class_tree.cpp class_optimizer.cpp 
	
clean:
	rm -f main
