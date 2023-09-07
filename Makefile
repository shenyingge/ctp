# Output binary name
OUTPUT = ctp_run

# Compiler
CXX = g++

# Flags for compilation
CXXFLAGS = -g -I./my

# Linker flags
LDFLAGS = -L./api ./api/thostmduserapi_se.so ./api/thosttraderapi_se.so

# Source files
SOURCES = main.cc my_ctp_api.cc my_main_thread.cc my_task_process_thread.cc
OBJECTS = $(SOURCES:.cc=.o)

# Rule for the final output
all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

%.o: %.cc
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -f $(OBJECTS) $(OUTPUT)

.PHONY: all clean
