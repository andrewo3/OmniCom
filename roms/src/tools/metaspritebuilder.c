/*
* Metasprite builder. Miguel Ángel Pérez Martínez 2012.
*
* metaspritebuilder columns rows nsprites sprite_sheet_width_in_pixels sprite_sheet_height_in_pixels sprite_sheet_24_bits_raw_bitmap_file
*
*/

//TODO export palettes
//TODO variable metasprite sizes and empty tiles removal
//TODO empty tiles optimization with non aligned spaced
//TODO find optimized layers

#include <stdlib.h>
#include <stdio.h>

typedef struct PaletteEntry
{
	unsigned long int c;
	unsigned char id;
	int count;
} PaletteEntry;

typedef struct ReducedPalette
{
	PaletteEntry **palette;
	struct BlockEntry *owner;
	int count;
}ReducedPalette;

typedef struct BlockEntry
{
	PaletteEntry **palette;
	ReducedPalette *rp;
	unsigned char paletteSize;
	unsigned int blockID;
	unsigned char paletteID;
}BlockEntry;

unsigned char *bitmap;
PaletteEntry *transparentColor;
ReducedPalette **reducedPalettes;
BlockEntry **blockEntries;
int nBlockEntries,nReducedPalettes;
unsigned char **graphicsData;
unsigned long int **graphicsDataC;
unsigned char *filenameBase,*filenameOnly,*filenameGraphics,*filenameS;
int nGraphicsData;

int bmpW,bmpH,cols,rows,nsprites;
int spriteW,spriteH;
int spriteBW,spriteBH,spriteBWH;

BlockEntry *allocBlockEntry()
{
	return calloc(1,sizeof(BlockEntry));
}

void addReducedPalette(BlockEntry *be)
{
	int i,j;
	ReducedPalette *rp;		
			
	rp=malloc(sizeof(ReducedPalette));
	rp->count=0;
	rp->palette=calloc(4,sizeof(PaletteEntry *));
	rp->owner=be;
	be->rp=rp;
	
	rp->palette[0]=be->palette[0];
	if(be->paletteSize>1)
	{	
		rp->palette[1]=be->palette[1];
		
		for(i=2;i<be->paletteSize;i++) 
		{
			for(j=i-1;j>=1;j--)
				if(rp->palette[j]->c<be->palette[i]->c) rp->palette[j+1]=rp->palette[j];
				else break;
				
			rp->palette[j+1]=be->palette[i];
		}
	}
	
			
	reducedPalettes=realloc(reducedPalettes,sizeof(ReducedPalette *)*(nReducedPalettes+1));
	reducedPalettes[nReducedPalettes]=rp;
	nReducedPalettes++;

}


unsigned char findColorInPalette(PaletteEntry **p,int psize,unsigned long int c)
{
	int i=0;
	for(i=0;i<psize;i++)
		if(p[i]->c==c) return i;	
	return 0;
}

unsigned char getPaletteID(BlockEntry *be,unsigned long int d)
{
	int i;
	unsigned char retID;
	
	for(i=0;i<be->paletteSize;i++)
		if(be->palette[i]->c==d) break;

	if(i>=be->paletteSize)
	{
		be->palette=realloc(be->palette,(be->paletteSize+1)*sizeof(PaletteEntry **));
		be->palette[be->paletteSize]=malloc(sizeof(PaletteEntry));
		be->palette[be->paletteSize]->count=0;
		be->palette[be->paletteSize]->c=d;
		be->palette[be->paletteSize]->id=be->paletteSize;
		be->paletteSize++;
	}
	
	if(transparentColor==0) transparentColor=be->palette[i];
	
	be->palette[i]->count++;
	retID=be->palette[i]->id;
	
	for(i=1;i<be->paletteSize;i++)
	{
		int j;
		PaletteEntry *pe=be->palette[i];
		for(j=i-1;j>=0;j--)
			if(be->palette[j]->count<pe->count) be->palette[j+1]=be->palette[j];
			else break;
		be->palette[j+1]=pe;
	}
			
	return retID;
}


unsigned int getBlockID(unsigned char *block,unsigned long int *cblock)
{
	int i;
	for(i=0;i<nGraphicsData;i++)
		if(memcmp(block,graphicsData[i],8*8)==0) return i;
	

	graphicsDataC=realloc(graphicsDataC,(nGraphicsData+1)*sizeof(unsigned long int *));	
	graphicsDataC[nGraphicsData]=cblock;	
		
	graphicsData=realloc(graphicsData,(nGraphicsData+1)*sizeof(unsigned char *));	
	graphicsData[nGraphicsData]=block;	
	return nGraphicsData++;	
}


void pushBlockEntry(BlockEntry *be,unsigned char *block,unsigned long int *cblock)
{
	blockEntries=realloc(blockEntries,(nBlockEntries+1)*sizeof(BlockEntry *));
	be->blockID=getBlockID(block,cblock);
	blockEntries[nBlockEntries]=be;
	nBlockEntries++;

}

unsigned long int getPixel(int x,int y)
{
	if(x<0 || y<0 || x>=bmpW || y>=bmpH) x=y=0;
	return bitmap[(y*bmpW+x)*3]|(bitmap[(y*bmpW+x)*3+1]<<8)|(bitmap[(y*bmpW+x)*3+2]<<16);
}

unsigned long int getPixelFromSprite(int C,int R,int x,int y)
{	
	int tx=C*spriteW+x;
	int ty=R*spriteH+y;	
	if(x<0 || y<0 || x>=spriteW || y>=spriteH) tx=ty=0;	
	return getPixel(tx,ty);
}

unsigned long int getPixelFromBlock(int C,int R,int BX,int BY,int x,int y)
{	
	int tx=BX*8+x;
	int ty=BY*8+y;	
	if(x<0 || y<0 || x>=8 || y>=8) tx=ty=0;	
	return getPixelFromSprite(C,R,tx,ty);
}

int main(int argc, char *argv[])
{
	int C,R,i,j,k,l;
	char *hogeChar;
	FILE *f;
	
	if(argc!=7) return 1;
	
	cols=atoi(argv[1]);
	rows=atoi(argv[2]);
	nsprites=atoi(argv[3]);
	bmpW=atoi(argv[4]);
	bmpH=atoi(argv[5]);
	
	transparentColor=0;
	
	printf("Cols:%d Rows:%d NSprites:%d W:%d H:%d\n",cols,rows,nsprites,bmpW,bmpH);
	
	filenameBase=strdup(argv[6]);
	hogeChar=strrchr(filenameBase,'.');
	if(hogeChar!=0) *hogeChar=0;
	
	filenameOnly=strdup(filenameBase);
	hogeChar=strpbrk(filenameOnly,"\\/");
	if(hogeChar!=0) memcpy(filenameOnly,hogeChar,strlen(hogeChar)+1);
	
	filenameGraphics=malloc(strlen(filenameBase)+5);
	strcpy(filenameGraphics,filenameBase);
	strcat(filenameGraphics,".pat");
	
	filenameS=malloc(strlen(filenameBase)+3);
	strcpy(filenameS,filenameBase);
	strcat(filenameS,".s");

	f=fopen(argv[6],"rb");
	bitmap=malloc(3*bmpW*bmpH);
	fread(bitmap,1,3*bmpW*bmpH,f);
	fclose(f);
		
	
	spriteW=bmpW/cols;
	spriteH=bmpH/rows;	
	spriteBW=((spriteW-1)>>3)+1;
	spriteBH=((spriteH-1)>>3)+1;
	spriteBWH=spriteBW*spriteBH;
	
	nBlockEntries=nReducedPalettes=nGraphicsData=0;
	reducedPalettes=0;
	graphicsData=0;
	blockEntries=0;
	
	//Parse Bitmap
	for(R=0;R<rows;R++)
		for(C=0;C<cols;C++)
		{
			if(R*cols+C>=nsprites) goto EndOfParseBitmap;
			for(i=0;i<spriteBH;i++)
				for(j=0;j<spriteBW;j++)
				{					
					BlockEntry *be=allocBlockEntry();
					unsigned char *block=malloc(8*8);
					unsigned long int *cblock=malloc(8*8*sizeof(unsigned long int));
					for(k=0;k<8;k++)for(l=0;l<8;l++)
					{
						unsigned long int px=getPixelFromBlock(C,R,j,i,l,k);					
						block[k*8+l]=getPaletteID(be,px);						
						cblock[k*8+l]=px;		
					}
					pushBlockEntry(be,block,cblock);		
				}				
		}
EndOfParseBitmap:	


	//Rebuild rgb blocks (yes, i'm an idiot and I dont have time to fix it properly)	
	/*graphicsDataC=calloc(1,sizeof(unsigned long int *)*nGraphicsData);
	
	for(i=0;i<nBlockEntries;i++)
	{	
		unsigned char *gd=graphicsData[blockEntries[i]->blockID];
		
		if(graphicsDataC[blockEntries[i]->blockID]==0) graphicsDataC[blockEntries[i]->blockID]=malloc(sizeof(unsigned long int)*8*8);
		
		for(j=0;j<8;j++)		
			for(k=0;k<8;k++)	
			{							
				graphicsDataC[blockEntries[i]->blockID][j*8+k]=blockEntries[i]->palette[gd[j*8+k]]->c;
				if(blockEntries[i]->blockID==0x84) printf("%x (%d)",graphicsDataC[blockEntries[i]->blockID][j*8+k],gd[j*8+k]);
			}

	}*/
	
	
	//Generate list of reduced palettes
	for(i=0;i<nBlockEntries;i++)
	{
		int offset=0;
			
		blockEntries[i]->palette=realloc(blockEntries[i]->palette,(blockEntries[i]->paletteSize+1)*sizeof(PaletteEntry **));		
				
		j=blockEntries[i]->paletteSize;
		while(j>0) blockEntries[i]->palette[j--]=blockEntries[i]->palette[j-1];				
		blockEntries[i]->paletteSize++;		
		
		blockEntries[i]->palette[0]=transparentColor;
		
		for(j=1;j<blockEntries[i]->paletteSize-offset;j++)
		{			
			blockEntries[i]->palette[j]=blockEntries[i]->palette[j+offset];
			if(blockEntries[i]->palette[j]->c==blockEntries[i]->palette[0]->c)
			{
				offset++;			
				j--;
				continue;
			}		
		}
				
		blockEntries[i]->paletteSize-=offset;
		addReducedPalette(blockEntries[i]);				
	}
	
	//Simplificar las paletas	
	for(i=0;i<nReducedPalettes;i++)
	{
		int offset=0;
		
		/*printf("%d: ",i);
			for(k=0;k<4;k++)
				printf(reducedPalettes[i]->palette[k]?" %x ":" N ",reducedPalettes[i]->palette[k]?reducedPalettes[i]->palette[k]->c:0);
			printf("\n");*/

		for(j=i+1;j<nReducedPalettes-offset;j++)
		{
			unsigned char replace=1;
			unsigned char subset=1;

			reducedPalettes[j]=reducedPalettes[j+offset];
																										
			//si i es subset de j, i=j, chau j.
			/*printf("\t%d: ",j);
			for(k=0;k<4;k++)
				printf(reducedPalettes[j]->palette[k]?" %x ":" N ",reducedPalettes[j]->palette[k]?reducedPalettes[j]->palette[k]->c:0);*/

			for(k=0;k<4;k++)
			{
				for(l=0;l<4;l++)	
					if(reducedPalettes[i]->palette[k]==0 || (reducedPalettes[j]->palette[l]!=0 && reducedPalettes[i]->palette[k]->c==reducedPalettes[j]->palette[l]->c)) break;
					
				if(l>=4)
				{
					subset=0;					
					break;
				}
			}
			
			//si j es subset de i, chau j.
			if(!subset)	
			{
				subset=1;
				replace=0;
				for(k=0;k<4;k++)
				{
					for(l=0;l<4;l++)	
						if(reducedPalettes[j]->palette[k]==0 || (reducedPalettes[i]->palette[l]!=0 && reducedPalettes[j]->palette[k]->c==reducedPalettes[i]->palette[l]->c)) break;
						
					if(l>=4)
					{
						subset=0;					
						break;
					}
				}
			}
					
				
			if(subset)
			{
				if(replace)
					for(k=0;k<4;k++) reducedPalettes[i]->palette[k]=reducedPalettes[j]->palette[k];
								
				reducedPalettes[j]->owner->rp=reducedPalettes[i];
				j--;
				offset++;
				continue;
			}
			
		}
		
		nReducedPalettes-=offset;
	}
	
	
	for(i=0;i<nReducedPalettes;i++)
	{
		
		printf("%x - %x - %x - %x\n",reducedPalettes[i]->palette[0]==0?-1:reducedPalettes[i]->palette[0]->c,
							reducedPalettes[i]->palette[1]==0?-1:reducedPalettes[i]->palette[1]->c,
							reducedPalettes[i]->palette[2]==0?-1:reducedPalettes[i]->palette[2]->c,
							reducedPalettes[i]->palette[3]==0?-1:reducedPalettes[i]->palette[3]->c);
	}
	
	//Reindex graphics data (Eventually find duplicate tiles with different color indexing)
	//if the color is not found in the reduced palette, use the second color. (Eventually add layers)
	
	for(i=0;i<nBlockEntries;i++)
	{
		unsigned char *gd=graphicsData[blockEntries[i]->blockID];
		unsigned long int *gdc=graphicsDataC[blockEntries[i]->blockID];
						
		for(j=0;j<8;j++)		
			for(k=0;k<8;k++)			
			{
				//if(gd[j*8+k]>=blockEntries[i]->paletteSize) printf("%d\n",i);
				gd[j*8+k]=findColorInPalette(blockEntries[i]->rp->palette,4,gdc[j*8+k]);								
			}
			
		for(l=0;l<nReducedPalettes;l++)
			if(blockEntries[i]->rp==reducedPalettes[l])
			{
				blockEntries[i]->paletteID=l;
				break;
			}
		
	}
	
		
	//Output Graphics data
	f=fopen(filenameGraphics,"wb");	
	for(i=0;i<nGraphicsData;i++)
	{
		unsigned char byte;
		for(j=0;j<8;j++)
		{
			byte=0;
			for(k=0;k<8;k++)
				byte|=(graphicsData[i][j*8+k]&1)<<(7-k);					
			fwrite(&byte,1,1,f);
		}
			
		for(j=0;j<8;j++)
		{
			byte=0;
			for(k=0;k<8;k++)
				byte|=((graphicsData[i][j*8+k]>>1)&1)<<(7-k);							
			fwrite(&byte,1,1,f);
		}			    
	}
	fclose(f);
	
	//Output Code
	f=fopen(filenameS,"wt");
	fprintf(f,"%s_PATTERN_OFFSET=0\n",filenameOnly);
	
	
	fprintf(f,"%s_frames: .byte %d\n",filenameOnly,nsprites);
	fprintf(f,"%s_bytes_per_frame: .byte %d\n",filenameOnly,spriteBWH*4);
		
	
	for(i=0;i<nBlockEntries;i++)
	{
		int base=i%spriteBWH,X,Y;
		if(base==0)
		{	
			fprintf(f,";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n");			
			fprintf(f,"%s_frame_%d:    ;(X,Y,PATTERN_ID,PAL)\n",filenameOnly,i/spriteBWH);		      
			fprintf(f,";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n");					
		}
		Y=base/spriteBW;
		X=base-(Y*spriteBW);
		fprintf(f,".byte %d, %d, (%s_PATTERN_OFFSET+%d), %d\n",X*8,Y*8,filenameOnly,blockEntries[i]->blockID,blockEntries[i]->paletteID);
	}
	
	fprintf(f,"%s_frametable_lo:\n",filenameOnly);
	for(i=0;i<nsprites;i++)
		fprintf(f,"\t .byte <%s_frame_%d\n",filenameOnly,i);
		
	fprintf(f,"%s_frametable_hi:\n",filenameOnly);
	for(i=0;i<nsprites;i++)
		fprintf(f,"\t .byte >%s_frame_%d\n",filenameOnly,i);		

	
	
	
	fclose(f);
	
	return 0;
}
