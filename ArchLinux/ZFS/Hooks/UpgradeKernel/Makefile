all: upgrade.o main.o KU

upgrade.o: upgrade.cc
	g++ --std=c++17 -g -c upgrade.cc -o upgrade.o

main.o: main.cc
	g++ --std=c++17 -g -c main.cc -o main.o

KU: upgrade.o main.o
	g++ --std=c++17 upgrade.o main.o -o KU

clean:
	rm {*.o,KU}

