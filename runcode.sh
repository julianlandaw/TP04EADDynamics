#!/bin/sh

make BEATS=1500 REMOVEBEATS=2000 NUMVAR=9 NUMPCL=1 VARIABLE=nacafac
./TTbif.out 1 5 2500 2500 0.05 
./TTap.out 1 2500 0.05 
./TTap.out 5 2500 0.05