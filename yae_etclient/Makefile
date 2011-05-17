D_O = build/obj
D_B = build/bin

LIBFLAGS=-L$(D_B) -lLogger -lws2_32 -lgdi32
INCFLAGS=-IK:\\Program\ Files\\Boost-1.41.0\\include\\boost-1_41\\ -I.
CFLAGS=-std=c++0x -g -Wl,--enable-auto-import
TARGETS=$(D_B)/Logger.dll $(D_B)/etclient.exe

all: $(TARGETS) 

clean:
	$(RM) $(TARGETS)
	$(RM) $(D_O)/*.o

$(D_B)/Logger.dll: _force_
	cd Logger && make all

$(D_B)/etclient.exe: main.cpp $(D_O)/ETClientConsole.o $(D_O)/ETClientInput.o $(D_O)/ETClientListener.o $(D_O)/ETClientInfo.o $(D_O)/utils.o $(D_O)/indent.o $(D_O)/WindowAuthDialog.o $(D_O)/TNetServer.o $(D_O)/TNetConnection.o $(D_O)/TNetClient.o $(D_O)/TNetMessage.o $(D_O)/CommunicationYaeAuthorization.o $(D_O)/CommunicationYaeServer.o $(D_O)/ETClient.o
	$(CXX) $(CFLAGS) $(INCFLAGS) $< $(filter %.o,$^) $(LIBFLAGS) -o $@ 

$(D_O)/utils.o: utils.cpp utils.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/indent.o: indent.cpp indent.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/ETClient.o: ETClient.cpp ETClient.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@

$(D_O)/CommunicationYaeAuthorization.o: Communication/Yae/Authorization.cpp Communication/Yae/Authorization.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@

$(D_O)/CommunicationYaeServer.o: Communication/Yae/Server.cpp Communication/Yae/Server.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/WindowAuthDialog.o: Window/AuthDialog.cpp Window/AuthDialog.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
		
$(D_O)/ETClientConsole.o: ETClient/Console.cpp ETClient/Console.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@

$(D_O)/ETClientInput.o: ETClient/Input.cpp ETClient/Input.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/ETClientListener.o: ETClient/Listener.cpp ETClient/Listener.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/ETClientInfo.o: ETClient/Info.cpp ETClient/Info.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/TNetServer.o: Tnet/Server.cpp Tnet/Server.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/TNetClient.o: Tnet/Client.cpp Tnet/Client.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/TNetConnection.o: Tnet/Connection.cpp Tnet/Connection.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@	
	
$(D_O)/TNetMessage.o: Tnet/Message.cpp Tnet/Message.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
_force_: