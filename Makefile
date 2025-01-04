CXX = g++ -g -std=c++20
FILES = build/commands.o  build/user.o build/resturant_methods.o build/system.o build/main.o
EXE = a

all: $(EXE)


build/commands.o: src/commands.cpp src/classes.hpp src/main.cpp src/reserve.hpp
	$(CXX) -c $< -o $@



build/user.o: src/user.cpp src/classes.hpp src/main.cpp
	$(CXX) -c $< -o $@


build/resturant_methods.o: src/resturant_methods.cpp src/classes.hpp
	$(CXX) -c $< -o $@

build/system.o: src/system.cpp src/classes.hpp
	$(CXX) -c $< -o $@


build/main.o: src/main.cpp 	src/header.hpp src/classes.hpp src/resturant_methods.cpp src/system.cpp 
	$(CXX) -c $< -o $@


$(EXE): $(FILES)
	$(CXX) $^ -o $@ 