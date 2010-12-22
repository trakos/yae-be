D_O = ../build/obj
D_B = ../build/bin
OBJS=$(patsubst %.cpp,$(D_O)/%.o,$(wildcard *.cpp))
LIB=Logger
CFLAGS=-g -Wl,--enable-auto-import -D debug


all: $(D_B)/${LIB}.dll
	
$(D_B)/${LIB}.dll: ${OBJS}
	${CXX} $(CFLAGS) -shared ${OBJS} -o $(D_B)/${LIB}.dll

$(D_O)/%.o: %.cpp %.h
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	${RM} ${OBJS}

