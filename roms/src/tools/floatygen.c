#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const float pps=0.5f;
const float basey=60.0f;
const float amplitude=15.0f;

const float pi2=6.28318530717958647693f;

int main(void)
{
	int i;
	FILE *f=fopen("floatytable.s","wt");

	fprintf(f,"floatytable_entries:\t.byte %d\n",(int)(60.0f/pps));
		
	fprintf(f,"floatytable:\t.byte ");
	for(i=0;i<60/pps;i++)
		fprintf(f,"%d%c ",(int)floor(basey+sinf((float)i*pi2*pps/60.0f)*amplitude),(i<60/pps-1)?',':' ');

	fclose(f);
}

