######################
# 
# new minimal makefile.
# nothing to see yet..
#
# Created: 2016.05.02
# Copyright: Steven E. Pav, 2016
# Author: Steven E. Pav
# SVN: $Id$
######################

CC 					?= gcc
CXX 				?= g++

HEADERS 		 = $(wildcard *.h)
GCHEADERS 	 = $(patsubst %.h,%.h.gch,$(HEADERS))

############## DEFAULT ##############

.DEFAULT_GOAL 	:= help

############## MARKERS ##############

.PHONY   : help 
.PHONY   : gcheaders

############ BUILD RULES ############

help:  ## generate this help message
	@grep -P '^(([^\s]+\s+)*([^\s]+))\s*:.*?##\s*.*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

$(GCHEADERS): %.h.gch : %.h
	$(CXX) -x c++-header -o $@ -c $<

gcheaders : $(GCHEADERS) ## precompile all header files.

