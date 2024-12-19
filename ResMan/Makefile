CC=gcc
CFLAGS=-Wall -Wextra
TARGET1=example
TARGET2=client

all: $(TARGET1) $(TARGET2)

$(TARGET1): example.c
	$(CC) $(CFLAGS) -o $(TARGET1) example.c -lrt
$(TARGET2): client.c
	$(CC) $(CFLAGS) -o $(TARGET2) client.c -lrt
