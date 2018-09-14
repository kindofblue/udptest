TARGET = udp_send udp_recv
CC = gcc
CFLAGS = -Os 
.PHONY: default all clean

all: $(TARGET)

OBJECTS := $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	ar ru $@ $^
	ranlib $@


clean:
	rm -f *.o
	rm -f $(TARGET)
