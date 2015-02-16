// calculate the phase shift between template and simulated (or real) data 
// error of phase can be calculated
// initial guess of phase shift is added
// try to do two-dim template matching
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "periodSSBLib.h"
//#include "T2toolkit.h"
//#include "tempo2pred.h"

int main (int argc, char *argv[])
{
	double segLength = 1800.0;
	int nfreq = 32;
	int ntime = 16;

	char fname[128];
	char eph[128];

	int i,k,ret;
	double batFreq;

	T2Predictor pred;

	pheader *header;
	header = (pheader *)malloc(sizeof(pheader));

	fitsfile *fp;

	//////////////////////////////////////////////////////

	int index, n;
	for (i = 0; i < argc; i++)
	{
		if (strcmp(argv[i],"-f") == 0)
		{
			index = i + 1;
			n = 0;
			while ((index + n) < argc && strcmp(argv[index+n],"-e") != 0)
			{
				n++;
			}
		}
		else if (strcmp(argv[i],"-e") == 0)
		{
			strcpy(eph,argv[++i]);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////
	// start to deal with different data file
	for (k = index; k < index + n; k++)
	{
		// get the data file name
		strcpy(fname,argv[k]);
		printf ("%s\n", fname);

		// open psrfits file
		fp = openFitsFile(fname);

		// read header info
		loadPrimaryHeader(fp,header);
		////////////////////////////////////////////////////
    // Create predictor
		
		T2Predictor_Init(&pred);
		runTempo2(header,segLength,nfreq,ntime,eph);
		if (ret=T2Predictor_Read(&pred,(char *)"t2pred.dat"))
		{
			printf("Error: unable to read predictor\n");
			exit(1);
		}
		      
		batFreq = calculateBatPeriod(header,pred);
		printf ("batFreq %.10lf\n", batFreq);
		writeChannels(fp, batFreq);

		closeFitsFile(fp);
	}

	free(header);
	
	return 0;
}

