CXX=g++
CXXFLAGS=-O2

.PHONY: clean default

word-counter: main.cpp
	$(CXX) main.cpp -o word-counter $(CXXFLAGS)

clean:
	-rm word-counter

default: word-counter
