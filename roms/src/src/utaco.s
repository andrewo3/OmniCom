;うたいたガールUTACO~「ＩＩコンは世界を救う？！」
;Miguel Ángel Pérez Martínez - 2012

.segment "HEADER"

.byte   "NES", $1A
.byte   8               ;PRG pages
.byte   4               ;CHR pages
.byte   $40             ;LOW Mapper && Mirroring && misc flags
.byte   $00             ;HI Mapper && misc flags
.byte   0
.byte   0
.res    6,0             

.include "src/globals.inc"
.include "src/songdata_1.inc"
.include "src/songdata_2.inc"
.include "src/songdata_3.inc"
.include "src/minisongs.inc"

.include "../neslib/nes.s"
.include "../neslib/fpa.s"

.import ft_music_init, ft_music_play

.export ft_music_addr

.define GRID_W 9
.define GRID_H 7

MINOS_HEADER_OFFSET=7
BG_GRID_OFFSET=$42

.macro WaitForVBlank
        lda vblank
:       cmp vblank
        beq :-
.endmacro

RTI_opcode = $40
JMP_opcode = $4C


.macro TurnPPUOn
	bit $2002
	lda #0
	sta $2005		
	sta $2005
	lda ToPPUControl
	sta $2000
	lda ToPPUMask
	sta $2001
.endmacro

.macro DisableIRQCounter	
	lda #RTI_opcode	
	sta $E000
	sta IRQMagic
.endmacro


.macro SetIRQAddr addr
	lda #RTI_opcode
	sta IRQMagic
	lda #<addr
	sta IRQMagic+1
	lda #>addr
	sta IRQMagic+2
	lda #JMP_opcode
	sta IRQMagic
.endmacro

.macro SetMinisongsBanks
	lda #6
	sta $8000
	sta $8001
	lda #7
	sta $8000
	sta $8001
.endmacro

.macro SetupFader addr,direction
        lda #<(addr)
	sta PtrAL
        lda #>(addr)
	sta PtrAH
	lda #8
	ldx direction
        jsr setupFader
.endmacro


.macro LoadFaderPalette
	bit $2002
	lda #$3F
	sta $2006
	lda #$00
	sta $2006
	ldy #0
	:
		lda fadePalette, y
		sta $2007
	iny
	cpy #32
	bne :-
.endmacro        

.macro BranchIfFadeIsNotYetOut TARGET
	lda fadeDirection
	cmp #$f0
	bne TARGET
	lda fadePaletteChanged
	cmp #5
	bne TARGET
.endmacro


.struct JumpGameData
        blockCount	.byte
        blockPos	.byte 2		
	blockCount30	.byte
        blockPos30	.byte 2
	scroll		.byte 3
	insertPoint	.byte 2
	prevInsertPoint	.byte 2
	needForInsert	.byte 1
	tilesForInsert  .byte 8
	whereToInsert	.byte 2				
	uSpriteCounter	.byte 3		
	beatLock	.byte 1		
	cmdPos		.byte 2
	cmdCount	.byte 1
	lyLines		.byte 2
	BeatBar1	.byte 20
	BeatBar2	.byte 20
	irqPos		.byte 1
	frozenScrolls	.byte 2
	score		.byte 3
	curBeat		.byte 1
	beatScored	.byte 1
	scY		.byte 1
	DeathCounter	.byte 1
	pI		.byte 3
	points		.byte 3
	pointsMinus     .byte 3
	missTriggerer	.byte 1
	lyricsAttribX	.byte 1
	lyricsAttribPos	.byte 2
	lyricsAttribPP	.byte 2
	lyricsAttribFil	.byte 1
	lyricsAttribB	.byte 1
	lyricsAttribCl	.byte 1
.endstruct

.struct HenshinData
	counter		.byte 2
	mode		.byte 1
	sx		.byte 2
	sy		.byte 2
	sprX		.byte 1
.endstruct

.struct TitleScreenData
	bspos		.byte 1
	fskip		.byte 1
.endstruct

.segment "ZEROPAGE"
        scratch:        	.res 12
        joyStatus:      	.res 1
        joyStatusLock:  	.res 1
	micStatus:		.res 1
	vblank:         	.res 1
	mode:           	.res 1
	vBlankSubHandler: 	.res 1
	
	MSLAddr:		.res 2		
	csStrAddr:		.res 2
	csStrAddr2:		.res 2
	csStrAddr3:		.res 2
	csStrAddr4:		.res 2
	IRQMagic:		.res 3
	tmp3:			.res 3

	ToPPUControl:		.res 1
	ToPPUMask:		.res 1
					
.segment "BSS"                
	jgd:			.tag JumpGameData
	tsd:			.tag TitleScreenData
	hd:			.tag HenshinData
	language:		.res 1
	curCutscene:		.res 1
	csCurStr:		.res 1
	csPPUPos:		.res 2
	csStringReady:		.res 1
	csFrameDelay:		.res 1
	csFrameCounter:		.res 1
	csColCounter:		.res 1
	csImFadingOut:		.res 1
	csHalt:			.res 1
	csFC:			.res 1
	csMIIVisible:		.res 1
	csCleanPlease:		.res 1
	csCurSong:		.res 1
	csNextSong:		.res 1
	curSong:		.res 1
	ft_music_addr:  	.res 2
	continue:		.res 1

	fadeMultiplier:		.res 1
	fadeCounter:		.res 1
	fadeDirection:		.res 1
	fadePalette:		.res 32
	fadePaletteTarget:	.res 32
	fadePaletteChanged:	.res 1

.segment "CODE"


;;||=============================================================||;;
reset:
        sei
        cld
        ldx #$FF
        txs
        
        ;Clear RAM	
	ldx #$00
	lda #$00
clearram:		
		.repeat 8,I
		sta I*$100,x	
		.endrepeat
		dex
		bne clearram
                 
:       bit $2002 
        bpl :-                          
:       bit $2002 
        bpl :-  
                    
        clearPPU
		
	;clear APU
        lda #$40
        sta $4017
				
	;Setup vert mirroring
	lda #0
	sta $A000		
	;Clear vBlankSubHandler
	sta vBlankSubHandler

	lda #0
	sta csHalt
		
	lda #Mode_SetLanguageSelectScreen
	sta mode

	lda #6
	sta $8000
	lda #8
	sta $8001
	lda #7
	sta $8000
	lda #9
	sta $8001
		                       		
        lda #%10001000
	sta ToPPUControl
        sta $2000
        lda #0
        sta $2001      				
	DisableIRQCounter
	cli
        
.proc MainLoop
        
	FPA_RAND        
        WaitForVBlank
                      
        ;;;;;;;;;;;;VBLANK LAND!     
	lda #0
        sta $2001        
	NES_DMASPRITES    	    
	lda vBlankSubHandler
        jsr vBlankSubHandlerDispatcher                                       
	lda ToPPUMask
        sta $2001        
	;;;;;;;;;;;;END OF VBLANK LAND!
        
        lda joyStatus
	pha
        
	lda #1
	sta micStatus
		
        ;Read Joystick
        lda #$1
		sta $4016
		lda #$0        
		sta $4016        
		ldx #$8	
		:						
			lda $4016
			lsr			
			ror joyStatus
			lsr
			and #1
			bne :+
				lda #0
				sta micStatus
			:
			dex		
        bne :--               
	pla
        eor #$FF
        and joyStatus
        sta joyStatusLock

                		
	lda mode
        jsr SuperMagicMainLoopDispatcher        
	jsr NES::clearRemainingSprites							
						
	jsr doFader
						
        jmp MainLoop
.endproc         

.segment "SBANK_5"


SetLanguageSelectScreen:
        lda #0
        sta $2001  
	sta vBlankSubHandler

	DisableIRQCounter

	;Disable APU Channels
        lda #$00
	sta $4015 

	;Setup vert mirroring
	lda #0
	sta $A000

	NES_MMC3SetBank #NES::MMC3TARGETS::CHRx0000, #8
	NES_MMC3SetBank #NES::MMC3TARGETS::CHRx0800, #10

	;;Setup banks
	ldx #4
	lda #$82
	jsr NES::MMC3SetBank
	ldx #3
	lda #$83
	jsr NES::MMC3SetBank	
	ldx #10
	lda #$84
	jsr NES::MMC3SetBank
	ldx #11
	lda #$85
	jsr NES::MMC3SetBank

	SetupFader PAL_langselect,#$10
        NES_loadNametableC NAM_langselect,0

	lda #0
	sta continue

        lda #Mode_LanguageSelectScreen
        sta mode
	lda #%10000000
	sta ToPPUControl
        lda #%00011110 
        sta ToPPUMask
	TurnPPUOn
	rts


LanguageSelectScreen:
	

        lda #NES::JOY::START
	ora #NES::JOY::BUT_B
        and joyStatusLock
        beq :+			
		jsr startFaderOut
        :       


	BranchIfFadeIsNotYetOut :++
		lda continue
		sta language
		cmp #2
		beq :+
			lda #Mode_SetTitleScreen
			sta mode
			rts
		:
		lda #Mode_SetChuuiScreen
		sta mode
		rts
	:

	ldx continue
        lda #NES::JOY::UP
        and joyStatusLock
        beq :+			
		dex
        :       
        lda #NES::JOY::DOWN
	ora #NES::JOY::SELECT
        and joyStatusLock
        beq :+			
		inx
        :  
	txa 
	and #$F0
	beq :+
		ldx #2
	:
	cpx #3
	bne :+
		ldx #0
	:	
	stx continue


	lda continue
	asl
	asl
	asl
	asl
	clcadc #(10*8)
	sta tmp3 		

	NES_SETNEXTSPRITE #(12*8)+4,tmp3,#$F8,#1

	rts

SetChuuiScreen:
        lda #0
        sta $2001  
	SetupFader PAL_langselect,#$10
        NES_loadNametableC NAM_chuui,0
        lda #Mode_ChuuiScreen
        sta mode
	TurnPPUOn    
	rts

ChuuiScreen:
	
        lda #NES::JOY::START
	ora #NES::JOY::BUT_B
        and joyStatusLock
        beq :+		
		jsr startFaderOut	
        :     

	BranchIfFadeIsNotYetOut :+
	    lda #Mode_SetTitleScreen
            sta mode
	:
	rts

.segment "CODE"

SetTitleScreen:			

        lda #0
        sta $2001  
	sta vBlankSubHandler
	
	DisableIRQCounter
	
	SetMinisongsBanks

	;Setup vert mirroring
	lda #0
	sta $A000

	sta jgd+TitleScreenData::bspos	
	sta jgd+TitleScreenData::fskip
	
	NES_MMC3SetBank #NES::MMC3TARGETS::CHRx0000, #0
	NES_MMC3SetBank #NES::MMC3TARGETS::CHRx0800, #2

	;;Setup banks
	ldx #0
	lda #$82
	jsr NES::MMC3SetBank
	ldx #1
	lda #$83
	jsr NES::MMC3SetBank	
	ldx #2
	lda #$84
	jsr NES::MMC3SetBank
	ldx #24
	lda #$85
	jsr NES::MMC3SetBank
				

	SetupFader PAL_title,#$10
        NES_loadNametableC NAM_title,0

	lda #<minisongs_musicdata
	sta ft_music_addr
	lda #>minisongs_musicdata
	sta ft_music_addr+1
		
	ldx #0
	lda #0
        jsr ft_music_init

	lda #0
	sta continue
        		
        lda #Mode_TitleScreen
        sta mode
	lda #%10000000
	sta ToPPUControl
        lda #%00011110 
        sta ToPPUMask
	TurnPPUOn    
        rts

.segment "SBANK_4"
        
TitleScreen:
		

	NES_SETNEXTSPRITE #176,#87,#$86,#1
	NES_SETNEXTSPRITE #160,#79,#$77,#0
	NES_SETNEXTSPRITE #148,#82,#$7E,#1
	NES_SETNEXTSPRITE #187,#93,#$80,#1
	NES_SETNEXTSPRITE #184,#81,#$8B,#3
	NES_SETNEXTSPRITE #160,#79,#$8C,#3
	NES_SETNEXTSPRITE #192,#81,#$95,#3

	NES_SETNEXTSPRITE #(8*1),#(8*3-1),#$A0,#2
	NES_SETNEXTSPRITE #(8*1),#(8*22-1),#$B0,#2
	NES_SETNEXTSPRITE #(8*30),#(8*3-1),#$A1,#2
	NES_SETNEXTSPRITE #(8*30),#(8*22-1),#$B1,#2


	jsr ft_music_play

	lda jgd+TitleScreenData::fskip
	bne :+
		lda #2
		sta jgd+TitleScreenData::fskip
		inc jgd+TitleScreenData::bspos
		lda jgd+TitleScreenData::bspos
		and #7
		sta jgd+TitleScreenData::bspos
		clcadc #24
		tax
		lda #$85
		jsr NES::MMC3SetBank
	:
	dec jgd+TitleScreenData::fskip		

	lda continue
	clcadc #(6*8)
	sta tmp3 		

	NES_SETNEXTSPRITE #(3*8)+4,tmp3,#$9F,#0
		
        lda #NES::JOY::START
        and joyStatusLock
        beq :+
		jsr startFaderOut			
        :        

	BranchIfFadeIsNotYetOut :++
		lda continue
		beq :+
			lda #Mode_SetCredits
			sta mode
			rts
		:
		lda #Mode_NewGame
		sta mode
		rts
	:

	lda #NES::JOY::UP
	ora #NES::JOY::DOWN
	ora #NES::JOY::SELECT
	and joyStatusLock
	beq :+++			
		lda continue
		beq :+
			lda #0
			jmp:++	
		:
			lda #16
		:
		sta continue
	:  

        rts

SetCredits:
        lda #0
        sta $2001  

	;;SET FONT BANKS
	ldx #8
	lda #$82
	jsr NES::MMC3SetBank
	ldx #9
	lda #$83
	jsr NES::MMC3SetBank
	ldx #10
	lda #$84
	jsr NES::MMC3SetBank
	ldx #11
	lda #$85
	jsr NES::MMC3SetBank
 
	SetupFader PAL_credits,#$10
        NES_loadNametableC NAM_credits,0

	lda #<minisongs_musicdata
	sta ft_music_addr
	lda #>minisongs_musicdata
	sta ft_music_addr+1

	ldx #0
	lda #4
	jsr ft_music_init

        lda #Mode_Credits
        sta mode
	TurnPPUOn  
	rts
Credits:

        lda #NES::JOY::START
	ora #NES::JOY::BUT_B
        and joyStatusLock
	beq :+
		jsr startFaderOut	
        :     

	jsr ft_music_play

	BranchIfFadeIsNotYetOut :+
		lda #Mode_SetTitleScreen
		sta mode
	:

	rts

NewGame:
	lda #0
	sta curSong
	ldy #0
	lda SDList_Cutscene,y
	sta curCutscene
	lda #Mode_CutSceneLoad
	sta mode
	rts
		
.segment "CODE"
		
;;;;;;;;;;;;;;;; JUMP GAME ;;;;;;;;;;;;;;;;;;;;
.include "src/jumpgame.s"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		
.proc LoadMetaSprite
		sprx=6
		spry=7
		pat=8
		basex=9
		basey=10
		sc=11
		pal=12
		
		stx scratch+basex
		sty scratch+basey
		sta scratch+sc		
		ldy #0
		
:		
		tya
		cmp scratch+sc
		bne :+
			rts
		:
		
		lda (MSLAddr),y
		sta scratch+sprx
		iny
		lda (MSLAddr),y
		sta scratch+spry
		iny
		lda (MSLAddr),y
		sta scratch+pat
		iny
		lda (MSLAddr),y
		sta scratch+pal
		iny
		
		lda scratch+sprx
		clcadc scratch+basex
		sta scratch+sprx
		
		lda scratch+spry
		clcadc scratch+basey
		sta scratch+spry
		
		tya
		pha
		NES_SETNEXTSPRITE scratch+sprx,scratch+spry,scratch+pat,scratch+pal	
		pla
		tay
		
		jmp :--
.EndProc


SelectSongResult:
		lda jgd+JumpGameData::score+2
		cmp #$1E
		bcc YouLose
		lda #Mode_SongPass
		sta mode
		rts

	YouLose:
		lda #Mode_SetFailScreen

		sta mode
	rts

SongPass:
		inc curSong
		ldy curSong		
		lda SDList_Cutscene,y
		sta curCutscene		
		lda #Mode_CutSceneLoad
		sta mode
	rts

SetFailScreen:		
		lda #0
		sta $2001  
		sta vBlankSubHandler
	
		DisableIRQCounter
	
		SetMinisongsBanks

		;Setup vert mirroring
		lda #0
		sta $A000
	
		NES_MMC3SetBank #NES::MMC3TARGETS::CHRx0000, #0
		NES_MMC3SetBank #NES::MMC3TARGETS::CHRx0800, #2

		;Setup banks
		ldx #20
		lda #$82
		jsr NES::MMC3SetBank
		ldx #21
		lda #$83
		jsr NES::MMC3SetBank	
		ldx #22
		lda #$84
		jsr NES::MMC3SetBank
		ldx #23
		lda #$85
		jsr NES::MMC3SetBank
			
		SetupFader PAL_title,#$10
		NES_loadNametableC NAM_gameover,0

		lda #<minisongs_musicdata
		sta ft_music_addr
		lda #>minisongs_musicdata
		sta ft_music_addr+1
		
		lda #250
		sta tmp3

		ldx #0
		lda #2
		jsr ft_music_init
				
		lda #Mode_FailScreen
		sta mode   
		lda #%10000000
		sta ToPPUControl
		lda #%00011110 
		sta ToPPUMask
		TurnPPUOn 

	rts

FailScreen:
		jsr ft_music_play
		;A decent person would do the following stuff properly
		;I'm not a decent person.
		dec tmp3
		lda tmp3
		bne :+
			lda #Mode_SetContinue
			sta mode
		:
	rts

SetContinue:
		lda #0
		sta $2001  

		NES_MMC3SetBank #NES::MMC3TARGETS::CHRx0000, #8
		NES_MMC3SetBank #NES::MMC3TARGETS::CHRx0800, #10

		ldx #8
		lda #$82
		jsr NES::MMC3SetBank
		ldx #9
		lda #$83
		jsr NES::MMC3SetBank
		ldx #10
		lda #$84
		jsr NES::MMC3SetBank
		ldx #11
		lda #$85
		jsr NES::MMC3SetBank

		;ldy #0
		;ldx cutscenes_palettes_lo,y
		;lda cutscenes_palettes_hi,y
		;tay
		;jsr NES::loadPalette	
		SetupFader PAL_continue,#$10	


		NES_loadNametableC NAM_continue,0

		lda #0
		sta continue

		ldx #0
		lda #3
		jsr ft_music_init
				
		lda #Mode_Continue
		sta mode
		lda #%10000000
		sta ToPPUControl
		lda #%00011110 
		sta ToPPUMask
		TurnPPUOn     
	rts

Continue:
		jsr ft_music_play


		lda #NES::JOY::START
		and joyStatusLock
		beq :++
			lda continue
			beq :+
				lda #Mode_SetTitleScreen
				sta mode
				rts
			:
				lda #Mode_SetJumpGame
				sta mode
				rts
		:     


		lda #NES::JOY::UP
		ora #NES::JOY::DOWN
		ora #NES::JOY::SELECT
		and joyStatusLock
		beq :+++			
			lda continue
			beq :+
				lda #0
				jmp:++	
			:
				lda #16
			:
			sta continue
		:   

		lda continue
		clcadc #(12*8)
		sta tmp3 		

		NES_SETNEXTSPRITE #(14*8),tmp3,#$F8,#0
		;#$F8
	rts

;;;;;;;;;;;;;;;; CUTSCENES ;;;;;;;;;;;;;;;;;;;;
.include "src/cutscener.s"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


SetHenshin:
        lda #0
        sta $2001  
	lda #3
	sta vBlankSubHandler
	DisableIRQCounter
	SetMinisongsBanks

	;Setup vert mirroring
	lda #0
	sta $A000

	NES_MMC3SetBank #NES::MMC3TARGETS::CHRx0000, #4
	NES_MMC3SetBank #NES::MMC3TARGETS::CHRx0800, #6

	;;Setup banks
	ldx #0
	lda #$82
	jsr NES::MMC3SetBank
	ldx #1
	lda #$83
	jsr NES::MMC3SetBank	
	ldx #14
	lda #$84
	jsr NES::MMC3SetBank
	ldx #15
	lda #$85
	jsr NES::MMC3SetBank
				
        NES_loadPalette	   PAL_henshin
        NES_loadNametableC NAM_henshin_empty,0
        NES_loadNametableC NAM_henshin_utaco,1


	lda #80
	sta hd+HenshinData::counter
	lda #0
	sta hd+HenshinData::mode
	sta hd+HenshinData::sx
	sta hd+HenshinData::sx+1
	sta hd+HenshinData::sy
	sta hd+HenshinData::sy+1

	lda #<minisongs_musicdata
	sta ft_music_addr
	lda #>minisongs_musicdata
	sta ft_music_addr+1
		
	ldx #0
	lda #1
        jsr ft_music_init

        		
        lda #Mode_Henshin
        sta mode
  
	lda #%10001000
	sta ToPPUControl
	lda #%00011110 
	sta ToPPUMask
	TurnPPUOn
  
	rts




Henshin:

	jsr ft_music_play

	lda hd+HenshinData::mode
	cmp #0	;INITIAL FLASH
	bne HenshinModes_Flasher
		dec hd+HenshinData::counter
		lda hd+HenshinData::counter
		bne :+
			lda #0
			sta $2001  
		        NES_loadPalette PAL_henshin
			TurnPPUOn
			lda #32
			sta hd+HenshinData::sx
			lda #160
			sta hd+HenshinData::counter
			inc hd+HenshinData::mode
		:		
		jmp EndOfHenshinModeSwitch	
	HenshinModes_Flasher:
	cmp #1	;INITIAL SCROLL
	bne HenshinModes_FirstScroller
		FPA_INC_16 hd+HenshinData::sx
		FPA_INC_16 hd+HenshinData::sx
		dec hd+HenshinData::counter
		lda hd+HenshinData::counter
		bne :+
			lda #0
			sta hd+HenshinData::sx
			sta hd+HenshinData::sx+1
			sta hd+HenshinData::sprX			
			lda #160
			sta hd+HenshinData::counter
			inc hd+HenshinData::mode
		:		
		jmp EndOfHenshinModeSwitch	
	HenshinModes_FirstScroller:
	cmp #2 ;SECOND SCROLLER
	bne HenshinModes_SecondScroller
		lda hd+HenshinData::sprX
		cmp #112
		bcs :+
			inc hd+HenshinData::sprX
		:
		NES_ADDSPRITE_NxM #4,#6,hd+HenshinData::sprX,#96,#$9C,#00
		dec hd+HenshinData::counter
		lda hd+HenshinData::counter
		bne :+
			lda #0
			sta hd+HenshinData::sprX
			lda #100
			sta hd+HenshinData::counter
			inc hd+HenshinData::mode
		:		
		jmp EndOfHenshinModeSwitch	
	HenshinModes_SecondScroller:
	cmp #3 ;NOTE DROP
	bne HenshinModes_NoteDrop
		inc hd+HenshinData::sprX
		NES_ADDSPRITE_NxM #4,#6,#112,#96,#$9C,#00

		lda hd+HenshinData::sprX
		lsr
		lsr
		tax
		and #3
		asl
		clcadc #$b0
		sta 6
		txa
		lsr
		lsr
		and #1
		asl
		asl
		asl
		asl
		asl
		clcadc 6
		sta 6
		

		NES_ADDSPRITE_NxM #2,#2,#120,hd+HenshinData::sprX,6,#00
		dec hd+HenshinData::counter
		lda hd+HenshinData::counter
		bne :+
			lda #100
			sta hd+HenshinData::counter
			inc hd+HenshinData::mode
		:		
		jmp EndOfHenshinModeSwitch	
	HenshinModes_NoteDrop:
	cmp #4 ;HENSHIN!!	
	bne HenshinModes_Henshin
		NES_ADDSPRITE_NxM #4,#6,#112,#96,#$98,#01
		dec hd+HenshinData::counter
		lda hd+HenshinData::counter
		bne :+
			lda #Mode_SetJumpGame
		        sta mode
			rts
		:		
		jmp EndOfHenshinModeSwitch	
	HenshinModes_Henshin:
	EndOfHenshinModeSwitch:



	rts

.segment "SBANK_4"


HenshinVBlank:
	lda hd+HenshinData::mode
	cmp #0	;INITIAL FLASH
	bne :+++	
		lda hd+HenshinData::counter
		lsr 
		and #1
		beq :+
			lda #$f
			jmp :++
		:
			lda #$4
		:

		bit $2002
		ldx #$3F
		stx $2006
		ldx #$00
		stx $2006		
		sta $2007
		sta $2007
		jmp EndOfHenshinModeSwitchVBlank	
	:
	cmp #4
	bne:+
		bit $2002
		ldx #$3F
		stx $2006
		ldx #$00
		stx $2006		
		lda #$30
		sta $2007
		sta $2007
		jmp EndOfHenshinModeSwitchVBlank	
	:
	EndOfHenshinModeSwitchVBlank:

	bit $2002
	lda hd+HenshinData::sx
	sta $2005
	lda hd+HenshinData::sy
	sta $2005

	lda hd+HenshinData::sx+1
	and #1
	ora ToPPUControl
	
	sta $2000
	bit $2002
	rts

.segment "CODE"		
	
StandardVBlank:
		LoadFaderPalette
		bit $2002
		lda #0
		sta $2005		
		sta $2005
		lda ToPPUControl
		sta $2000
	rts
		
SuperMagicMainLoopDispatcher:
    asl                                 
    tax
    lda MainLoopModesTable+1,x                                       
    pha
    lda MainLoopModesTable,x      
    pha
    rts             
	
vBlankSubHandlerDispatcher:    
    asl                                 
    tax
    lda vBlankSubHandlersTable+1,x                                       
    pha
    lda vBlankSubHandlersTable,x      
    pha 
	rts     	





;Takes palette addr from PtrA
;A: multiplier
;X: direction
setupFader:
	sta fadeMultiplier
	sta fadeCounter
	stx fadeDirection
	
	ldy #0
	
	:
		lda (PtrA),y
		and #$0f
		sta fadePalette,y
		lda (PtrA),y
		sta fadePaletteTarget,y				

	iny
	cpy #32
	bne :-

	lda #0
	sta fadePaletteChanged
	

	rts

startFaderOut:
	lda #0
	sta fadePaletteChanged
	lda #$f0
	sta fadeDirection
	rts

doFader:
	lda fadePaletteChanged
	cmp #5
	bne :+
		rts
	:
	dec fadeCounter
	bne fadeCounterIsNotReady

		lda fadeMultiplier
		sta fadeCounter
		
		ldy #0
		
		paletteLoop:
			lda fadePalette,y
			clcadc fadeDirection

			ldx fadeDirection
			cpx #$10
			bne :++
				cmp fadePaletteTarget,y
				bcc :+
					lda fadePaletteTarget,y
				:
				jmp :+++
			:
				cmp fadePaletteTarget,y
				bcc :+
					lda #$0f
				:
				;Goin down
			:
			
		
			sta fadePalette,y
		iny
		cpy #32
		bne paletteLoop
		
		inc fadePaletteChanged

	fadeCounterIsNotReady:


	rts
    
                
;;||=============================================================||;;
NMI:                        
        inc vblank         
        rti

;;||=============================================================||;;        

		
;Consts
MinusOne16: .byte $FF,$FF
MinusDosCincuenta24: .byte $00,$06,$FF

CSTextBase: .byte $00,$22

MainLoopModesTable:
	.word SetTitleScreen-1
	.word TitleScreen-1
	.word SetJumpGame-1
	.word JumpGame-1
	.word CutSceneLoad-1
	.word CutSceneLoop-1
	.word NewGame-1
	.word SelectSongResult-1
	.word SongPass-1
	.word SetFailScreen-1
	.word FailScreen-1
	.word SetContinue-1
	.word Continue-1
	.word SetLanguageSelectScreen-1
	.word LanguageSelectScreen-1
	.word SetChuuiScreen-1
	.word ChuuiScreen-1
	.word SetCredits-1
	.word Credits-1
	.word SetHenshin-1
	.word Henshin-1
	.word EOG-1
	.word EOGRun-1

Mode_SetTitleScreen=0
Mode_TitleScreen=1
Mode_SetJumpGame=2
Mode_JumpGame=3
Mode_CutSceneLoad=4
Mode_CutSceneLoop=5
Mode_NewGame=6
Mode_SelectSongResult=7
Mode_SongPass=8
Mode_SetFailScreen=9
Mode_FailScreen=10
Mode_SetContinue=11
Mode_Continue=12
Mode_SetLanguageSelectScreen=13
Mode_LanguageSelectScreen=14
Mode_SetChuuiScreen=15
Mode_ChuuiScreen=16
Mode_SetCredits=17
Mode_Credits=18
Mode_SetHenshin=19
Mode_Henshin=20
Mode_EOG=21
Mode_EOGRun=22


	
vBlankSubHandlersTable:
.word StandardVBlank-1
.word JumpGameVBlanker-1	
.word CutsceneVBlank-1
.word HenshinVBlank-1

JumpColumnEmpty:	.byte $01,$00,$00,$00,$00,$00,$00,$00
JumpColumnPylon0:	.byte $02,$64,$54,$44,$34,$24,$14,$04
JumpColumnPylon1:	.byte $01,$6E,$6E,$6E,$6E,$6E,$5E,$4E
JumpColumnPylonE:	.byte $02,$00,$00,$00,$00,$00,$00,$00

EmptyBeatBar1:	.byte $42,$43,$42,$43,$42,$43,$42,$43,$42,$43,$42,$43,$42,$43,$42,$43,$42,$43,$42,$43
EmptyBeatBar2:	.byte $52,$53,$52,$53,$52,$53,$52,$53,$52,$53,$52,$53,$52,$53,$52,$53,$52,$53,$52,$53

PAL_ingame:	
	.byte $0f,$1c,$20,$15,$0f,$01,$21,$16,$0f,$10,$00,$30,$0f,$09,$1c,$2c
	.byte $0f,$0f,$2c,$30,$0f,$0f,$36,$2c,$0f,$36,$2c,$30,$0f,$3f,$36,$30


NAM_ingame_attribs: .incbin "romdata/nam/ingame_attribs.nm"

.include "romdata/meta_sprites/utaco_run.s"
UTACOSP:          FPA_MAKEFP 1,0,30

	
;Song data tables	
SDList_MusicLo: .byte <songdata1_musicdata,<songdata2_musicdata,<songdata3_musicdata
SDList_MusicHi: .byte >songdata1_musicdata,>songdata2_musicdata,>songdata3_musicdata
SDList_Bank1:   .byte 0,2,4
SDList_Bank2:   .byte 1,3,5
SDList_CommandsLo: .byte <first_stage_beats_commands,<second_stage_beats_commands,<third_stage_beats_commands
SDList_CommandsHi: .byte >first_stage_beats_commands,>second_stage_beats_commands,>third_stage_beats_commands
SDList_BeatsLo: .byte <first_stage_beats_beats,<second_stage_beats_beats,<third_stage_beats_beats
SDList_BeatsHi: .byte >first_stage_beats_beats,>second_stage_beats_beats,>third_stage_beats_beats


SDList_Cutscene: .byte cutscene_destroyed_city,cutscene_utaco_temple_2,cutscene_mothership_2,cutscene_no_mothership-1


SDList_pILo: .byte <first_stage_beats_pI,<second_stage_beats_pI,<third_stage_beats_pI
SDList_pIHi: .byte >first_stage_beats_pI,>second_stage_beats_pI,>third_stage_beats_pI

SDList_pointsLo: .byte <first_stage_beats_points,<second_stage_beats_points,<third_stage_beats_points
SDList_pointsHi: .byte >first_stage_beats_points,>second_stage_beats_points,>third_stage_beats_points
SDList_pointsMinusLo: .byte <first_stage_beats_points_minus,<second_stage_beats_points_minus,<third_stage_beats_points_minus
SDList_pointsMinusHi: .byte >first_stage_beats_points_minus,>second_stage_beats_points_minus,>third_stage_beats_points_minus

SDList_StringsLoLo: .byte <first_stage_beats_strings_lo,<second_stage_beats_strings_lo,<third_stage_beats_strings_lo
SDList_StringsLoHi: .byte >first_stage_beats_strings_lo,>second_stage_beats_strings_lo,>third_stage_beats_strings_lo
SDList_StringsHiLo: .byte <first_stage_beats_strings_hi,<second_stage_beats_strings_hi,<third_stage_beats_strings_hi
SDList_StringsHiHi: .byte >first_stage_beats_strings_hi,>second_stage_beats_strings_hi,>third_stage_beats_strings_hi

PAL_title: .incbin "romdata/title.dat"
NAM_gameover: .incbin "romdata/nam/gameover.nm"
NAM_continue: .incbin "romdata/nam/continue.nm"

NAM_langselect: .incbin "romdata/nam/langselect.nm"
NAM_chuui: 	.incbin "romdata/nam/chuui.nm"
NAM_title: 	.incbin "romdata/nam/title.nm"   
NAM_credits:	.incbin "romdata/nam/credits.nm"
PAL_credits:	.byte $30,$00,$10,$0f,$30,$01,$21,$16,$30,$06,$16,$00,$30,$09,$1c,$1c,$30

NAM_henshin_utaco: .incbin "romdata/nam/henshin_utaco.nm"
NAM_henshin_empty: .incbin "romdata/nam/henshin_empty.nm"

PAL_henshin:	.byte $0f,$04,$2c,$36,$0f,$25,$2c,$36,$0f,$04,$1c,$0c,$0f,$09,$19,$29
		.byte $0f,$36,$0f,$1c,$0f,$36,$1c,$0f,$0f,$04,$24,$15,$0f,$09,$19,$29


PAL_continue:	.byte $0f,$16,$16,$30,$0f,$01,$21,$31,$0f,$06,$16,$26,$0f,$09,$19,$30
		.byte $0f,$16,$16,$2C,$0f,$01,$21,$31,$0f,$06,$16,$26,$0f,$09,$19,$29

.segment "SBANK_5"
.include "romdata/cutscenes.s"

	
.segment "DPCM"
    .incbin "romdata/music/first_stage_samples.bin"        	
	

.segment "INT_VECTORS"
		.word NMI,reset,IRQMagic      

.segment "SBANK_5"

PAL_langselect: .byte $0f,$16,$16,$30,$0f,$01,$21,$31,$0f,$06,$16,$26,$0f,$09,$19,$29
		.byte $0f,$16,$16,$30,$0f,$01,$21,$31,$0f,$06,$16,$26,$0f,$09,$19,$29
		
		
