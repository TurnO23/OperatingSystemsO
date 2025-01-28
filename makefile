CC = gcc
CFLAGS = -Wall -lm
TARGET = ass1
SRC = *.c

# Default target - compile and create executable
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS)

# Clean target - remove compiled files and directories
clean:
	rm -f $(TARGET)
	rm -f *.o
	rm -rf DATA
	rm -rf HISTOGRAM
	rm -f *.txt
	rm -f *.pdf

# Create necessary directories
directories:
	mkdir -p DATA/Scenario1
	mkdir -p DATA/Scenario2
	mkdir -p DATA/Scenario3
	mkdir -p HISTOGRAM

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean directories run