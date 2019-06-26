CC=g++
#the following is for normal use:
CFLAGS=-O3 -Wall

SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

TARGET=triangleSimulation
LIBS=-lm

all: $(SOURCES) $(TARGET)

debug: CFLAGS=-O3
debug: CFLAGS += -g
debug: all

# How to make objects and executables
.cpp.o:
	$(CC) -c $(CFLAGS) $^ -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(CFLAGS) -o $@ $(LIBS) $(DEFINES)

clean:
	rm -rf *.o $(TARGET)
