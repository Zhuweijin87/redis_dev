CC=gcc
CFLAG=
INC_HOME=

TARGET=test note

all:$(TARGET)
%.c:%.o
	$(CC) -c $<

test:test.o sds.o
	$(CC) $(CFLAG) -o $@ test.o sds.o

note:note.o
	$(CC) $(CFLAG) -o $@ note.o

clean:
	rm -f $(TARGET) *.o


