CXX       = g++
CXXFLAGS  = -c -Wall -std=c++14
LDFLAGS   =
BDIR      = build
VPATH     = src src/Utilities/Parser src/Utilities/IO
SOURCES   = Compiler.cpp readFile.cpp Parser.cpp Parse.cpp
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
