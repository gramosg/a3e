CC = gcc
#CC = $(ARMCC)
CFLAGS = -Wall -Wextra -std=c99 -pedantic -O3
LDFLAGS =
TARGET = a3e
OBJECTS = $(TARGET).o exec.o memory.o parser.o pipeline.o
PREFIX=/usr/local/bin

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $<

release: $(OBJECTS) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $(TARGET)

clean:
	rm -f *.o *~ core tags $(TARGET)

install: release
	install -D -m755 $(TARGET) $(PREFIX)/$(TARGET)

re: clean release

tags:
	ctags *
