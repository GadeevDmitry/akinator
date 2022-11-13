main: main.o akinator.o ../lib/stack/stack.o ../lib/algorithm/algorithm.o ../lib/logs/log.o ../lib/read_write/read_write.o
	g++ main.cpp akinator.o ../lib/stack/stack.o ../lib/algorithm/algorithm.o ../lib/logs/log.o ../lib/read_write/read_write.o -lsfml-graphics -lsfml-window -lsfml-system -o ./akinator

../lib/stack/stack.o: ../lib/stack/stack.cpp
	g++ -c ../lib/stack/stack.cpp			-o ../lib/stack/stack.o

../lib/algorithm/algorithm.o: ../lib/algorithm/algorithm.cpp
	g++ -c ../lib/algorithm/algorithm.cpp 	-o ../lib/algorithm/algorithm.o

../lib/logs/log.o: ../lib/logs/log.cpp
	g++ -c ../lib/logs/log.cpp			  	-o ../lib/logs/log.o

../lib/read_write/read_write.o: ../lib/read_write/read_write.cpp
	g++ -c ../lib/read_write/read_write.cpp	-o ../lib/read_write/read_write.o

akinator.o: akinator.cpp
	g++ -c akinator.cpp 					-o akinator.o

main.o: main.cpp
	g++ -c main.cpp			  				-o main.o
