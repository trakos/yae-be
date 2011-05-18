D_O = build/obj
D_B = build/bin
CFLAGS=-g -Wl,--enable-auto-import -D debug -I.
INCFLAGS=-IK:\\Program\ Files\\Boost-1.41.0\\include\\boost-1_41\\ -I.
TARGET=$(D_B)/${LIB}.lib
AR=ar
LIBPREFIX=
LIBSUFFIX=lib
MAINTARGETS=$(D_B)/$(LIBPREFIX)etclient.$(LIBSUFFIX) $(D_B)/$(LIBPREFIX)tlogger.$(LIBSUFFIX) $(D_B)/$(LIBPREFIX)tnet.$(LIBSUFFIX) $(D_B)/$(LIBPREFIX)indent.$(LIBSUFFIX) $(D_B)/$(LIBPREFIX)utils.$(LIBSUFFIX)
OSTARGETS=

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
  TARGET=$(D_B)/lib${LIB}.a
  CXX=gcc44
  CFLAGS=-g -D debug -fPIC -I.
  LIBPREFIX=lib
  LIBSUFFIX=a
  OSTARGETS=$(D_B)/$(LIBPREFIX)tmysql.$(LIBSUFFIX)
endif


# ETCLIENT
PATH_PREFIX=ET/Client
NAME_PREFIX=ETClient
LIBNAME=etclient
OBJS=$(patsubst $(PATH_PREFIX)/%.cpp,$(D_O)/$(NAME_PREFIX)%.o,$(wildcard $(PATH_PREFIX)/*.cpp))
$(D_O)/$(NAME_PREFIX)%.o: $(PATH_PREFIX)/%.cpp $(PATH_PREFIX)/%.h
	$(CXX) $(INCFLAGS) $(CFLAGS) -c $< -o $@
$(D_B)/$(LIBPREFIX)$(LIBNAME).$(LIBSUFFIX): ${OBJS}
	$(AR) rcs $@ $^
# TLOGGER
PATH_PREFIX=Tlogger
NAME_PREFIX=Tlogger
LIBNAME=tlogger
OBJS=$(patsubst $(PATH_PREFIX)/%.cpp,$(D_O)/$(NAME_PREFIX)%.o,$(wildcard $(PATH_PREFIX)/*.cpp))
$(D_O)/$(NAME_PREFIX)%.o: $(PATH_PREFIX)/%.cpp $(PATH_PREFIX)/%.h
	$(CXX) $(INCFLAGS) $(CFLAGS) -c $< -o $@
$(D_B)/$(LIBPREFIX)$(LIBNAME).$(LIBSUFFIX): ${OBJS}
	$(AR) rcs $@ $^
# TMYSQL	
PATH_PREFIX=Tmysql
NAME_PREFIX=Tmysql
LIBNAME=tmysql
OBJS=$(patsubst $(PATH_PREFIX)/%.cpp,$(D_O)/$(NAME_PREFIX)%.o,$(wildcard $(PATH_PREFIX)/*.cpp))
$(D_O)/$(NAME_PREFIX)%.o: $(PATH_PREFIX)/%.cpp $(PATH_PREFIX)/%.h
	$(CXX) $(INCFLAGS) $(CFLAGS) -c $< -o $@
$(D_B)/$(LIBPREFIX)$(LIBNAME).$(LIBSUFFIX): ${OBJS}
	$(AR) rcs $@ $^
# TNET	
PATH_PREFIX=Tnet
NAME_PREFIX=Tnet
LIBNAME=tnet
OBJS=$(patsubst $(PATH_PREFIX)/%.cpp,$(D_O)/$(NAME_PREFIX)%.o,$(wildcard $(PATH_PREFIX)/*.cpp))
$(D_O)/$(NAME_PREFIX)%.o: $(PATH_PREFIX)/%.cpp $(PATH_PREFIX)/%.h
	$(CXX) $(INCFLAGS) $(CFLAGS) -c $< -o $@
$(D_B)/$(LIBPREFIX)$(LIBNAME).$(LIBSUFFIX): ${OBJS}
	$(AR) rcs $@ $^
# UTILS & INDENT
$(D_O)/%.o: %.cpp %.h
	$(CXX) $(CFLAGS) $(INCFLAGS) -c $< -o $@
$(D_B)/$(LIBPREFIX)utils.$(LIBSUFFIX): $(D_O)/utils.o
	$(AR) rcs $@ $^
$(D_B)/$(LIBPREFIX)indent.$(LIBSUFFIX): $(D_O)/indent.o
	$(AR) rcs $@ $^

all: $(MAINTARGETS) $(OSTARGETS)

clean:
	${RM} $(D_B)/*
	${RM} $(D_O)/*
