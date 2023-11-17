#include <stdio.h>
#include <stdlib.h>

#define LINESIZE 1024

int main(int argc, char *argv[])
{
	FILE *in,*out;
	float BPM,pixelI;
	unsigned char headOffset;
	int startTime,tI,bl;
	char line[LINESIZE];
	int onbeat;
	
	unsigned int beatCount,spaceCount;
	
	if(argc!=3)
	{
		printf("Incorrect number of parameters\n");
		return 1;
	}
		
	in=fopen(argv[1],"rt");
	out=fopen(argv[2],"wt");	
	
	fgets(line,LINESIZE,in);//BPM 	
	fprintf(out,"%s",line);
	fgets(line,LINESIZE,in);//BPM 	
	BPM=strtod(line,0);
	fgets(line,LINESIZE,in);//STARTTIME
	startTime=atoi(line);
	fgets(line,LINESIZE,in);//Target Increment
	tI=atoi(line);	

	printf("BPM=%f,ST=%d,TI=%d\n",BPM,startTime,tI);
			
	bl=(int)(tI*(60.0f*60.0f)/(8.0f*BPM));	
	pixelI=8.0f*bl*BPM/(60.0f*60.0f);
								
	fprintf(out,"%f;pI\n",pixelI);		
	fprintf(out,"%d;Bl\n",bl);		
		
	beatCount=0;	
	spaceCount=0;	
	onbeat=0;
	
	while(fgets(line,LINESIZE,in))
	{		
		if(*line=='2')
		{
			if(spaceCount>0) fprintf(out,"%d:N:\n",spaceCount);
			break;
		}
		
		if(strpbrk(line,"143")==0)
		{
			if(onbeat)
			{
				fprintf(out,"%d:\n",beatCount);
				onbeat=0;				
			}
			spaceCount++;
		}
		else
		{
			if(strchr(line,'1')!=0)
			{												
				if(onbeat)
				{
					fprintf(out,"%d:\n",beatCount);
				}
				fprintf(out,"%d:",spaceCount);
				beatCount=0;
				spaceCount=0;
				onbeat=1;				
			}		
			beatCount++;
		}
				
	}
				
	fclose(in);
	fclose(out);
	
	
	return 0;
}

