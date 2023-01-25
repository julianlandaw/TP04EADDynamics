OBJECT1 = apdbifurcationmain.cpp
OBJECT2 = apmain.cpp
EXECUTABLE1 = bif.out
EXECUTABLE2 = ap.out
CC = g++
FLAGS = -std=c++11

VARIABLE = nacafac

gnaped = 0
alphafcass = 0

NUMPCL = 1
NUMVAR = 1
BEATS = 2000
REMOVEBEATS = 1500
PRECTYPE = double

MACROS = -D NUMPCL=${NUMPCL} -D NUMVAR=${NUMVAR} -D BEATS=${BEATS} -D REMOVEBEATS=${REMOVEBEATS} -D PRECTYPE=${PRECTYPE}

DV_MAX = 0.1

default: all

all: TTbif TTap

TTbif:
	$(CC) $(FLAGS) $(OBJECT1) -o TT$(EXECUTABLE1) $(MACROS) -D TT -D VARIABLE=${VARIABLE} -D xiaodong -D gnaped=${gnaped} -D alphafcass=${alphafcass} -D stimulus=-26.0 -D stimduration=2.0 -D UCLAito -D EPI
    
TTap:
	$(CC) $(FLAGS) $(OBJECT2) -o TT$(EXECUTABLE2) $(MACROS) -D TT -D VARIABLE=${VARIABLE} -D xiaodong -D gnaped=${gnaped} -D alphafcass=${alphafcass} -D stimulus=-26.0 -D stimduration=2.0 -D UCLAIto -D EPI

clean:
	-rm -f *$(EXECUTABLE)

