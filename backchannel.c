//definitions
#include "backchannel.h"
//functions and logic
#include "read_data.c"
#include "write_data.c"
#include "generate_matrix.c"

int main(int argc, char *argv[])
{

  if(argc!=2)
    {
      printf("\nbackchannel parameter_file\n--------------------------\n\n");
      printf("Generates a spectrum corresponding to a single channel out of spectra corresponding to different gates.\n\n");
      exit(-1);
    }
  
  //allocate structures
  bc_par *p=(bc_par*)malloc(sizeof(bc_par));
  readParFile(argv[1],p);
  computeWeights(p);
  printWeights(p);
  computeInvWeights(p);
  printInvWeights(p);
  
  int i,j,k;
  for(i=0;i<p->numSpectra;i++)
  	for(j=0;j<S32K;j++)
			{
				p->outHist[i][j]=0.;
				for(k=0;k<p->numGateData;k++)
					p->outHist[i][j]+=p->soln[k]*p->gateData[k].hist[i][j];
			}
	
	writeDataFile(p);

  return 0; //great success
}
