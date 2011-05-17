D_O = build/obj
D_B = build/bin

CXX=gcc44
DEPS_TLIB=tlib/array/array.cpp tlib/array/array.h
LIBFLAGS=-lmysqlclient_r -lm -lgmpxx -lpthread -L/usr/lib64/mysql
INCFLAGS=-Itlib
CFLAGS=-g
TARGETS=$(D_B)/yae $(D_B)/tmysql_example $(D_B)/testrcon $(D_B)/testip $(D_B)/enc $(D_B)/testerrors

# main definitions
all: $(TARGETS)
clean:
	$(RM) $(TARGETS)

$(D_B)/tmysql_example: tmysql/example.cpp $(D_O)/tmysql.o
	$(CXX) $(CFLAGS) $(INCFLAGS) $(LIBFLAGS) $< $(filter %.o,$^) -o $@
$(D_B)/yae: main.cpp $(DEPS_TLIB) $(D_O)/tmysql.o $(D_O)/yae.o $(D_O)/rcon.o $(D_O)/dec.o 
	$(CXX) $(CFLAGS) $(INCFLAGS) $(LIBFLAGS) $< $(filter %.o,$^) -o $@
$(D_B)/testrcon: testrcon.cpp $(D_O)/rcon.o
	$(CXX) $(CFLAGS) $(INCFLAGS) $(LIBFLAGS) $< $(filter %.o,$^) -o $@
$(D_B)/testip: testip.cpp $(DEPS_TLIB)
	$(CXX) $(CFLAGS) $(INCFLAGS) $(LIBFLAGS) $< $(filter %.o,$^) -o $@
$(D_B)/testerrors: testerrors.cpp rcon.cpp $(DEPS_TLIB) $(D_O)/tmysql.o $(D_O)/yae.o $(D_O)/dec.o
	$(CXX) $(CFLAGS) $(INCFLAGS) $(LIBFLAGS) $< $(filter %.o,$^) -o $@
$(D_B)/enc: enc.cpp
	$(CXX) $(CFLAGS) $(INCFLAGS) $(LIBFLAGS) $< $(filter %.o,$^) -o $@

$(D_O)/tmysql.o: tmysql/tmysql.cpp
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
$(D_O)/rcon.o: rcon.cpp rcon.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
$(D_O)/yae.o: yae.cpp yae.h dec.h $(DEPS_TLIB)
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
$(D_O)/dec.o: dec.cpp dec.h
	$(CXX) $(CFLAGS) $(INCFLAGS) $< -c -o $@
