CC=cc
CFLAGS=-std=c99 -Wall -pedantic -g
TARGET=bgr
RM=rm -f

all: $(TARGET)

$(TARGET): $(TARGET).c config.h *.c
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET)

clean:
	$(RM) $(TARGET)
	@$(RM) $(TARGET).core
	@$(RM) ktrace.out
