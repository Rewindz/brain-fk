CC = gcc
CFLAGS = -Wall -Wextra -I./src
DEBUGFLAGS = -g

SRCDIR = src
BUILDDIR = build
SRC = $(wildcard $(SRCDIR)/*.c)
TARGET = $(BUILDDIR)/brain-fk

$(info Found source files: $(SRC))

all: $(TARGET)

debug: CFLAGS += $(DEBUGFLAGS)
debug: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(BUILDDIR)

.PHONY: all clean
