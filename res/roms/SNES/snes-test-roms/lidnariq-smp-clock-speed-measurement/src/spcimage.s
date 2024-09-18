.setcpu "none"
.include "spc-65c02.inc"

.macro stya addr
.local addr1
addr1 = addr
  .assert addr <= $00FE, error, "stya works only in zero page"
  movw <addr, ya
.endmacro

; MMIO at $00F0-$00FF
TIMEREN     := $F1  ; 0-2: enable timer; 7: enable ROM in $FFC0-$FFFF
DSPADDR     := $F2
DSPDATA     := $F3
SNESIO0     := $F4
SNESIO1     := $F5
SNESIO2     := $F6
SNESIO3     := $F7
TIMERPERIOD := $FA  ; Divisors for timers (0, 1: 8 kHz base; 2: 64 kHz base)
TIMERVAL    := $FD  ; Number of times timer incremented (bits 3-0; cleared on read)

DSP_CLVOL    = $00
DSP_CRVOL    = $01
DSP_CFREQLO  = $02  ; Playback frequency in 7.8125 Hz units
DSP_CFREQHI  = $03  ; (ignored 
DSP_CSAMPNUM = $04  
DSP_CATTACK  = $05  ; 7: set; 6-4: decay rate; 3-0: attack rate
DSP_CSUSTAIN = $06  ; 7-5: sustain level; 4-0: sustain decay rate
DSP_CGAIN    = $07  ; Used only when attack is disabled

DSP_LVOL     = $0C
DSP_RVOL     = $1C
DSP_LECHOVOL = $2C
DSP_RECHOVOL = $3C
DSP_KEYON    = $4C
DSP_KEYOFF   = $5C
DSP_FLAGS    = $6C  ; 5: disable echo; 4-0: set LFSR rate
DSP_FMCH     = $2D  ; Modulate these channels' frequency by the amplitude before it
DSP_NOISECH  = $3D  ; Replace these channels with LFSR noise
DSP_ECHOCH   = $4D  ; Echo comes from these channels
DSP_SAMPDIR  = $5D  ; High byte of base address of sample table

.segment "SPCZEROPAGE"

.segment "SPCIMAGE"
.align 256
.global spc_entry	
spc_entry:
;	lda SNESIO0

waitForChange:	
	cmp <SNESIO0	; 2
	beq waitForChange	; 2
	lda <SNESIO0	; 3

	ldx #0	; 2
	ldy #0	; 2
spinloop:	
	inx	; 2
	bne spinloop	; 4 -> 256*6=1536-2 = 1534
	iny	; 2
	bne spinloop	; 4 -> 256*1534=392704-2 = 392702
	sta <SNESIO0	; 3 -> +14 = 392716 / 1024000 nom. = 383.5ms
	jmp waitForChange

;;; on Mesen2
;;; start - 298919036
;;; end - 299707528
;;; diff = 788492 ??
;;; apparently Mesen2 "cycles" are 2 for each of the above clocks
;;; 394246? 1530 extra? Where'd I err?
