;
;       24 bit helpers functions for fixed-point arithmetic
;       zlashstudios 2010
;

;Helper macro that outputs 3 .byte directives for a 
;base 2 24-8 fixed-point number from a base 10 
;decimal-integer number pair
;sign must be 1 or -1

.macro FPA_MAKEFP sign, i, d
        .local R
        R:=sign*((i<<8)|($FF&(($100*d)/100)))
        .byte R&$FF
        .word (R>>8)&$FFFF
.endmacro


;Compares 24-bit numbers
.macro FPA_CMP_24 pa,pb
        .repeat 3,I
        lda pa+2-I
		cmp pb+2-I
        bne :+
        .endrepeat
:
.endmacro

;Increments a 48-bit number
.macro FPA_INC_48 pa
        .local done
        inc pa
        bne done
        inc pa+1
        bne done
        inc pa+2
        bne done
        inc pa+3
        bne done
        inc pa+4
        bne done
        inc pa+5        
done:    
.endmacro

;Increments a 24-bit number
.macro FPA_INC_24 pa
        .local done
        inc pa
        bne done
        inc pa+1
        bne done
        inc pa+2
done:    
.endmacro

;Decrements a 24-bit number
.macro FPA_DEC_24 pa
        clc
        .repeat 3,I
        lda pa+I
        adc #$FF
        sta pa+I
        .endrepeat
.endmacro

;Increments a 16-bit number
.macro FPA_INC_16 pa
        .local done
        inc pa
        bne done
        inc pa+1        
done:    
.endmacro


;Adds an 8-bit number to a 24 bit one
.macro FPA_ADD_24_8  pa, pb
        clc
        lda pa
        adc pb
        sta pa
		.repeat 2,I
        lda pa+I+1
        adc #$00
        sta pa+I+1
        .endrepeat
.endmacro

;Adds the content of the A register to a 16 bit number
.macro FPA_ADD_16_A  pa
        clc
        adc pa
        sta pa
        lda #0
        adc pa+1
        sta pa+1
.endmacro

;Adds two 24-bit numbers
.macro FPA_ADD_24  pa, pb
        clc
        .repeat 3,I
        lda pa+I
        adc pb+I
        sta pa+I
        .endrepeat
.endmacro

;Adds two 16-bit numbers
.macro FPA_ADD_16  pa, pb
        clc
        .repeat 2,I
        lda pa+I
        adc pb+I
        sta pa+I
        .endrepeat
.endmacro

;Adds two 48-bit numbers
.macro FPA_ADD_48  pa, pb
        clc
        .repeat 6,I
        lda pa+I
        adc pb+I
        sta pa+I
        .endrepeat
.endmacro


;Shifts right 48 bits
.macro FPA_LSR_48 pa
        clc
        .repeat 6,I
        ror pa+(5-I)
        .endrepeat
.endmacro

;Shifts right 24 bits
.macro FPA_LSR_24 pa
        clc
        .repeat 3,I
        ror pa+(2-I)
        .endrepeat
.endmacro

;Shifts right 16 bits
.macro FPA_LSR_16 pa
        clc
        .repeat 2,I
        ror pa+(1-I)
        .endrepeat
.endmacro

;Shifts left 48 bits
.macro FPA_ASL_48 pa
        clc
        .repeat 6,I
        rol pa+I
        .endrepeat
.endmacro

;Shifts left 24 bits
.macro FPA_ASL_24 pa
        clc
        .repeat 3,I
        rol pa+I
        .endrepeat
.endmacro

;Helper macro for the FPA multiplication routine
; ONLY POSITIVE NUMBERS!!
.macro FPA_MUL_24  pa, pb
        MEMCPY TempA, pa, #3
        MEMCPY TempC, pb, #3
        jsr FPA::mul24
        MEMCPY pa, TempD+1, #3        
.endmacro

;Helper macro for the FPA multiplication routine
;accepts also negative numbers
.macro FPA_MUL_24_S  pa, pb
        .local done
        .local noneg1
        .local noneg2
        MEMCPY TempA, pa, #3
        MEMCPY TempC, pb, #3
        
        ldx #0
        lda TempA+2
        bpl noneg1
        FPA_NEG_24 TempA
        inx
noneg1:        
        lda TempC+2
        bpl noneg2
        FPA_NEG_24 TempC
        inx
noneg2:                
        txa
        pha
        jsr FPA::mul24     
        MEMCPY pa, TempD+1, #3 
        pla
        and #1
        beq done
        FPA_NEG_24 pa
done:        
.endmacro

;Changes the sign of a 48-bit number
.macro FPA_NEG_48 pa
        .repeat 6,I
        lda pa+I
        eor #$FF
        sta pa+I
        .endrepeat
        FPA_INC_48 pa
.endmacro   


;Changes the sign of a 24-bit number
.macro FPA_NEG_24 pa
        .repeat 3,I
        lda pa+I
        eor #$FF
        sta pa+I
        .endrepeat
        FPA_INC_24 pa
.endmacro        

;Changes the sign of a 16-bit number
.macro FPA_NEG_16 pa
        .repeat 2,I
        lda pa+I
        eor #$FF
        sta pa+I
        .endrepeat
        FPA_INC_16 pa
.endmacro  

;Divide macro helper (Only positive!)
.macro FPA_DIVIDE_24 pa,pb
        MEMCPY TempA,pa,#3
        MEMCPY TempD,pb,#3        
        jsr FPA::divide_24
.endmacro        

;Divide macro with sign-check and preupscale
.macro FPA_DIVIDE_24_PREUP_S pa,pb
        .local done
        .local noneg1
        .local noneg2
        lda #0
        sta TempAL
        MEMCPY TempAH,pa,#2
        MEMCPY TempD,pb,#3

        ldx #0
        lda TempA+2
        bpl noneg1
        FPA_NEG_24 TempA
        inx
noneg1:        
        lda TempD+2
        bpl noneg2
        FPA_NEG_24 TempC
        inx
noneg2:                
        txa
        pha
        jsr FPA::divide_24
        pla
        and #1
        beq done
        FPA_NEG_24 TempE
done:        
.endmacro

;Divide macro helper with a preupscale to keep resolution
.macro FPA_DIVIDE_24_PREUP pa,pb        
        lda #0
        sta TempAL
        MEMCPY TempAH,pa,#2
        MEMCPY TempD,pb,#3                
        jsr FPA::divide_24
.endmacro    

;ADVANCES RANDOM VALUE
.macro FPA_RAND
		jsr FPA::rand
.endmacro


.scope FPA

.segment "ZEROPAGE"

.segment "BSS"

	RandomValue:		.res 3


.segment "CODE"


;Multiplies two 24-bit numbers, but shifts the result as
;it were a 24-bit fixed-point variable with 8-bits of
;decimal part
;TempD+1  = TempA*TempC
;!!! Both MUST be POSITIVE !!!
mul24:
        lda #0
        .repeat 3,I
        sta TempB+I
        sta TempD+I
        sta TempE+I
        .endrepeat

loop:        
        lda #1
        and TempC
        beq noadd
        FPA_ADD_48 TempD,TempA
noadd: 
        lda TempC
        lsr A        
        bne keepon                
        lda TempC+1
        bne keepon
        lda TempC+2
        bne keepon        
        rts
keepon:     
        FPA_LSR_24 TempC
        FPA_ASL_48 TempA
        jmp loop

; Magical Punga Cheap Divide
; Divides 24-bit POSITIVE Numbers
; TempA/TempD
; Q at TempE
; R at TempB
divide_24:
        lda #0
        sta TempBL
        sta TempBH
        sta TempBHP
        sta TempCL
        sta TempCH
        sta TempCHP
        sta TempEL
        sta TempEH
        sta TempEHP
        MEMCPY nzpTempA,TempC,#6        
        FPA_NEG_48 TempC
        ldy #24
divLoop:        
        FPA_ASL_48 TempA
        FPA_ADD_48 TempA,TempC
        lda TempBHP
        bmi isNegative
        lda TempEL
        ora #1
        sta TempEL
        jmp loopCondCheck
isNegative:        
        FPA_ADD_48 TempA,nzpTempA
loopCondCheck:                
        dey
        beq end_divide_24
        FPA_ASL_24 TempE        
        jmp divLoop
end_divide_24:
        rts

		
rand:
		FPA_MUL_24 RandomValue,LCGA
		FPA_ADD_24 RandomValue,LCGC
		rts
		
.macro TOBCDMACRO

.endmacro        
        
;Transform the 16 bits number at TempA to BCD at TempC (5 bytes)
toBCD:  
        lda #0
        sta TempC
        sta TempC+1
        sta TempC+2
        sta TempC+3
        sta TempC+4
        
        .repeat 5,I        
        MEMCPY TempB,bcdTable+(I*2),#2
        FPA_NEG_16 TempB
:
        FPA_ADD_16 TempA,TempB
        lda TempA+1
        bmi :+
        inc TempC+I
        jmp :-
:
        FPA_ADD_16 TempA,bcdTable+(I*2)

        .endrepeat        
        rts

;CONSTS        
bcdTable:   .byte <10000,>10000,<1000,>1000,<100,>100,<10,>10,<1,>1


LCGC:  .byte $E0,$FF,$FF 
LCGA:  .byte $56,$55,$55        
.endscope
 