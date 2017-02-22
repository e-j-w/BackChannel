#include "generate_matrix.h"

double choose(int a, int b)
{
	if(b>a)
		return 0.;
		
	int i;
	double num=1.;
	double denom=1.;
	
	for(i=a;i>0;i--)
		num=num*1.0*i;
	for(i=b;i>0;i--)
		denom=denom*1.0*i;
	for(i=(a-b);i>0;i--)
		denom=denom*1.0*i;
	
	return num/denom;
	
}

//calculate the weights used in the matrix transforming gate data to channel data
void computeWeights(bc_par * p)
{
	int i,j,k,l;
	
	for(i=0;i<p->numGateData;i++)
		for(j=0;j<p->numGateData;j++)
			for(k=0;k<p->numParticles;k++)
				{
					p->weights[i][j]=1.;
					for(l=0;l<p->gateData[i].gateCh[k];l++)
						p->weights[i][j]=p->weights[i][j]*p->eff[k];
					for(l=0;l<(p->gateData[j].gateCh[k] - p->gateData[i].gateCh[k]);l++)
						p->weights[i][j]=p->weights[i][j]*(1.- p->eff[k]);
					p->weights[i][j]=p->weights[i][j]*choose(p->gateData[j].gateCh[k],p->gateData[i].gateCh[k]);
				}
	
	/*for(i=0;i<gateP;i++)
		PEffFactor=PEffFactor*PEff;
	for(i=0;i<(chP-gateP);i++)
		PEffFactor=PEffFactor*PEff;
	for(i=0;i<gateA;i++)
		AEffFactor=AEffFactor*AEff;
	for(i=0;i<(chA-gateA);i++)
		AEffFactor=AEffFactor*AEff;
	return choose(chP,gateP)*PEffFactor*choose(chA,gateA)*AEffFactor;*/

}

void printWeights(bc_par * p)
{
	int i,j;
	
	printf("CHANNEL  MATRIX                                       GATE\n");
	for(i=0;i<p->numGateData;i++)
		{
			for(j=0;j<p->numParticles;j++)
				printf("%i ",p->gateData[i].gateCh[j]);
			printf("    ");
			for(j=0;j<p->numGateData;j++)
				printf("%6.3f ",p->weights[i][j]);
			printf("    ");
			for(j=0;j<p->numParticles;j++)
				printf("%i ",p->gateData[i].gateCh[j]);
			printf("\n");
		}
}


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
					readDataFile(opt[i]->par[opt[i]->numPar-1], 1, p->gateData[p->numGateData].hist);//only one spectrum specified...
					p->numGateData++;
				}
		}
	
	printf("Parameters read from file: %s\n",fileName);
	printf("Number of particle types: %i\n",p->numParticles);
	printf("Number of gate spectra: %i\n",p->numGateData);
  
}
