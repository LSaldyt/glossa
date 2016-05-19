CC        := g++
LD        := g++
CFLAGS     = -c -std=c++14

VPATH = src

BDIR = build

$(BDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $<

build/test: $(BDIR)/Compiler.o $(BDIR)/readFile.o
	$(LD) $(BDIR)/Compiler.o $(BDIR)/readFile.o -o build/Compiler
