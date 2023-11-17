#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <vector>
#include <map>

using namespace std;

#define LINESIZE 1024
#define rln(line,size,f) lineTrim(fgets(line,size,f))

struct StringBeat
{
	int delay,len;
	char *str;
	int charactersCount;
	unsigned char data[100];
	int nData;
	int strId;
	int subStrGap;
	bool clearOverride;
};

struct LyCommand
{
	unsigned char v;
	LyCommand *next;
};

vector<char *> charMappings;

int getCharMap(const char *str)
{
	if(strcmp(str,"|")==0) return 64;	
	for(int i=0;i<charMappings.size();i++)
		if(strcmp(str,charMappings[i])==0) return i;	
	printf("No mapping for %s\n",str);
	return -1;
}


void pushCommand(map<int,LyCommand *> &commands,int key,unsigned char command)
{
	LyCommand *root;	
	
	if(key<0) key=0;
	
	if(commands.find(key)==commands.end()) 
	{
		root=(commands[key]=new LyCommand);		
	}
	else
	{
		root=commands[key];	
		while(root->next!=0) root=root->next;
		root=(root->next=new LyCommand);
	}
	
	root->v=command;
	root->next=0;		
}



char *lineTrim(char *ln)
{
	if(ln)
	{
		char *c=strpbrk(ln,"\n\r");
		if(c) *c=0;
	}
	return ln;
}


bool mapStringJa(unsigned char *dst,const char *inStr,int *count)
{	
	char *tmpstr=strdup(inStr);
	char *kn=strtok(tmpstr," ");
	
	*count=0;

	while(kn)
	{
		if(strcmp(kn,"$")==0) return true;
		int cm=getCharMap(kn);
		if((cm&0xFF)==cm) dst[(*count)++]=cm;
		kn=strtok(0," ");
	}
	
	free(tmpstr);	
	return false;
}

void outputFP(FILE *f,float n)
{
	unsigned short integerPart;
	unsigned char decimalPart;
		
	bool minus=n<0;
	
	if(n<0) n=-n;
	
	decimalPart=(unsigned char) (256.0f*floor((n-floor(n))*100.0f)/100.0f);
	integerPart=(unsigned short) n;
	
	if(minus)
	{
		unsigned long int n=(~(decimalPart|(integerPart<<8)))+1;
		decimalPart=n&0xFF;
		integerPart=(n>>8)&0xFFFF;
	}
	
	fprintf(f,"$%x,$%x,$%x",decimalPart,integerPart&0xFF,(integerPart>>8)&0xFF);	
}

int main(int argc, char *argv[])
{
	FILE *in,*out,*outi;
	char line[LINESIZE];
	char *name;
	float pI;
	int bL;
	
	if(argc!=5)
	{
		printf("Incorrect number of parameters\n");
		return 1;
	}
	
	FILE *f=fopen(argv[3],"rt");	
	while(rln(line,LINESIZE,f))	
		charMappings.push_back(strdup(line));	
	fclose(f);	
	
		
	in=fopen(argv[1],"rt");
	out=fopen(argv[2],"wt");	
	outi=fopen(argv[4],"wt");
	
	rln(line,LINESIZE,in);//Name
	name=strdup(line);
	rln(line,LINESIZE,in);//PixelIncrement
	pI=atof(line);
	rln(line,LINESIZE,in);//BL
	bL=atoi(line);
				
	fprintf(out,";Beat definitions for %s\n",name);
	
	fprintf(out,"%s_pI: .byte ",name); outputFP(out,pI); fprintf(out,"\n");
	fprintf(outi,".global %s_pI\n",name);
	
	fprintf(out,"%s_beats:\n",name);
	fprintf(outi,".global %s_beats\n",name);
	
	int tcount=0;
	int strBeatCount=0;

	std::vector<StringBeat *> strbeats;
	
	int totalBlocks=0;
	int totalBeats=0;
	
	while(rln(line,LINESIZE,in))
	{
		char *beatC;
		char *blankC=strtok(line,":");
		if(blankC) beatC=strtok(0,":");
		if(!beatC) {printf("Ugly error.\n");exit(1);}
		
		
		char *string=strtok(0,";");
						
		//Rules
		// bit7 on = beat, off = blank
		// blank=0; end of song; beat=0, 0x77 ligature
		// >=F8 special commands.
		// 0: next string command.
		
		int blank=atoi(blankC)*bL;		

		totalBlocks+=blank;
		
		while(blank>0)
		{			
			fprintf(out,"\t.byte $%02x\n",(blank>0x7F)?(0x7F):(blank));
			++tcount;
			blank-=0x7F;
		}
		
		if(*beatC=='N') break;
		int beat=atoi(beatC)*bL;
		
		totalBeats+=beat;
		
			
		if(string)
		{
			unsigned char somedata[1000];
			StringBeat *sb=new StringBeat();
			sb->delay=totalBlocks;
			sb->len=beat;
			sb->str=strdup(string);				
			mapStringJa(somedata,sb->str,&sb->charactersCount);	
			sb->nData=0;
			sb->clearOverride=false;
			strbeats.push_back(sb);			
		}
		
		totalBlocks+=beat;		
		
		while(beat>0x77)
		{			
			fprintf(out,"\t.byte $%02x\n",0x80);
			printf("Warning, long long long hold\n");
			++tcount;
			blank-=0x77;
		}
		
		fprintf(out,"\t.byte $%02x\n",beat|0x80);				
		++tcount;
	}
	fprintf(out,"\t.byte $0\n");
	++tcount;
	
	
	float points=1.25*10000.0f/totalBeats;
	fprintf(out,"%s_points: .byte ",name); outputFP(out,points); fprintf(out,"\n");
	fprintf(outi,".global %s_points\n",name);
	fprintf(out,"%s_points_minus: .byte ",name); outputFP(out,-points*0.7); fprintf(out,"\n");
	fprintf(outi,".global %s_points_minus\n",name);
	
	//Build strings for screen
	StringBeat *curLine=0;
	int curstrid=1;

	for(int i=0;i<strbeats.size();i++)
	{
		if(curLine==0) 
		{
			curLine=strbeats[i];
			curLine->strId=curstrid++;
		}
		int dlen;
		bool endof=mapStringJa(&curLine->data[curLine->nData],strbeats[i]->str,&dlen);
		strbeats[i]->subStrGap=dlen;		
		curLine->nData+=dlen;		
		if(endof) 
		{
			curLine->len=strbeats[i]->delay-curLine->delay+strbeats[i]->len;
			curLine=0;							
		}
	}
	
		
	fprintf(out,"%s_string_%d:.byte ",name,0);	
	/* The first line is empty*/for(int i=0;i<29;i++) fprintf(out,"($%02x+FontOffset),",0x40);fprintf(out,"($%02x+FontOffset)\n",0x40);
	for(int i=0;i<strbeats.size();i++)
	{
		if(strbeats[i]->nData>=30) printf("Warning: Lyrics line is too long!\n");
		if(strbeats[i]->nData>0)
		{
			fprintf(out,"%s_string_%d:.byte ",name,strbeats[i]->strId);				
			for(int j=0;j<30;j++)
				fprintf(out,"($%02x+FontOffset)%c",(j<strbeats[i]->nData)?strbeats[i]->data[j]:0x40,(j==30-1)?'\n':',');
		}
	}
	
	
	fprintf(out,"%s_strings_lo:\n",name);
	fprintf(outi,".global %s_strings_lo\n",name);
	for(int i=0;i<curstrid;i++) fprintf(out,"\t.byte <%s_string_%d\n",name,i);
	
	fprintf(out,"%s_strings_hi:\n",name);
	fprintf(outi,".global %s_strings_hi\n",name);
	for(int i=0;i<curstrid;i++) fprintf(out,"\t.byte >%s_string_%d\n",name,i);
	
	
				
	// Bit 8 not flagged: wait
	// Bit 8 flagged: Command
	// Commands
	// 1: load next byte indexed string at first position
	// 2: load next byte indexed string at second position.
	// 0x10~0x20: Advance color counter command-$10 veces.
	
	
	// some beats before, show the line
	// If first line is empty use it, if its occupied use the second.
	// If second line is occupied, enqueue it
	// On beat, move to first line, if second line was waiting on line insert it.
	// After duration, remove the first line.
	

	map<int,LyCommand *> commands;
	map<int,LyCommand *>::iterator it,prevIt;
	
	int preDelay=(int)((1.0f*60.0f*pI)/8.0f);
	StringBeat *lines[]={0,0,0};
	int queue=-1;
	int nextSpace=0;
	
	preDelay=0;
	
	if(preDelay>0x7F) printf("Warning! Pre delay is too long! (%d)\n",preDelay);
	
	for(int i=0;i<strbeats.size();i++)
	{

		if(strbeats[i]->nData>0)
		{
			if(lines[0]!=0 && lines[0]->delay+lines[0]->len<strbeats[i]->delay )
			{				
				lines[0]=0;
				if(lines[1]!=0 || lines[2]!=0) printf("warning, this should not happen.\n");
			}
		
		
			if(lines[0]==0)
			{
				lines[0]=strbeats[i];
				pushCommand(commands,strbeats[i]->delay-preDelay,0x81);	
			}
			else if(lines[1]==0)
			{
				lines[1]=strbeats[i];
				pushCommand(commands,strbeats[i]->delay-preDelay,0x82);	
			}
			else if(lines[2]==0) lines[2]=strbeats[i];
			else printf("Error, lines queue overflow.\n");

			pushCommand(commands,strbeats[i]->delay-preDelay,strbeats[i]->strId);

			if(lines[1]==strbeats[i])
			{
				lines[0]->clearOverride=true;
				lines[0]=lines[1];
				lines[1]=lines[2];
				lines[2]=0;
				pushCommand(commands,strbeats[i]->delay,0x81);	
				pushCommand(commands,strbeats[i]->delay,lines[0]->strId);				
				
				pushCommand(commands,strbeats[i]->delay,0x82);	
				pushCommand(commands,strbeats[i]->delay,(lines[1])?lines[1]->strId:0);								
			}
			
			
		}

		if(strbeats[i]->charactersCount==0) printf("zero size beat.\n");
		pushCommand(commands,strbeats[i]->delay,0x80 | (strbeats[i]->charactersCount+0x10));
		
	}
	
	for(int i=0;i<strbeats.size();i++)
	{
		if(strbeats[i]->nData>0 && !strbeats[i]->clearOverride)
		{
			pushCommand(commands,strbeats[i]->delay+strbeats[i]->len,0x81);	
			pushCommand(commands,strbeats[i]->delay+strbeats[i]->len,0);	
		}
	}
	
	
	fprintf(out,"%s_commands:\n",name); 
	fprintf(outi,".global %s_commands\n",name);
	
	
	prevIt=commands.end();
	
	for (it=commands.begin();it != commands.end();++it)
	{
		int gap=(*it).first-((prevIt!=commands.end())?(*prevIt).first:0);
						
		
		fprintf(out,"\t.byte "); 
		
		while(gap>0)
		{
			fprintf(out,"$%02x,",(gap>0x7F)?0x7F:gap); 	
			gap-=0x7F;
		}
		
		LyCommand *cmd=(*it).second;
		while(cmd!=0)
		{
			fprintf(out,"$%02x%c",cmd->v,(cmd->next==0)?'\n':','); 	
			cmd=cmd->next;
		}
		
		prevIt=it;
	}
	
	fprintf(out,"\t.byte $ff\n"); 
    		
	fclose(in);
	fclose(out);
	fclose(outi);
	
	
	return 0;
}

