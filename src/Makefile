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

HDRS = cvector.h hash.hpp dic.hpp extract.hpp address.hpp http_fetcher.h text2token.h getAddress.h cgiparser.h displayHtml.h
SRCS = cvector.c hash.cpp dic.cpp extract.cpp address.cpp http_fetcher.c text2token.c getAddress.c geo.c cgiparser.c displayHtml.c
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
extract.o: extract.hpp
dic.o: dic.hpp
hash.o: hash.hpp
text2token.o: text2token.h
address.o: address.h address.l
http_fetcher.o: http_fetcher.h
getAddress.o: getAddress.h
cgiparser.o: cgiparser.h
displayHtml.o: displayHtml.h displayHtml.l

all :
	make
put:
	cp geo /usr/local/apache/cgi-bin/geo.cgi
clean : 
	@echo "Removing all object files..."
	rm -f core *.o $(TARGET)
