CC = g++
CFLAGS = -Wall -g
TARGET = mygame

# Find all .cpp files in the current directory
SRC = $(wildcard *.cpp)
# Convert each .cpp file to a corresponding .o file
OBJ = $(SRC:.cpp=.o)

# The default target
all: $(TARGET)

# Rule to create the target executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) -lsfml-graphics -lsfml-window -lsfml-system

# Rule to compile .cpp files to .o files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the build
clean:
	rm -f $(TARGET) $(OBJ)

