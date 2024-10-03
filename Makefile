# Makefile

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = `pkg-config --cflags gtk+-3.0`

# Linker flags
LDFLAGS = `pkg-config --libs gtk+-3.0`

# Source files
SRCS = main.cpp cpu_info.cpp gpu_info.cpp ui_helpers.cpp graph_temps.cpp bar_temps.cpp

# Object files (replacing .cpp with .o)
OBJS = $(SRCS:.cpp=.o)

# Output executable name (change this to your desired name)
TARGET = temp_guru
# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up the build
clean:
	rm -f $(OBJS) $(TARGET)
