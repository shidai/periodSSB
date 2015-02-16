#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "fitsio.h"
#include "readPfits.h"
#include "tempo2pred.h"

void writeChannels(fitsfile *fptr, double batFreq);
void runTempo2(pheader *header, double segLength, int nfreq, int ntime, char *eph);
double calculateBatPeriod(pheader *header, T2Predictor pred);

