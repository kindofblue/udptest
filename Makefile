CC = gcc
CFLAGS = -Os 
#.PHONY: default all clean


OBJ := $(patsubst %.c, %.o, $(wildcard *.c))
INC = $(wildcard *.h)
PROGS := udp_send udp_recv


$(foreach P,$(PROGS),$(eval OBJ_$(P) := $(wildcard $(P).c)))

$(foreach P,$(PROGS),$(eval $(P) := $(OBJ_$(P))))


all: $(PROGS)

clean:
	rm -f $(OBJ)
	rm -f $(PROGS)
