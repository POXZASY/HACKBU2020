main: main.cpp glew.o
	g++ -o main main.cpp glew.o -lopengl32 -L./lib/ -lglfw3 -lglew32 -Wall -g
tester: tester.cpp GLFW/glfw3.h
	g++ -o tester tester.cpp -lopengl32 -L./lib_test/ -lglfw3 -Wall -g
