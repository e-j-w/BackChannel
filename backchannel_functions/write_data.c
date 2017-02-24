//function writes an .mca file to disk
void writeMCA(FILE * out, const char * filename, const int numSpec, double outHist[NSPECT][S32K])
{
	int i,j;
	int tmpHist[S32K];

	for (i=0;i<numSpec;i++)
		{
			for(j=0;j<S32K;j++)
				tmpHist[j]=(int)outHist[i][j];//truncate to integer
				
			if(fwrite(tmpHist,S32K*sizeof(int),1,out)!=1)
				{
					printf("ERROR: Cannot write spectrum %i to the .mca file: %s\n",i,filename);
					exit(-1);
				}
		}
  
}

void writeDataFile(bc_par * p)
{
	FILE *out;
	if((out=fopen(p->outFilename,"w"))==NULL)
    {
      printf("ERROR: Cannot open the output file: %s\n",p->outFilename);
      exit(-1);
    }
	
	const char *dot = strrchr(p->outFilename, '.');//get the file extension
	if(strcmp(dot + 1,"mca")==0)
		writeMCA(out, p->outFilename, p->numSpectra, p->outHist);
	else
		{
			printf("ERROR: Improper extension for output file: %s\n",p->outFilename);
      printf("Integer array (.mca) files are supported.\n");
      exit(-1);
		}
	
	printf("Output data written to file: %s\n",p->outFilename);
	
	fclose(out);
}
