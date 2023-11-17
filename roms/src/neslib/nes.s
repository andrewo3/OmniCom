;
;       Some NES utility functions and things
;       zlashstudios 2010
;


;Adds another sprite to the sprite buffer in ram 
;and advances the index to the next space
.macro NES_SETNEXTSPRITE sx, sy, index, attrb
	lda NES::spritesIndex
        tay
        sec
        sbc #$4	        
        sta NES::spritesIndex
	lda sy
	sta NES::sprites,y
	lda index
	sta NES::sprites+1,y
	lda attrb
	sta NES::sprites+2,y
	lda sx
	sta NES::sprites+3,y		
.endmacro

;Draws a NxM sprite at sx,sy using index as the top-left corner index
.macro NES_ADDSPRITE_NxM N,M,sx,sy,index,attrb
        lda sx
        sta TempAL
        lda sy
        sta TempAH
        lda index
        sta TempBL
        lda attrb
        sta TempBH
        ldx N
        ldy M
        jsr NES::loadNxMSprite
.endmacro

;Sends the current batch of sprites through DMA to PPU
;and resets the counter.
.macro  NES_DMASPRITES
        ldx #$05
        stx $4014
        ldx #$FC
        stx NES::spritesIndex
.endmacro

;MMC3 helper to change bank
.macro NES_MMC3SetBank target,bnk
        ldx bnk
        lda target
        jsr NES::MMC3SetBank
.endmacro

;Macro helper to load palettes
.macro NES_loadPalette addr
        ldx #<(addr)
        ldy #>(addr)
        jsr NES::loadPalette
.endmacro

;Macro helper to load nametables
.macro NES_loadNametable addr, pos
        ldx #<(addr)
        ldy #>(addr)
        lda #($20 + pos*8)
        jsr NES::loadNametable     
.endmacro

;Macro helper to load compressed nametables
.macro NES_loadNametableC addr, pos
        ldx #<(addr)
        ldy #>(addr)
        lda #($20 + pos*4)
        jsr NES::loadNametableC     
.endmacro

.macro phy operand
        tya
        pha
.endmacro
.macro phx operand
        txa
        pha
.endmacro
.macro ply operand
        pla
        tay
.endmacro
.macro plx operand
        pla
        tax
.endmacro

;Macro helper to avoid writing a clc and an adc everytime
.macro clcadc operand
        clc
        adc operand
.endmacro

;Macro helper to avoid writing a sec and an sbc everytime
.macro secsbc operand
        sec
        sbc operand
.endmacro

;Macro helper that acts as a mod
.macro pseudomod operand
.local check,end
check:
    cmp operand
    bcc end
    secsbc operand
    jmp check
end:
.endmacro

;Fills memory at dst with a
.macro EXPANDED_MEMSET dst,n
	.repeat n,I        
        sta dst+I
    .endrepeat
.endmacro

;Macro helper to memcpy
.macro MEMCPY dst,src,n        
        .if ( .xmatch ({n}, #1) )                
                lda src
                sta dst
        .elseif ( .xmatch ({n}, #2) )                
                lda src
                sta dst
                lda src+1
                sta dst+1
        .elseif ( .xmatch ({n}, #3) )                
                lda src
                sta dst
                lda src+1
                sta dst+1
                lda src+2
                sta dst+2
        .else
                lda #<(src)
                sta PtrAL
                lda #>(src)
                sta PtrAH
                lda #<(dst)
                sta PtrBL
                lda #>(dst)
                sta PtrBH
                ldy n
                jsr NES::memcpy
       .endif
.endmacro

.macro clearPPU
        lda #%00000000 
        sta $2005
		sta $2005
		sta $2000
        sta $2001
		
.endmacro

.segment "ZEROPAGE"
;8 Temp values to use across all the modules
TempA:
TempAL:         .res 1
TempAH:         .res 1
TempAHP:        .res 1
TempB:
TempBL:         .res 1
TempBH:         .res 1
TempBHP:        .res 1
TempC:
TempCL:         .res 1
TempCH:         .res 1
TempCHP:        .res 1
TempD:
TempDL:         .res 1
TempDH:         .res 1
TempDHP:        .res 1
TempE:
TempEL:         .res 1
TempEH:         .res 1
TempEHP:        .res 1

PtrA:
PtrAL:          .res 1
PtrAH:          .res 1
PtrB:
PtrBL:          .res 1
PtrBH:          .res 1

.segment "BSS"

nzpTempA:
nzpTempAL:         .res 1
nzpTempAH:         .res 1
nzpTempAHP:        .res 1
nzpTempB:
nzpTempBL:         .res 1
nzpTempBH:         .res 1
nzpTempBHP:        .res 1


.scope NES

.scope MMC3TARGETS
        CHRx0000 = 0
        CHRx0800 = 1
        CHRx1000 = 2
        CHRx1400 = 3
        CHRx1800 = 4
        CHRx1C00 = 5
        PRGx8000 = 6
        PRGxA000 = 7                
.endscope

.scope JOY
        BUT_A   = $01
        BUT_B   = $02
        SELECT  = $04
        START   = $08
        UP      = $10
        DOWN    = $20
        LEFT    = $40
        RIGHT   = $80
.endscope


.segment "ZEROPAGE"
spritesIndex:    .res 1

.segment "BSS"
scrollX:        .res 1
scrollY:        .res 1
mmc3Setting:    .res 1

.export mmc3Setting

.segment "DMA500"
sprites:        .res $ff


.segment "CODE"

;Copies y bytes from (PtrA) to (PtrB)
memcpy:
        dey
        lda (PtrA),y
        sta (PtrB),y        
        tya
        bne memcpy
        rts
	

;Loads a nametable on the address pointed by the 
; x (low) and y(high) registers A (origin)
loadNametable:                
        stx PtrAL
        sty PtrAH           
        sta $2006
        lda #$00
        sta $2006
        ldx #4
        ldy #0
:
        lda (PtrA),y
        sta $2007
        iny
        bne :-
        inc PtrAH
        dex
        bne :- 
        rts
        
;Loads a RLE compressed nametable on the address pointed by the 
; x (low) and y(high) registers A (origin)
loadNametableC:                
        stx PtrAL
        sty PtrAH           
        sta $2006
        lda #$00
        sta $2006
        
beginNTRLE:        
        ldy #0        
        lda (PtrA),y
        beq endNTRLE
        tax
        iny        
        lda (PtrA),y
:        
        sta $2007
        dex
        bne :- 
        lda PtrAL
        clcadc #2
        sta PtrAL
        lda #0
        adc PtrAH
        sta PtrAH
        jmp beginNTRLE
endNTRLE:        
        rts

;Fills a nametable with the X value registers A (origin)
fillNametable:                                
        sta $2006
        lda #$00
        sta $2006
        txa
        ldx #4
        ldy #0
:        
        sta $2007
        iny
        bne :-        
        dex
        bne :- 
        rts        
        
        
;Loads a palette on the address pointed by the 
; x (low) and y(high) registers
loadPalette:	
		stx PtrAL
        sty PtrAH     
		lda #$3F
		sta $2006
		lda #$00
		sta $2006
		ldy #$00
:
		lda (PtrA), y
		sta $2007
		iny
		cpy #$20
		bne :-
        rts
        
        
;Clears all the sprites that haven't been used
clearRemainingSprites:
        lda #$F7
        ldy spritesIndex
:                
        sta sprites,y
        dey
        dey
        dey
        dey
        cpy #$FC
        bne :-
        rts
       
;MMC3 Swaps the x bank into the A target
MMC3SetBank:
        sta TempA
        lda mmc3Setting
        and #$C0
        ora TempA
        sta $8000               
        stx $8001
        rts

;Loads a NxM Sprite to sx,sy: TempA
; index, attrb: TempB
; n=x, m=y
loadNxMSprite:
        stx TempCL
        lda TempAL
        sta TempEL
yloop:        
xloop:        
        tya
        pha
        NES_SETNEXTSPRITE TempAL,TempAH,TempBL,TempBH
        pla
        tay
        
        lda TempAL
        clcadc #8
        sta TempAL
        inc TempBL        
        dex
        bne xloop
        ldx TempCL
        
        lda TempEL
        sta TempAL
        lda TempAH
        clcadc #8
        sta TempAH
        lda TempBL
        clcadc #$10
        secsbc TempCL        
        sta TempBL
        
        dey
        bne yloop

        rts

.endscope
