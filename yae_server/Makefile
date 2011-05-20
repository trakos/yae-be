D_O = build/obj
D_B = build/bin

CXX=gcc44
LIBFLAGS=-lmysqlclient_r -lm -lgmpxx -lpthread -L/usr/lib64/mysql -L../yae_libraries/build/bin -ltlogger -ltmysql -ltnet -lindent -lutils -letclient
INCFLAGS=-I../yae_libraries -I.
CFLAGS=-g -std=c++0x
TARGETS=$(D_B)/yaeServer

all: $(TARGETS) 

clean:
	$(RM) $(TARGETS)
	$(RM) $(D_O)/*.o

$(D_B)/yaeServer: main.cpp $(D_O)/YaeMaster.o $(D_O)/YaeMasterDatabase.o $(D_O)/YaeMasterException.o $(D_O)/YaeMasterProtocol.o $(D_O)/base64.o
	$(CXX) $(CFLAGS) $(INCFLAGS) $< $(filter %.o,$^) $(LIBFLAGS) -o $@ 

$(D_O)/YaeMaster.o: YaeMaster.cpp YaeMaster.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/YaeMasterDatabase.o: YaeMaster/Database.cpp YaeMaster/Database.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/YaeMasterException.o: YaeMaster/Exception.cpp YaeMaster/Exception.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/YaeMasterProtocol.o: YaeMaster/Protocol.cpp YaeMaster/Protocol.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/base64.o: base64.cpp base64.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
		
_force_: