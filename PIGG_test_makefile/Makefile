# SOURCE_DIR = ../haha_web

# SOURCE_FILES = $(wildcard $(SOURCE_DIR)/*.cpp)
# SOURCE_FILES += $(wildcard $(SOURCE_DIR)/*/*.cpp)

# INCLUDE_DIR = -I ../haha_web

BINARY_DIR = ../bin/test

test: PIGG_test.cpp
	g++ -std=c++11 -ggdb PIGG_test.cpp -o $(BINARY_DIR)/PIGG_test.out

# config: test_config.cc $(SOURCE_FILES)
# 	g++ -std=c++2a -ggdb $(INCLUDE_DIR) test_config.cc $(SOURCE_FILES) -lpthread -luuid -o $(BINARY_DIR)/configTest.out

# timestamp: test_timeStamp.cc
# 	g++ -std=c++2a -ggdb $(INCLUDE_DIR) test_timeStamp.cc $(SOURCE_FILES) -lpthread -luuid -o $(BINARY_DIR)/timeStampTest.out

# timerheap: test_timerheap.cc
# 	g++ -std=c++2a -ggdb $(INCLUDE_DIR) test_timerheap.cc $(SOURCE_FILES) -lpthread -luuid -o $(BINARY_DIR)/timerheapTest.out

