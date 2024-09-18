; Portions copyright 2014-2015 Damian Yerrick

;;; other bits written by lidnariq

; Copying and distribution of this file, with or without
; modification, are permitted in any medium without royalty provided
; the copyright notice and this notice are preserved in all source
; code copies.  This file is offered as-is, without any warranty.

;;; Damian Yerrick's original work is released under the zlib license.

;;; Lidnariq's modifications are also available under zlib

.include "snes.inc"
.macpack generic
	
.smart
.export main, nmi_handler
.import spc_boot_apu	

.ZEROPAGE
nmicount:	.byte 0
apuphase:	.byte 0
timetaken:	.dword 0
fastesttime:	.dword 0    ; these will be raw counts
slowesttime:	.dword 0
ptr:	.word 0
dividend:	.word 0,0,0
subtrahend:	.word 0,0,0
difference:	.word 0,0,0
divisor:	.word 0,0
quotient:	.dword 0
nextbit:	.dword 0
multiplicand:	.byte 0,0,0
multiplier:	.word 0
product:	.byte 0,0,0,0,0
ppm:	.dword 0

;;; 24-bit input plus 7 nybble output fits in 8 bytes
bcdres:	.dword 0,0
bcdTemp:	.byte 0
bcdStep:	.byte 0
flags:	.byte 0

.BSS
nametable:	.res 1024
	
.RODATA
zeroword:	.word 0
hex:	.byte "0123456789abcdef"
font:	.incbin "pearl.chr2"
afterfont:
;;; screen layout
;;;    0123456789a123456789b123456789c1
;;; 00
;;; 01
;;; 02
;;; 03  SNES PPU:              60 Hz
;;; 04  Assumed CPU clock: 21.477 MHz
;;; 05
;;; 06  Raw reading:   .....
;;; 07
;;; 08  Meaning:      ...... us
;;; 09  Slowest:      ...... us
;;; 10  Fastest:      ...... us
;;; 11
;;; 12  S-SMP clock:   ....... Hz
;;; 13  relative:       +..... ppm
;;; 14  Slowest:       ....... Hz
;;; 15  Fastest:       ....... Hz
;;; 16
;;; 17  DSP sample rate: ..... Hz
;;; 18
;;; 19         animation-here
;;; 20
;;; 21
;;; 22
;;; 23         Copyright 2023 lidnariq
;;; 24         redistribute under zlib

initscreen:
	.res 32,0
	.res 32,0
	.res 32,0
Lppu:	.incbin "M_ppu"
Lcpu:	.incbin "M_cpu"
	.res 32,0
Lraw:	.incbin "M_raw"
	.res 32,0
Lus:	.incbin "M_us"
Lsus:	.incbin "M_slowest"
Lfus:	.incbin "M_fastest"
	.res 32,0
Lsmp:	.incbin "M_smp"
Lrel:	.incbin "M_rel"
Lshz:	.incbin "M_slowestHz"
Lfhz:	.incbin "M_fastestHz"
	.res 32,0
Ldsp:	.incbin "M_dsp"
	.res 32,0
throbber:	.res 32,0
	.res 32,0
	.res 32,0
	.res 32,0
copyright:	.incbin "M_copyright"
license:	.incbin "M_license"
endinit:
	
	
	
.CODE
; Minimalist NMI handler that takes note of NMI
.proc nmi_handler
	seta8
	inc nmicount
	rti
.endproc
	
;;; init.s sends us here
.proc main
	seta8
	setxy16

	;; empty PPU memory
	stz HDMASTART

	ldx #0      ;=65536
	stx PPUADDR
	stx DMALEN
	lda #^zeroword
	sta DMAADDRBANK
	ldx #.loword(zeroword)
	stx DMAADDR
	ldx #DMAMODE_PPUFILL
	stx DMAMODE
	lda #1
	sta COPYSTART

	;; clear system RAM
	ldx #0
	stx WMADDL
	stx DMALEN
	;; not changing source
	stz WMADDH
	ldx #(<WMDATA << 8) | DMA_LINEAR | DMA_CONST
	stx DMAMODE
	lda #1
	sta COPYSTART ; bottom 64KB
	sta WMADDL
	sta COPYSTART ; upper 64KB
	
	;; copy font in
	ldx #afterfont - font
	stx DMALEN
	ldx #$1000
	stx PPUADDR
	lda #^font
	sta DMAADDRBANK
	ldx #.loword(font)
	stx DMAADDR
	ldx #DMAMODE_PPUDATA
	stx DMAMODE
	lda #1
	sta COPYSTART


	;; configure mode 0
	lda #$FF
	sta BG1VOFS ; Yscroll = -1
	sta BG1VOFS
	stz BG1HOFS ; Xscroll = 0
	stz BG1HOFS

	
	stz BGMODE  ; mode 0
	lda #1
	sta TM      ; only layer 0
	sta TS

	stz BG1SC   ; both nametables at the very bottom of memory
	stz BG2SC

	lda #$11
	sta BG12NBA ; tiles from word $1000
	stz PPURES  ; no fancy features

	stz CGADDR
	stz CGDATA
	stz CGDATA
	lda #$FF
	sta CGDATA
	sta CGDATA
	sta CGDATA
	sta CGDATA
	sta CGDATA
	sta CGDATA

	stz PPUCTRL

	setaxy16
	lda #$FFFF
	sta fastesttime
	sta fastesttime+2

	
	ldx #.loword(initscreen)
	ldy #.loword(nametable)
	lda #endinit-initscreen
	mvn ^initscreen,^nametable

	seta8
	lda #$F
	sta PPUBRIGHT

	jsl spc_boot_apu
	
	;; At this point we can start the timing loop:

	lda #$AA
	sta apuphase

startcount:
	ldx #0
	phx
	ldx #$2100
	phx
	ldx #0
	ldy #0
	lda apuphase
	sta APU0

	pld         ; D <- $2100
timing:	
	cmp z:<APU0    ; 3 -> each count of X is 10cy
	beq time    ; 2 ->  or ~3us
	inx         ; 2
	bne timing  ; 3 usu
	iny         ; 2
	bne timing  ; 3 usu
overtime:
time:
	pld         ; D <- 0
	;; at this point we have a 32-bit number in YYYY:XXXX
	stx timetaken
	sty timetaken+2
	;; XXXX is 10cy per iteration,
	;; but YYYY is 655364cy, but that 6ppm error we don't care

	;; There's a bunch of things we need to change depending on
	;; vsync rate...

	;; check PPU reg for vsync rate
	lda #$10
	bit PPUSTATUS2
	bze SixtyHz
FiftyHz:
	;; 50 Hz
	lda #$35
	sta nametable+Lppu-initscreen+24

	;; 21.281 MHz
	ldx #$3832
	stx nametable+Lcpu-initscreen+23
	ldx #$31
	sta nametable+Lcpu-initscreen+25

;;; How many microseconds per count?
;;; Loop is 10cy, 60 Mcy
;;; 1364 Mcy per scanline, but 40 are stolen -> 1324 Mcy per scanline
;;; scanlines are
;;;  (PAL)  64.094 us / 22.06 counts -> 2.905 us / count (2 + 232/256)
	ldx #$02E8
	stx multiplier

;;; We want to do: [constant] / S-SMP clock x S-CPU clock = [result] x 10

;;; to display the S-SMP clock,
;;; S-SMP clock = [constant] x S-CPU clock / (10 x [result])
;;;  [constant] = 394246  -  from spcimage.s
	
;;;  S-CPU clock = 21281370 / 6 x 1324 / 1364 ~~ 3442880.384 ( PAL SNES )
;;;   numerator = 135734185924 = 1f 9a63 4fc4
	ldx #$001F
	stx dividend+4
	ldx #$9A63
	stx dividend+2
	ldx #$4FC4
	stx dividend

	bra ForSystem
SixtyHz:
	;; 60 Hz
	lda #$36
	sta nametable+Lppu-initscreen+24

	;; 21.477 MHz
	ldx #$3734
	stx nametable+Lcpu-initscreen+24
	stx nametable+Lcpu-initscreen+23

;;;  (NTSC) 63.509 us / 22.06 counts -> 2.878 us / count (2 + 225/256)
	ldx #$02e1
	stx multiplier

;;;  S-CPU clock = 3579545... x 1324 / 1364 ~~ 3474573.447Hz ( NTSC SNES )
;;;   numerator = 136983668322 = 1f e4dc e662

;;;  --- actually the SNES's missing dot makes this just a teeeeensy bit slower.
;;;      each vsync has 1324 x 262 - 2 = 346886 Mcy for the CPU
;;;      each vsync takes 1364 x 262 - 2 = 357366 Mcy
;;;      this is an error of 174 ppb, but we may as well fix it:
;;;   numerator = 136983645161 = 1f e4dc 8be9

	ldx #$001F
	stx dividend+4
	ldx #$E4DC
	stx dividend+2
	ldx #$8BE9
	stx dividend

ForSystem:	

	;; print raw count
	seta8
	ldy #nametable+Lraw-initscreen+15
	lda timetaken+2
	jsr printHex
	lda timetaken+1
	jsr printHex
	lda timetaken
	jsr printHex

	;; Now take note if this is a fastest-ever
	;; or slowest-ever...
	;; this doesn't update the screen yet...
	stz flags
	
	seta16
	lda timetaken
	cmp fastesttime
	lda timetaken+2
	sbc fastesttime+2
	bge NotNewFastest
NewFastest:	
	lda timetaken
	sta fastesttime
	lda timetaken+2
	sta fastesttime+2
	
	seta8
	lda #$80    ; remember fastest
	sta flags
	seta16
NotNewFastest:
	lda slowesttime
	cmp timetaken
	lda slowesttime+2
	sbc timetaken+2
	bge NotNewSlowest
NewSlowest:
	lda timetaken
	sta slowesttime
	lda timetaken+2
	sta slowesttime+2

	seta8
	lda #$40    ; remember slowest
	ora flags
	sta flags
NotNewSlowest:	
	seta8
	;; Convert current measurement to microseconds
	ldx timetaken
	stx multiplicand
	lda timetaken+2
	sta multiplicand+2
	jsr u16u24_u40


	;; then convert the multiplication result to BCD
	seta16
	lda product+1
	sta bcdres
	lda product+3
	sta bcdres+2
	stz bcdres+4
	stz bcdres+6
	jsr doubledabble

	;; then print
	seta8
	ldy #nametable+Lus-initscreen+15
	lda bcdres+5
	jsr printHex
	lda bcdres+4
	jsr printHex
	lda bcdres+3
	jsr printHex

	bit flags
	bpl NotFastest
	;; here- update "fastest"
	ldx nametable+Lus-initscreen+15
	stx nametable+Lfus-initscreen+15
	ldx nametable+Lus-initscreen+17
	stx nametable+Lfus-initscreen+17
	ldx nametable+Lus-initscreen+19
	stx nametable+Lfus-initscreen+19
NotFastest:
	bit flags
	bvc NotSlowest
	;; here- update "slowest"
	ldx nametable+Lus-initscreen+15
	stx nametable+Lsus-initscreen+15
	ldx nametable+Lus-initscreen+17
	stx nametable+Lsus-initscreen+17
	ldx nametable+Lus-initscreen+19
	stx nametable+Lsus-initscreen+19
NotSlowest:

	;; now do the stupid division
	ldx timetaken
	stx divisor
	ldx timetaken+2
	stx divisor+2
	jsr udiv48

	;; now convert the quotient to BCD
	ldx quotient
	stx bcdres
	ldx quotient+2
	stx bcdres+2
	seta16
	stz bcdres+3
	stz bcdres+5
	stz bcdres+6
	jsr doubledabble

	;; then print
	seta8
	ldy #nametable+Lsmp-initscreen+15
	lda bcdres+6
	jsr printHex
	lda bcdres+5
	jsr printHex
	lda bcdres+4
	jsr printHex
	lda bcdres+3
	jsr printHex

	bit flags
	bvc NotFastest2
	ldx nametable+Lsmp-initscreen+15
	stx nametable+Lshz-initscreen+15
	ldx nametable+Lsmp-initscreen+17
	stx nametable+Lshz-initscreen+17
	ldx nametable+Lsmp-initscreen+19
	stx nametable+Lshz-initscreen+19
	ldx nametable+Lsmp-initscreen+21
	stx nametable+Lshz-initscreen+21
NotFastest2:
	bit flags
	bpl NotSlowest2
	ldx nametable+Lsmp-initscreen+15
	stx nametable+Lfhz-initscreen+15
	ldx nametable+Lsmp-initscreen+17
	stx nametable+Lfhz-initscreen+17
	ldx nametable+Lsmp-initscreen+19
	stx nametable+Lfhz-initscreen+19
	ldx nametable+Lsmp-initscreen+21
	stx nametable+Lfhz-initscreen+21
NotSlowest2:

	;; DSP sample rate
	seta16
	stz bcdres+6
	stz bcdres+4
	lda quotient+2
	sta bcdres+2
	lda quotient
	sta bcdres
	;; divide by 32...
	lsr bcdres+2
	ror bcdres
	lsr bcdres+2
	ror bcdres
	lsr bcdres+2
	ror bcdres
	lsr bcdres+2
	ror bcdres
	lsr bcdres+2
	ror bcdres
	;; convert to BCD
	jsr doubledabble
	;; then print

	ldy #nametable+Ldsp-initscreen+17
	seta8
	lda bcdres+5
	jsr printHex
	lda bcdres+4
	jsr printHex
	lda bcdres+3
	jsr printHex

	;; finally, calculate ppm error?
	;; load result...
	seta16
	sec
	lda quotient
	sbc #.loword(1024000)
	sta bcdres
	lda quotient+2
	sbc #.hiword(1024000)
	sta bcdres+2
	;; make sure the result is still positive!
	bcc ppmWasNegative
ppmWasPositive:	
	jsr commonPPMbits
	.a8
	lda #'+'
	sta nametable+Lrel-initscreen+17
	bra cleanup
ppmWasNegative:
	.a16
	lda #.loword(1024000)
	sbc quotient
	sta bcdres
	lda #.hiword(1024000)
	sbc quotient+2
	sta bcdres+2

	jsr commonPPMbits
	.a8
	lda #'-'
	sta nametable+Lrel-initscreen+17

cleanup:	
	;; Now clean up all the leading zeroes
	lda #0
	sta nametable+Lraw-initscreen+15
	sta nametable+Lsmp-initscreen+15
	sta nametable+Lshz-initscreen+15
	sta nametable+Lfhz-initscreen+15
	sta nametable+Ldsp-initscreen+17

	;; and we really need a throbber of some sort

	seta8
	lda #$10
	bit apuphase
	bze toRight ; going to higher numbers
toLeft:
	lda apuphase
	seta16
	and #$000F
	clc
	adc #nametable+throbber-initscreen+8
	tay

	lda #$0700
	bra drawDot
toRight:
	.a8
	lda #$F
	eor apuphase
	seta16
	and #$000F
	clc
	adc #nametable+throbber-initscreen+8
	tay

	lda #$0007
drawDot:
	sta 0,y
	
	ldx #0
	ldy #0

	jsr enableAndWaitForNMI
	;; then do DMA
	setxy16
	seta8
	ldx #0
	stx PPUADDR ; nametable at 0
	ldx #endinit-initscreen
	stx DMALEN
	lda #^nametable
	sta DMAADDRBANK
	ldx #.loword(nametable)
	stx DMAADDR
	ldx #DMAMODE_PPULODATA
	stx DMAMODE
	lda #1
	sta COPYSTART

	setxy16
	inc apuphase
	jmp startcount
.endproc

.proc enableAndWaitForNMI
	seta8
	lda #VBLANK_NMI
	sta PPUNMI
	
	lda #0
	sta nmicount ; this way late detection of an NMI won't run out of vsync time
	lda #255
waitnmi:
	bit nmicount
	bze waitnmi

	stz PPUNMI  ; re-disable

	rts
.endproc

;;; long division in binary is
;;;  "shift the divisor all the way to left"
;;;  "subtract from the dividend"
;;;  "see if the result is still positive - if so, 1 and keep the result, otherwise 0 and discard the result"
;;;  "shift the divisor one to the right
;;; repeat until done

;;; despite the name, this "only" divides a 37-bit number by a 17- or 18- bit number.
;;;  You can fix that if you want, I didn't.
.proc udiv48
	setaxy16
;;; zero out the quotient
	stz quotient
	stz quotient+2
	stz nextbit

;;; We already know the dividend is 37 bits and the top 5 bits are all set
;;; 001F E4DC E662
;;; We can safely assume the divisor is 17 or 18 bits (65536 through 262143)
;;; ---- 0003 FFFF
;;; so first step is shifting the divisor over 20 or 19 bits
	lda divisor+2
	sta subtrahend+4
	lda divisor
	sta subtrahend+2
	stz subtrahend
	;; 16 bits

	stz nextbit
	lda #8
	sta nextbit+2

	asl subtrahend+2
	rol subtrahend+4
	asl subtrahend+2
	rol subtrahend+4
	asl subtrahend+2
	rol subtrahend+4
	;; 19 bits

	lda #$0010
	bit subtrahend+4
	bnz @DoneRotating
	asl subtrahend+2
	rol subtrahend+4
	asl nextbit+2
@DoneRotating:
@LongDivisionLoop:
	jsr usub48
	;; check carry bit to see if we commit or rollback
	bcc @nextbit0        ; rollback
@nextbit1:
	;; here, carry=borrow was clear, so the result is still positive, so
	;; 1- commit the subtraction
	lda difference
	sta dividend
	lda difference+2
	sta dividend+2
	lda difference+4
	sta dividend+4

	;; 2- mark the 1 bit
	lda quotient
	ora nextbit
	sta quotient
	lda quotient+2
	ora nextbit+2
	sta quotient+2
@nextbit0:
	;; here, borrow was set, so the result is negative, so do nothing

	;; the next possible bit of quotient
	lsr subtrahend+4
	ror subtrahend+2
	ror subtrahend

	lsr nextbit+2
	ror nextbit
	;; If the bit fell out, we're done
	bcc @LongDivisionLoop

	rts
.endproc

.proc usub48
	setaxy16
	
	sec
	lda dividend
	sbc subtrahend
	sta difference
	lda dividend+2
	sbc subtrahend+2
	sta difference+2
	lda dividend+4
	sbc subtrahend+4
	sta difference+4
	rts
.endproc

.proc printHex
	seta16
	and #$FF    ; we have to do this to make sure tax works correctly
	seta8
	pha
	lsr
	lsr
	lsr
	lsr
	tax
	lda hex,x
	sta 0,y
	iny
	pla
	and #15
	tax
	lda hex,x
	sta 0,y
	iny
	rts
.endproc

;;; Multiplies u16 (in "multiplier") by u24 (in "multiplicand") and stores u40 in "product"
.proc u16u24_u40
	setaxy16
	;; clear product{0,1} below
	stz product+2 ; {2,3}
	stz product+3 ; {3,4}
	;; redundant clear faster than 3rd 8-bit ins'n
	
	seta8
	lda multiplicand
	sta CPUMCAND
	lda multiplier
	sta CPUMUL
	nop         ; 1 2
	lda multiplier+1 ; 3 4 5
	ldx CPUPROD      ; 6 7 8 ><
	sta CPUMUL
	stx product ; 1 2 3 4
	seta16      ; 5 6
	clc         ; 7 8
	lda CPUPROD ; 9 10 11 ><
	adc product+1
	sta product+1

	;; first u8 x u16 done

	seta8
	lda multiplicand+1
	sta CPUMCAND
	lda multiplier
	sta CPUMUL
	clc
	seta16
	lda product+1
	adc CPUPROD
	sta product+1
	seta8
	lda multiplier+1
	sta CPUMUL
	clc
	seta16
	lda product+2
	adc CPUPROD
	sta product+2

	seta8
	lda multiplicand+2
	sta CPUMCAND
	lda multiplier
	sta CPUMUL
	clc
	seta16
	lda product+2
	adc CPUPROD
	sta product+2
	seta8
	lda multiplier+1
	sta CPUMUL
	clc
	seta16
	lda product+3
	adc CPUPROD
	sta product+3

	seta8
	rts
.endproc

;;; Double dabble is a binary-to-packed-BCD converter.
;;; Storage needed is [enough bits for the entire input] plus [enough nybbles for the entire output]
;;; This one accepts a 24-bit input and produces a 8-nybble output
;;; See wikipedia's explanation for how it works.
.proc doubledabble
	seta8
	setxy16

	lda #24
	sta bcdStep

@doubleloop:
	jsr checkFives
	jsr asl64
	dec bcdStep
	bnz @doubleloop

	rts
.endproc

.proc asl64
	php
	seta16
	asl bcdres
	rol bcdres+2
	rol bcdres+4
	rol bcdres+6
	plp
	rts
.endproc

.proc checkFives
	seta8
	setxy16
	ldy #3

@fivesLoop:
	lda #0
	sta bcdTemp
	lda bcdres,y
	and #15
	cmp #5
	blt @noaddL
	lda #3
	sta bcdTemp
@noaddL:
	lda bcdres,y
	cmp #$50
	blt @noaddH
	lda #$30
	ora bcdTemp
	sta bcdTemp
@noaddH:
	clc
	lda bcdres,y
	adc bcdTemp
	sta bcdres,y

	iny
	cpy #7
	blt @fivesLoop

	rts
.endproc


;;; just a merged fragment for the common code for positive and negative deviation from nominal
.proc commonPPMbits
	;; then multiply by 250 and divide by 256...
	stz ppm+2
	seta8
	lda #250
	sta CPUMCAND
	lda bcdres
	sta CPUMUL
	nop
	nop
	nop
	ldx CPUPROD
	stx ppm
	lda bcdres+1
	sta CPUMUL
	nop
	clc
	seta16
	lda CPUPROD
	adc ppm+1
	sta ppm+1
	seta8
	lda bcdres+2
	sta CPUMUL
	nop
	clc
	seta16
	lda CPUPROD
	adc ppm+2
	sta ppm+2

	lda ppm+1   ; here's our implicit /256
	sta bcdres
	lda ppm+3
	sta bcdres+2
	stz bcdres+3
	stz bcdres+5
	stz bcdres+6
	jsr doubledabble

	ldy #nametable+Lrel-initscreen+17
	seta8
	lda bcdres+5
	jsr printHex
	lda bcdres+4
	jsr printHex
	lda bcdres+3
	jsr printHex
	rts
.endproc
