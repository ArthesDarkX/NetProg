CXX      = g++
CXXFLAGS = -Wall -Wextra -O2
TARGET   = echo_server
SRC      = main.cpp

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: clean
