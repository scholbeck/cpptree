CC = g++
CFLAGS = -Wl,--start-group

main: main.cpp
	g++ -o main main.cpp class_arguments.cpp class_data.cpp class_split.cpp
	
	
clean:
	rm -f main
