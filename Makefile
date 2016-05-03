
######################
######################
# makefile generated 
# lopping makefile 
# created by s.e.pav 
# $Id: Makefile,v 1.33 2005/03/16 00:45:01 spav Exp $
# see also http://www.opussoftware.com/tutorial/TutMakefile.htm
# $Log: Makefile,v $
# Revision 1.33  2005/03/16 00:45:01  spav
# adding quick and dirty Chew mode
#
# Revision 1.32  2005/03/11 03:46:57  spav
# problem was ball splitting? -- changed whether subsegs of ball get encroachment checking.
#
# Revision 1.31  2005/03/11 02:04:53  spav
# something wrong with cubics?
#
# Revision 1.30  2005/02/19 00:02:49  spav
# looks like cubic bezier is working
#
# Revision 1.29  2005/02/15 23:51:54  spav
# still working on bez_3
#
# Revision 1.28  2005/02/12 01:37:16  spav
# fixed xfig read bug.
#
# Revision 1.26  2005/01/26 23:11:45  spav
# looks good.
#
# Revision 1.25  2005/01/18 22:47:10  spav
# changing provoker to runtime option.
#
# Revision 1.24  2005/01/04 23:54:46  spav
# some tweaks.
#
# Revision 1.23  2005/01/03 03:59:54  spav
# home from san fran.
#
# Revision 1.22  2004/12/21 22:42:57  spav
# looks like it is working; added command line parser.
#
# Revision 1.21  2004/12/10 23:52:29  spav
# pong back to you.
#
# Revision 1.20  2004/12/06 03:54:19  spav
# added more stuff
#
# Revision 1.19  2004/12/01 00:02:47  spav
# seems to work for arcs. start to add bezier.
#
# Revision 1.18  2004/11/29 07:05:03  spav
# post thanksgiving
#
# Revision 1.17  2004/11/19 02:10:55  spav
# something odd: an infinite vertex and repeated ballsplitting
#
# Revision 1.16  2004/11/18 17:46:10  spav
# fixing makefile
#
# Revision 1.15  2004/11/18 17:45:10  spav
# fixing makefile
#
# Revision 1.14  2004/11/11 02:41:29  spav
# fixed map_to_map?
#
# Revision 1.13  2004/11/09 23:58:25  spav
# slow going. have to rewrite how the endtable works?
#
# Revision 1.12  2004/11/09 02:32:54  spav
# fixed some stuff...
#
# Revision 1.11  2004/11/08 05:56:38  spav
# another lost weekend.
#
# Revision 1.10  2004/11/06 02:47:57  spav
# small changes, back to you.
#
# Revision 1.9  2004/11/05 01:48:59  spav
# ughhhhh!
#
# Revision 1.8  2004/11/05 00:47:48  spav
# a total mess
#
# Revision 1.7  2004/11/04 23:31:25  spav
# got a running readinput--still cannot hook it up, though.
#
# Revision 1.6  2004/11/04 02:40:39  spav
# pong
#
# Revision 1.5  2004/10/29 05:26:50  spav
# pong.
#
# Revision 1.4  2004/10/26 23:48:04  spav
# gaining momentum./
#
# Revision 1.3  2004/10/23 00:38:58  spav
# checking in.
#
# Revision 1.2  2004/10/22 04:52:35  spav
# pong back to you.
#
# Revision 1.1  2004/10/20 21:50:26  spav
# starting code project
#
#
######################
######################

# project name
PROJECT      = lopping

EDITOR      := $(shell which vim)
PAGER   		:= $(shell which less)
GDB     		:= $(shell which gdb)
FLEX     		:= $(shell which flex)
BISON   		:= $(shell which bison)
TIME        := $(shell perl -e 'print time();')

HOME_HOST    = shabbychef.ucsd.edu
STUB         = $(HOME)/sys/STUB
DIR_STUB     = work
CODE_DIR     = $(DIR_STUB)/$(PROJECT)/LOPPER

QTDIR        = /usr/lib/qt3/include
TCLAPDIR     = $(HOME)/$(CODE_DIR)/tclap/include
TAR         := $(shell which tar)


# put all source files here
SRCS         = work_queue Lop_triangulation_2 lookups segment_base 
C_SRCS       = $(patsubst %,%.c,$(SRCS))
CPP_SRCS     = $(patsubst %,%.cpp,$(SRCS))
H_SRCS       = $(patsubst %,%.h,$(SRCS))
C_OBJS       = $(patsubst %,%.o,$(SRCS))

ALL_SRCS     = $(SRCS) work_queue_test Lop_triangulation_2_test lookups_test utility

# CGAL makefile?
CGAL_DIR      = $(HOME)/sys/etc/cgal
ifeq ($(HOSTNAME), sdna-lab-10)
	CGAL_MAKEFILE = $(CGAL_DIR)/make/makefile_i686_Linux-2.4.20-64GB-SMP_g++-3.3.0
endif
ifeq ($(HOSTNAME), cam)
	CGAL_MAKEFILE = $(CGAL_DIR)/make/makefile_i686_Linux-2.4.20-64GB-SMP_g++-3.3.0
endif
ifeq ($(HOSTNAME), shabbychef)
	SHOUTOUT 		:= $(shell echo hello?)
	CGAL_DIR      = /usr/local/CGAL-3.1
	CGAL_MAKEFILE = $(CGAL_DIR)/make/makefile_i686_Linux-2.6.11-gentoo-r9_g++-3.3.5
	BISON++  		:= $(shell which bison++)
endif
include $(CGAL_MAKEFILE)

# testing
TESTRUNNER   = Lop_triangulation_2_test
TEST_SRCS    = $(TESTRUNNER) 
TEST_SRCS    = $(TESTRUNNER) $(SRCS) utility
TEST_OBJS    = $(patsubst %,%.o,$(TEST_SRCS))
TEST_CPPS    = $(patsubst %,%.cpp,$(TEST_SRCS))
TEST_HS      = $(patsubst %,%.h,$(TEST_SRCS))

#with profiler
TESTRUNNERPG := $(TESTRUNNER)_pg
GPROF_FLAGS  = -P -q -w 72

# generated figures
RAN_FIGS		 = as_input.ps curvature_bounded.ps finished_groom.ps \
							 input_conforming.ps quality_insured.ps
GEND_FIGS		 = $(RAN_FIGS) catchend.ps
FIG_CACHE    = figtars/

# This is the name of your output file
OUT          = $(PROJECT)

# CC speficies the name of the C compiler; CCC is the C++ compiler.
CC          ?= gcc
CCC         ?= g++

# This specifies all your include directories
INCLUDES     = -I. -I$(QTDIR) -I$(TCLAPDIR)

# use a profiler?
USE_GP_FLAG  = -pg
GP_FLAGS     = $(USE_GP_FLAG)
GP_FLAGS     = 
# optimization:
OPT_FLAGS    = -O2
OPT_FLAGS    = 
# Put any flags you want to pass to the C,C++ compiler here.
CFLAGS       = -g -Wall $(GP_FLAGS) $(OPT_FLAGS)
CCFLAGS      = $(CGAL_CXXFLAGS) $(LONG_NAME_PROBLEM_CXXFLAGS) $(CFLAGS) -time

# Put any libraries here.
PLOTLIBS     = -lplotter -lXaw -lXmu -lXt -lXext -lX11 -lm
NEWPLOTLIBS  = $(PLOTLIBS) -lSM -lICE
PRELIBS      = -L/usr/local/lib -L. -L/usr/X11R6/lib 
POSTLIBS     = -lc -lm -lz -lstdc++
LIBS         = $(PRELIBS) $(CGAL_LD_LIBPATH) $(POSTLIBS) 
FAIL_IS_OK   = -Wl,-noinhibit-exec
FAIL_IS_OK   = 
LDFLAGS      = -v $(CGAL_LDFLAGS) $(FAIL_IS_OK) -freorder-functions $(GP_FLAGS)

# this is how to set flags via compiler. 
LTF_PARANOID = -D'PARANOID=2'

# lop triangulator binary flags
# -L itlimit -d maxdeg -t tau -q quality -b beta -g gamma

LTF_ITLIMIT  = -L 25
LTF_ITLIMIT  = -L 653
LTF_ITLIMIT  = -L 66000
LTF_MAXDEG   = -d 3
LTF_MAXDEG   = -d 4
LTF_TAU      = -t 0.4
LTF_TAU      = -t 0.25
LTF_TAU      = -t 0.5
LTF_BETA     = -b 0.8
LTF_BETA     = -b 0.6
LTF_BETA     = -b 0.75
LTF_BETA     = -b 0.3
LTF_BETA     = -b 0.75
LTF_BETA     = -b 0.66666666666
LTF_BETA     = -b 0.5
LTF_BETA     = -b 0.9
LTF_GAMMA    = -g 0.41421356237

LTF_CHEW     = -C 0.00025
LTF_CHEW     = -C 0.0

LTF_VERBOSITY = -V 2
LTF_PROVO    = --use_provoker

# quality for 30 degree aka arcsin 2^-1
LTF_QUAL     = -q 1.0
# quality for 20.07 degree aka arcsin 2^-3/2 
LTF_QUAL     = -q 0.5
# quality for 26.45 degree aka arcsin 2^-7/6 
LTF_QUAL     = -q 0.793700525984100

LTF_CORE     = $(LTF_ITLIMIT) $(LTF_MAXDEG) $(LTF_TAU) $(LTF_BETA) \
							 $(LTF_GAMMA) $(LTF_QUAL) $(LTF_VERBOSITY) $(LTF_CHEW)
LOPTRIFLAGS  = $(LTF_CORE)

# Debugging
ADD_PARANOIA = 1
ifeq ($(ADD_PARANOIA),1)
CCFLAGS      += $(LTF_PARANOID)
endif
DEBUG_FLAGS  = -g -ggdb

# stuff for STLfilter
DO_FILTER    = 0
STLFILT      = gSTLFilt.pl               
                                        
DOPTS        = -hdr:m -cand:L -iter:L -with:S -banner:N

DECRYPTOR_ARGS=
TMPDIR       = /tmp
TMPSTUB      = $(TMPDIR)/tmp_compile
TMPPOST      = .cout

# stuff for flex
FLEX_FLAGS   = -v
FLEX_LIBS    = -lfl

# stuff for bison
BISON_FLAGS  = -v -d
BISON_LIBS   = 

# stuff for bison++
B++_FLAGS    = 
B++_LIBS     = 

# meta inputs
METAIN       = $(HOME)/work/sangria/sml/METAINPUT

############## DEFAULT ##############

default : build_it

############## MARKERS ##############

.PHONY   : %.test
.SUFFIXES: .c .c .ln .cpp .o .testo _test.o
.PRECIOUS: %.testo %.o %.yy.cpp %.tab.cpp %.tab.hpp %.tab.c %.tab.h

############ BUILD RULES ############
		
%.o : %.c %.h
		$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@ 2>&1 | $(PAGER)

ifeq ($(DO_FILTER),1)
define compilecpp
		@touch $(TMPSTUB)$*$(TMPPOST);
		@trap "rm $(TMPSTUB)$*$(TMPPOST); exit 1" 1 2 3 15
		-$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@ > $(TMPSTUB)$*$(TMPPOST) 2>&1
		@echo compiled, now filter through STLfilt;
		@perl $(STLFILT) $(DOPTS) $(DECRYPTOR_ARGS) < $(TMPSTUB)$*$(TMPPOST);
		@-rm $(TMPSTUB)$*$(TMPPOST);
endef
else
define compilecpp
		$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@  
endef
endif

%.o : %.cpp %.h
		$(compilecpp)

%.pg.o : CCFLAGS += $(USE_GP_FLAG)
%.pg.o : %.cpp %.h
		$(compilecpp)

define linkos 
@echo --------------------------------------;
@echo link $^ into $@
$(CCC) $(LDFLAGS) $^ $(LIBS) -o $@;
chmod 700 $@
endef

%.bin : %.o
		$(linkos)

%.pg.bin : LDFLAGS += $(USE_GP_FLAG)
%.pg.bin : %.pg.o
		$(linkos)

%.testo : %.o %_test.o 
		@echo --------------------------------------;
		@echo dependencies constructed. now compile:
		$(linkos)

%.test : %.testo %.o %_test.o
		@echo --------------------------------------;
		@echo running $< :
		@./$< ;
		@echo --------------------------------------;
		@echo done;
		@-rm $*_test.o $*.o;

%.debug : %.cpp %.h %_test.cpp %_test.h
		$(CCC) $(INCLUDES) $(DEBUG_FLAGS) $(CCFLAGS) -c $*.cpp -o $*.o;
		$(CCC) $(INCLUDES) $(DEBUG_FLAGS) $(CCFLAGS) -c $*_test.cpp -o $*_test.o;
		$(CCC) $(LDFLAGS) $(LIBS) $*.o $*_test.o -o $*.test.o;
		chmod 700 $*.test.o;
		@echo --------------------------------------;
		@echo $(GDB) $*.test.o :
		@$(GDB) $*.test.o 
		@echo --------------------------------------;
		@echo done;

############ LEX & YACC #############

%.yy.cpp : %.lpp %.tab.hpp
		$(FLEX) $(FLEX_FLAGS) -+ -o$@ $<;

%.yy.c   : %.l %.tab.h
		$(FLEX) $(FLEX_FLAGS) -o$@ $<;

%.tab.c %.tab.h : %.y
		$(BISON) $(BISON_FLAGS) $<;

%.tab.cpp %.tab.hpp : %.ypp
		$(BISON++) $(B++_FLAGS) -o $@ -h %*.tab.hpp $<;

%.tabo  : %.tab.o %.yy.o
		$(CCC) $(LDFLAGS) $*.tab.o $*.yy.o -o $@ $(BISON_LIBS) $(FLEX_LIBS);
		chmod 700 $@;

############ BUILD BINS #############

build_it : $(TESTRUNNER) 

$(TESTRUNNER) : $(TEST_OBJS) 
		@echo dependencies constructed. now compile:
		$(linkos)

$(TESTRUNNERPG) : LDFLAGS += $(USE_GP_FLAG)
$(TESTRUNNERPG) : $(patsubst %.o,%.pg.o,$(TEST_OBJS))
		@echo dependencies constructed. now compile:
		$(linkos)

############# RUNNING ###############

# a rule used multiple times is `define'd

define testarunner
chmod 700 $(TESTRUNNER);
@echo --------------------------------------;
@echo running $(TESTRUNNER);
@echo --------------------------------------;
@./$(TESTRUNNER) $(LOPTRIFLAGS) $< $@ 
@echo --------------------------------------;
@echo done;
endef

# run from dat or fig

%.pcmp : %.dat $(TESTRUNNER)
		$(testarunner)
		make $*.capfigs;

%.pcmp : %.fig $(TESTRUNNER)
		$(testarunner)
		make $*.capfigs;

%.pcmp : %.wdb $(TESTRUNNER)
		$(testarunner)
		make $*.capfigs;

# create dat file?

%.dat : $(METAIN)/%Gen.pl
	perl -P $(METAIN)/$*Gen.pl > $@;

# run with provoker flag set

%.ppcmp : LOPTRIFLAGS = $(LTF_CORE) $(LTF_PROVO)
%.ppcmp : %.dat $(TESTRUNNER)
		$(testarunner)
		make $*_provo.capfigs;

%.ppcmp : LOPTRIFLAGS = $(LTF_CORE) $(LTF_PROVO)
%.ppcmp : %.fig $(TESTRUNNER)
		$(testarunner)
		make $*_provo.capfigs;

%.ppcmp : LOPTRIFLAGS = $(LTF_CORE) $(LTF_PROVO)
%.ppcmp : %.wdb $(TESTRUNNER)
		$(testarunner)
		make $*_provo.capfigs;

# run with profiler

%.prof : TESTRUNNER := $(TESTRUNNERPG)
%.prof : %.dat $(TESTRUNNER)
		@-rm gmon.out
		$(testarunner)
		gprof $(GPROF_FLAGS) $(TESTRUNNER) > $@;

%.prof : TESTRUNNER := $(TESTRUNNERPG)
%.prof : %.fig $(TESTRUNNER)
		@-rm gmon.out
		$(testarunner)
		gprof $(GPROF_FLAGS) $(TESTRUNNER) > $@;


testing : input.pcmp

# roll the figs together 
%.capfigs : $(patsubst %.ps,%.eps,$(RAN_FIGS))
		-rename .eps _$*_$(TIME).eps $(patsubst %.ps,%.eps,$(RAN_FIGS))
		@-tar -hczvf figs_$*_$(TIME).tgz $(patsubst %.ps,%_$*_$(TIME).eps,$(RAN_FIGS))
		@-tar -tzvf figs_$*_$(TIME).tgz
		mv figs_$*_$(TIME).tgz $(FIG_CACHE)
		-rm $(patsubst %.ps,%_$*_$(TIME).eps,$(RAN_FIGS))

capfigs : $(patsubst %.ps,%.eps,$(RAN_FIGS))
		-rename .eps $(TIME).eps $(patsubst %.ps,%.eps,$(RAN_FIGS))
		@-tar -hczvf figs_$(TIME).tgz $(patsubst %.ps,%$(TIME).eps,$(RAN_FIGS))
		@-tar -tzvf figs_$(TIME).tgz 
		mv figs_$(TIME).tgz $(FIG_CACHE)
		-rm $(patsubst %.ps,%$(TIME).eps,$(RAN_FIGS))

# make correct bounding box
%.eps : %.ps 
		@echo find real bounding box for $<;
		@echo -n '   was: ';
		@if grep Bounding $<; then\
  	perl -p -e 'BEGIN { $$spit = 0;\
   	$$bb = qx[gs -q -dNOPAUSE -dSAFER -dDELAYSAFER -dBATCH -sDEVICE=bbox -sOutputFile=/dev/null $< 2>&1 | awk "/\%\%Bounding/ {print}"];} \
   	$$spit ||= (/^%%Bounding/ and $$_ = qq[$$bb]);' $< > $@;\
  	else\
		echo 'noBoundingBox';\
  	perl -p -e 'BEGIN { $$spit = 0;\
   	$$bb = qx[gs -q -dNOPAUSE -dSAFER -dDELAYSAFER -dBATCH -sDEVICE=bbox -sOutputFile=/dev/null $< 2>&1 | awk "/\%\%Bounding/ {print}"];} \
   	$$spit ||= ($$_ = qq[$$bb$$_]);' $< > $@;\
  	fi;
		@echo -n 'is now: ';
		@head $@ | grep Bounding

############# UTILITIES #############

%.ppc : %.h
		@cat copyright.txt > $@;
		@echo -e "#include \"$<\"\n\nnamespace lopping { } \n\n" >> $@;

%.start : %.h
		@cat copyright.txt > $*.cpp;
		@perl -e 'print qq(#include "$<"\n\n\/\/because template export is not ), \
             qq(supported, ), $*, qq(.h \n\/\/may include this file, so we ), \
						 qq(add this gate:\n\n#ifndef ), uc($*), qq(_CPP\n#define ), \
						 uc($*), qq(_CPP\n\nnamespace lopping {\n\n} \n\n#endif \/\/), \
						 uc($*), qq(_CPP\n);' >> $*.cpp

%.dep : %.c
		makedepend -- $(CFLAGS) -- $(INCLUDES) $<

%.dep : %.cpp
		makedepend -- $(CCFLAGS) -- $(INCLUDES) $<

%.clean :
		-/bin/rm -f $*.o;

%.kleen :
		@-/bin/rm -f $*.o 

help :
		egrep '^[^\s]+\s*:' Makefile

############### RULES ###############

############# CLEAN UP ##############

clean : $(patsubst %,%.clean,$(ALL_SRCS))
kleen : $(patsubst %,%.kleen,$(ALL_SRCS))

realclean : kleen
		@-rm *.pcmp 
		
cleanfigs :
		@-rm $(GEND_FIGS)

readclean :
		@-rm readinput.output readinput.tab.cpp readinput.tab.hpp readinput.tab.o\
			readinput.yy.cpp readinput.yy.o;

############ MAINTENANCE ############

# remake the makefile from the stub directory
makefile:
		cp Makefile .oldMakefile;
		perl -pe 's/\s+patmak/ $(PROJECT)/;s{\s+CWD_NAME}{ $(DIR_STUB)};' < $(STUB)/CMakefile > Makefile;
		-diff Makefile .oldMakefile;
		-rm -i .oldMakefile;

########### LOST N FOUND ############

#default: dep $(OUT)

$(PROJECT) : $(C_OBJS)
				$(CCC) $(C_OBJS) $(LDFLAGS) $(LIBS) -o $@;

dep   : $(patsubst %,%.dep,$(SRCS))
depend: dep

bummer : bummer.o bummer.h bummer.cpp
		@echo --------------------------------------;
		@echo dependencies constructed. now compile:
		$(CCC) $(LDFLAGS) bummer.o $(LIBS) -o bummer.test.o;
		chmod 700 bummer.test.o;
		@echo --------------------------------------;
		@echo running bummer.test.o :
		@./bummer.test.o;
		@echo --------------------------------------;
		@echo done;

superbummer : superbummer.o superbummer.cpp bummer.o
		@echo --------------------------------------;
		@echo dependencies constructed. now compile:
		$(CCC) $(LDFLAGS) bummer.o superbummer.o $(LIBS) -o superbummer.test.o;
		chmod 700 superbummer.test.o;
		@echo --------------------------------------;
		@echo running superbummer.test.o :
		@./superbummer.test.o;
		@echo --------------------------------------;
		@echo done;

readtest : readinput.tabo
		@echo --------------------------------------;
		@echo dependencies constructed. now run:
		cat A.dat | ./readinput.tabo;
		@echo --------------------------------------;

radtest : readinput.tab.o readinput.yy.o myFlexLexer.o
		$(CCC) $(LDFLAGS) $^ -o $@ $(BISON_LIBS) $(FLEX_LIBS);
		@echo --------------------------------------;
		@echo dependencies constructed. now run:
		cat A.dat | ./readinput.tabo;
		@echo --------------------------------------;

# DO NOT DELETE THIS LINE -- make  depend  depends  on  it.

segment_base.o: segment_base.h utility.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/basic.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/config.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/version.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/config/i686_Linux-2.6.11-gentoo-r9_g++-3.3.5/CGAL/compiler_config.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/assertions.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/kernel_assertions.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Object.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Handle_for_virtual.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/enum.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/functions_on_enums.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/tags.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/aff_transformation_tags.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/IO/io_tags.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/number_type_basic.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Number_type_traits.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/number_utils.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/double.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/float.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/int.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/number_utils_classes.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/functional_base.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/IO/io.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/IO/Color.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Handle.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Handle_for.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/memory.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/kernel_basic.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Kernel_traits.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Kernel/global_functions.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Kernel/global_functions_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/user_classes.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Point_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Origin.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Vector_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Direction_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Line_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Ray_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Segment_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangle_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Iso_rectangle_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Circle_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Conic_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/IO/forward_decl_window_stream.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Conic_misc.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Aff_transformation_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Point_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Plane_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Vector_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Direction_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Line_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Ray_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Segment_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangle_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Tetrahedron_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Iso_cuboid_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Sphere_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Aff_transformation_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Kernel/global_functions_internal_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Kernel/mpl.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Kernel/global_functions_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Kernel/global_functions_internal_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/known_bit_size_integers.h
segment_base.o: utility.cpp /usr/local/CGAL-3.1/include/CGAL/Cartesian.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Cartesian_base.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/basic_classes.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Bbox_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Fourtuple.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Bbox_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Sixtuple.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/representation_tags.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Point_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Twotuple.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Vector_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Direction_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Line_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Threetuple.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/predicates/kernel_ftC2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/predicates/sign_of_determinant.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/determinant.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/constructions/kernel_ftC2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Ray_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Segment_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/predicates_on_points_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Triangle_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Circle_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/utility.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Interval_arithmetic.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/FPU.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Iso_rectangle_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Aff_transformation_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Aff_transformation_rep_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Translation_rep_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Rotation_rep_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/rational_rotation.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Scaling_rep_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Data_accessor_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/ConicCPA2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/predicates_on_directions_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/basic_constructions_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/point_constructions_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/line_constructions_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/ft_constructions_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Point_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Vector_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Direction_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Line_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Plane_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Ray_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Segment_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Triangle_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Tetrahedron_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Iso_cuboid_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/predicates_on_points_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/predicates/kernel_ftC3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/constructions/kernel_ftC3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Sphere_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Aff_transformation_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Aff_transformation_rep_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Translation_rep_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/Scaling_rep_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/predicates_on_planes_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/basic_constructions_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/point_constructions_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/plane_constructions_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/ft_constructions_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/function_objects.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Kernel/function_objects.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Kernel/Cartesian_coordinate_iterator_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Kernel/Cartesian_coordinate_iterator_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/squared_distance_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/squared_distance_2_1.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/utils.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/wmult.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Kernel/Wutils.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/squared_distance_utils.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/squared_distance_2_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/squared_distance_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/squared_distance_3_0.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/squared_distance_3_1.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/squared_distance_3_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/intersection_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/intersection_2_1.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Line_2_Line_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Segment_2_Line_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Segment_2_Segment_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/predicates_on_points_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Ray_2_Line_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Ray_2_Segment_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Ray_2_Ray_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Point_2_Line_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Point_2_Ray_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Point_2_Segment_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Point_2_Point_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Point_2_Triangle_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Straight_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/intersection_2_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangle_2_Triangle_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangle_2_Triangle_2_do_intersect.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangle_2_Triangle_2_intersection.C
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangle_2_Line_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Line_2_Triangle_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangle_2_Ray_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Ray_2_Triangle_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangle_2_Segment_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Segment_2_Triangle_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Line_2_Iso_rectangle_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Ray_2_Iso_rectangle_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Segment_2_Iso_rectangle_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Point_2_Iso_rectangle_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Iso_rectangle_2_Iso_rectangle_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/intersection_2_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Circle_2_Circle_2_intersection.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/intersection_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/intersection_3_1.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/bbox_intersection_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/intersection_3_1.C
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangle_3_Line_3_do_intersect.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangle_3_Plane_3_do_intersect.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangle_3_Point_3_do_intersect.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangle_3_Ray_3_do_intersect.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangle_3_Segment_3_do_intersect.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangle_3_Triangle_3_do_intersect.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Cartesian/solve_3.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/solve.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Kernel/interface_macros.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Kernel/Type_equality_wrapper.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangulation_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/iterator.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/circulator.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/circulator_bases.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Iterator_project.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/function_objects.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangulation_short_names_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/triangulation_assertions.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangulation_utils_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangulation_data_structure_2.h
segment_base.o: /usr/local/CGAL-3.1/include/boost/tuple/tuple.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/config.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/config/user.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/config/select_compiler_config.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/config/compiler/gcc.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/config/select_stdlib_config.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/config/select_platform_config.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/config/suffix.hpp
segment_base.o: /usr/include/limits.h /usr/include/features.h
segment_base.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
segment_base.o: /usr/local/CGAL-3.1/include/boost/static_assert.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/config.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/detail/workaround.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/ref.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/utility/addressof.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/bool.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/bool_fwd.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/aux_/adl_barrier.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/aux_/config/adl.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/aux_/config/msvc.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/aux_/config/intel.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/aux_/config/gcc.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/aux_/config/workaround.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/integral_c_tag.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/aux_/config/static_constant.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/tuple/detail/tuple_basic.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/cv_traits.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/add_const.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/detail/type_trait_def.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/detail/template_arity_spec.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/int.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/int_fwd.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/aux_/nttp_decl.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/aux_/config/nttp.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/preprocessor/cat.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/preprocessor/config/config.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/aux_/integral_wrapper.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/aux_/static_cast.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/aux_/template_arity_fwd.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/aux_/preprocessor/params.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/aux_/config/preprocessor.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/preprocessor/comma_if.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/preprocessor/punctuation/comma_if.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/preprocessor/control/if.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/preprocessor/control/iif.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/preprocessor/logical/bool.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/preprocessor/facilities/empty.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/preprocessor/punctuation/comma.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/preprocessor/repeat.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/preprocessor/repetition/repeat.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/preprocessor/debug/error.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/preprocessor/detail/auto_rec.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/preprocessor/tuple/eat.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/preprocessor/inc.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/preprocessor/arithmetic/inc.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/aux_/config/lambda.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/aux_/config/ttp.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/aux_/config/ctps.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/aux_/config/overload_resolution.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/mpl/aux_/lambda_support.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/detail/type_trait_undef.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/add_volatile.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/add_cv.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/is_const.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/detail/cv_traits_impl.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/is_reference.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/config.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/detail/bool_trait_def.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/detail/bool_trait_undef.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/is_volatile.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/remove_const.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/broken_compiler_spec.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/remove_volatile.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/remove_cv.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/function_traits.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/is_function.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/detail/false_result.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/detail/is_function_ptr_helper.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/add_pointer.hpp
segment_base.o: /usr/local/CGAL-3.1/include/boost/type_traits/remove_reference.hpp
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Compact_container.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangulation_ds_face_base_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Dummy_tds_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangulation_ds_vertex_base_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangulation_ds_face_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangulation_ds_vertex_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangulation_ds_iterators_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangulation_ds_circulators_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/IO/File_header_OFF.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/IO/File_header_extended_OFF.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/IO/File_scanner_OFF.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/IO/binary_file_io.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangulation_vertex_base_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangulation_face_base_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Triangulation_line_face_circulator_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Random.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/Delaunay_triangulation_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/point_generators_2.h
segment_base.o: /usr/local/CGAL-3.1/include/CGAL/generators.h
