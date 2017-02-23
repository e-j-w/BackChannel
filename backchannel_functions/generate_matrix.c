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
	printf("Generating matrix of weight values...\n");
	
	int i,j,k,l;
	
	for(i=0;i<p->numGateData;i++)
		for(j=0;j<p->numGateData;j++)
			{
				p->weights[i][j]=1.;
				for(k=0;k<p->numParticles;k++)
					{
						for(l=0;l<p->gateData[j].gateCh[k];l++)
							p->weights[i][j]=p->weights[i][j]*p->eff[k];
						for(l=0;l<(p->gateData[i].gateCh[k] - p->gateData[j].gateCh[k]);l++)
							p->weights[i][j]=p->weights[i][j]*(1.- p->eff[k]);
						p->weights[i][j]=p->weights[i][j]*choose(p->gateData[i].gateCh[k],p->gateData[j].gateCh[k]);
					}
			}

}

void computeInvWeights(bc_par * p)
{
	printf("Inverting...");

	int i,j;
	
	lin_eq_type *li=(lin_eq_type*)calloc(1,sizeof(lin_eq_type));
	li->dim=p->numGateData;
	for(i=0;i<p->numGateData;i++)
		{
			li->vector[i]=1.;
			for(j=0;j<p->numParticles;j++)
				if(p->gateData[i].gateCh[j]!=p->sortCh[j])
					{
						li->vector[i]=0.;
						break;
					}
		}
	for(i=0;i<p->numGateData;i++)
		for(j=0;j<p->numGateData;j++)
			li->matrix[i][j]=p->weights[i][j];
	
	if(!(solve_lin_eq(li)==1))
		{
			printf("\nERROR: cannot solve system of equations!\n");
			exit(-1);
		}
		
	for(i=0;i<p->numGateData;i++)	
		p->soln[i]=(double)li->solution[i];
	
	
	
	for(i=0;i<p->numGateData;i++)
		for(j=0;j<p->numGateData;j++)
			p->invweights[i][j]=li->inv_matrix[i][j];
	
	free(li);
	
	printf(" done!\n");
	
}

void printWeights(bc_par * p)
{
	int i,j;
	
	printf("MATRIX\n");
	for(i=0;i<p->numGateData;i++)
		{
			printf("CHANNEL= ");
			for(j=0;j<p->numParticles;j++)
				printf("%i ",p->gateData[i].gateCh[j]);
			printf("  MATRIX= ");
			for(j=0;j<p->numGateData;j++)
				printf("%7.3f ",p->weights[i][j]);
			printf("  GATE= ");
			for(j=0;j<p->numParticles;j++)
				printf("%i ",p->gateData[i].gateCh[j]);
			printf("\n");
		}
}

void printInvWeights(bc_par * p)
{
	int i,j;
	
	printf("INV MATRIX\n");
	for(i=0;i<p->numGateData;i++)
		{
			printf("CHANNEL= ");
			for(j=0;j<p->numParticles;j++)
				printf("%i ",p->gateData[i].gateCh[j]);
			printf("  MATRIX= ");
			for(j=0;j<p->numGateData;j++)
				printf("%7.3f ",p->invweights[i][j]);
			printf("  GATE= ");
			for(j=0;j<p->numParticles;j++)
				printf("%i ",p->gateData[i].gateCh[j]);
			printf("\n");
		}
	
	printf("SOLUTION\n");
	for(i=0;i<p->numGateData;i++)
		printf("%f\n",p->soln[i]);
}
