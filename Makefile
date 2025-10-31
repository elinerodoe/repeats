INC_DIR = inc
SRC_DIR = src
SOURCES = $(sort $(shell find $(SRC_DIR) -name '*.cc'))
OBJECTS = $(SOURCES:.cc=.o)
DEPS = $(OBJECTS:.o=.d)
TARGET = repeats
CC = g++
CFLAGS = -fPIC -Wall -Wextra -std=c++11
CPPFLAGS = $(addprefix -I, $(INC_DIR)) 

.PHONY: all clean debug release
release: CFLAGS += -O3 -DNDEBUG
release: all
debug: CFLAGS += -O0 -DDEBUG -ggdb3
debug: all
all: $(TARGET)
clean:
	rm -f $(OBJECTS) $(DEPS) $(TARGET)
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^
-include $(DEPS)
%.o: %.cc
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -o $@ -c $<
