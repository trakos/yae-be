D_O = build/obj
D_B = build/bin

LIBFLAGS=
INCFLAGS=-IK:\\Program\ Files\\Boost-1.41.0\\include\\boost-1_41\\ -I.
CFLAGS=-std=c++0x -g -Wl,--enable-auto-import
TARGETS=$(D_B)/etclient.exe

all: $(TARGETS)

clean:
	$(RM) $(TARGETS)
	$(RM) $(D_O)/*.o

$(D_B)/etclient.exe: main.cpp $(D_O)/ETClientConsole.o $(D_O)/ETClientInput.o $(D_O)/ETClientListener.o $(D_O)/utils.o $(D_O)/indent.o
	$(CXX) $(CFLAGS) $(INCFLAGS) $(LIBFLAGS) $< $(filter %.o,$^) -o $@

$(D_O)/utils.o: utils.cpp utils.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/indent.o: indent.cpp indent.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/ETClientConsole.o: ETClient/Console.cpp ETClient/Console.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@

$(D_O)/ETClientInput.o: ETClient/Input.cpp ETClient/Input.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
	
$(D_O)/ETClientListener.o: ETClient/Listener.cpp ETClient/Listener.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@