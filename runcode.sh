#!/bin/sh

make BEATS=2000 REMOVEBEATS=1500 NUMVAR=9 NUMPCL=1 VARIABLE=nacafac
./TTbif.out 1 5 2500 2500 0.05 
./TTap.out 1 2500 0.05 
./TTap.out 5 2500 0.05

if [ ! -d "results" ] 
then
	mkdir results
	mv TT*.txt results
else
	mv TT*.txt results
fi
	
