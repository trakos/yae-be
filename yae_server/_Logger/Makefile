D_O = ../build/obj
D_B = ../build/bin
OBJS=$(patsubst %.cpp,$(D_O)/%.o,$(wildcard *.cpp))
LIB=Logger
CFLAGS=-g -Wl,--enable-auto-import -D debug
LINUX_TARGET=
TARGET=$(D_B)/${LIB}.dll

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
	TARGET=$(D_B)/lib${LIB}.so
	CXX=gcc44
	CFLAGS=-g -D debug -fPIC
endif

all: $(TARGET)
	
$(TARGET): ${OBJS}
	${CXX} $(CFLAGS) -shared ${OBJS} -o $@

$(D_O)/%.o: %.cpp %.h
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	${RM} ${OBJS}

