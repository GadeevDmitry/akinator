all:
	g++ -c ../lib/algorithm/algorithm.cpp 	-o ../lib/algorithm/algorithm.o
	g++ -c ../lib/logs/log.cpp			  	-o ../lib/logs/log.o
	g++ -c ../lib/read_write/read_write.cpp	-o ../lib/read_write/read_write.o
	
	g++ -c tree_binary.cpp 				-o tree_binary.o

	g++ main.cpp tree_binary.o ../lib/algorithm/algorithm.o ../lib/logs/log.o ../lib/read_write/read_write.o