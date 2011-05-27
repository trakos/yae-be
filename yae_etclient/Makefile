D_O = build/obj
D_B = build/bin
D_L = ../yae_libraries/build/bin

LIBFLAGS=-L$(D_L) -letstatus -letclient -ltlogger -ltnet -ltutils -lws2_32 -lgdi32
INCFLAGS=-IK:\\Program\ Files\\Boost-1.41.0\\include\\boost-1_41\\ -I. -I../yae_libraries/
CFLAGS=-std=c++0x -g -Wl,--enable-auto-import -gstabs
TARGETS=$(D_B)/etclient.exe

all: $(TARGETS) 

clean:
	$(RM) $(TARGETS)
	$(RM) $(D_O)/*.o

$(D_B)/etclient.exe: main.cpp $(wildcard $(D_L)/*) $(D_O)/WindowAuthDialog.o $(D_O)/CommunicationYaeAuthorization.o $(D_O)/CommunicationYaeMaster.o $(D_O)/ETClient.o
	$(CXX) $(CFLAGS) $(INCFLAGS) $< $(filter %.o,$^) $(LIBFLAGS) -o $@ 

$(D_O)/ETClient.o: ETClient.cpp ETClient.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@

$(D_O)/CommunicationYaeAuthorization.o: Communication/Yae/Authorization.cpp Communication/Yae/Authorization.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@

$(D_O)/CommunicationYaeMaster.o: Communication/Yae/Master.cpp Communication/Yae/Master.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/WindowAuthDialog.o: Window/AuthDialog.cpp Window/AuthDialog.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
		
_force_: