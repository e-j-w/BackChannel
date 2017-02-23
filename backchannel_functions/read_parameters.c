#include "read_parameters.h"

//parses a string into an option
par_option * readOption(char * str)
{
	par_option *opt=(par_option*)calloc(1,sizeof(par_option));
	char *tok;
	tok=strtok (str," ,");
	strcpy(opt->name,tok);
	int i=0;
	while (tok != NULL)
	{
		tok = strtok (NULL," ,");
		if(tok!=NULL)
			{
				strcpy(opt->par[i],tok);
				i++;
				if(i>=MAX_OPT_PAR)
					{
						printf("WARNING: too many parameters specified in parameter file option: %s\n",opt->name);
						break;
					}
			}
	}
	opt->numPar=i;
	return (par_option*)opt;
}


//function reads parameter files for the topspek code
void readParFile(const char * fileName, bc_par * p) 
{
  FILE *config;
  char str[256],str1[256];
  int i,j;
  par_option *opt[MAX_NUM_OPT];
  for(i=0;i<MAX_NUM_OPT;i++)
  	opt[i]=(par_option*)malloc(sizeof(par_option));
  int numOpt=0;
  p->numGateData=0;
  p->numParticles=0;
  
  //open the file and read all parameters
  if((config=fopen(fileName,"r"))==NULL)
    {
      printf("ERROR: Cannot open the parameter file %s!\n",fileName);
      exit(-1);
    }
  while(!(feof(config)))//go until the end of file is reached
    {
			if(fgets(str,256,config)!=NULL)
				{
					sscanf(str,"%s",str1);
					if(strcmp(str1,"<---END_OF_PARAMETERS--->")==0)
          	break;
        	opt[numOpt]=readOption(str);
        	numOpt++;
        	if(numOpt>=MAX_NUM_OPT)
        		{
        			printf("ERROR: The maximum number of options (%i) in the parameter file %s has been exceeded!\nIncrease the value of MAX_NUM_OPT in read_parameters.h and recompile.\n",MAX_NUM_OPT,fileName);
							exit(-1);
        		}
				}
		}
	fclose(config);
	
	//parse the parameters which were just read in
	for(i=0;i<numOpt;i++)
		{
			if(strcmp(opt[i]->name,"SORT_CH")==0)
				{
					for(j=0;j<opt[i]->numPar;j++)
						if(j<MAXNUMPARTICLES)
							p->sortCh[j]=atoi(opt[i]->par[j]);
				}
			else if(strcmp(opt[i]->name,"EFF")==0)
				{
					for(j=0;j<opt[i]->numPar;j++)
						if(j<MAXNUMPARTICLES)
							p->eff[j]=atof(opt[i]->par[j]);
				}
			else if(strcmp(opt[i]->name,"GATE_SP")==0)
				{
					for(j=0;j<opt[i]->numPar-1;j++)
						if(j<MAXNUMPARTICLES)
							p->gateData[p->numGateData].gateCh[j]=atoi(opt[i]->par[j]);
					if((opt[i]->numPar-1)>p->numParticles)
						p->numParticles=opt[i]->numPar-1;
					opt[i]->par[opt[i]->numPar-1][strcspn(opt[i]->par[opt[i]->numPar-1], "\r\n")] = 0;//strips newline characters from the filename
					p->numSpectra=readDataFile(opt[i]->par[opt[i]->numPar-1], p->gateData[p->numGateData].hist);
					p->numGateData++;
				}
			else if(strcmp(opt[i]->name,"OUT_SP")==0)
				{
					opt[i]->par[0][strcspn(opt[i]->par[0], "\r\n")] = 0;//strips newline characters from the filename
					p->outFilename=opt[i]->par[0];
				}
		}
	
	printf("Parameters read from file: %s\n",fileName);
	printf("Number of particle types: %i\n",p->numParticles);
	printf("Number of gate spectra: %i\n",p->numGateData);
	printf("Will write output data to file: %s\n",p->outFilename);
  
}
