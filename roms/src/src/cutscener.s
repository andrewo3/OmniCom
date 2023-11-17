.segment "CODE"	

CutSceneLoad:		
	lda #0
        sta $2001  
	lda #2
	sta vBlankSubHandler
	
	DisableIRQCounter

	lda #6
	sta $8000
	lda #8
	sta $8001
	lda #7
	sta $8000
	lda #9
	sta $8001

	;Setup vert mirroring
	lda #1
	sta $A000
	
	;Si end, goto next stage		
	ldy curCutscene		
	ldx cutscenes_commands,y
	cpx #$FF
	bne :++
		lda csHalt
		beq :+
			lda #Mode_EOG
			sta mode
			rts
		:
			lda #Mode_SetHenshin
			sta mode
			rts
	:
	cpx #$FE
	bne :+
		lda #1
		sta csHalt
		inc curCutscene
		rts
	:

	;Sprite banks
	NES_MMC3SetBank #NES::MMC3TARGETS::CHRx0000, #$C
	NES_MMC3SetBank #NES::MMC3TARGETS::CHRx0800, #$E

	;;SET CUTSCENE BANKS
	ldy curCutscene		
	ldx cutscenes_banks_0,y
	lda #$82
	jsr NES::MMC3SetBank

	ldy curCutscene		
	ldx cutscenes_banks_1,y
	lda #$83
	jsr NES::MMC3SetBank
		
	;;SET FONT BANKS
	ldx #10
	lda #$84
	jsr NES::MMC3SetBank
	ldx #11
	lda #$85
	jsr NES::MMC3SetBank
																
	;; SET NAMETABLE
	ldy curCutscene		
	ldx cutscenes_nametables_lo,y
	lda cutscenes_nametables_hi,y
	tay
	lda #$20
	jsr NES::loadNametableC    

	;; SET MICII Visibility
	lda #0
	ldy curCutscene		
	ldx cutscenes_nametables_lo,y
	cpx #<CS_NAM_utaco_temple
	bne :+
	ldx cutscenes_nametables_hi,y
	cpx #>CS_NAM_utaco_temple	
	bne :+
		lda #1
	:
	sta csMIIVisible

	;; SET NEXT SONG
	lda #0
	ldy curCutscene		
	ldx cutscenes_nametables_lo,y
	cpx #<CS_NAM_lab
	bne :+
	ldx cutscenes_nametables_hi,y
	cpx #>CS_NAM_lab	
	bne :+
		lda #1
	:
	sta csNextSong
	
		
	;;SET PALETTES		
	ldy curCutscene		
	lda cutscenes_palettes_lo,y
	sta PtrAL
        lda cutscenes_palettes_hi,y
	sta PtrAH
	      
	lda #8
	ldx #$10
        jsr setupFader

	lda #0
	sta csImFadingOut
	sta csCleanPlease

	lda #$FF
	sta csCurStr

	lda csNextSong
	cmp csCurSong
	bne CambiaLaCancion

	lda ft_music_addr
	cmp #<minisongs_musicdata_at_cutscenes	
	beq:+
		CambiaLaCancion:
		lda #<minisongs_musicdata_at_cutscenes
		sta ft_music_addr
		lda #>minisongs_musicdata_at_cutscenes
		sta ft_music_addr+1
		ldx #0
		lda csNextSong
		jsr ft_music_init
	:

	lda csNextSong
	sta csCurSong	

	jsr ResetAndLoadNextString_zenpen
	jsr ResetAndLoadNextString_kouhen

	lda #%10001000
	sta ToPPUControl
	lda #%00011110 
	sta ToPPUMask

	lda #Mode_CutSceneLoop
	sta mode		
	rts

ResetAndLoadNextString_zenpen:
		
		lda #$00
		sta csStringReady		
		lda #$66
		sta csPPUPos
		lda #$22
		sta csPPUPos+1				
		lda #2
		sta csFrameDelay
		sta csFrameCounter
		
		lda #20
		sta csColCounter
		
		lda language
		cmp #0
		bne :+
			ldy curCutscene
			lda cutscenes_strings_en_lo_lo,y
			sta NES::PtrAL
			lda cutscenes_strings_en_lo_hi,y
			sta NES::PtrAH
			lda cutscenes_strings_en_hi_lo,y
			sta NES::PtrBL
			lda cutscenes_strings_en_hi_hi,y
			sta NES::PtrBH
			jmp EndOfLanguageSelectionForNextString
		:
		cmp #1
		bne :+
			ldy curCutscene
			lda cutscenes_strings_es_lo_lo,y
			sta NES::PtrAL
			lda cutscenes_strings_es_lo_hi,y
			sta NES::PtrAH
			lda cutscenes_strings_es_hi_lo,y
			sta NES::PtrBL
			lda cutscenes_strings_es_hi_hi,y
			sta NES::PtrBH
			jmp EndOfLanguageSelectionForNextString
		:

		ldy curCutscene
		lda cutscenes_strings_ja_lo_lo,y
		sta NES::PtrAL
		lda cutscenes_strings_ja_lo_hi,y
		sta NES::PtrAH
		lda cutscenes_strings_ja_hi_lo,y
		sta NES::PtrBL
		lda cutscenes_strings_ja_hi_hi,y
		sta NES::PtrBH		

	EndOfLanguageSelectionForNextString:	
		

		inc csCurStr
		lda csCurStr ;String number for this cutscene
		tay
		lda (NES::PtrA),y
		sta csStrAddr
		lda (NES::PtrB),y
		sta csStrAddr+1
		cmp #0
		bne :+
			lda #$1
			sta csStringReady
			rts
		:

rts
		
ResetAndLoadNextString_kouhen:
	;;; Clear cutscene area
	lda #0
	sta $2001

	bit $2002
	lda #$22
	sta $2006
	lda #$66
	sta $2006
	lda #$C0
	ldy #$F4
	:	sta $2007
		dey
		bne :-

	;bit $2002
	;lda #0
	;sta $2005		
	;sta $2005
	;lda ToPPUControl
	;sta $2000
	;lda ToPPUMask
        ;sta $2001        
				
		
rts


TextColumns=20

		
CutSceneLoop:
	
		jsr ft_music_play
		lda csImFadingOut
		beq :+
			jmp HereIsWhereIGoDirectlyWhenFadingOut
		:

		lda csCleanPlease
		beq :+
			jsr ResetAndLoadNextString_kouhen
			lda #0
			sta csCleanPlease			
		:

		;fade in
		;if faded in, output text 
		;when text is over, increment scene
		; and fade out		
		lda csStringReady
		bne CSStringIsDone	
			dec csFrameDelay
			bne EndOfNewCharacter
				lda csFrameCounter
				sta csFrameDelay
				FPA_INC_16 csPPUPos
				FPA_INC_16 csStrAddr
				
				ldy #0
				lda (csStrAddr),y			
				cmp #<($FE+FontOffset)
				bne :+					
					lda #$E0
					and csPPUPos
					sta csPPUPos
					lda #38
					FPA_ADD_16_A csPPUPos
					FPA_INC_16 csStrAddr
					lda #TextColumns
					sta csColCounter				
					jmp :++
				:
					dec csColCounter
				:
								
				bne :+
					lda #$20-TextColumns
					FPA_ADD_16_A csPPUPos
					lda #TextColumns
					sta csColCounter
				:
			EndOfNewCharacter:
		CSStringIsDone:
		
		;Skip on Start
		lda csHalt
		bne :+
		lda #NES::JOY::START
		and joyStatusLock
		beq :+
		    lda #Mode_SetHenshin
		    sta mode
		:    
		
		;Next!
		lda #NES::JOY::BUT_B	
		and joyStatusLock
	        beq :++++
			lda csStringReady
			beq :++				
				jsr ResetAndLoadNextString_zenpen
				lda #1
				sta csCleanPlease
				lda csStrAddr+1
				bne :+
					jsr startFaderOut
					lda #1
					sta csImFadingOut
				:
				jmp :++
			:
				lda #1
				sta csFrameCounter
			:
        	:  		


		;mic2
		lda csMIIVisible
		beq DontDrawMicII
			inc csFC
			lda csFC
			cmp floatytable_entries
			bcc :+
				lda #0
				sta csFC
			:
			tay
			lda floatytable,y
			sta tmp3
			NES_ADDSPRITE_NxM #3,#4,#60,tmp3,#$C3,#0
			NES_SETNEXTSPRITE #68,tmp3,#$C4,#1
			lda tmp3
			clcadc #16
			sta tmp3
			NES_ADDSPRITE_NxM #3,#1,#60,tmp3,#$B3,#2
		DontDrawMicII:

HereIsWhereIGoDirectlyWhenFadingOut:

		BranchIfFadeIsNotYetOut :+
			inc curCutscene
			lda #Mode_CutSceneLoad
			sta mode	
		:
		
		;fade out (when fade out, go to load)
		rts

CutsceneVBlank:		
		bit $2002
		LoadFaderPalette				


		lda csStringReady
		bne :++
			lda csPPUPos+1
			sta $2006
			lda csPPUPos
			sta $2006
			ldy #0
			lda (csStrAddr),y		
			cmp #<($FF+FontOffset)
			bne :+
				inc csStringReady
				jmp :++
			:
			sta $2007
		:
		
		bit $2002
		lda #0
		sta $2005		
		sta $2005
		lda ToPPUControl
		sta $2000
	rts

EOG:
        lda #0
        sta $2001  
	sta vBlankSubHandler

	DisableIRQCounter

	;Disable APU Channels
        lda #$00
	sta $4015 
 
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

	SetupFader cutscenes_pal_destroyed_city,#$10
        NES_loadNametableC CS_NAM_fin,0

        lda #Mode_EOGRun
        sta mode
	TurnPPUOn  
	rts

EOGRun:
	rts



.include "src/floatytable.s"

.segment "SBANK_5"

CS_NAM_destroyed_city:	.incbin "romdata/nam/destroyed_city.nm"   	
CS_NAM_mothership:	.incbin "romdata/nam/mothership.nm"   	
CS_NAM_temple_ruins:	.incbin "romdata/nam/temple_ruins.nm"   	
CS_NAM_utaco_temple:	.incbin "romdata/nam/utaco_temple.nm"   
CS_NAM_lab:		.incbin "romdata/nam/lab.nm"		
CS_NAM_no_mothership:	.incbin "romdata/nam/no_mothership.nm"   	
CS_NAM_fin:		.incbin "romdata/nam/fin.nm"   	
CS_NAM_cutscene_empty:	.incbin "romdata/nam/cutscene_empty.nm"   	

minisongs_musicdata_at_cutscenes:    .incbin "romdata/music/cutscenes_music.bin"


.segment "CODE"
