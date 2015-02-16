#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "periodSSBLib.h"

void writeChannels(fitsfile *fptr, double batFreq)
{
  int status=0;
  int colnum;

  fits_movnam_hdu(fptr,BINARY_TBL,(char *)"SUBINT",0,&status);
  if (status) {fits_report_error(stdout,status); exit(1);}
  
	int ncol;
	fits_get_num_cols(fptr,&ncol,&status);

	fits_insert_col(fptr,ncol+1,"BATFREQ",(char *)"D",&status);
  
	fits_get_colnum(fptr,CASEINSEN,"BATFREQ",&colnum,&status);
	fits_write_col(fptr,TDOUBLE,colnum,1,1,1,&batFreq,&status);
}

void runTempo2(pheader *header, double segLength, int nfreq, int ntime, char *eph)
{
  char execString[1024];
  double seg_length = segLength;
  int nfreqcoeff = nfreq;
  int ntimecoeff = ntime;
  //printf("nfreqCoeff: %d\n", nfreqcoeff);

  double freq1 = header->freq - fabs(header->bw); 
  double freq2 = header->freq + fabs(header->bw);
  long double mjd1 = header->imjd + header->smjd/86400.0L - 60*60/86400.0L; // MUST FIX
  long double mjd2 = header->imjd + header->smjd/86400.0L + (header->nsub)*60.0/86400.0L + 60*60/86400.0L; // MUST FIX

	
	sprintf(execString,"tempo2 -pred \"%s %Lf %Lf %g %g %d %d %g\" -f %s","BAT",mjd1,mjd2,freq1,freq2,ntimecoeff,nfreqcoeff,seg_length,eph);
 
	printf("Running tempo2 to get predictor\n");

  system(execString);
  printf("Complete running tempo2\n");
}

double calculateBatPeriod(pheader *header, T2Predictor pred)
{
  long double mjd0;
  long double freq;
  long double toff;
	double batFreq;

	//header->tSubint = 3840.0;
	printf ("%lf\n", header->tSubint);
  freq = header->freq;
  toff = header->tSubint/2.0;
  mjd0 = header->imjd + (header->smjd + header->stt_offs)/86400.0L + (toff)/86400.0L; // Centre of subint
	//printf ("%Lf %Lf %Lf\n", freq, toff, mjd0);
  batFreq = T2Predictor_GetFrequency(&pred,mjd0,freq);

	return batFreq;
}

