#!/bin/sh

gcc -lm -L/usr/local/lib/cfitsio -I/usr/include/cfitsio/ -lcfitsio periodSSB.c periodSSBLib.c readPfits.c tempo2pred.c cheby2d.c t1polyco.c -o periodSSB 

