TARGETS=can_test
SRCDIR=src

# Compiler setup
CXX=g++
CPPFLAGS=-Isrc
CXXFLAGS=-std=gnu++14 -pedantic -Wall -Wextra
LIBS=

# Programs
RM=rm -f

# Rules
.PHONY: all debug clean rebuild

all: CPPFLAGS+=-DNDEBUG
all: CXXFLAGS+=-O3
all: $(TARGETS)

debug: CXXFLAGS+=-g
debug: $(TARGETS)

can_test: $(SRCDIR)/can_test.o
	$(CXX) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	$(RM) $(SRCDIR)/*.o
	$(RM) can_test


rebuild: clean all
