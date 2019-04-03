CXX			= g++
CXX_FLAGS	= -std=c++17 -O2 -g

TARGET		= argparse.o
SOURCE		= argparse.cpp

TEST		= tester
TEST_SRC	= test.cpp

$(TARGET): $(SOURCE)
	$(CXX) $(CXX_FLAGS) -c $(SOURCE) -o $(TARGET)

.PHONY: all
all: clean $(TARGET) $(TEST)

.PHONY: test
test: $(TEST)

$(TEST): $(TARGET)
	$(CXX) $(CXX_FLAGS) $(TEST_SRC) $(SOURCE) -o $(TEST)

.PHONY: clean
clean:
	rm -rf $(TEST) $(TARGET) core *.dSYM
