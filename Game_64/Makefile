FLAGS = -lopengl32 -L./lib/ -lglfw3 -lglew32 -Wall -g -m64 -std=c++17 -static-libgcc -static-libstdc++

main: main.cpp glew.o
	g++ -o main main.cpp glew.o $(FLAGS)

triangle: triangle.cpp triangle.h
	g++ -o triangle triangle.cpp triangle.h glew.o $(FLAGS)
run: main
	./main.exe

clean:
	del *.exe
