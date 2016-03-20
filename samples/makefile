# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS = -Wall -Wpedantic -Wextra -Wundef

# the build target executable:
TARGET = ArrayTests

TARGETDIR = bin

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	mkdir $(TARGETDIR)
	$(CC) $(CFLAGS) -o $(TARGETDIR)/$(TARGET) $(TARGET).cpp

clean:
	$(RM) $(TARGETDIR)/$(TARGET)
	rmdir $(TARGETDIR)
