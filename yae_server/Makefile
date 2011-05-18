D_O = build/obj
D_B = build/bin

CXX=gcc44
DEPS_TLIB=tlib/array/array.cpp tlib/array/array.h
LIBFLAGS=-lmysqlclient_r -lm -lgmpxx -lpthread -L/usr/lib64/mysql
INCFLAGS=-Itlib -I.
CFLAGS=-g -std=c++0x
TARGETS=$(D_B)/yaeServer $(D_B)/TmysqlExample

# main definitions
all: $(TARGETS)
clean:
	$(RM) $(TARGETS)
	$(RM) $(D_O)/*.o
	
# object files categories

TMYSQL=$(D_O)/TmysqlConnection.o $(D_O)/TmysqlException.o $(D_O)/TmysqlLiveRow.o $(D_O)/TmysqlString.o
TNET=$(D_O)/TnetServer.o $(D_O)/TnetConnection.o $(D_O)/TnetClient.o $(D_O)/TnetMessage.o
	
$(D_B)/yaeServer: main.cpp $(D_O)/utils.o $(D_O)/indent.o $(TMYSQL) $(TNET)
	$(CXX) $(CFLAGS) $(INCFLAGS) $(LIBFLAGS) $< $(filter %.o,$^) -o $@
	
$(D_B)/TmysqlExample: Tmysql/Example.cpp $(TMYSQL) $(D_O)/utils.o $(D_O)/indent.o  
	$(CXX) $(CFLAGS) $(INCFLAGS) $(LIBFLAGS) $< $(filter %.o,$^) -o $@

$(D_O)/TmysqlConnection.o: Tmysql/Connection.cpp Tmysql/Connection.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/TmysqlException.o: Tmysql/Exception.cpp Tmysql/Exception.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/TmysqlLiveRow.o: Tmysql/LiveRow.cpp Tmysql/LiveRow.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/TmysqlString.o: Tmysql/String.cpp Tmysql/String.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
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
