CC = clang
#CC = $(ARMCC)
CFLAGS = -Wall -Wextra -std=gnu99 -pedantic -O3
LDFLAGS = -ldisarm
TARGET = a3e
OBJECTS = $(TARGET).o exec.o memory.o parser.o pipeline.o types.o
PREFIX=/usr/local/bin

%.o: %.c
	$(CC) $(CFLAGS) -c $<

release: $(OBJECTS) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(TARGET)

clean:
	rm -f *.o *~ core tags $(TARGET)

install: release
	install -D -m755 $(TARGET) $(PREFIX)/$(TARGET)

re: clean release

tags:
	ctags *
