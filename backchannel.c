//definitions
#include "backchannel.h"
//functions and logic
#include "read_data.c"
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
	
	/*char fileName[256],str[8];
	int i;
	
	for(i=1;i<200;i++)
		{
			strcpy(fileName,"");
			strcat(fileName,getenv("ENSDF"));
			if(i<10)
				strcat(fileName,"ensdf.00");
			else if(i<100)
				strcat(fileName,"ensdf.0");
			else
				strcat(fileName,"ensdf.");
			sprintf(str,"%i",i);
			strcat(fileName,str);
			readENSDFFile(fileName,gd); //grab data from the ENSDF file (see parse_ENSDF.c)
		}*/

  return 0; //great success
}