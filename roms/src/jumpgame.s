PaletteForLyricsOn=3
PaletteForLyricsOff=1


;In->Puntero a la posicion actual de memoria		
.proc executeCommands
	
	lda jgd+JumpGameData::cmdCount
	cmp #$ff
	bne :+
		rts
	:
	lda jgd+JumpGameData::cmdCount
	beq :+
			dec jgd+JumpGameData::cmdCount
			rts
	:	
	
	doCommand:
	
	FPA_INC_16 csStrAddr
	ldy #0
	lda (csStrAddr),y
	and #$80
	bne :+
		lda (csStrAddr),y	
		sta jgd+JumpGameData::cmdCount
		jmp executeCommands
	:
	
	;DO STUFF			
	lda (csStrAddr),y	
	cmp #$FF
	bne :+
		sta jgd+JumpGameData::cmdCount
		rts
	:
	cmp #$81
	bne :+
		FPA_INC_16 csStrAddr
		lda (csStrAddr),y	
		sta jgd+JumpGameData::lyLines
		lda #3
		sta jgd+JumpGameData::lyricsAttribX	
		lda #1
		sta jgd+JumpGameData::lyricsAttribCl
		lda #$E8
		sta jgd+JumpGameData::lyricsAttribPP
		lda #$23
		sta jgd+JumpGameData::lyricsAttribPP+1
	:
	cmp #$82
	bne :+
		FPA_INC_16 csStrAddr
		lda (csStrAddr),y	
		sta jgd+JumpGameData::lyLines+1
	:

	;The color advance case (destructive case)
	tax
	and #$90
	cmp #$90
	bne :+

		lda #$E8
		sta jgd+JumpGameData::lyricsAttribPP
		lda #$23
		sta jgd+JumpGameData::lyricsAttribPP+1

		lda jgd+JumpGameData::lyricsAttribX		
		lsr
		lsr
		FPA_ADD_16_A jgd+JumpGameData::lyricsAttribPP

		txa
		and #$7F
		secsbc #$10
		clcadc jgd+JumpGameData::lyricsAttribX
		sta jgd+JumpGameData::lyricsAttribX


		
	:
	
	jmp doCommand
	
	
	rts
.endproc 		
		
;In->Puntero a la posicion actual de memoria
;En A la cuenta actual
;devuevle en A el count actual
;devuelve en X el block actual. (0: empty, 1: beat,3 hold, ff: finish song)
;Si es necesario aumenta al puntero.
.proc __GetNextBlock			
		and #$FF
		beq :+
			secsbc #1
			pha
			jmp SetUpX
		:		
		FPA_INC_16 csStrAddr
		ldy #0
		lda (csStrAddr),y
		bne :+
			ldx #$FF
			lda #$7F
			rts
		:
		and #$7F						
		secsbc #1
		pha
	SetUpX:
		ldy #0
		lda (csStrAddr),y
		and #$80
		beq :+			
			ldx #1
			pla
			rts
		:
			ldx #0
			pla
			rts
.endproc


;Devuelve en A el block actual
.macro GetNextBlock pos,count		
		MEMCPY csStrAddr,pos,#2
		lda count
		jsr __GetNextBlock
		sta count		
		phx
		MEMCPY pos,csStrAddr,#2
		pla	   
.endmacro			
		
MemCpyZP:				
	   :
	    dey
		lda (csStrAddr),y
		sta (csStrAddr2),y
		cpy #$FF
		bne :-				
		rts

.macro MEMCPYZP N
		phy
		ldy N
		jsr MemCpyZP
		ply
.endmacro

.macro SET_ADDR DST,SRC
		lda #<(SRC)
		sta DST
		lda #>(SRC)
		sta DST+1	
.endmacro

.macro SET_YNDX_ADDR DST,SRC1,SRC2
		lda SRC1,y
		sta DST
		lda SRC2,y
		sta DST+1	
.endmacro

.macro MEMCPYZP_YNDX DST,SRC1,SRC2,N
	  SET_YNDX_ADDR csStrAddr,SRC1,SRC2
	  SET_ADDR csStrAddr2,DST
	  MEMCPYZP N
.endmacro
		
		
SetJumpGame:			
		lda #0
       		sta $2001  
		
		SetIRQAddr JumpGameIRQ
		;Setup vert mirroring
		lda #0
		sta $A000
		

		;LOAD DATA FOR THE CURRENT SONG	
		ldy curSong
		
		;musicdata
		lda SDList_MusicLo,y
		sta ft_music_addr
		lda SDList_MusicHi,y
		sta ft_music_addr+1
		;banks
		lda #6
		sta $8000
		lda SDList_Bank1,y
		sta $8001
		lda #7
		sta $8000
		lda SDList_Bank2,y
		sta $8001
		;commands
		lda SDList_CommandsLo,y
		sta jgd+JumpGameData::cmdPos
		lda SDList_CommandsHi,y
		sta jgd+JumpGameData::cmdPos+1		
		FPA_ADD_16 jgd+JumpGameData::cmdPos,MinusOne16
		;beats		
		lda SDList_BeatsLo,y
		sta jgd+JumpGameData::blockPos
		lda SDList_BeatsHi,y
		sta jgd+JumpGameData::blockPos+1		
		FPA_ADD_16 jgd+JumpGameData::blockPos,MinusOne16
		MEMCPY jgd+JumpGameData::blockPos30,jgd+JumpGameData::blockPos,#2

		lda #0
		sta jgd+JumpGameData::missTriggerer		
		sta jgd+JumpGameData::lyricsAttribCl
		lda #3
		sta jgd+JumpGameData::lyricsAttribX
		lda #$E8
		sta jgd+JumpGameData::lyricsAttribPP
		lda #$23
		sta jgd+JumpGameData::lyricsAttribPP+1
		
		MEMCPYZP_YNDX jgd+JumpGameData::pI,SDList_pILo,SDList_pIHi,#3
		MEMCPYZP_YNDX jgd+JumpGameData::points,SDList_pointsLo,SDList_pointsHi,#3
		MEMCPYZP_YNDX jgd+JumpGameData::pointsMinus,SDList_pointsMinusLo,SDList_pointsMinusHi,#3
		
		SET_YNDX_ADDR csStrAddr3,SDList_StringsLoLo,SDList_StringsLoHi
		SET_YNDX_ADDR csStrAddr4,SDList_StringsHiLo,SDList_StringsHiHi
		
		
		;;SET BANKS		
		ldx #16
        	lda #$82
		jsr NES::MMC3SetBank
		ldx #24
	        lda #$83
		jsr NES::MMC3SetBank
		ldx #10
        	lda #$84
		jsr NES::MMC3SetBank
		ldx #11
		lda #$85
		jsr NES::MMC3SetBank


		ldx #4
	        lda #$80
		jsr NES::MMC3SetBank
		ldx #6
        	lda #$81
		jsr NES::MMC3SetBank		
		
		NES_loadPalette PAL_ingame
		NES_loadNametableC NAM_ingame_attribs,0
		NES_loadNametableC NAM_ingame_attribs,1
			
		lda #0					
		;Init block counter 				
		EXPANDED_MEMSET jgd+JumpGameData::insertPoint,2				
		sta jgd+JumpGameData::needForInsert

		
		EXPANDED_MEMSET jgd+JumpGameData::scroll,3
		EXPANDED_MEMSET jgd+JumpGameData::uSpriteCounter,3
		EXPANDED_MEMSET jgd+JumpGameData::lyLines,2
		EXPANDED_MEMSET jgd+JumpGameData::score,3
		
		sta jgd+JumpGameData::blockCount		
		sta jgd+JumpGameData::blockCount30
		sta jgd+JumpGameData::cmdCount		
		
		lda #$FF
		sta jgd+JumpGameData::DeathCounter
		
		lda #1
		sta vBlankSubHandler		
		
		
		ldx #0
		lda #0
	        jsr ft_music_init
				
		lda #$22
		sta $2006
		lda #$80
		sta $2006
		
		lda #$1
				
		ldy #32
		:			
		    sta $2007			
			dey
		bne :-
		
		;Pre-consumption of blocks.
		ldy #24
		:			
			phy
			GetNextBlock jgd+JumpGameData::blockPos30,jgd+JumpGameData::blockCount30
			ply			
			dey
		bne :-
		
	
		lda #Mode_JumpGame
		sta mode
		lda #%10001000
		sta ToPPUControl
		lda #%00011110 
		sta ToPPUMask
		TurnPPUOn
		rts
        
.proc JumpGame
		sy_temp=1
		miss_flag=1
		beat_input=2
		cur_beat=3
						
		jsr ft_music_play						
						
		MEMCPY jgd+JumpGameData::prevInsertPoint,jgd+JumpGameData::insertPoint,#2
		FPA_ADD_24 jgd+JumpGameData::scroll,jgd+JumpGameData::pI		
		MEMCPY jgd+JumpGameData::insertPoint,jgd+JumpGameData::scroll+1,#2
		FPA_LSR_16 jgd+JumpGameData::insertPoint
		FPA_LSR_16 jgd+JumpGameData::insertPoint
		FPA_LSR_16 jgd+JumpGameData::insertPoint
		
		lda #31
        	FPA_ADD_16_A jgd+JumpGameData::insertPoint

		
		;Find if tile insertion is needed for scrolling
		lda jgd+JumpGameData::insertPoint
		cmp jgd+JumpGameData::prevInsertPoint
		bne :+
			jmp ElseEndOfInsertion
		:
		clcadc #1	
		and #$3F		
		tay
		and #32
		bne :+
			lda #$21
			jmp :++
		:
			lda #$25
		:
		sta jgd+JumpGameData::whereToInsert+1
		tya
		and #$1F
		clcadc #$A0
		sta jgd+JumpGameData::whereToInsert
		
		;Do Commands
		MEMCPY csStrAddr,jgd+JumpGameData::cmdPos,#2
		jsr executeCommands
		MEMCPY jgd+JumpGameData::cmdPos,csStrAddr,#2
					
		;Get beat blocks
		GetNextBlock jgd+JumpGameData::blockPos,jgd+JumpGameData::blockCount
		
		cmp #$FF
		bne :+
			lda #0
			sta ToPPUMask			
			lda #0
			sta jgd+JumpGameData::DeathCounter
		:
		sta jgd+JumpGameData::curBeat
		
		
		lda #0
		sta jgd+JumpGameData::beatScored
		
		GetNextBlock jgd+JumpGameData::blockPos30,jgd+JumpGameData::blockCount30
		cmp #$FF
		beq :+
		and #$FF
		beq :+
			MEMCPY jgd+JumpGameData::tilesForInsert,JumpColumnPylon1,#8
			jmp :++
		:
			MEMCPY jgd+JumpGameData::tilesForInsert,JumpColumnEmpty,#8
		:
		
		
		lda jgd+JumpGameData::DeathCounter
		bmi :++
			inc jgd+JumpGameData::DeathCounter
			lda jgd+JumpGameData::DeathCounter
			cmp #$7F
			bne :+
				lda #Mode_SelectSongResult
				sta mode
				rts
			:
		:
			

									
		lda #1
		sta jgd+JumpGameData::needForInsert
		
			
		jmp EndOfInsertion
		ElseEndOfInsertion:
		lda #0
		sta jgd+JumpGameData::needForInsert			
		EndOfInsertion:				
		
		;Read input
		lda #NES::JOY::BUT_B		
		and joyStatus
		ora micStatus
		beq :+
				ldx #1            
				jmp :++
		:      
			ldx #0
		:

		;Score continuous input		
		lda #0
		sta jgd+JumpGameData::scY
		
		lda jgd+JumpGameData::beatScored
		bne EndOfBeatScoring
			lda #0
			sta miss_flag
			cpx #1
			bne ThereIsNoInputActivity
				lda jgd+JumpGameData::curBeat
				beq :+
					FPA_ADD_24 jgd+JumpGameData::score,jgd+JumpGameData::points
					jmp :++
				:
					lda #1
					sta miss_flag
					lda jgd+JumpGameData::missTriggerer
					cmp #6 ; MISS TRIGGER LIMIT
					beq MissTriggerIsReady
						inc jgd+JumpGameData::missTriggerer
						jmp :+
					MissTriggerIsReady:				

					lda FPA::RandomValue
					and #3
					sta jgd+JumpGameData::scY
					FPA_ADD_24 jgd+JumpGameData::score,jgd+JumpGameData::pointsMinus
					lda jgd+JumpGameData::score+2
					bpl :+
						lda #0
						EXPANDED_MEMSET jgd+JumpGameData::score,3
				:
				inc jgd+JumpGameData::beatScored
			ThereIsNoInputActivity:						
		EndOfBeatScoring:



		lda miss_flag
		bne :+
			lda #0
			sta jgd+JumpGameData::missTriggerer
		:
		
		lda #0
		sta scratch+beat_input
		ldy jgd+JumpGameData::beatLock
		cpx #0
		bne :+
			;clear
		jmp :+++
		:
			;not clear			
			cpy #0
			bne :+
				inc scratch+beat_input
			:
			
		:
		
		lda #111
		clcadc jgd+JumpGameData::scY
		sta scratch+sy_temp
		
		
		lda jgd+JumpGameData::scY
		beq :+
			NES_SETNEXTSPRITE #60,#105,#$90,#2
			NES_SETNEXTSPRITE #68,#105,#$91,#2
			NES_SETNEXTSPRITE #76,#105,#$92,#2
		:
		
		lda #0
		sta jgd+JumpGameData::scY
						
		FPA_ADD_24 jgd+JumpGameData::uSpriteCounter,UTACOSP						
		lda jgd+JumpGameData::uSpriteCounter+1
		pseudomod #6
		tax
											
		;Build utaco sprite				
		lda utaco_run_frametable_lo,x
		sta MSLAddr
		lda utaco_run_frametable_hi,x
		sta MSLAddr+1		
		lda #60
		secsbc #16
		tax
		ldy scratch+sy_temp
		lda utaco_run_bytes_per_frame
		jsr LoadMetaSprite

		
		
		;Process beat bar
		
		MEMCPY jgd+JumpGameData::BeatBar1,EmptyBeatBar1,#20
		MEMCPY jgd+JumpGameData::BeatBar2,EmptyBeatBar2,#20
		
		MEMCPY tmp3,jgd+JumpGameData::score,#3
		
		ldy #0
		
	NextBarSection:	
		phy
		FPA_ADD_24 tmp3,MinusDosCincuenta24
		ply
		lda tmp3+2
		bmi NoMoreBar			
			
			lda #2
			clc
			adc jgd+JumpGameData::BeatBar1,y
			sta jgd+JumpGameData::BeatBar1,y
			lda #2
			clc
			adc jgd+JumpGameData::BeatBar1+1,y
			sta jgd+JumpGameData::BeatBar1+1,y
			lda #2
			clc
			adc jgd+JumpGameData::BeatBar2,y
			sta jgd+JumpGameData::BeatBar2,y
			lda #2
			clc
			adc jgd+JumpGameData::BeatBar2+1,y
			sta jgd+JumpGameData::BeatBar2+1,y
			
			
			lda jgd+JumpGameData::BeatBar1,y
			cmp #$4A
			bne :+
				iny
				iny
				cpy #20
				beq NoMoreBar
			:
			jmp NextBarSection
			
		NoMoreBar:

		;Load lines
		
		
		
		ldy jgd+JumpGameData::lyLines
		lda (csStrAddr3),y
		sta csStrAddr
		lda (csStrAddr4),y
		sta csStrAddr+1
		ldy jgd+JumpGameData::lyLines+1
		lda (csStrAddr3),y
		sta csStrAddr2
		lda (csStrAddr4),y
		sta csStrAddr2+1

		; Set the attributes setter variables
		lda #$E8
		sta jgd+JumpGameData::lyricsAttribPos
		lda #$23
		sta jgd+JumpGameData::lyricsAttribPos+1

		lda jgd+JumpGameData::lyricsAttribX		
		lsr
		lsr
		FPA_ADD_16_A jgd+JumpGameData::lyricsAttribPos

		MEMCPY PtrA,jgd+JumpGameData::lyricsAttribPP,#2      
		FPA_NEG_16 PtrA
		FPA_ADD_16 PtrA, jgd+JumpGameData::lyricsAttribPos
		
		lda PtrA
		sta jgd+JumpGameData::lyricsAttribFil

		; bottom left attrib
		ldx #(PaletteForLyricsOn<<4)
		lda jgd+JumpGameData::lyricsAttribX
		clcadc #$1
		and #3
		cmp #1
		bne :+
			ldx #(PaletteForLyricsOff<<4)
		:
		stx jgd+JumpGameData::lyricsAttribB

		; bottom right attrib
		ldx #(PaletteForLyricsOff<<6)
		lda jgd+JumpGameData::lyricsAttribX
		clcadc #$1
		and #3
		bne :+
			ldx #(PaletteForLyricsOn<<6)
		:
		txa 
		ora jgd+JumpGameData::lyricsAttribB	
		ora #$A	
		sta jgd+JumpGameData::lyricsAttribB		

		; Sprites
		lda jgd+JumpGameData::lyricsAttribX
		clcadc #$1
		and #1
		beq :+
			lda jgd+JumpGameData::lyricsAttribX
			asl
			asl
			asl
			sta tmp3

			lda jgd+JumpGameData::lyricsAttribX
			secsbc #4
			tay
			lda (csStrAddr),y
			;lda #$80
			sta tmp3+1
			NES_SETNEXTSPRITE tmp3,#183,tmp3+1,#1
			
		:
		

		
		
        rts		
.EndProc

JumpGameVBlanker:
		pha
		bit $2002
		lda jgd+JumpGameData::needForInsert
		beq NoNeedForInsert
			
			ldy #8
			:
			lda jgd+JumpGameData::whereToInsert+1
			sta $2006
			lda jgd+JumpGameData::whereToInsert
			sta $2006
			lda jgd+JumpGameData::tilesForInsert-1,y
			sta $2007
			lda #$20
			FPA_ADD_16_A jgd+JumpGameData::whereToInsert
			dey
			bne :-
			
		NoNeedForInsert:


		;Attrib clean
		lda jgd+JumpGameData::lyricsAttribCl
		beq :+
			lda #$23
			sta $2006
			lda #$E8
			sta $2006
			lda #($A|(PaletteForLyricsOff<<4)|(PaletteForLyricsOff<<6))
			.repeat 8,I
				sta $2007
			.endrepeat
			lda #0
			sta jgd+JumpGameData::lyricsAttribCl
		:

		;Attrib backfill
		ldx jgd+JumpGameData::lyricsAttribFil
		beq :++
			lda jgd+JumpGameData::lyricsAttribPP+1
			sta $2006
			lda jgd+JumpGameData::lyricsAttribPP
			sta $2006
			lda #($A|(PaletteForLyricsOn<<4)|(PaletteForLyricsOn<<6))

			:
				sta $2007
				dex
			bne :-
			
			MEMCPY jgd+JumpGameData::lyricsAttribPP,jgd+JumpGameData::lyricsAttribPos,#2      
		:	
		

		lda jgd+JumpGameData::lyricsAttribPos+1
		sta $2006
		lda jgd+JumpGameData::lyricsAttribPos
		sta $2006
		lda jgd+JumpGameData::lyricsAttribB			
		sta $2007

				
		;;22c1
		lda #$22
		sta $2006
		lda #$E4
		sta $2006		
		ldy #0
		:
			lda (csStrAddr),y
			sta $2007
			iny
			cpy #30
			bne :-

		
		lda #$20
		sta $2006
		lda #$41
		sta $2006		
		ldy #($FF-20+1)
		:
			lda jgd+JumpGameData::BeatBar1-$FF+20-1,y
			sta $2007
			iny
			bne :-
			
		lda #$20
		sta $2006
		lda #$61
		sta $2006		
		ldy #($FF-20+1)
		:
			lda jgd+JumpGameData::BeatBar2-$FF+20-1,y
			sta $2007
			iny
			bne :-
										
		bit $2002
		lda #0
		sta $2005		
		lda jgd+JumpGameData::scY		
		sta $2005			
		lda #%10001000
		sta $2000		
		
		lda jgd+JumpGameData::scroll+1
		sta jgd+JumpGameData::frozenScrolls
		lda jgd+JumpGameData::scroll+2		
		and #1
		ora #%10001000
		sta jgd+JumpGameData::frozenScrolls+1

		
		lda #((6+6)*8)
		sta $C000
		sta $C001
		sta $E001
		
		lda #0
		sta jgd+JumpGameData::irqPos
		
		
 		pla
		rts
		
JumpGameIRQ:
		pha		
		sta $E000
		lda jgd+JumpGameData::irqPos
		bne :+
			bit $2002
			lda jgd+JumpGameData::frozenScrolls
			sta $2005
			lda jgd+JumpGameData::scY		
			sta $2005
			lda jgd+JumpGameData::frozenScrolls+1
			sta $2000		
			lda #((10)*8)
			sta $C000
			sta $C001
			sta $E001
			jmp :++
		:
			lda #0
			sta $2005		
			lda jgd+JumpGameData::scY		
			sta $2005			
			lda #%10000000
			sta $2000		
		:
		inc jgd+JumpGameData::irqPos
		pla
		rti
		
