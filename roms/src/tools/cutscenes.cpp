#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cstring>

using namespace std;


#define rln(line,size,f) lineTrim(fgets(line,size,f))

struct DialogLine
{
	char *str[3];
};

struct Cutscene
{
	char *name;
	char *nam;
	int banks[2];
	char *pal[2];	
	vector<DialogLine *> lines;
};

vector<char *> charMappings;
vector<Cutscene *> cutscenes;
const int LineSize=0x2000;


char *lineTrim(char *ln)
{
	if(ln)
	{
		char *c=strpbrk(ln,"\n\r");
		if(c) *c=0;
	}
	return ln;
}

unsigned char getCharMap(const char *str)
{
	if(strcmp(str,"|")==0) return 64;	
	for(int i=0;i<charMappings.size();i++)
		if(strcmp(str,charMappings[i])==0) return i;	
	return 0;
}

const int TextColumns=20;

//FF ended
void outputString(const char *str,FILE *f,bool special)
{
	char searchString[512];
	unsigned char bufferString[256];
	char *search=searchString;	
	unsigned char *buffer=bufferString;
	int colCounter=0;
	unsigned char nextChar;
	bool lastTime=false;
	fprintf(f,"\t .byte ");	
	while(*str!=0)
	{
		*(search++)=*str;
		if(!special || *str==' ')
		{
			if(special) --search;
			*search=0;
			
			nextChar=getCharMap(searchString);
									
			if(nextChar!=64)
			{
				*buffer=nextChar;
				++buffer;
			}
			else
			{
			PleaseOutputTheLastWord:
				int len=buffer-bufferString;
				colCounter+=len;
				bool space=colCounter!=TextColumns;
				if(colCounter>TextColumns)
				{
					fprintf(f,"<($FE+FontOffset),");
					colCounter=len;
				}				
				for(int i=0;i<len;i++)				
					fprintf(f,"($%X+FontOffset),",bufferString[i]);		
				
				if(space) 
				{
					fprintf(f,"($%X+FontOffset),",getCharMap(" "));		
					++colCounter;
				}
				buffer=bufferString;
				if(lastTime) goto TheEnd;
			}
			
			//if(colCounter>=TextColumns) colCounter=0;
			
			search=searchString;
		}		
		++str;
	}
	
	lastTime=true;
	goto PleaseOutputTheLastWord;
	
TheEnd:
	fprintf(f,"<($FF+FontOffset)\n");
	
}

int main(int argc,char *argv[])
{
	FILE *f;
	char line[LineSize];
	if(argc!=4) return 1;
		
	f=fopen(argv[2],"rt");	
	while(rln(line,LineSize,f))	
		charMappings.push_back(strdup(line));	
	fclose(f);	
	
	f=fopen(argv[1],"rt");	
		
	while(rln(line,LineSize,f))
	{		
		if(line[0]=='\n' || line[0]=='\r') continue;
		Cutscene *c=new Cutscene();
		c->name=strdup(line);
		if(c->name[0]!='#')
		{
			rln(line,LineSize,f);c->nam=strdup(line);
			rln(line,LineSize,f);//BANKS
			c->banks[0]=atoi(strtok(line,","));
			c->banks[1]=atoi(strtok(0,"\n\r "));
			rln(line,LineSize,f);c->pal[0]=strdup(line);
			rln(line,LineSize,f);c->pal[1]=strdup(line);			
			
			char *rlnres=rln(line,LineSize,f);
			while(line[0]!=';' && rlnres)
			{
				DialogLine *dl=new DialogLine();
				dl->str[0]=strdup(line);
				rln(line,LineSize,f);dl->str[1]=strdup(line);
				rln(line,LineSize,f);dl->str[2]=strdup(line);
				rlnres=rln(line,LineSize,f);
				c->lines.push_back(dl);
			}			
		}		
		cutscenes.push_back(c);
		
	}
	fclose(f);
	
	f=fopen(argv[3],"wt");	
	fprintf(f,";;;;;;;;;;;;;;;;;;;;;;;;;;\n; Cutscenes Data\n;;;;;;;;;;;;;;;;;;;;;;;;;;\n\n\n");
	
	fprintf(f,";Cutscenes Defines:\n");	
	for(int i=0;i<cutscenes.size();i++) 
		if(cutscenes[i]->name[0]!='#')		
			fprintf(f,"cutscene_%s=%d\n",cutscenes[i]->name,i);
					
	//commands list
	fprintf(f,";Commands:\n"
			  ";$00 ~ 7F show corrspoding cutscene\n"
			  ";$FE - HALT!! (halt mode disables skipping, after all the cutscenes goes to end instead of henshin.\n"
			  ";$FF - end cutscenes\n"
			  "cutscenes_commands:\n");
	for(int i=0;i<cutscenes.size();i++) 
	{
		unsigned char cmd;
		if(cutscenes[i]->name[0]=='#')
		{
			cmd=(cutscenes[i]->name[1]=='e')?0xFF:0xFE;
		}
		else cmd=0;
		fprintf(f,"\t.byte $%X\n",cmd);		  
	}
	
	fprintf(f,";Nametables references:\n");	
	//nametablereferencestable
	fprintf(f,"cutscenes_nametables_lo:\n");
	for(int i=0;i<cutscenes.size();i++) fprintf(f,"\t.byte <%s\n",(cutscenes[i]->name[0]=='#')?("0"):(cutscenes[i]->nam));
	fprintf(f,"cutscenes_nametables_hi:\n");
	for(int i=0;i<cutscenes.size();i++) fprintf(f,"\t.byte >%s\n",(cutscenes[i]->name[0]=='#')?("0"):(cutscenes[i]->nam));
	
	
	
	fprintf(f,";Banks:\n");	
	fprintf(f,"cutscenes_banks_0:\n");
	for(int i=0;i<cutscenes.size();i++) fprintf(f,"\t.byte $%X\n",(cutscenes[i]->name[0]=='#')?(0):(cutscenes[i]->banks[0]));		  
	fprintf(f,"cutscenes_banks_1:\n");
	for(int i=0;i<cutscenes.size();i++) fprintf(f,"\t.byte $%X\n",(cutscenes[i]->name[0]=='#')?(0):(cutscenes[i]->banks[1]));		  
	
	
	fprintf(f,";Palettes:\n");	
	for(int i=0;i<cutscenes.size();i++) 
		if(cutscenes[i]->name[0]!='#')
		{
			fprintf(f,"cutscenes_pal_%s:\n",cutscenes[i]->name);
			fprintf(f,"\t%s\n",cutscenes[i]->pal[0]);
			fprintf(f,"\t%s\n",cutscenes[i]->pal[1]);
		}
	
	fprintf(f,";Palette Tables:\n");	
	
	fprintf(f,"cutscenes_palettes_lo:\n");
	for(int i=0;i<cutscenes.size();i++) fprintf(f,(cutscenes[i]->name[0]=='#')?("\t.byte 0\n"):("\t.byte <cutscenes_pal_%s\n"),cutscenes[i]->name);
	fprintf(f,"cutscenes_palettes_hi:\n");
	for(int i=0;i<cutscenes.size();i++) fprintf(f,(cutscenes[i]->name[0]=='#')?("\t.byte 0\n"):("\t.byte >cutscenes_pal_%s\n"),cutscenes[i]->name);
	
	
	//strings
	fprintf(f,";Strings: ($FF-ended strings)\n");
	
	for(int i=0;i<cutscenes.size();i++) 
		if(cutscenes[i]->name[0]!='#')
			for(int j=0;j<cutscenes[i]->lines.size();j++)
			{
				fprintf(f,"cutscenes_string_%s_%d_en:\n",cutscenes[i]->name,j);
				outputString(cutscenes[i]->lines[j]->str[0],f,false);
				fprintf(f,"cutscenes_string_%s_%d_es:\n",cutscenes[i]->name,j);
				outputString(cutscenes[i]->lines[j]->str[1],f,false);
				fprintf(f,"cutscenes_string_%s_%d_ja:\n",cutscenes[i]->name,j);
				outputString(cutscenes[i]->lines[j]->str[2],f,true);
			}
	
	fprintf(f,";Strings lists:\n");	
	for(int i=0;i<cutscenes.size();i++) 
		if(cutscenes[i]->name[0]!='#')
		{
			for(int k=0;k<3;k++)
			{
				const char *denom=(k==0)?("en"):((k==1)?("es"):("ja"));
				fprintf(f,"cutscenes_strings_list_def_%s_%s_lo:\n",cutscenes[i]->name,denom);
				for(int j=0;j<cutscenes[i]->lines.size();j++)			
					fprintf(f,"\t.byte <cutscenes_string_%s_%d_%s\n",cutscenes[i]->name,j,denom);				
				fprintf(f,"\t.byte 0\n");								
				fprintf(f,"cutscenes_strings_list_def_%s_%s_hi:\n",cutscenes[i]->name,denom);
				for(int j=0;j<cutscenes[i]->lines.size();j++)			
					fprintf(f,"\t.byte >cutscenes_string_%s_%d_%s\n",cutscenes[i]->name,j,denom);				
				fprintf(f,"\t.byte 0\n");								
			}
			
		}
	
	for(int k=0;k<3;k++)
	{
		const char *denom=(k==0)?("en"):((k==1)?("es"):("ja"));	
		fprintf(f,"cutscenes_strings_%s_lo_lo:\n",denom);
		for(int i=0;i<cutscenes.size();i++) fprintf(f,(cutscenes[i]->name[0]=='#')?("\t.byte 0\n"):("\t.byte <cutscenes_strings_list_def_%s_%s_lo\n"),cutscenes[i]->name,denom);
		fprintf(f,"cutscenes_strings_%s_lo_hi:\n",denom);
		for(int i=0;i<cutscenes.size();i++) fprintf(f,(cutscenes[i]->name[0]=='#')?("\t.byte 0\n"):("\t.byte >cutscenes_strings_list_def_%s_%s_lo\n"),cutscenes[i]->name,denom);
		fprintf(f,"cutscenes_strings_%s_hi_lo:\n",denom);
		for(int i=0;i<cutscenes.size();i++) fprintf(f,(cutscenes[i]->name[0]=='#')?("\t.byte 0\n"):("\t.byte <cutscenes_strings_list_def_%s_%s_hi\n"),cutscenes[i]->name,denom);
		fprintf(f,"cutscenes_strings_%s_hi_hi:\n",denom);
		for(int i=0;i<cutscenes.size();i++) fprintf(f,(cutscenes[i]->name[0]=='#')?("\t.byte 0\n"):("\t.byte >cutscenes_strings_list_def_%s_%s_hi\n"),cutscenes[i]->name,denom);
		
		
	}
	
	fclose(f);
	
	return 0;
}
