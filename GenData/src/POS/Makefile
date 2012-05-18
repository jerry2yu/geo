# ======================================================================
# Makefile - for POST
#
# 020501: Benjamin Han <benhdj@cs.cmu.edu> Created (pulled out from
#	      Analyzer).
# 021601: Benjamin Han <benhdj@cs.cmu.edu> Added ePost2 for parallel
#         corpora.
# 080401: Benjamin Han <benhdj@cs.cmu.edu> Adapted for MSVC++; chaned 
#         license to LGPL.
# 071802: Benjamin Han <benhdj@cs.cmu.edu> Changed -march to i686; added
#         a new target 'tar' for packaging.
# ======================================================================

#   Copyright (C) 2001 Benjamin Han <benhdj@cs.cmu.edu>
#   The original Transformation-based Part-of-Speech Tagger is written
#   by Eric Brill <brill@cs.jhu.edu> - see the separate copyright notice
#   under post directory
#
#    This library is free software; you can redistribute it and/or
#    modify it under the terms of the GNU Lesser General Public
#    License as published by the Free Software Foundation; either
#    version 2.1 of the License, or (at your option) any later version.
#
#    This library is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#    Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public
#    License along with this library; if not, write to the Free Software
#    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

# Modify the following two to suit your needs
CPPFLAGS     = -DNDEBUG
CXXFLAGS     = -O6 -march=i686
#CXXFLAGS     = -g
TGZ=ePost.tar.gz

POST_DIR     = post

# Note this implies ePost must run in its own directory! Annoying but
# I'm just too lazy to change this
LEXICON      = ${POST_DIR}/Bin_and_Data/LEXICON
BIGRAMS      = ${POST_DIR}/Bin_and_Data/BIGRAMS
LRULEFILE    = ${POST_DIR}/Bin_and_Data/LEXICALRULEFILE
CRULEFILE    = ${POST_DIR}/Bin_and_Data/CONTEXTUALRULEFILE

POST_INC_DIR = -I${POST_DIR}/Utils -I${POST_DIR}/Tagger_Code
BRILL_DEFINE = -DLEXICON=\"${LEXICON}\" -DBIGRAMS=\"${BIGRAMS}\" -DLRULEFILE=\"${LRULEFILE}\" -DCRULEFILE=\"${CRULEFILE}\"

# all targets
BIN          = ePost
BIN_OBJ      = ${BIN:=.o}

POST_OBJ     = ${POST_DIR}/Tagger_Code/BrillTagger.o \
	       ${POST_DIR}/Utils/lex.o \
	       ${POST_DIR}/Utils/useful.o \
	       ${POST_DIR}/Utils/HashSet.o \

# dependencies for executables
all: ${BIN}

ePost: ${POST_OBJ} ePost.cpp
	${CXX} ${CPPFLAGS} ${CXXFLAGS} ${POST_INC_DIR} ${BRILL_DEFINE} -o $@ $^

# Brill's POST
${POST_OBJ}:
	${MAKE} -C ${POST_DIR}

# header dependencies

.PHONY: clean tar

tar:
	-${RM} -rf *~ ${BIN} ${BIN_OBJ} ${POST_OBJ} *.o ${DEPS} ${TGZ} .[^.]* core
	tar -C ../ -czvf ${TGZ} --exclude ${TGZ} --exclude-from exfile ePost

clean:
	-${RM} ${BIN} ${BIN_OBJ} *.o ${DEPS} ${TGZ} .[^.]* core
	${MAKE} -C ${POST_DIR} clean
