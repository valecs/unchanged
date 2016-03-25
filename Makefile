EXECUTABLE:=unchanged

BINDIR := ./bin

SHELL = /bin/sh

WARNINGS := -Werror -Wall -pedantic -W -Wmissing-prototypes -Wstrict-prototypes -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wnested-externs -Wuninitialized
CFLAGS:=-std=c99 -O2

CFLAGS += $(WARNINGS)

SOURCES := unchanged.c
OBJECTS := $(patsubst %.c,$(BINDIR)/%.o, $(SOURCES))

all: $(SOURCES) $(EXECUTABLE)

$(BINDIR)/%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(EXECUTABLE): $(OBJECTS) $(HEADERS)
	$(CC) $(OBJECTS) -o $(BINDIR)/$@

clean:
	cd $(BINDIR) && rm * || true

echo:
	@echo $(SOURCES)
