all: main

main: main.cpp
	g++ main.cpp -o main

clean:
	rm -f *.o main
