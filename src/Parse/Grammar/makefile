CXX       = g++
CXXFLAGS  = -c -std=c++14
LDFLAGS   =
BDIR      = build
VPATH     = ../../Match ../../IO ../../Match/Base ../../Lex ../../Gen ../../Syntax ../
SOURCES   = run.cpp construct.cpp main.cpp IO.cpp Seperate.cpp Locale.cpp Templates.cpp Lex.cpp \
            StatementBuilders.cpp StatementHelpers.cpp Language.cpp StatementResult.cpp \
            Statements.cpp TokenParsers.cpp grammar.cpp 
OBJECTS   = $(SOURCES:%.cpp=$(BDIR)/%.o)
EXECUTABLE= $(BDIR)/Grammar

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

$(BDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@
