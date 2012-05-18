# Variables definition
ECHO = @echo
INSTALLDIR = /tmp
LIBSDIR = ../
SRC_DIR = ../src
.SILENT:

INCLUDES = -I$(SRC_DIR) -I.

LIBFILES =
LDFLAGS = -L$(LIBSDIR) $(INCLUDES)
CC = g++
CFLAGS =

# Default Target
TARGET = DictionaryTest

default : UnitTest

rebuild :
	make clean
	make
UnitTest : $(TARGET)
	$(ECHO) Building UnitTest
	$(CC) $(LDFLAGS) $(CFLAGS) UnitTest.cpp IUnitTest.cpp -o unitTest $(LIBFILES)  *.o
# Test String
DictionaryTest: Dictionary.o
	$(ECHO) Building DictionaryTest
	$(CC) $(LDFLAGS) $(CFLAGS) DictionaryTest.cpp -c
Dictionary.o: TernarySearchTree.o
	$(CC) $(LDFLAGS) $(CFLAGS) $(SRC_DIR)/Dictionary.cpp -c
TernarySearchTree.o:
	$(CC) $(LDFLAGS) $(CFLAGS) $(SRC_DIR)/string.cpp -c
install:
	cp $(TARGET) $(INSTALLDIR)
clean:
	rm -f unitTest $(TARGET) *.o *~
