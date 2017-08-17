parser: parser.cpp
	g++ -Wall -Wextra -std=c++11 -O2 -o $@ $^ -lm
