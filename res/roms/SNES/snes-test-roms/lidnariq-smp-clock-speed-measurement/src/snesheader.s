; Copyright 2014-2015 Damian Yerrick
; 
; Copying and distribution of this file, with or without
; modification, are permitted in any medium without royalty provided
; the copyright notice and this notice are preserved in all source
; code copies.  This file is offered as-is, without any warranty.
;

;
; Definition of the internal header and vectors at $00FFC0-$00FFFF
; in the Super NES address space.
;

;.include "snes.inc"
.p816
.smart
.import main, nmi_handler, __ZEROPAGE_RUN__
.import __DATA_SIZE__, __DATA_RUN__, __DATA_LOAD__



; LoROM is mapped to $808000-$FFFFFF in 32K blocks,
; skipping a15 and a23. Most is mirrored down to $008000.
MAPPER_LOROM = $20    ; a15 skipped
; HiROM is mapped to $C00000-$FFFFFF linearly.
; It is mirrored down to $400000, and the second half of
; each 64K bank is mirrored to $008000 and $808000.
MAPPER_HIROM = $21    ; C00000-FFFFFF skipping a22, a23
; ExHiROM is mapped to $C00000-$FFFFFF followed by
; $400000-$7FFFFF.  There are two inaccessible 32K holes
; near the end, and two 32K blocks that are accessible only
; through their mirrors to $3E8000 and $3F8000.
MAPPER_EXHIROM = $25  ; skipping a22, inverting a23

; If ROMSPEED_120NS is turned on, the game will be pressed
; on more expensive "fast ROM" that allows the CPU to run
; at its full 3.6 MHz when accessing ROM at $808000 and up.
ROMSPEED_200NS = $00
ROMSPEED_120NS = $10

; ROM and backup RAM sizes are expressed as
; ceil(log2(size in bytes)) - 10
MEMSIZE_NONE  = $00
MEMSIZE_2KB   = $01
MEMSIZE_4KB   = $02
MEMSIZE_8KB   = $03
MEMSIZE_16KB  = $04
MEMSIZE_32KB  = $05
MEMSIZE_64KB  = $06
MEMSIZE_128KB = $07  ; values from here up are for SRAM
MEMSIZE_256KB = $08  ; values from here down are for ROM
MEMSIZE_512KB = $09  ; Super Mario World
MEMSIZE_1MB   = $0A  ; Mario Paint
MEMSIZE_2MB   = $0B  ; Mega Man X, Super Mario All-Stars, original SF2
MEMSIZE_4MB   = $0C  ; Turbo/Super SF2, all Donkey Kong Country
MEMSIZE_8MB   = $0D  ; ExHiROM only: Tales of Phantasia, SF Alpha 2

REGION_JAPAN = $00
REGION_AMERICA = $01
REGION_PAL = $02

.segment "SNESHEADER"
	.byte "li"
	.byte "shvc"
	.res 7,0
	.byte MEMSIZE_NONE
	.byte 0
	.byte 0
romname:
  ; The ROM name must be no longer than 21 characters.
  ; Longest possible is "PINO'S LOROM TEMPLATE"
  .byte "s-smp clock measurer"
  .assert * - romname <= 21, error, "ROM name too long"
  .if * - romname < 21
    .res romname + 21 - *, $20  ; space padding
  .endif
map_mode:
  .byte MAPPER_LOROM|ROMSPEED_120NS
  .byte $0   ; 0 - ROM, nothing else present
  .byte MEMSIZE_32KB  ; ROM size (08-0C typical)
  .byte MEMSIZE_NONE   ; backup RAM size (01,03,05 typical; Dezaemon has 07)
  .byte REGION_AMERICA
  .byte $33   ; publisher id, or $33 for see 16 bytes before header
  .byte $00   ; ROM revision number
  .word $0000 ; sum of all bytes will be poked here after linking
  .word $0000 ; $FFFF minus above sum will also be poked here
  ; clcxce mode vectors
  ; reset unused because reset switches to 6502 mode
  .addr fictitious, fictitious  ; unused vectors
  .addr cop_handler, brk_handler, abort_handler
  .addr nmi_handler, fictitious, irq_handler
  ; 6502 mode vectors
  ; brk unused because 6502 mode uses irq handler and pushes the
  ; X flag clear for /IRQ or set for BRK
  .addr fictitious, fictitious ; more unused vectors
  .addr ecop_handler, fictitious, eabort_handler
  .addr enmi_handler, resetstub, eirq_handler
  
.CODE

; Jumping out of bank $00 is especially important if you're using
; ROMSPEED_120NS.

           ; Unused exception handlers
irq_handler:	
cop_handler:
brk_handler:
abort_handler:
ecop_handler:
eabort_handler:
enmi_handler:
eirq_handler:
fictitious:
  rti

.smart

; Mask off low byte to allow use of $000000-$00000F as local variables
ZEROPAGE_BASE   = __ZEROPAGE_RUN__ & $FF00

; Make sure these conform to the linker script (e.g. lorom256.cfg).
STACK_BASE      = $0100
STACK_SIZE      = $0100
LAST_STACK_ADDR = STACK_BASE + STACK_SIZE - 1

PPU_BASE        = $2100
CPUIO_BASE      = $4200

; MMIO is mirrored into $21xx, $42xx, and $43xx of all banks $00-$3F
; and $80-$BF.  To make it work no matter the current data bank, we
; can use a long address in a nonzero bank.
; Bit 0 of MEMSEL enables fast ROM access above $808000.
MEMSEL          = $01420D

; A tiny stub in bank $00 needs to set interrupt priority to 1,
; leave 6502 emulation mode, and long jump to the rest of init code
; in another bank. This should set 16-bit mode, turn off decimal
; mode, set the stack pointer, load a predictable state into writable
; MMIO ports of the S-PPU and S-CPU, and set the direct page base.
; For explanation of the values that this writes, see docs/init.txt
;
; For advanced users: Long stretches of STZ are a useful place to
; shuffle code when watermarking your binary.

.CODE
.proc resetstub
	sei	; turn off IRQs
	clc
	xce	; turn off 6502 emulation mode
	cld	; turn off decimal ADC/SBC

	rep #$30	; 16-bit AXY
	ldx #LAST_STACK_ADDR
	txs	; set the stack pointer

	; Initialize the CPU I/O registers to predictable values
	lda #CPUIO_BASE
	tad	; temporarily move direct page to S-CPU I/O area
	lda #$FF00
	sta $00     ; PPUNMI and JOYOUT
	stz $02     ; CPUMCAND and CPUMUL
	stz $04     ; CPUNUM<16>
	stz $06     ; CPUDEN and HTIME
	stz $08     ; HTIMEHI and VTIME
	stz $0A     ; VTIMEHI and COPYSTART
	stz $0C     ; HDMASTART and ROMSPEED

	; Initialize the PPU registers to predictable values
	lda #PPU_BASE
	tad	; temporarily move direct page to PPU I/O area

	; first clear the regs that take a 16-bit write
	lda #$0080
	sta $00	; Enable forced blank
	stz $02
	stz $05
	stz $07
	stz $09
	stz $0B
	stz $16
	stz $24
	stz $26
	stz $28
	stz $2A
	stz $2C
	stz $2E
	ldx #$0030
	stx $30	; Disable color math
	ldy #$00E0
	sty $32	; Clear red, green, and blue components of COLDATA

	; now clear the regs that need 8-bit writes
	sep #$20
	sta $15	; still $80: Inc VRAM pointer after high byte write
	stz $1A
	stz $21
	stz $23

	; The scroll registers $210D-$2114 need double 8-bit writes
	.repeat 8, I
	  stz $0D+I
	  stz $0D+I
	.endrepeat

	; As do the mode 7 registers, which we set to the identity matrix
	; [ $0100  $0000 ]
	; [ $0000  $0100 ]
	lda #$01
	stz $1B
	sta $1B
	stz $1C
	stz $1C
	stz $1D
	stz $1D
	stz $1E
	sta $1E
	stz $1F
	stz $1F
	stz $20
	stz $20

	; Set fast ROM if the internal header so requests
	lda map_mode
	and #$10
	beq not_fastrom
	lda #$01
	sta MEMSEL
not_fastrom:

	rep #$30	; don't have setaxy816 macros yet
	lda #ZEROPAGE_BASE
	tad	; return direct page to real zero page

	; Unlike on the NES, we don't have to wait 2 vblanks to do
	; any of the following remaining tasks.
	; * Fill or clear areas of VRAM that will be used
	; * Clear areas of WRAM that will be used
	; * Load palette data into CGRAM
	; * Fill shadow OAM and then copy it to OAM
	; * Boot the S-SMP
	; The main routine can do these in any order.

	;; finally, initialize DATA segment ... if extant

;	ldx #(__DATA_LOAD__ & $FFFF)
;	ldy #(__DATA_RUN__ & $FFFF)
;	lda #(__DATA_SIZE__)
	;; why mvn instead of DMA? it's simpler to describe.
;	mvn ^__DATA_LOAD__, ^__DATA_RUN__

	jml main
.endproc

