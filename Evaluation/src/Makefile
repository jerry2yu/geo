# Variables definition
ECHO = @echo
INSTALLDIR = /tmp
LIBSDIR = ../
SRC_DIR = . 
.SILENT:

INCLUDES = -IPOS -IGeoCoder -I../ -I./ -I./../../GenData/src/
POS_DIR = POS

LEXICON      = ${POS_DIR}/Bin_and_Data/LEXICON
BIGRAMS      = ${POS_DIR}/Bin_and_Data/BIGRAMS
LRULEFILE    = ${POS_DIR}/Bin_and_Data/LEXICALRULEFILE
CRULEFILE    = ${POS_DIR}/Bin_and_Data/CONTEXTUALRULEFILE

LIBFILES =
LDFLAGS = -L$(LIBSDIR) $(INCLUDES)
CC = g++
CFLAGS = -DLEXICON -DBIGRAMS -DLRULEFILE -DCRULEFILE
SRCS = GetExtractionResult.cpp
OBJS = $(SRCS:.cpp=.o)

# Default Target
TARGET = getExtractionResult sum10FoldCV

default : getExtractionResult sum10FoldCV

getExtractionResult : $(OBJS)
	$(ECHO) Building GetExtractionResult
	@echo $(CC) $(LDFLAGS) $(CFLAGS) $(OBJS) -o getExtractionResult $(LIBFILES)
	$(CC) $(LDFLAGS) $(CFLAGS) $(OBJS) ../../GenData/src/string.cpp -o getExtractionResult $(LIBFILES)
GetExtractionResult.o : GetExtractionResult.cpp GetExtractionResult.h
sum10FoldCV : Sum10FoldCV.o
	$(ECHO) Building Sum10FoldCV 
	$(CC) $(LDFLAGS) $(CFLAGS) Sum10FoldCV.o ../../GenData/src/string.cpp -o sum10FoldCV $(LIBFILES)
GetExtractionResult.o : GetExtractionResult.cpp GetExtractionResult.h
Sum10FoldCV.o : Sum10FoldCV.cpp Sum10FoldCV.h
rebuild:
	make clean
	make
install:
	cp $(TARGET) $(INSTALLDIR)
clean:
	rm -f $(TARGET) *.o *~
put:
	rsync -avz -e ssh getExtractionResult sum10FoldCV zyu@minerva.cs.dal.ca:"~/geo/Evaluation/src/"
	rsync -avz -e ssh C45/c4.5 C45/c45label zyu@minerva.cs.dal.ca:"~/geo/Evaluation/src/C45"
put1:
	rsync -avz -e ssh getExtractionResult sum10FoldCV jerry2yu@mpisubversion.mariner.local:"~/geo/Evaluation/src/"
	rsync -avz -e ssh C45/c4.5 C45/c45label jerry2yu@mpisubversion.mariner.local:"~/geo/Evaluation/src/C45"
put2:
	rsync -avz -e ssh getExtractionResult sum10FoldCV root@156.34.185.202:"/home/workspace/geo/Evaluation/src/"
	rsync -avz -e ssh C45/c4.5 C45/c45label root@156.34.185.202:"/home/workspace/geo/Evaluation/src/C45"
put3:
	rsync -avz -e ssh getExtractionResult sum10FoldCV zyu@apollo.cs.dal.ca:"~/geo/Evaluation/src/"
	rsync -avz -e ssh C45/c4.5 C45/c45label zyu@apollo.cs.dal.ca:"~/geo/Evaluation/src/C45"
put4:
	rsync -avz -e ssh getExtractionResult sum10FoldCV zyu@artemis.cs.dal.ca:"~/geo/Evaluation/src/"
	rsync -avz -e ssh C45/c4.5 C45/c45label zyu@artemis.cs.dal.ca:"~/geo/Evaluation/src/C45"
