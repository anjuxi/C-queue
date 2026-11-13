CC       := gcc
CFLAGS   ?= -std=c99 -Wall -Wextra -Wpedantic -O2
CPPFLAGS += -I.

ifeq ($(OS),Windows_NT)
EXE      := .exe
RUN      :=
RM       := del /Q
else
EXE      :=
RUN      := ./
RM       := rm -f
endif

TARGET   := demo$(EXE)
TESTBIN  := test_queue$(EXE)

.PHONY: all test run clean

all: $(TARGET) $(TESTBIN)

$(TARGET): demo.c queue.c queue.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ demo.c queue.c

$(TESTBIN): tests/test_queue.c queue.c queue.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ tests/test_queue.c queue.c

test: $(TESTBIN)
	$(RUN)$(TESTBIN)

run: $(TARGET)
	$(RUN)$(TARGET)

clean:
	-$(RM) $(TARGET) $(TESTBIN) queue.o
