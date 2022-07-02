CC = gcc
CXX = g++
TARGET = shortest.exe

ARCH := $(shell getconf LONG_BIT)
CFLAGS_32 := -m32
CFLAGS_64 := -m64

CFLAGS += -Wall -g -D_GNU_SOURCE -O3 -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 $(CFLAGS_$(ARCH))
CFLAGS_ONLY_C += -fgnu89-inline
INCLUDE += -I/usr/include
LDFLAGS = -lpthread
OBJS = main.o

ifdef DEBUG
CFLAGS += -DDEBUG
endif
ifdef CHECK_WHEN_READ
CFLAGS += -DCHECK_WHEN_READ
endif

all: main.o
	$(CXX) -o $(TARGET) $(OBJS) -Wl,-rpath=$(LIBS)/ -L$(LIBS)/ $(LDFLAGS)
	chmod 777 ${TARGET}

%.o:	%.cpp
	$(CXX) $(INCLUDE) -D_CPP $(CFLAGS) -std=c++17 -c $< -o $@ -lstdc++fs

%.o :	%.c
	$(CC) $(INCLUDE) $(CFLAGS) $(CFLAGS_ONLY_C) -c $< -o $@

clean:
	rm -rf *.o ${TARGET}