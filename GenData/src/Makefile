# Variables definition
ECHO = @echo
INSTALLDIR = /tmp
LIBSDIR = ../
SRC_DIR = . 
.SILENT:

INCLUDES = -IPOS -IGeoCoder -I../ -I./
POS_DIR = POS

LEXICON      = ${POS_DIR}/Bin_and_Data/LEXICON
BIGRAMS      = ${POS_DIR}/Bin_and_Data/BIGRAMS
LRULEFILE    = ${POS_DIR}/Bin_and_Data/LEXICALRULEFILE
CRULEFILE    = ${POS_DIR}/Bin_and_Data/CONTEXTUALRULEFILE

LIBFILES =
LDFLAGS = -L$(LIBSDIR) $(INCLUDES)
CC = g++
CFLAGS = -DLEXICON -DBIGRAMS -DLRULEFILE -DCRULEFILE
SRCS = GenData.cpp IToken.cpp ITokenizer.cpp string.cpp GeoToken.cpp GeoTokenizer.cpp GeoWordNgrams.cpp WordNgrams.cpp GeoCharNgrams.cpp CharNgrams.cpp ngrams.cpp INgrams.cpp POS/BrillTagger.cpp POS/HashSet.cpp GeoCoder/geo-client.c GeoCoder/geo-find.c GeoCoder/io-common.c GeoCoder/io-stdio.c GeoCoder/GeoCoder.cpp WekaHandler.cpp Dictionary.cpp WekaWordNgrams.cpp C45Handler.cpp C45WordNgrams.cpp RE.o REExtractor.o

OBJS = $(SRCS:.cpp=.o)

# Default Target
TARGET = genData

default : genData

genData : $(OBJS)
	$(ECHO) Building GenData
	@echo $(CC) $(LDFLAGS) $(CFLAGS) $(OBJS) -o genData $(LIBFILES)
	$(CC) $(LDFLAGS) $(CFLAGS) $(OBJS) -o genData $(LIBFILES)
GenData.o : GenData.cpp GenData.h GeoTokenizer.o GeoWordNgrams.o GeoCharNgrams.o Dictionary.o WekaHandler.o WekaWordNgrams.o
string.o : string.cpp mystring.h
GeoTokenizer.o : GeoTokenizer.cpp GeoTokenizer.h GeoToken.o ITokenizer.o
GeoTokenizer.cpp : GeoTokenizer.l
	flex GeoTokenizer.l
ITokenizer.o : ITokenizer.cpp ITokenizer.h
GeoToken.o : GeoToken.cpp GeoToken.h IToken.o
IToken.o : IToken.cpp IToken.h
GeoWordNgrams : GeoWordNgrams.cpp GeoWordNgrams.h WordNgrams.o CharNgrams.o GeoToken.o
WordNgrams.o : WordNgrams.cpp WordNgrams.h ngrams.o
GeoCharNgrams.o : GeoCharNgrams.cpp GeoCharNgrams.h CharNgrams.o GeoToken.o
CharNgrams.o : CharNgrams.cpp CharNgrams.h ngrams.o
ngrams.o : ngrams.cpp ngrams.h INgrams.o
INgrams.o : INgrams.cpp INgrams.h
GeoCoder.o : GeoCoder.cpp GeoCoder.h geo-find.o io-common.o io-stdio.o
geo-client.o : geo-client.c geo-find.o
geo-find.o : geo-find.c io-common.o io-mmap.o
io-common.o : io-common.c
io-stdio.o : io-mmap.c
WekaWordNgrams.o : WekaWordNgrams.cpp WekaWordNgrams.h GeoWordNgrams.o WekaHandler.o
WekaHandler.o : WekaHandler.cpp WekaHandler.h
C45WordNgrams.o : C45WordNgrams.cpp C45WordNgrams.h GeoWordNgrams.o C45Handler.o
C45Handler.o : C45Handler.cpp C45Handler.h
Dictionary.o : Dictionary.cpp Dictionary.h
RE.o : RE.cpp RE.h REExtractor.o
REExtractor.o : REExtractor.cpp REExtractor.h
REExtractor.cpp : REExtractor.l
	flex REExtractor.l
rebuild:
	make clean
	make
split:
	echo "Making App for Splitting File List" 
	$(CC) $(LDFLAGS) $(CFLAGS) -o splitFileList SplitFileList.cpp string.cpp
install:
	cp $(TARGET) $(INSTALLDIR)
clean:
	rm -f $(TARGET) *.o *~ POS/*.o GeoCoder/*.o
put2:
	rsync -avz -e ssh splitFileList genDataLocal genDataGeo genDataPos genDataLocalGeo genDataLocalPos genDataGeoPos genDataLocalGeoPos genDataLocalGeoPosPuncLayout genDataLocalGeoPosPuncLayoutFips genDataLocalGeoPosPuncLayoutNoDic genDataLocalGeoPosPuncLayoutRe genDataLocalGeoPosPuncLayoutGeocoder genDataLocalGeoPosPuncLayoutReGeocoder  root@156.34.185.202:"/home/workspace/geo/GenData/src/"
put1:
	rsync -avz -e ssh splitFileList genDataLocal genDataGeo genDataPos genDataLocalGeo genDataLocalPos genDataGeoPos genDataLocalGeoPos genDataLocalGeoPosPuncLayout genDataLocalGeoPosPuncLayoutFips genDataLocalGeoPosPuncLayoutNoDic genDataLocalGeoPosPuncLayoutRe genDataLocalGeoPosPuncLayoutGeocoder genDataLocalGeoPosPuncLayoutReGeocoder  jerry2yu@mpisubversion.mariner.local:"~/geo/GenData/src/"
put:
	rsync -avz -e ssh splitFileList genDataLocal genDataGeo genDataPos genDataLocalGeo genDataLocalPos genDataGeoPos genDataLocalGeoPos genDataLocalGeoPosPuncLayout genDataLocalGeoPosPuncLayoutFips genDataLocalGeoPosPuncLayoutNoDic genDataLocalGeoPosPuncLayoutRe genDataLocalGeoPosPuncLayoutGeocoder genDataLocalGeoPosPuncLayoutReGeocoder  zyu@minerva.cs.dal.ca:"~/geo/GenData/src/"
putAll:
	rsync -avz -e ssh genData zyu@minerva.cs.dal.ca:"~/geo/GenData/src/"
	rsync -avz -e ssh genData zyu@artemis.cs.dal.ca:"~/geo/GenData/src/"
