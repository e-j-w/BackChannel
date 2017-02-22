#ifndef BC_H
#define BC_H

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//.mca format
#define S32K        32768
#define NSPECT      100

#define MAXNUMPARTICLES 10
#define MAXNUMGATESP 50

//structures
typedef struct
{
  int gateCh[MAXNUMPARTICLES]; //number of particles in the gate
  double hist[NSPECT][S32K]; //gated spectrum data
}gate_sp; //gate spectrum data

typedef struct
{
	int numParticles;
  int sortCh[MAXNUMPARTICLES]; //number of particles in the channel to be sorted
  double eff[MAXNUMPARTICLES]; //particle detection efficiencies
  int numGateData;
  gate_sp gateData[MAXNUMGATESP];
  double weights[MAXNUMGATESP][MAXNUMGATESP];
}bc_par; //parameters

#endif
