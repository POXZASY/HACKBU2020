
FLAGS = -lopengl32 -L./lib/ -lglfw3 -lglew32 -Wall -g -m64 -std=c++17 -static-libgcc -static-libstdc++

main: main.cpp glew.o
	g++ -o main main.cpp glew.o $(FLAGS)

run: main
	./main.exe

clean:
	del *.exe

miguel_clean:
	rm *.exe
	

