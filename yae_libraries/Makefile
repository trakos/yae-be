D_O = build/obj
D_B = build/bin
CFLAGS=-g -Wl,--enable-auto-import -D __debug__ -I. -gstabs
INCFLAGS=-IK:\\Program\ Files\\Boost-1.41.0\\include\\boost-1_41\\ -I.
TARGET=$(D_B)/${LIB}.lib
AR=ar
LIBPREFIX=
LIBSUFFIX=lib
MAINTARGETS=$(D_B)/$(LIBPREFIX)etserver.$(LIBSUFFIX) $(D_B)/$(LIBPREFIX)etstatus.$(LIBSUFFIX) $(D_B)/$(LIBPREFIX)etclient.$(LIBSUFFIX) $(D_B)/$(LIBPREFIX)tlogger.$(LIBSUFFIX) $(D_B)/$(LIBPREFIX)tnet.$(LIBSUFFIX) $(D_B)/$(LIBPREFIX)tutils.$(LIBSUFFIX)
OSTARGETS=

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
  TARGET=$(D_B)/lib${LIB}.a
  CXX=gcc44
  CFLAGS=-g3 -D __debug__ -fPIC -I. -std=c++0x
  LIBPREFIX=lib
  LIBSUFFIX=a
  OSTARGETS=$(D_B)/$(LIBPREFIX)tmysql.$(LIBSUFFIX) $(D_B)/tmysqlExample
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
# ETSTATUS
PATH_PREFIX=ET
NAME_PREFIX=ET
LIBNAME=etstatus
OBJS=$(patsubst $(PATH_PREFIX)/%.cpp,$(D_O)/$(NAME_PREFIX)%.o,$(wildcard $(PATH_PREFIX)/*.cpp))
$(D_O)/$(NAME_PREFIX)%.o: $(PATH_PREFIX)/%.cpp $(PATH_PREFIX)/%.h
	$(CXX) $(INCFLAGS) $(CFLAGS) -c $< -o $@
LIBNAME=etstatus
$(D_B)/$(LIBPREFIX)$(LIBNAME).$(LIBSUFFIX): $(D_O)/ETStatus.o
	$(AR) rcs $@ $^
LIBNAME=etserver
$(D_B)/$(LIBPREFIX)$(LIBNAME).$(LIBSUFFIX): $(D_O)/ETServer.o
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
LIBNAME=tnet

PATH_PREFIX=Tnet
NAME_PREFIX=Tnet
OBJS=$(patsubst $(PATH_PREFIX)/%.cpp,$(D_O)/$(NAME_PREFIX)%.o,$(wildcard $(PATH_PREFIX)/*.cpp))
$(D_O)/$(NAME_PREFIX)%.o: $(PATH_PREFIX)/%.cpp $(PATH_PREFIX)/%.h
	$(CXX) $(INCFLAGS) $(CFLAGS) -c $< -o $@

UDPPATH_PREFIX=Tnet/UDP
UDPNAME_PREFIX=TnetUDP
UDPOBJS=$(patsubst $(UDPPATH_PREFIX)/%.cpp,$(D_O)/$(UDPNAME_PREFIX)%.o,$(wildcard $(UDPPATH_PREFIX)/*.cpp))
$(D_O)/$(UDPNAME_PREFIX)%.o: $(UDPPATH_PREFIX)/%.cpp $(UDPPATH_PREFIX)/%.h
	$(CXX) $(INCFLAGS) $(CFLAGS) -c $< -o $@
	
$(D_B)/$(LIBPREFIX)$(LIBNAME).$(LIBSUFFIX): ${OBJS} $(UDPOBJS)
	$(AR) rcs $@ $^
	
# TUTILS	
PATH_PREFIX=Tutils
NAME_PREFIX=Tutils
LIBNAME=tutils
OBJS=$(patsubst $(PATH_PREFIX)/%.cpp,$(D_O)/$(NAME_PREFIX)%.o,$(wildcard $(PATH_PREFIX)/*.cpp))
$(D_O)/$(NAME_PREFIX)%.o: $(PATH_PREFIX)/%.cpp $(PATH_PREFIX)/%.h
	$(CXX) $(INCFLAGS) $(CFLAGS) -c $< -o $@
$(D_B)/$(LIBPREFIX)$(LIBNAME).$(LIBSUFFIX): ${OBJS}
	$(AR) rcs $@ $^
# EXAMPLES
$(D_B)/tmysqlExample: Examples/Tmysql.cpp $(D_B)/$(LIBPREFIX)tlogger.$(LIBSUFFIX) $(D_B)/$(LIBPREFIX)tmysql.$(LIBSUFFIX) $(D_B)/$(LIBPREFIX)tutils.$(LIBSUFFIX)
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -L$(D_B) -ltmysql -ltlogger -ltutils -lmysqlclient_r -lm -L/usr/lib64/mysql $(filter %.o,$^) -o $@

all: $(MAINTARGETS) $(OSTARGETS)

clean:
	${RM} $(D_B)/*
	${RM} $(D_O)/*
