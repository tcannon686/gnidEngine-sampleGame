
INCLUDEDIR = include
SRCDIR = src
DEPSDIR = deps
BINDIR = bin

UNAME = $(shell uname)

LIBS = -lgnid -lglfw

CC = g++
CFLAGS = -g -Wall -c -I$(DEPSDIR) -I$(DEPSDIR)/gnidEngine/include\
		 -I$(DEPSDIR)/gnidEngine/deps -I$(DEPSDIR)/gnidEngine/deps/glad/include\
		 -I$(INCLUDEDIR)

EXEURCES = $(shell find $(SRCDIR) -name "*.cpp")
HEADERS = $(shell find $(INCLUDEDIR) -name "*.hpp")
OBJECTS = $(shell find $(SRCDIR) -name "*.cpp" | sed -e 's|.cpp$$|.o|' | sed -e 's|^$(SRCDIR)|$(BINDIR)|')

EXE = game

$(EXE) : $(OBJECTS) $(DEPSDIR)/gnidEngine/libgnid.so
	cp $(DEPSDIR)/gnidEngine/libgnid.so ./
	$(CC) -L./ $(OBJECTS) $(LIBS) -o $(EXE)

$(BINDIR)/%.o : $(SRCDIR)/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

clean :
	rm -f $(OBJECTS) $(EXE)

