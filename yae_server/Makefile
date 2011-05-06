D_O = build/obj
D_B = build/bin

CXX=gcc44
DEPS_TLIB=tlib/array/array.cpp tlib/array/array.h
LIBFLAGS=-lmysqlclient_r -lm -lgmpxx -lpthread -L/usr/lib64/mysql
INCFLAGS=-Itlib -I.
CFLAGS=-g
TARGETS=$(D_B)/yaeServer

# main definitions
all: $(TARGETS)
clean:
	$(RM) $(TARGETS)
	
$(D_B)/yaeServer: yaeServer.cpp $(D_O)/TnetServer.o $(D_O)/TnetConnection.o $(D_O)/TnetClient.o $(D_O)/TnetMessage.o $(D_O)/utils.o $(D_O)/indent.o
	$(CXX) $(CFLAGS) $(INCFLAGS) $(LIBFLAGS) $< $(filter %.o,$^) -o $@
	
$(D_O)/TnetServer.o: Tnet/Server.cpp Tnet/Server.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/TnetClient.o: Tnet/Client.cpp Tnet/Client.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/TnetConnection.o: Tnet/Connection.cpp Tnet/Connection.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@	
	
$(D_O)/TnetMessage.o: Tnet/Message.cpp Tnet/Message.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@

$(D_O)/utils.o: utils.cpp utils.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/indent.o: indent.cpp indent.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
