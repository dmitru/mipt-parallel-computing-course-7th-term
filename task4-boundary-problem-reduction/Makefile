
EXECUTABLE=reduction

CC= gcc
LDFLAGS=`pkg-config --libs glib-2.0` -lm
CFLAGS=-std=c11 `pkg-config --cflags glib-2.0` -O3 -g -Wall -DDEBUG -fopenmp 

SOURCES=parallel-reduction.c model.c
OBJECTS=$(SOURCES:.cpp=.o)

all: $(SOURCES) ${EXECUTABLE}
	@echo "Done!"

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean: 
	rm -f *.o
	rm ${EXECUTABLE}
