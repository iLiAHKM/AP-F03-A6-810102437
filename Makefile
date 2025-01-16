CXX = g++
CXXFLAGS = -std=c++11 -Wall -pedantic
BUILD_DIR = build
TEMPLATE_DIR = .template
OUT_EXE = UTast
FILES = build/commands.o  build/user.o build/resturant_methods.o build/system.o build/main.o

ifeq ($(OS),Windows_NT)
	LDLIBS += -l Ws2_32
endif

all: $(BUILD_DIR) $(OUT_EXE)

$(OUT_EXE): $(BUILD_DIR)/main.o  $(BUILD_DIR)/response.o $(BUILD_DIR)/request.o $(BUILD_DIR)/utilities.o $(BUILD_DIR)/strutils.o $(BUILD_DIR)/server.o $(BUILD_DIR)/route.o $(BUILD_DIR)/template_parser.o
	$(CXX) $(CXXFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/template_parser.o: APHTTP-main/utils/template_parser.cpp APHTTP-main/utils/template_parser.hpp APHTTP-main/utils/request.cpp APHTTP-main/utils/request.hpp APHTTP-main/utils/utilities.hpp APHTTP-main/utils/utilities.cpp APHTTP-main/utils/strutils.hpp APHTTP-main/utils/strutils.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/response.o: APHTTP-main/utils/response.cpp APHTTP-main/utils/response.hpp APHTTP-main/utils/include.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/request.o: APHTTP-main/utils/request.cpp APHTTP-main/utils/request.hpp APHTTP-main/utils/include.hpp APHTTP-main/utils/utilities.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/utilities.o: APHTTP-main/utils/utilities.cpp APHTTP-main/utils/utilities.hpp APHTTP-main/utils/strutils.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/strutils.o: APHTTP-main/utils/strutils.cpp APHTTP-main/utils/strutils.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/server.o: APHTTP-main/server/server.cpp APHTTP-main/server/server.hpp APHTTP-main/server/route.hpp APHTTP-main/utils/utilities.hpp APHTTP-main/utils/strutils.hpp APHTTP-main/utils/response.hpp APHTTP-main/utils/request.hpp APHTTP-main/utils/include.hpp APHTTP-main/utils/template_parser.hpp APHTTP-main/utils/template_parser.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/route.o: APHTTP-main/server/route.cpp APHTTP-main/server/route.hpp APHTTP-main/utils/utilities.hpp APHTTP-main/utils/response.hpp APHTTP-main/utils/request.hpp APHTTP-main/utils/include.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

#$(BUILD_DIR)/handlers.o: examples/handlers.cpp APHTTP-main/server/server.hpp APHTTP-main/utils/utilities.hpp APHTTP-main/utils/response.hpp APHTTP-main/utils/request.hpp APHTTP-main/utils/include.hpp
#	$(CXX) $(CXXFLAGS) -c $< -o $@


build/commands.o: src/commands.cpp src/classes.hpp src/main.cpp src/reserve.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@



build/user.o: src/user.cpp src/classes.hpp src/main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


build/resturant_methods.o: src/resturant_methods.cpp src/classes.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/system.o: src/system.cpp src/classes.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


build/main.o: src/main.cpp APHTTP-main/server/server.hpp APHTTP-main/utils/utilities.hpp APHTTP-main/utils/response.hpp APHTTP-main/utils/request.hpp APHTTP-main/utils/include.hpp	src/header.hpp src/classes.hpp src/resturant_methods.cpp src/system.cpp 
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean
clean:
	rm -rf $(BUILD_DIR) $(TEMPLATE_DIR) *.o *.out &> /dev/null
