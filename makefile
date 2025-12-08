# Compiler
CXX = g++

CXXFLAGS = -std=c++17 -Wall \
           -I/opt/homebrew/Cellar/sfml@2.6/2.6.0/include \
           -I./include

LDFLAGS = -L/opt/homebrew/Cellar/sfml@2.6/2.6.0/lib \
          -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system

TARGET = xmas_app

# Detect all .cpp files
SRC := $(wildcard src/*.cpp)

# Convert to object files
OBJ := $(SRC:src/%.cpp=obj/%.o)

# Create obj folder if needed
$(shell mkdir -p obj)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run:
	./$(TARGET)