CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# name of the executable
TARGET = minimat

# object files
OBJS = main.o math.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c math.h
	$(CC) $(CFLAGS) -c main.c

math.o: math.c math.h
	$(CC) $(CFLAGS) -c math.c

clean:
	rm -f $(OBJS) $(TARGET)
