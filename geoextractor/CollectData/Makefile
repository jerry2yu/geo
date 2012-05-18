##
## Makefile for Address Extractor
##
##

## Set the compiler to be a known ANSI compliant one
CC = g++

## adding the '-DNDEBUG' defines the CT symbol NDEBUG which
## suppresses all the asserts and what not. You'd do that for
## a production build, but it is a good idea to leave it in
## for the purposes of this class.
DFLAG = ## -DNDEBUG

## the -I allows you to include any local header files for our
## class libraries.  
CFLAGS = -g -Wall -Wpointer-arith $(DFLAG)
LDFLAGS = -g  -lresolv 

HDRS = cvector.h SimpleDic.h SimpleExtract.h Address.h HttpFetcher.h text2token.h GetAddress.h cgiparser.h UrlHandler.h HashSet.h
SRCS = cvector.c SimpleDic.cpp SimpleExtract.cpp Address.cpp HttpFetcher.c text2token.c GetAddress.c Geo.cpp cgiparser.c UrlHandler.c HashSet.cpp
OBJS = $(SRCS:.c.cpp=.o)
TARGET = geo listFiles

default : $(TARGET)

geo : $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o geo 
listFiles: listFiles.c
	$(CC) listFiles.c $(CFLAGS) $(LDFLAGS) -o listFiles

# The dependencies below make use of make's default rules,
# under which a .o automatically depends on its .c and
# the action taken uses the $(CC) and $(CFLAGS) variables.
# These lines describe a few extra dependencies involved.

cvector.o: cvector.h
SimpleExtract.o: SimpleExtract.h
SimpleDic.o: SimpleDic.h
HashSet.o: HashSet.hpp
text2token.o: text2token.h
Address.o: Address.h Address.l
HttpFetcher.o: HttpFetcher.h
GetAddress.o: GetAddress.h
cgiparser.o: cgiparser.h
UrlHandler.o: UrlHandler.h UrlHandler.l

all :
	make
rebuild :
	make clean
	make
install:
	sudo cp geo.html /var/www
	sudo cp geo /usr/lib/cgi-bin/geo
	sudo cp listFiles /usr/lib/cgi-bin/listFiles
clean : 
	@echo "Removing all object files..."
	rm -f core *.o $(TARGET) Address.cpp UrlHandler.c text2token.c
