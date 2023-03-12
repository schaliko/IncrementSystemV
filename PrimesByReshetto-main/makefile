all: init destroy check set

init: init.cpp
	g++ init.cpp -o init

destroy: destroy.cpp
	g++ destroy.cpp -o destroy

check: check.cpp
	g++ check.cpp -o check

set: set.cpp
	g++ set.cpp -o set

clean:
	rm -f *.o init destroy check set
