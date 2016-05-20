# CC         = g++
# CFLAGS     = -c -std=c++14
# LDFLAGS    =
# VPATH      = src src/Utilities/Parser src/Utilities/IO
# BDIR       = build
# SOURCES    = Compiler.cpp Parser.cpp readFile.cpp
# OBJECTS    = $(BDIR)/Compiler.o $(BDIR)/Parser.o $(BDIR)/readFile.o
# EXECUTABLE = $(BDIR)/Compiler
#
# all: $(SOURCES) $(EXECUTABLE)
#
# $(EXECUTABLE): $(OBJECTS)
# 	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
#
# .cpp.o:
# 	$(CC) $(CFLAGS) $< -o $@

#This below works but doesn't put object files in the right place
#
# CC        = g++
# CFLAGS    = -c -Wall -std=c++14
# LDFLAGS   =
# BDIR      = build
# VPATH     = src src/Utilities/Parser src/Utilities/IO
# SOURCES   = Compiler.cpp Parser.cpp readFile.cpp
# OBJECTS   = $(SOURCES:.cpp=.o)
# EXECUTABLE= $(BDIR)/Compiler
#
# all: $(SOURCES) $(EXECUTABLE)
#
# # build/Compiler: $(OBJECTS)
# # g++ flags objects -o build/Compiler
# $(EXECUTABLE): $(OBJECTS)
# 	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
#
# .cpp.o:
# 	$(CC) $(CFLAGS) $< -o $@


CXX       = g++
CXXFLAGS  = -c -Wall -std=c++14
LDFLAGS   =
BDIR      = build
VPATH     = src src/Utilities/Parser src/Utilities/IO
SOURCES   = Compiler.cpp Parser.cpp readFile.cpp
OBJECTS   = $(SOURCES:.cpp=.o)
# OBJECTS   =
EXECUTABLE= $(BDIR)/Compiler

all: $(SOURCES) $(EXECUTABLE)


# build/Compiler: $(OBJECTS)
# g++ flags objects -o build/Compiler
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(SOURCES:%.cpp=$(BDIR)/%.o) -o $@


.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $(BDIR)/$@
