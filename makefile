CXX=g++

PROJECT_O= Topograph.o Bmp.o

CXXFLAGS= -c -Wall -g -std=c++14

EXE = M05Assignment

default: main.o $(PROJECT_O)
	$(CXX) main.o $(PROJECT_O) -o $(EXE)

Topograph.o: Topograph.cpp
	$(CXX) $(CXXFLAGS) Topograph.cpp
main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp
Bmp.o: Bmp.cpp
	$(CXX) $(CXXFLAGS) Bmp.cpp

clean:
	rm *.o $(EXE)
run:
	./$(EXE)