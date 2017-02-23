//function writes an .mca file to disk
void writeMCA(FILE * out, const char * filename, const int numSpec, double outHist[NSPECT][S32K])
{
	int i,j;
	int tmpHist[S32K];

	for (i=0;i<numSpec;i++)
		{
			for(j=0;j<S32K;j++)
				{
					tmpHist[j]=(int)outHist[i][j];//truncate to integer
				}
				
			if(fwrite(tmpHist,S32K*sizeof(int),1,out)!=1)
				{
					printf("ERROR: Cannot write spectrum %i to the .mca file: %s\n",i,filename);
					exit(-1);
				}
		}
  
}

void writeDataFile(const char * filename, bc_par * p)
{
	FILE *out;
	if((out=fopen(filename,"w"))==NULL)
    {
      printf("ERROR: Cannot open the output file: %s\n",filename);
      exit(-1);
    }
	
	const char *dot = strrchr(filename, '.');//get the file extension
	if(strcmp(dot + 1,"mca")==0)
		writeMCA(out, filename, 1, p->gateData[0].hist);
	else
		{
			printf("ERROR: Improper extension for output file: %s\n",filename);
      printf("Integer array (.mca) files are supported.\n");
      exit(-1);
		}
	
	fclose(out);
}
