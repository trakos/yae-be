D_O = build/obj
D_B = build/bin

CXX=gcc44
LIBFLAGS=-lmysqlclient_r -lm -lgmpxx -lpthread -L/usr/lib64/mysql -L$(D_B) -lLogger
INCFLAGS=-Itlib -I.
CFLAGS=-g -std=c++0x
TARGETS=$(D_B)/yaeServer $(D_B)/TmysqlExample

# main definitions
all: $(TARGETS)
clean:
	$(RM) $(TARGETS)
	$(RM) $(D_O)/*.o

$(D_B)/libLogger.so: _force_
	cd Logger && make all
	
# object files categories

TMYSQL=$(D_O)/TmysqlConnection.o $(D_O)/TmysqlException.o $(D_O)/TmysqlLiveRow.o $(D_O)/TmysqlString.o
TNET=$(D_O)/TnetServer.o $(D_O)/TnetConnection.o $(D_O)/TnetClient.o $(D_O)/TnetMessage.o
ETCLIENT=$(D_O)/ETClientStatus.o
YAEMASTER=$(D_O)/YaeMaster.o $(D_O)/YaeMasterDatabase.o $(D_O)/YaeMasterException.o $(D_O)/YaeMasterProtocol.o
	
$(D_B)/yaeServer: main.cpp $(D_O)/utils.o $(D_O)/indent.o $(TMYSQL) $(TNET) $(ETCLIENT) $(YAEMASTER)
	$(CXX) $(CFLAGS) $(INCFLAGS) $(LIBFLAGS) $< $(filter %.o,$^) -o $@
	
#TMYSQL

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

#TNET
	
$(D_O)/TnetServer.o: Tnet/Server.cpp Tnet/Server.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/TnetClient.o: Tnet/Client.cpp Tnet/Client.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/TnetConnection.o: Tnet/Connection.cpp Tnet/Connection.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@	
	
$(D_O)/TnetMessage.o: Tnet/Message.cpp Tnet/Message.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/ETClientStatus.o: ETClient/Status.cpp ETClient/Status.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
#YAEMASTER
	
$(D_O)/YaeMaster.o: YaeMaster.cpp YaeMaster.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/YaeMasterDatabase.o: YaeMaster/Database.cpp YaeMaster/Database.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/YaeMasterException.o: YaeMaster/Exception.cpp YaeMaster/Exception.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/YaeMasterProtocol.o: YaeMaster/Protocol.cpp YaeMaster/Protocol.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@

#OTHERS

$(D_O)/utils.o: utils.cpp utils.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/indent.o: indent.cpp indent.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@

_force_: