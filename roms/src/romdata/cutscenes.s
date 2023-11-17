;;;;;;;;;;;;;;;;;;;;;;;;;;
; Cutscenes Data
;;;;;;;;;;;;;;;;;;;;;;;;;;


;Cutscenes Defines:
cutscene_destroyed_city=0
cutscene_mothership=1
cutscene_destroyed_city_2=2
cutscene_temple_ruins=3
cutscene_utaco_temple=4
cutscene_utaco_temple_2=6
cutscene_mothership_2=8
cutscene_utaco_temple_3=9
cutscene_the_lab=10
cutscene_utaco_temple_4=11
cutscene_no_mothership=14
cutscene_no_mothership_2=15
cutscene_utaco_temple_5=16
cutscene_cutscene_empty=17
cutscene_the_lab_2=18
cutscene_cutscene_empty_2=19
;Commands:
;$00 ~ 7F show corrspoding cutscene
;$FE - HALT!! (halt mode disables skipping, after all the cutscenes goes to end instead of henshin.
;$FF - end cutscenes
cutscenes_commands:
	.byte $0
	.byte $0
	.byte $0
	.byte $0
	.byte $0
	.byte $FF
	.byte $0
	.byte $FF
	.byte $0
	.byte $0
	.byte $0
	.byte $0
	.byte $FF
	.byte $FE
	.byte $0
	.byte $0
	.byte $0
	.byte $0
	.byte $0
	.byte $0
	.byte $FF
;Nametables references:
cutscenes_nametables_lo:
	.byte <CS_NAM_destroyed_city
	.byte <CS_NAM_mothership
	.byte <CS_NAM_destroyed_city
	.byte <CS_NAM_temple_ruins
	.byte <CS_NAM_utaco_temple
	.byte <0
	.byte <CS_NAM_utaco_temple
	.byte <0
	.byte <CS_NAM_mothership
	.byte <CS_NAM_utaco_temple
	.byte <CS_NAM_lab
	.byte <CS_NAM_utaco_temple
	.byte <0
	.byte <0
	.byte <CS_NAM_mothership
	.byte <CS_NAM_no_mothership
	.byte <CS_NAM_utaco_temple
	.byte <CS_NAM_cutscene_empty
	.byte <CS_NAM_lab
	.byte <CS_NAM_cutscene_empty
	.byte <0
cutscenes_nametables_hi:
	.byte >CS_NAM_destroyed_city
	.byte >CS_NAM_mothership
	.byte >CS_NAM_destroyed_city
	.byte >CS_NAM_temple_ruins
	.byte >CS_NAM_utaco_temple
	.byte >0
	.byte >CS_NAM_utaco_temple
	.byte >0
	.byte >CS_NAM_mothership
	.byte >CS_NAM_utaco_temple
	.byte >CS_NAM_lab
	.byte >CS_NAM_utaco_temple
	.byte >0
	.byte >0
	.byte >CS_NAM_mothership
	.byte >CS_NAM_no_mothership
	.byte >CS_NAM_utaco_temple
	.byte >CS_NAM_cutscene_empty
	.byte >CS_NAM_lab
	.byte >CS_NAM_cutscene_empty
	.byte >0
;Banks:
cutscenes_banks_0:
	.byte $8
	.byte $C
	.byte $8
	.byte $D
	.byte $E
	.byte $0
	.byte $E
	.byte $0
	.byte $C
	.byte $E
	.byte $12
	.byte $E
	.byte $0
	.byte $0
	.byte $C
	.byte $C
	.byte $E
	.byte $C
	.byte $12
	.byte $C
	.byte $0
cutscenes_banks_1:
	.byte $9
	.byte $9
	.byte $9
	.byte $9
	.byte $F
	.byte $0
	.byte $F
	.byte $0
	.byte $9
	.byte $F
	.byte $13
	.byte $F
	.byte $0
	.byte $0
	.byte $9
	.byte $9
	.byte $F
	.byte $9
	.byte $13
	.byte $9
	.byte $0
;Palettes:
cutscenes_pal_destroyed_city:
	.byte $0f,$00,$10,$3c,$0f,$07,$06,$05,$0f,$07,$17,$27,$0f,$06,$0a,$30
	.byte $0f,$00,$10,$3c,$0f,$07,$06,$05,$0f,$07,$17,$27,$0f,$06,$0a,$30
cutscenes_pal_mothership:
	.byte $0f,$03,$13,$23,$0f,$2d,$10,$32,$0f,$0c,$21,$30,$0f,$00,$0c,$23
	.byte $0f,$03,$13,$23,$0f,$2d,$10,$32,$0f,$0c,$21,$30,$0f,$10,$23,$1c
cutscenes_pal_destroyed_city_2:
	.byte $0f,$00,$10,$3c,$0f,$07,$06,$05,$0f,$07,$17,$27,$0f,$06,$0a,$30
	.byte $0f,$00,$10,$3c,$0f,$07,$06,$05,$0f,$07,$17,$27,$0f,$06,$0a,$30
cutscenes_pal_temple_ruins:
	.byte $0f,$06,$16,$26,$0f,$06,$16,$2d,$0f,$0c,$21,$30,$0f,$3d,$16,$2d
	.byte $0f,$06,$16,$26,$0f,$06,$16,$2d,$0f,$0c,$21,$30,$0f,$3d,$16,$2d
cutscenes_pal_utaco_temple:
	.byte $0f,$16,$2c,$36,$0f,$25,$2c,$36,$0f,$16,$25,$05,$0f,$16,$06,$30
	.byte $0f,$0f,$35,$25,$0f,$0f,$14,$36,$0f,$0f,$2c,$30,$0f,$16,$06,$30
cutscenes_pal_utaco_temple_2:
	.byte $0f,$16,$2c,$36,$0f,$25,$2c,$36,$0f,$16,$25,$05,$0f,$16,$06,$30
	.byte $0f,$0f,$35,$25,$0f,$0f,$14,$36,$0f,$0f,$2c,$30,$0f,$16,$06,$30
cutscenes_pal_mothership_2:
	.byte $0f,$03,$13,$23,$0f,$2d,$10,$32,$0f,$0c,$21,$30,$0f,$00,$0c,$23
	.byte $0f,$03,$13,$23,$0f,$2d,$10,$32,$0f,$0c,$21,$30,$0f,$10,$23,$1c
cutscenes_pal_utaco_temple_3:
	.byte $0f,$16,$2c,$36,$0f,$25,$2c,$36,$0f,$16,$25,$05,$0f,$16,$06,$30
	.byte $0f,$0f,$35,$25,$0f,$0f,$14,$36,$0f,$0f,$2c,$30,$0f,$16,$06,$30
cutscenes_pal_the_lab:
	.byte $0f,$30,$36,$27,$0f,$30,$3c,$27,$0f,$11,$3c,$02,$0f,$16,$06,$30
	.byte $0f,$16,$2c,$36,$0f,$25,$2c,$36,$0f,$16,$25,$05,$0f,$16,$06,$30
cutscenes_pal_utaco_temple_4:
	.byte $0f,$16,$2c,$36,$0f,$25,$2c,$36,$0f,$16,$25,$05,$0f,$16,$06,$30
	.byte $0f,$0f,$35,$25,$0f,$0f,$14,$36,$0f,$0f,$2c,$30,$0f,$16,$06,$30
cutscenes_pal_no_mothership:
	.byte $0f,$03,$13,$23,$0f,$2d,$10,$32,$0f,$0c,$21,$30,$0f,$00,$0c,$23
	.byte $0f,$03,$13,$23,$0f,$2d,$10,$32,$0f,$0c,$21,$30,$0f,$10,$23,$1c
cutscenes_pal_no_mothership_2:
	.byte $0f,$03,$13,$23,$0f,$2d,$10,$32,$0f,$0c,$21,$30,$0f,$00,$0c,$23
	.byte $0f,$03,$13,$23,$0f,$2d,$10,$32,$0f,$0c,$21,$30,$0f,$10,$23,$1c
cutscenes_pal_utaco_temple_5:
	.byte $0f,$16,$2c,$36,$0f,$25,$2c,$36,$0f,$16,$25,$05,$0f,$16,$06,$30
	.byte $0f,$0f,$35,$25,$0f,$0f,$14,$36,$0f,$0f,$2c,$30,$0f,$16,$06,$30
cutscenes_pal_cutscene_empty:
	.byte $0f,$03,$13,$23,$0f,$2d,$10,$32,$0f,$0c,$21,$30,$0f,$00,$0c,$23
	.byte $0f,$03,$13,$23,$0f,$2d,$10,$32,$0f,$0c,$21,$30,$0f,$10,$23,$1c
cutscenes_pal_the_lab_2:
	.byte $0f,$30,$36,$27,$0f,$30,$3c,$27,$0f,$11,$3c,$02,$0f,$16,$06,$30
	.byte $0f,$16,$2c,$36,$0f,$25,$2c,$36,$0f,$16,$25,$05,$0f,$16,$06,$30
cutscenes_pal_cutscene_empty_2:
	.byte $0f,$03,$13,$23,$0f,$2d,$10,$32,$0f,$0c,$21,$30,$0f,$00,$0c,$23
	.byte $0f,$03,$13,$23,$0f,$2d,$10,$32,$0f,$0c,$21,$30,$0f,$10,$23,$1c
;Palette Tables:
cutscenes_palettes_lo:
	.byte <cutscenes_pal_destroyed_city
	.byte <cutscenes_pal_mothership
	.byte <cutscenes_pal_destroyed_city_2
	.byte <cutscenes_pal_temple_ruins
	.byte <cutscenes_pal_utaco_temple
	.byte 0
	.byte <cutscenes_pal_utaco_temple_2
	.byte 0
	.byte <cutscenes_pal_mothership_2
	.byte <cutscenes_pal_utaco_temple_3
	.byte <cutscenes_pal_the_lab
	.byte <cutscenes_pal_utaco_temple_4
	.byte 0
	.byte 0
	.byte <cutscenes_pal_no_mothership
	.byte <cutscenes_pal_no_mothership_2
	.byte <cutscenes_pal_utaco_temple_5
	.byte <cutscenes_pal_cutscene_empty
	.byte <cutscenes_pal_the_lab_2
	.byte <cutscenes_pal_cutscene_empty_2
	.byte 0
cutscenes_palettes_hi:
	.byte >cutscenes_pal_destroyed_city
	.byte >cutscenes_pal_mothership
	.byte >cutscenes_pal_destroyed_city_2
	.byte >cutscenes_pal_temple_ruins
	.byte >cutscenes_pal_utaco_temple
	.byte 0
	.byte >cutscenes_pal_utaco_temple_2
	.byte 0
	.byte >cutscenes_pal_mothership_2
	.byte >cutscenes_pal_utaco_temple_3
	.byte >cutscenes_pal_the_lab
	.byte >cutscenes_pal_utaco_temple_4
	.byte 0
	.byte 0
	.byte >cutscenes_pal_no_mothership
	.byte >cutscenes_pal_no_mothership_2
	.byte >cutscenes_pal_utaco_temple_5
	.byte >cutscenes_pal_cutscene_empty
	.byte >cutscenes_pal_the_lab_2
	.byte >cutscenes_pal_cutscene_empty_2
	.byte 0
;Strings: ($FF-ended strings)
cutscenes_string_destroyed_city_0_en:
	 .byte ($13+FontOffset),($22+FontOffset),($2C+FontOffset),($1F+FontOffset),($1F+FontOffset),($40+FontOffset),($1E+FontOffset),($1B+FontOffset),($33+FontOffset),($2D+FontOffset),($40+FontOffset),($22+FontOffset),($1B+FontOffset),($30+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($1F+FontOffset),($26+FontOffset),($1B+FontOffset),($2A+FontOffset),($2D+FontOffset),($1F+FontOffset),($1E+FontOffset),($40+FontOffset),($2D+FontOffset),($23+FontOffset),($28+FontOffset),($1D+FontOffset),($1F+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($1B+FontOffset),($2E+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($25+FontOffset),($40+FontOffset),($2D+FontOffset),($2E+FontOffset),($1B+FontOffset),($2C+FontOffset),($2E+FontOffset),($1F+FontOffset),($1E+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_destroyed_city_0_es:
	 .byte ($F+FontOffset),($1B+FontOffset),($2D+FontOffset),($1B+FontOffset),($2C+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),($2E+FontOffset),($2C+FontOffset),($1F+FontOffset),($2D+FontOffset),($40+FontOffset),($1E+FontOffset),($23+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($1E+FontOffset),($1F+FontOffset),($2D+FontOffset),($1E+FontOffset),($1F+FontOffset),($40+FontOffset),($1F+FontOffset),($26+FontOffset),($40+FontOffset),($1D+FontOffset),($29+FontOffset),($27+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($34+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($1E+FontOffset),($1F+FontOffset),($26+FontOffset),($40+FontOffset),($1B+FontOffset),($2E+FontOffset),($1B+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_destroyed_city_0_ja:
	 .byte ($4A+FontOffset),($43+FontOffset),($49+FontOffset),($70+FontOffset),($47+FontOffset),($40+FontOffset),($59+FontOffset),($40+FontOffset),($5A+FontOffset),($4C+FontOffset),($70+FontOffset),($5F+FontOffset),($68+FontOffset),($40+FontOffset),($46+FontOffset),($67+FontOffset),($40+FontOffset),<($FE+FontOffset),($60+FontOffset),($79+FontOffset),($46+FontOffset),($46+FontOffset),($6E+FontOffset),($40+FontOffset),($50+FontOffset),($79+FontOffset),($50+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_destroyed_city_1_en:
	 .byte ($D+FontOffset),($29+FontOffset),($1C+FontOffset),($29+FontOffset),($1E+FontOffset),($33+FontOffset),($40+FontOffset),($23+FontOffset),($28+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),($31+FontOffset),($29+FontOffset),($2C+FontOffset),($26+FontOffset),($1E+FontOffset),($40+FontOffset),<($FE+FontOffset),($31+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),($1F+FontOffset),($32+FontOffset),($2A+FontOffset),($1F+FontOffset),($1D+FontOffset),($2E+FontOffset),($23+FontOffset),($28+FontOffset),($21+FontOffset),($40+FontOffset),<($FE+FontOffset),($2D+FontOffset),($29+FontOffset),($27+FontOffset),($1F+FontOffset),($2E+FontOffset),($22+FontOffset),($23+FontOffset),($28+FontOffset),($21+FontOffset),($40+FontOffset),($26+FontOffset),($23+FontOffset),($25+FontOffset),($1F+FontOffset),($40+FontOffset),($23+FontOffset),($2E+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_destroyed_city_1_es:
	 .byte ($D+FontOffset),($1B+FontOffset),($1E+FontOffset),($23+FontOffset),($1F+FontOffset),($40+FontOffset),($1F+FontOffset),($28+FontOffset),($40+FontOffset),($1F+FontOffset),($26+FontOffset),($40+FontOffset),($27+FontOffset),($2F+FontOffset),($28+FontOffset),($1E+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($1F+FontOffset),($2D+FontOffset),($2A+FontOffset),($1F+FontOffset),($2C+FontOffset),($1B+FontOffset),($1C+FontOffset),($1B+FontOffset),($40+FontOffset),($1B+FontOffset),($26+FontOffset),($21+FontOffset),($29+FontOffset),($40+FontOffset),($1B+FontOffset),($2D+FontOffset),($23+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_destroyed_city_1_ja:
	 .byte ($4E+FontOffset),($46+FontOffset),($42+FontOffset),($40+FontOffset),($53+FontOffset),($70+FontOffset),($40+FontOffset),($50+FontOffset),($70+FontOffset),($6A+FontOffset),($63+FontOffset),($40+FontOffset),($4A+FontOffset),($6E+FontOffset),($55+FontOffset),($40+FontOffset),($4A+FontOffset),($54+FontOffset),($40+FontOffset),($6D+FontOffset),<($FE+FontOffset),($66+FontOffset),($47+FontOffset),($4C+FontOffset),($55+FontOffset),($46+FontOffset),($79+FontOffset),($50+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_destroyed_city_2_en:
	 .byte ($12+FontOffset),($29+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($2C+FontOffset),($1F+FontOffset),($40+FontOffset),($31+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),($2E+FontOffset),($22+FontOffset),($23+FontOffset),($28+FontOffset),($21+FontOffset),<($FE+FontOffset),($2E+FontOffset),($22+FontOffset),($1B+FontOffset),($2E+FontOffset),($40+FontOffset),($1D+FontOffset),($29+FontOffset),($2F+FontOffset),($26+FontOffset),($1E+FontOffset),($40+FontOffset),($1C+FontOffset),($1F+FontOffset),($40+FontOffset),($1E+FontOffset),($29+FontOffset),($28+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($29+FontOffset),($40+FontOffset),($2D+FontOffset),($2E+FontOffset),($29+FontOffset),($2A+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($27+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_destroyed_city_2_es:
	 .byte ($18+FontOffset),($40+FontOffset),($2A+FontOffset),($29+FontOffset),($2C+FontOffset),($40+FontOffset),($1F+FontOffset),($2D+FontOffset),($29+FontOffset),($40+FontOffset),($28+FontOffset),($1B+FontOffset),($1E+FontOffset),($1B+FontOffset),($40+FontOffset),($2A+FontOffset),($2F+FontOffset),($1E+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($22+FontOffset),($1B+FontOffset),($1D+FontOffset),($1F+FontOffset),($2C+FontOffset),($2D+FontOffset),($1F+FontOffset),($40+FontOffset),($2A+FontOffset),($1B+FontOffset),($2C+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($1E+FontOffset),($1F+FontOffset),($2E+FontOffset),($1F+FontOffset),($28+FontOffset),($1F+FontOffset),($2C+FontOffset),($26+FontOffset),($29+FontOffset),($2D+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_destroyed_city_2_ja:
	 .byte ($4F+FontOffset),($59+FontOffset),($40+FontOffset),($68+FontOffset),($65+FontOffset),($43+FontOffset),($40+FontOffset),($46+FontOffset),($67+FontOffset),($75+FontOffset),($40+FontOffset),($50+FontOffset),($70+FontOffset),($6A+FontOffset),($63+FontOffset),($40+FontOffset),($4F+FontOffset),($42+FontOffset),($52+FontOffset),($40+FontOffset),<($FE+FontOffset),($6D+FontOffset),($40+FontOffset),($54+FontOffset),($62+FontOffset),($67+FontOffset),($6A+FontOffset),($55+FontOffset),($42+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_mothership_0_en:
	 .byte ($13+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),($31+FontOffset),($29+FontOffset),($2C+FontOffset),($26+FontOffset),($1E+FontOffset),($40+FontOffset),($31+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),($23+FontOffset),($28+FontOffset),($40+FontOffset),<($FE+FontOffset),($1E+FontOffset),($1F+FontOffset),($2D+FontOffset),($2A+FontOffset),($1B+FontOffset),($23+FontOffset),($2C+FontOffset),($40+FontOffset),($31+FontOffset),($22+FontOffset),($1F+FontOffset),($28+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($23+FontOffset),($27+FontOffset),($2A+FontOffset),($1F+FontOffset),($2C+FontOffset),($23+FontOffset),($29+FontOffset),($2F+FontOffset),($2D+FontOffset),($40+FontOffset),($27+FontOffset),($29+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($2C+FontOffset),($2D+FontOffset),($22+FontOffset),($23+FontOffset),($2A+FontOffset),<($FE+FontOffset),($1B+FontOffset),($2A+FontOffset),($2A+FontOffset),($1F+FontOffset),($1B+FontOffset),($2C+FontOffset),($1F+FontOffset),($1E+FontOffset),($40+FontOffset),($20+FontOffset),($26+FontOffset),($29+FontOffset),($1B+FontOffset),($2E+FontOffset),($23+FontOffset),($28+FontOffset),($21+FontOffset),($40+FontOffset),($23+FontOffset),($28+FontOffset),<($FE+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),($2D+FontOffset),($25+FontOffset),($33+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_mothership_0_es:
	 .byte ($4+FontOffset),($26+FontOffset),($40+FontOffset),($27+FontOffset),($2F+FontOffset),($28+FontOffset),($1E+FontOffset),($29+FontOffset),($40+FontOffset),($2D+FontOffset),($1F+FontOffset),($40+FontOffset),($22+FontOffset),($2F+FontOffset),($28+FontOffset),($1E+FontOffset),($23+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($1F+FontOffset),($28+FontOffset),($40+FontOffset),($26+FontOffset),($1B+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($2D+FontOffset),($1F+FontOffset),($2D+FontOffset),($2A+FontOffset),($1F+FontOffset),($2C+FontOffset),($1B+FontOffset),($1D+FontOffset),($23+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),<($FE+FontOffset),($1D+FontOffset),($2F+FontOffset),($1B+FontOffset),($28+FontOffset),($1E+FontOffset),($29+FontOffset),($40+FontOffset),($26+FontOffset),($1B+FontOffset),($40+FontOffset),($28+FontOffset),($1B+FontOffset),($30+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($28+FontOffset),($29+FontOffset),($1E+FontOffset),($2C+FontOffset),($23+FontOffset),($34+FontOffset),($1B+FontOffset),($40+FontOffset),($1B+FontOffset),($2A+FontOffset),($1B+FontOffset),($2C+FontOffset),($1F+FontOffset),($1D+FontOffset),($23+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($20+FontOffset),($26+FontOffset),($29+FontOffset),($2E+FontOffset),($1B+FontOffset),($28+FontOffset),($1E+FontOffset),($29+FontOffset),($40+FontOffset),($1F+FontOffset),($28+FontOffset),($40+FontOffset),($1F+FontOffset),($26+FontOffset),($40+FontOffset),<($FE+FontOffset),($1D+FontOffset),($23+FontOffset),($1F+FontOffset),($26+FontOffset),($29+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_mothership_0_ja:
	 .byte ($5E+FontOffset),($70+FontOffset),($4E+FontOffset),($6E+FontOffset),($40+FontOffset),($46+FontOffset),($70+FontOffset),($40+FontOffset),($4F+FontOffset),($67+FontOffset),($40+FontOffset),($56+FontOffset),($40+FontOffset),($41+FontOffset),($67+FontOffset),($6C+FontOffset),($6A+FontOffset),($50+FontOffset),($40+FontOffset),<($FE+FontOffset),($54+FontOffset),($47+FontOffset),($75+FontOffset),($40+FontOffset),($4E+FontOffset),($46+FontOffset),($42+FontOffset),($40+FontOffset),($5A+FontOffset),($40+FontOffset),($4E+FontOffset),($70+FontOffset),($52+FontOffset),($5E+FontOffset),($70+FontOffset),($43+FontOffset),($40+FontOffset),($4C+FontOffset),($50+FontOffset),($73+FontOffset),<($FF+FontOffset)
cutscenes_string_mothership_1_en:
	 .byte ($0+FontOffset),($28+FontOffset),($1E+FontOffset),($40+FontOffset),($2D+FontOffset),($29+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),($1B+FontOffset),($20+FontOffset),($2E+FontOffset),($1F+FontOffset),($2C+FontOffset),($75+FontOffset),($40+FontOffset),<($FE+FontOffset),($1C+FontOffset),($1B+FontOffset),($26+FontOffset),($26+FontOffset),($2D+FontOffset),($40+FontOffset),($29+FontOffset),($20+FontOffset),($40+FontOffset),($20+FontOffset),($23+FontOffset),($2C+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($1F+FontOffset),($27+FontOffset),($1F+FontOffset),($2C+FontOffset),($21+FontOffset),($1F+FontOffset),($1E+FontOffset),($40+FontOffset),($20+FontOffset),($2C+FontOffset),($29+FontOffset),($27+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($2D+FontOffset),($22+FontOffset),($23+FontOffset),($2A+FontOffset),($40+FontOffset),($1B+FontOffset),($28+FontOffset),($1E+FontOffset),($40+FontOffset),($1C+FontOffset),($1F+FontOffset),($21+FontOffset),($1B+FontOffset),($28+FontOffset),($40+FontOffset),($2E+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($31+FontOffset),($2C+FontOffset),($1F+FontOffset),($1B+FontOffset),($25+FontOffset),($40+FontOffset),($1D+FontOffset),($22+FontOffset),($1B+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($1B+FontOffset),($1D+FontOffset),($2C+FontOffset),($29+FontOffset),($2D+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),($21+FontOffset),($26+FontOffset),($29+FontOffset),($1C+FontOffset),($1F+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_mothership_1_es:
	 .byte ($18+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),($40+FontOffset),($27+FontOffset),($2F+FontOffset),($1D+FontOffset),($22+FontOffset),($29+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($2D+FontOffset),($2A+FontOffset),($2F+FontOffset),($1F+FontOffset),($2D+FontOffset),($75+FontOffset),($40+FontOffset),<($FE+FontOffset),($1C+FontOffset),($29+FontOffset),($26+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($40+FontOffset),($20+FontOffset),($2F+FontOffset),($1F+FontOffset),($21+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($1F+FontOffset),($27+FontOffset),($1F+FontOffset),($2C+FontOffset),($21+FontOffset),($23+FontOffset),($1F+FontOffset),($2C+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($40+FontOffset),($26+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($28+FontOffset),($1B+FontOffset),($30+FontOffset),($1F+FontOffset),($40+FontOffset),($33+FontOffset),($40+FontOffset),($1D+FontOffset),($29+FontOffset),($27+FontOffset),($1F+FontOffset),($28+FontOffset),($34+FontOffset),($1B+FontOffset),($2C+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($1F+FontOffset),($2D+FontOffset),($2A+FontOffset),($1B+FontOffset),($2C+FontOffset),($1D+FontOffset),($23+FontOffset),($2C+FontOffset),($40+FontOffset),($1F+FontOffset),($26+FontOffset),($40+FontOffset),($1D+FontOffset),($1B+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($2A+FontOffset),($29+FontOffset),($2C+FontOffset),<($FE+FontOffset),($2E+FontOffset),($29+FontOffset),($1E+FontOffset),($29+FontOffset),($40+FontOffset),($1F+FontOffset),($26+FontOffset),($40+FontOffset),($2A+FontOffset),($26+FontOffset),($1B+FontOffset),($28+FontOffset),($1F+FontOffset),($2E+FontOffset),($1B+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_mothership_1_ja:
	 .byte ($5F+FontOffset),($63+FontOffset),($55+FontOffset),($48+FontOffset),($75+FontOffset),($40+FontOffset),($5B+FontOffset),($40+FontOffset),($59+FontOffset),($40+FontOffset),($50+FontOffset),($5F+FontOffset),($40+FontOffset),($46+FontOffset),($70+FontOffset),($40+FontOffset),($5E+FontOffset),($70+FontOffset),($4E+FontOffset),($6E+FontOffset),<($FE+FontOffset),($46+FontOffset),($67+FontOffset),($40+FontOffset),($53+FontOffset),($70+FontOffset),($53+FontOffset),($75+FontOffset),($40+FontOffset),($4E+FontOffset),($46+FontOffset),($42+FontOffset),($40+FontOffset),($5A+FontOffset),($40+FontOffset),<($FE+FontOffset),($50+FontOffset),($70+FontOffset),($42+FontOffset),($4A+FontOffset),($6E+FontOffset),($67+FontOffset),($6E+FontOffset),($40+FontOffset),($56+FontOffset),($40+FontOffset),($55+FontOffset),($79+FontOffset),($50+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_destroyed_city_2_0_en:
	 .byte ($13+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),($2A+FontOffset),($1F+FontOffset),($29+FontOffset),($2A+FontOffset),($26+FontOffset),($1F+FontOffset),($40+FontOffset),($1B+FontOffset),($2C+FontOffset),($29+FontOffset),($2F+FontOffset),($28+FontOffset),($1E+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),($31+FontOffset),($29+FontOffset),($2C+FontOffset),($26+FontOffset),($1E+FontOffset),($40+FontOffset),($23+FontOffset),($2D+FontOffset),($40+FontOffset),($26+FontOffset),($29+FontOffset),($2D+FontOffset),($23+FontOffset),($28+FontOffset),($21+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($23+FontOffset),($2C+FontOffset),($40+FontOffset),($22+FontOffset),($29+FontOffset),($27+FontOffset),($1F+FontOffset),($2D+FontOffset),($75+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($23+FontOffset),($2C+FontOffset),($40+FontOffset),<($FE+FontOffset),($20+FontOffset),($1B+FontOffset),($27+FontOffset),($23+FontOffset),($26+FontOffset),($23+FontOffset),($1F+FontOffset),($2D+FontOffset),($75+FontOffset),($40+FontOffset),($1B+FontOffset),($28+FontOffset),($1E+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($23+FontOffset),($2C+FontOffset),($40+FontOffset),<($FE+FontOffset),($22+FontOffset),($29+FontOffset),($2A+FontOffset),($1F+FontOffset),($2D+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_destroyed_city_2_0_es:
	 .byte ($B+FontOffset),($1B+FontOffset),($40+FontOffset),($21+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($1F+FontOffset),($40+FontOffset),($1B+FontOffset),($26+FontOffset),($2C+FontOffset),($1F+FontOffset),($1E+FontOffset),($1F+FontOffset),($1E+FontOffset),($29+FontOffset),($2C+FontOffset),($40+FontOffset),<($FE+FontOffset),($1E+FontOffset),($1F+FontOffset),($26+FontOffset),($40+FontOffset),($27+FontOffset),($2F+FontOffset),($28+FontOffset),($1E+FontOffset),($29+FontOffset),($40+FontOffset),($1F+FontOffset),($2D+FontOffset),($2E+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($2A+FontOffset),($1F+FontOffset),($2C+FontOffset),($1E+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($1E+FontOffset),($29+FontOffset),($40+FontOffset),($2D+FontOffset),($2F+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($22+FontOffset),($29+FontOffset),($21+FontOffset),($1B+FontOffset),($2C+FontOffset),($1F+FontOffset),($2D+FontOffset),($75+FontOffset),($40+FontOffset),($2D+FontOffset),($2F+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($20+FontOffset),($1B+FontOffset),($27+FontOffset),($23+FontOffset),($26+FontOffset),($23+FontOffset),($1B+FontOffset),($2D+FontOffset),($75+FontOffset),($40+FontOffset),($33+FontOffset),($40+FontOffset),($2D+FontOffset),($2F+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($1F+FontOffset),($2D+FontOffset),($2A+FontOffset),($1F+FontOffset),($2C+FontOffset),($1B+FontOffset),($28+FontOffset),($34+FontOffset),($1B+FontOffset),($2D+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_destroyed_city_2_0_ja:
	 .byte ($4E+FontOffset),($46+FontOffset),($42+FontOffset),($40+FontOffset),($59+FontOffset),($40+FontOffset),($5B+FontOffset),($54+FontOffset),($5B+FontOffset),($70+FontOffset),($54+FontOffset),($40+FontOffset),($5A+FontOffset),($40+FontOffset),($42+FontOffset),($44+FontOffset),($40+FontOffset),($54+FontOffset),($40+FontOffset),<($FE+FontOffset),($46+FontOffset),($4F+FontOffset),($70+FontOffset),($48+FontOffset),($40+FontOffset),($54+FontOffset),($40+FontOffset),($47+FontOffset),($5E+FontOffset),($70+FontOffset),($43+FontOffset),($40+FontOffset),($6D+FontOffset),($40+FontOffset),<($FE+FontOffset),($43+FontOffset),($4C+FontOffset),($55+FontOffset),($79+FontOffset),($53+FontOffset),($42+FontOffset),($69+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_destroyed_city_2_1_en:
	 .byte ($0+FontOffset),($28+FontOffset),($1E+FontOffset),($40+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($2C+FontOffset),($1F+FontOffset),($40+FontOffset),($1B+FontOffset),($2C+FontOffset),($1F+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($23+FontOffset),($28+FontOffset),($1E+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($2E+FontOffset),($23+FontOffset),($29+FontOffset),($28+FontOffset),($2D+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1B+FontOffset),($2E+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),<($FE+FontOffset),($1D+FontOffset),($29+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($28+FontOffset),($2F+FontOffset),($29+FontOffset),($2F+FontOffset),($2D+FontOffset),($40+FontOffset),($1C+FontOffset),($1B+FontOffset),($2C+FontOffset),($2C+FontOffset),($1B+FontOffset),($21+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($31+FontOffset),($23+FontOffset),($26+FontOffset),($26+FontOffset),($40+FontOffset),($1F+FontOffset),($30+FontOffset),($1F+FontOffset),($2C+FontOffset),($40+FontOffset),($2D+FontOffset),($2E+FontOffset),($29+FontOffset),($2A+FontOffset),($75+FontOffset),($40+FontOffset),($23+FontOffset),($2E+FontOffset),($40+FontOffset),<($FE+FontOffset),($23+FontOffset),($2D+FontOffset),($40+FontOffset),($29+FontOffset),($28+FontOffset),($26+FontOffset),($33+FontOffset),($40+FontOffset),($1B+FontOffset),($40+FontOffset),($27+FontOffset),($1B+FontOffset),($2E+FontOffset),($2E+FontOffset),($1F+FontOffset),($2C+FontOffset),($40+FontOffset),($29+FontOffset),($20+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($23+FontOffset),($27+FontOffset),($1F+FontOffset),($40+FontOffset),($1C+FontOffset),($1F+FontOffset),($20+FontOffset),($29+FontOffset),($2C+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($1F+FontOffset),($30+FontOffset),($1F+FontOffset),($2C+FontOffset),($33+FontOffset),($2E+FontOffset),($22+FontOffset),($23+FontOffset),($28+FontOffset),($21+FontOffset),($40+FontOffset),($23+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($2C+FontOffset),($1F+FontOffset),($1E+FontOffset),($2F+FontOffset),($1D+FontOffset),($1F+FontOffset),($1E+FontOffset),($40+FontOffset),($2E+FontOffset),($29+FontOffset),($40+FontOffset),($1E+FontOffset),($2F+FontOffset),($2D+FontOffset),($2E+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_destroyed_city_2_1_es:
	 .byte ($18+FontOffset),($40+FontOffset),($1D+FontOffset),($29+FontOffset),($27+FontOffset),($29+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),($40+FontOffset),($22+FontOffset),($1B+FontOffset),($33+FontOffset),($40+FontOffset),<($FE+FontOffset),($23+FontOffset),($28+FontOffset),($1E+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($1D+FontOffset),($23+FontOffset),($29+FontOffset),($28+FontOffset),($1F+FontOffset),($2D+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($1F+FontOffset),($26+FontOffset),($40+FontOffset),($1C+FontOffset),($29+FontOffset),($27+FontOffset),($1C+FontOffset),($1B+FontOffset),($2C+FontOffset),($1E+FontOffset),($1F+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($1D+FontOffset),($29+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($28+FontOffset),($2F+FontOffset),($29+FontOffset),($40+FontOffset),($30+FontOffset),($1B+FontOffset),($33+FontOffset),($1B+FontOffset),($40+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($2A+FontOffset),($1B+FontOffset),($2C+FontOffset),($1B+FontOffset),($2C+FontOffset),($75+FontOffset),($40+FontOffset),($1F+FontOffset),($2D+FontOffset),($40+FontOffset),($2D+FontOffset),($29+FontOffset),($26+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($1D+FontOffset),($2F+FontOffset),($1F+FontOffset),($2D+FontOffset),($2E+FontOffset),($23+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($40+FontOffset),($2E+FontOffset),($23+FontOffset),($1F+FontOffset),($27+FontOffset),($2A+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($1B+FontOffset),($28+FontOffset),($2E+FontOffset),($1F+FontOffset),($2D+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($2E+FontOffset),($29+FontOffset),($1E+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($2D+FontOffset),($1F+FontOffset),($1B+FontOffset),($40+FontOffset),($2C+FontOffset),($1F+FontOffset),($1E+FontOffset),($2F+FontOffset),($1D+FontOffset),($23+FontOffset),($1E+FontOffset),($29+FontOffset),($40+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($2A+FontOffset),($29+FontOffset),($26+FontOffset),($30+FontOffset),($29+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_destroyed_city_2_1_ja:
	 .byte ($4A+FontOffset),($59+FontOffset),($40+FontOffset),($50+FontOffset),($70+FontOffset),($6E+FontOffset),($5F+FontOffset),($48+FontOffset),($40+FontOffset),($46+FontOffset),($70+FontOffset),($40+FontOffset),($45+FontOffset),($6C+FontOffset),($69+FontOffset),($40+FontOffset),($59+FontOffset),($5A+FontOffset),($40+FontOffset),<($FE+FontOffset),($4C+FontOffset),($69+FontOffset),($4C+FontOffset),($40+FontOffset),($46+FontOffset),($70+FontOffset),($40+FontOffset),($55+FontOffset),($42+FontOffset),($40+FontOffset),($46+FontOffset),($67+FontOffset),($75+FontOffset),($40+FontOffset),($4D+FontOffset),($5D+FontOffset),($70+FontOffset),($53+FontOffset),($40+FontOffset),<($FE+FontOffset),($46+FontOffset),($70+FontOffset),($40+FontOffset),($51+FontOffset),($68+FontOffset),($40+FontOffset),($56+FontOffset),($40+FontOffset),($55+FontOffset),($69+FontOffset),($40+FontOffset),($5F+FontOffset),($53+FontOffset),($70+FontOffset),($40+FontOffset),($41+FontOffset),($5F+FontOffset),($68+FontOffset),($40+FontOffset),<($FE+FontOffset),($4C+FontOffset),($70+FontOffset),($46+FontOffset),($6E+FontOffset),($40+FontOffset),($46+FontOffset),($70+FontOffset),($40+FontOffset),($55+FontOffset),($42+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_temple_ruins_0_en:
	 .byte ($40+FontOffset),($40+FontOffset),($40+FontOffset),($40+FontOffset),($13+FontOffset),($1F+FontOffset),($27+FontOffset),($2A+FontOffset),($26+FontOffset),($1F+FontOffset),($40+FontOffset),($11+FontOffset),($2F+FontOffset),($23+FontOffset),($28+FontOffset),($2D+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_temple_ruins_0_es:
	 .byte ($40+FontOffset),($40+FontOffset),($11+FontOffset),($2F+FontOffset),($23+FontOffset),($28+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($26+FontOffset),($40+FontOffset),($13+FontOffset),($1F+FontOffset),($27+FontOffset),($2A+FontOffset),($26+FontOffset),($29+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_temple_ruins_0_ja:
	 .byte ($40+FontOffset),($40+FontOffset),($40+FontOffset),($40+FontOffset),($53+FontOffset),($67+FontOffset),($40+FontOffset),($59+FontOffset),($40+FontOffset),($4B+FontOffset),($70+FontOffset),($6E+FontOffset),($46+FontOffset),($70+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_0_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($75+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),($2E+FontOffset),($23+FontOffset),($27+FontOffset),($1F+FontOffset),<($FE+FontOffset),($23+FontOffset),($2D+FontOffset),($40+FontOffset),($2C+FontOffset),($2F+FontOffset),($28+FontOffset),($28+FontOffset),($23+FontOffset),($28+FontOffset),($21+FontOffset),($40+FontOffset),($29+FontOffset),($2F+FontOffset),($2E+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_0_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($75+FontOffset),($40+FontOffset),($1F+FontOffset),($26+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($23+FontOffset),($1F+FontOffset),($27+FontOffset),($2A+FontOffset),($29+FontOffset),($40+FontOffset),($2D+FontOffset),($1F+FontOffset),($40+FontOffset),($1B+FontOffset),($1D+FontOffset),($1B+FontOffset),($1C+FontOffset),($1B+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_0_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($75+FontOffset),($40+FontOffset),($4C+FontOffset),($70+FontOffset),($46+FontOffset),($6E+FontOffset),($40+FontOffset),($46+FontOffset),($70+FontOffset),($40+FontOffset),<($FE+FontOffset),($55+FontOffset),($42+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_1_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($16+FontOffset),($1F+FontOffset),($40+FontOffset),($28+FontOffset),($1F+FontOffset),($1F+FontOffset),($1E+FontOffset),($40+FontOffset),($33+FontOffset),($29+FontOffset),($2F+FontOffset),($2C+FontOffset),($40+FontOffset),<($FE+FontOffset),($2D+FontOffset),($29+FontOffset),($28+FontOffset),($21+FontOffset),($40+FontOffset),($2E+FontOffset),($29+FontOffset),($40+FontOffset),($1B+FontOffset),($1D+FontOffset),($2E+FontOffset),($23+FontOffset),($30+FontOffset),($1B+FontOffset),($2E+FontOffset),($1F+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),<($FE+FontOffset),($2A+FontOffset),($33+FontOffset),($26+FontOffset),($29+FontOffset),($28+FontOffset),($2D+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_1_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($D+FontOffset),($1F+FontOffset),($1D+FontOffset),($1F+FontOffset),($2D+FontOffset),($23+FontOffset),($2E+FontOffset),($1B+FontOffset),($27+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($2E+FontOffset),($2F+FontOffset),($40+FontOffset),<($FE+FontOffset),($1D+FontOffset),($1B+FontOffset),($28+FontOffset),($1D+FontOffset),($23+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),($2A+FontOffset),($1B+FontOffset),($2C+FontOffset),($1B+FontOffset),($40+FontOffset),($1B+FontOffset),($1D+FontOffset),($2E+FontOffset),($23+FontOffset),($30+FontOffset),($1B+FontOffset),($2C+FontOffset),<($FE+FontOffset),($26+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($2A+FontOffset),($23+FontOffset),($26+FontOffset),($29+FontOffset),($28+FontOffset),($1F+FontOffset),($2D+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_1_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($5A+FontOffset),($6F+FontOffset),($42+FontOffset),($6B+FontOffset),($6E+FontOffset),($40+FontOffset),($6D+FontOffset),($40+FontOffset),($46+FontOffset),($79+FontOffset),($4E+FontOffset),($42+FontOffset),($46+FontOffset),($40+FontOffset),<($FE+FontOffset),($4D+FontOffset),($69+FontOffset),($40+FontOffset),($50+FontOffset),($62+FontOffset),($56+FontOffset),($75+FontOffset),($40+FontOffset),($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($40+FontOffset),($59+FontOffset),($40+FontOffset),($43+FontOffset),($50+FontOffset),($40+FontOffset),<($FE+FontOffset),($46+FontOffset),($70+FontOffset),($40+FontOffset),($5B+FontOffset),($52+FontOffset),($66+FontOffset),($43+FontOffset),($40+FontOffset),($50+FontOffset),($70+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_en:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($8+FontOffset),($40+FontOffset),($1E+FontOffset),($29+FontOffset),($28+FontOffset),($74+FontOffset),($2E+FontOffset),($40+FontOffset),($25+FontOffset),($28+FontOffset),($29+FontOffset),($31+FontOffset),($40+FontOffset),<($FE+FontOffset),($23+FontOffset),($20+FontOffset),($40+FontOffset),($8+FontOffset),($40+FontOffset),($1D+FontOffset),($1B+FontOffset),($28+FontOffset),($40+FontOffset),($1E+FontOffset),($29+FontOffset),($40+FontOffset),($23+FontOffset),($2E+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_es:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($D+FontOffset),($29+FontOffset),($40+FontOffset),($2D+FontOffset),($1F+FontOffset),($40+FontOffset),($2D+FontOffset),($23+FontOffset),($40+FontOffset),<($FE+FontOffset),($2A+FontOffset),($29+FontOffset),($1E+FontOffset),($2C+FontOffset),($1F+FontOffset),($40+FontOffset),($22+FontOffset),($1B+FontOffset),($1D+FontOffset),($1F+FontOffset),($2C+FontOffset),($26+FontOffset),($29+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_ja:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($43+FontOffset),($50+FontOffset),($43+FontOffset),($40+FontOffset),($4A+FontOffset),($54+FontOffset),($40+FontOffset),($53+FontOffset),($70+FontOffset),($47+FontOffset),($69+FontOffset),($40+FontOffset),<($FE+FontOffset),($59+FontOffset),($5A+FontOffset),($40+FontOffset),($4C+FontOffset),($67+FontOffset),($55+FontOffset),($42+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($18+FontOffset),($29+FontOffset),($2F+FontOffset),($40+FontOffset),($2E+FontOffset),($2C+FontOffset),($1B+FontOffset),($23+FontOffset),($28+FontOffset),($1F+FontOffset),($1E+FontOffset),($40+FontOffset),<($FE+FontOffset),($27+FontOffset),($29+FontOffset),($2D+FontOffset),($2E+FontOffset),($40+FontOffset),($29+FontOffset),($20+FontOffset),($40+FontOffset),($33+FontOffset),($29+FontOffset),($2F+FontOffset),($2C+FontOffset),($40+FontOffset),($26+FontOffset),($23+FontOffset),($20+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($20+FontOffset),($29+FontOffset),($2C+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($23+FontOffset),($2D+FontOffset),($75+FontOffset),($40+FontOffset),($33+FontOffset),($29+FontOffset),($2F+FontOffset),($40+FontOffset),($31+FontOffset),($23+FontOffset),($26+FontOffset),($26+FontOffset),($40+FontOffset),<($FE+FontOffset),($1C+FontOffset),($1F+FontOffset),($40+FontOffset),($20+FontOffset),($23+FontOffset),($28+FontOffset),($1F+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($7+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($2C+FontOffset),($1F+FontOffset),($28+FontOffset),($1B+FontOffset),($1E+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($2A+FontOffset),($29+FontOffset),($2C+FontOffset),($40+FontOffset),($1D+FontOffset),($1B+FontOffset),($2D+FontOffset),($23+FontOffset),($40+FontOffset),($2E+FontOffset),($29+FontOffset),($1E+FontOffset),($1B+FontOffset),($40+FontOffset),($2E+FontOffset),($2F+FontOffset),($40+FontOffset),<($FE+FontOffset),($30+FontOffset),($23+FontOffset),($1E+FontOffset),($1B+FontOffset),($40+FontOffset),($2A+FontOffset),($1B+FontOffset),($2C+FontOffset),($1B+FontOffset),($40+FontOffset),($1F+FontOffset),($2D+FontOffset),($2E+FontOffset),($29+FontOffset),($75+FontOffset),($40+FontOffset),($30+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($1B+FontOffset),($40+FontOffset),($1F+FontOffset),($2D+FontOffset),($2E+FontOffset),($1B+FontOffset),($2C+FontOffset),($40+FontOffset),($1C+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($40+FontOffset),($5A+FontOffset),($40+FontOffset),($42+FontOffset),($5F+FontOffset),($40+FontOffset),($59+FontOffset),($40+FontOffset),<($FE+FontOffset),($50+FontOffset),($62+FontOffset),($56+FontOffset),($40+FontOffset),($6A+FontOffset),($6E+FontOffset),($4C+FontOffset),($7E+FontOffset),($43+FontOffset),($40+FontOffset),($4D+FontOffset),($69+FontOffset),($40+FontOffset),($50+FontOffset),($62+FontOffset),($56+FontOffset),($40+FontOffset),<($FE+FontOffset),($5E+FontOffset),($54+FontOffset),($6E+FontOffset),($54+FontOffset),($70+FontOffset),($40+FontOffset),($4E+FontOffset),($70+FontOffset),($6E+FontOffset),($4C+FontOffset),($70+FontOffset),($6E+FontOffset),($4E+FontOffset),($42+FontOffset),($40+FontOffset),($6D+FontOffset),($40+FontOffset),<($FE+FontOffset),($52+FontOffset),($42+FontOffset),($64+FontOffset),($4C+FontOffset),($50+FontOffset),($73+FontOffset),($40+FontOffset),($50+FontOffset),($70+FontOffset),($42+FontOffset),($4C+FontOffset),($70+FontOffset),($7F+FontOffset),($43+FontOffset),($5C+FontOffset),($70+FontOffset),($40+FontOffset),($50+FontOffset),($70+FontOffset),($73+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_4_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($D+FontOffset),($29+FontOffset),($31+FontOffset),($71+FontOffset),($40+FontOffset),($0+FontOffset),($1D+FontOffset),($2E+FontOffset),($23+FontOffset),($30+FontOffset),($1B+FontOffset),($2E+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),($C+FontOffset),($2F+FontOffset),($2D+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($26+FontOffset),($40+FontOffset),($2+FontOffset),($22+FontOffset),($1B+FontOffset),($28+FontOffset),($21+FontOffset),($1F+FontOffset),($2C+FontOffset),($71+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_4_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($0+FontOffset),($22+FontOffset),($29+FontOffset),($2C+FontOffset),($1B+FontOffset),($71+FontOffset),($40+FontOffset),($0+FontOffset),($1D+FontOffset),($2E+FontOffset),($23+FontOffset),($30+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($1F+FontOffset),($26+FontOffset),($40+FontOffset),($C+FontOffset),($2F+FontOffset),($2D+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($26+FontOffset),($40+FontOffset),($2+FontOffset),($22+FontOffset),($1B+FontOffset),($28+FontOffset),($21+FontOffset),($1F+FontOffset),($2C+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_4_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($4C+FontOffset),($70+FontOffset),($7D+FontOffset),($71+FontOffset),($40+FontOffset),($60+FontOffset),($7E+FontOffset),($7A+FontOffset),($4C+FontOffset),($70+FontOffset),($46+FontOffset),($69+FontOffset),($40+FontOffset),<($FE+FontOffset),($51+FontOffset),($44+FontOffset),($6E+FontOffset),($4C+FontOffset),($70+FontOffset),($7D+FontOffset),($7A+FontOffset),($40+FontOffset),($6D+FontOffset),($40+FontOffset),($46+FontOffset),($79+FontOffset),($4E+FontOffset),($42+FontOffset),($46+FontOffset),($40+FontOffset),($4C+FontOffset),($6B+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_5_en:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($8+FontOffset),($40+FontOffset),($28+FontOffset),($1F+FontOffset),($30+FontOffset),($1F+FontOffset),($2C+FontOffset),($40+FontOffset),<($FE+FontOffset),($1B+FontOffset),($1D+FontOffset),($2E+FontOffset),($23+FontOffset),($30+FontOffset),($1B+FontOffset),($2E+FontOffset),($1F+FontOffset),($1E+FontOffset),($40+FontOffset),($23+FontOffset),($2E+FontOffset),($40+FontOffset),<($FE+FontOffset),($1C+FontOffset),($1F+FontOffset),($20+FontOffset),($29+FontOffset),($2C+FontOffset),($1F+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),($16+FontOffset),($23+FontOffset),($26+FontOffset),($26+FontOffset),($40+FontOffset),($23+FontOffset),($2E+FontOffset),($40+FontOffset),<($FE+FontOffset),($31+FontOffset),($29+FontOffset),($2C+FontOffset),($25+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_5_es:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($D+FontOffset),($2F+FontOffset),($28+FontOffset),($1D+FontOffset),($1B+FontOffset),($40+FontOffset),($26+FontOffset),($29+FontOffset),($40+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($1B+FontOffset),($1D+FontOffset),($2E+FontOffset),($23+FontOffset),($30+FontOffset),($1B+FontOffset),($1E+FontOffset),($29+FontOffset),($40+FontOffset),($1B+FontOffset),($28+FontOffset),($2E+FontOffset),($1F+FontOffset),($2D+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),<($FE+FontOffset),($5+FontOffset),($2F+FontOffset),($28+FontOffset),($1D+FontOffset),($23+FontOffset),($29+FontOffset),($28+FontOffset),($1B+FontOffset),($2C+FontOffset),($1B+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_5_ja:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($46+FontOffset),($79+FontOffset),($4E+FontOffset),($42+FontOffset),($46+FontOffset),($40+FontOffset),($4C+FontOffset),($50+FontOffset),($40+FontOffset),($4A+FontOffset),($54+FontOffset),($40+FontOffset),<($FE+FontOffset),($46+FontOffset),($70+FontOffset),($40+FontOffset),($55+FontOffset),($42+FontOffset),($73+FontOffset),($54+FontOffset),($70+FontOffset),($43+FontOffset),($4B+FontOffset),($4B+FontOffset),($4E+FontOffset),($69+FontOffset),($40+FontOffset),($59+FontOffset),($5A+FontOffset),($40+FontOffset),($41+FontOffset),($50+FontOffset),($4C+FontOffset),<($FE+FontOffset),($46+FontOffset),($70+FontOffset),($40+FontOffset),($53+FontOffset),($70+FontOffset),($47+FontOffset),($5F+FontOffset),($4D+FontOffset),($46+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_6_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($16+FontOffset),($1F+FontOffset),($40+FontOffset),($1E+FontOffset),($29+FontOffset),($28+FontOffset),($74+FontOffset),($2E+FontOffset),($40+FontOffset),($22+FontOffset),($1B+FontOffset),($30+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($1B+FontOffset),($28+FontOffset),($29+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($2C+FontOffset),($40+FontOffset),($1D+FontOffset),($22+FontOffset),($29+FontOffset),($23+FontOffset),($1D+FontOffset),($1F+FontOffset),($75+FontOffset),($40+FontOffset),($31+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($31+FontOffset),($23+FontOffset),($26+FontOffset),($26+FontOffset),($40+FontOffset),($28+FontOffset),($1F+FontOffset),($30+FontOffset),($1F+FontOffset),($2C+FontOffset),($40+FontOffset),($25+FontOffset),($28+FontOffset),($29+FontOffset),($31+FontOffset),($40+FontOffset),<($FE+FontOffset),($2F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($26+FontOffset),($40+FontOffset),($33+FontOffset),($29+FontOffset),($2F+FontOffset),($40+FontOffset),($2E+FontOffset),($2C+FontOffset),($33+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_6_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($D+FontOffset),($29+FontOffset),($40+FontOffset),($2E+FontOffset),($1F+FontOffset),($28+FontOffset),($1F+FontOffset),($27+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($29+FontOffset),($2E+FontOffset),($2C+FontOffset),($1B+FontOffset),<($FE+FontOffset),($1B+FontOffset),($26+FontOffset),($2E+FontOffset),($1F+FontOffset),($2C+FontOffset),($28+FontOffset),($1B+FontOffset),($2E+FontOffset),($23+FontOffset),($30+FontOffset),($1B+FontOffset),($75+FontOffset),($40+FontOffset),($28+FontOffset),($2F+FontOffset),($28+FontOffset),($1D+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($26+FontOffset),($29+FontOffset),($40+FontOffset),($2D+FontOffset),($1B+FontOffset),($1C+FontOffset),($2C+FontOffset),($1F+FontOffset),($27+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($22+FontOffset),($1B+FontOffset),($2D+FontOffset),($2E+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($26+FontOffset),($29+FontOffset),($40+FontOffset),($23+FontOffset),($28+FontOffset),($2E+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($1F+FontOffset),($2D+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_6_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($4C+FontOffset),($46+FontOffset),($50+FontOffset),($55+FontOffset),($42+FontOffset),($73+FontOffset),($40+FontOffset),($4C+FontOffset),($66+FontOffset),($43+FontOffset),($40+FontOffset),($54+FontOffset),($40+FontOffset),($4D+FontOffset),($69+FontOffset),<($FE+FontOffset),($5F+FontOffset),($53+FontOffset),($70+FontOffset),($40+FontOffset),($4F+FontOffset),($6A+FontOffset),($40+FontOffset),($6D+FontOffset),($40+FontOffset),($4C+FontOffset),($79+FontOffset),($53+FontOffset),($69+FontOffset),($40+FontOffset),($5E+FontOffset),($43+FontOffset),($5E+FontOffset),($43+FontOffset),($40+FontOffset),<($FE+FontOffset),($46+FontOffset),($70+FontOffset),($40+FontOffset),($55+FontOffset),($42+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_7_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($12+FontOffset),($29+FontOffset),($75+FontOffset),($40+FontOffset),($26+FontOffset),($1F+FontOffset),($2E+FontOffset),($74+FontOffset),($2D+FontOffset),($40+FontOffset),($21+FontOffset),($29+FontOffset),($71+FontOffset),($40+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_7_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($0+FontOffset),($2D+FontOffset),($23+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($75+FontOffset),($40+FontOffset),($30+FontOffset),($1B+FontOffset),($27+FontOffset),($29+FontOffset),($2D+FontOffset),($71+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_7_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($4F+FontOffset),($6A+FontOffset),($4C+FontOffset),($70+FontOffset),($7D+FontOffset),($71+FontOffset),($42+FontOffset),($49+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_8_en:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($E+FontOffset),($25+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),($C+FontOffset),($2F+FontOffset),($2D+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($26+FontOffset),<($FE+FontOffset),($1+FontOffset),($1B+FontOffset),($2E+FontOffset),($2E+FontOffset),($26+FontOffset),($1F+FontOffset),($40+FontOffset),($2+FontOffset),($22+FontOffset),($1B+FontOffset),($28+FontOffset),($21+FontOffset),($1F+FontOffset),($71+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_8_es:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($4+FontOffset),($2D+FontOffset),($2E+FontOffset),($1B+FontOffset),($40+FontOffset),($1C+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),<($FE+FontOffset),($C+FontOffset),($2F+FontOffset),($2D+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($26+FontOffset),($40+FontOffset),($1+FontOffset),($1B+FontOffset),($2E+FontOffset),($2E+FontOffset),($26+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($2+FontOffset),($22+FontOffset),($1B+FontOffset),($28+FontOffset),($21+FontOffset),($1F+FontOffset),($71+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_8_ja:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($43+FontOffset),($6E+FontOffset),($71+FontOffset),($40+FontOffset),($60+FontOffset),($7E+FontOffset),($7A+FontOffset),($4C+FontOffset),($70+FontOffset),($46+FontOffset),($69+FontOffset),($40+FontOffset),<($FE+FontOffset),($5A+FontOffset),($70+FontOffset),($54+FontOffset),($69+FontOffset),($40+FontOffset),($51+FontOffset),($44+FontOffset),($6E+FontOffset),($4C+FontOffset),($70+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_0_en:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($3+FontOffset),($23+FontOffset),($1E+FontOffset),($40+FontOffset),($23+FontOffset),($2E+FontOffset),($40+FontOffset),($31+FontOffset),($29+FontOffset),($2C+FontOffset),($25+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_0_es:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($5+FontOffset),($2F+FontOffset),($28+FontOffset),($1D+FontOffset),($23+FontOffset),($29+FontOffset),($28+FontOffset),($29+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_0_ja:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($43+FontOffset),($5F+FontOffset),($48+FontOffset),($42+FontOffset),($79+FontOffset),($50+FontOffset),($46+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_1_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($8+FontOffset),($2E+FontOffset),($40+FontOffset),($26+FontOffset),($29+FontOffset),($29+FontOffset),($25+FontOffset),($2D+FontOffset),($40+FontOffset),($26+FontOffset),($23+FontOffset),($25+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($23+FontOffset),($2E+FontOffset),($40+FontOffset),($1E+FontOffset),($23+FontOffset),($1E+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),($2E+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_1_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($F+FontOffset),($1B+FontOffset),($2C+FontOffset),($1F+FontOffset),($1D+FontOffset),($1F+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_1_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($43+FontOffset),($5F+FontOffset),($48+FontOffset),($42+FontOffset),($46+FontOffset),($55+FontOffset),($46+FontOffset),($79+FontOffset),($50+FontOffset),($4F+FontOffset),($43+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_2_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($16+FontOffset),($1F+FontOffset),($26+FontOffset),($26+FontOffset),($75+FontOffset),($40+FontOffset),($27+FontOffset),($1B+FontOffset),($33+FontOffset),($1C+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($2D+FontOffset),($1F+FontOffset),($40+FontOffset),($1B+FontOffset),($2C+FontOffset),($1F+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),($2E+FontOffset),($40+FontOffset),<($FE+FontOffset),($27+FontOffset),($1B+FontOffset),($21+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($26+FontOffset),($40+FontOffset),($2A+FontOffset),($33+FontOffset),($26+FontOffset),($29+FontOffset),($28+FontOffset),($2D+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_2_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($1+FontOffset),($2F+FontOffset),($1F+FontOffset),($28+FontOffset),($29+FontOffset),($75+FontOffset),($40+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($29+FontOffset),($28+FontOffset),($1D+FontOffset),($1F+FontOffset),($2D+FontOffset),<($FE+FontOffset),($2B+FontOffset),($2F+FontOffset),($23+FontOffset),($34+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),($1F+FontOffset),($2D+FontOffset),($2E+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),($40+FontOffset),($2D+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),<($FE+FontOffset),($2A+FontOffset),($23+FontOffset),($26+FontOffset),($29+FontOffset),($28+FontOffset),($1F+FontOffset),($2D+FontOffset),($40+FontOffset),($27+FontOffset),($1B+FontOffset),($21+FontOffset),($23+FontOffset),($1D+FontOffset),($29+FontOffset),($2D+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_2_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($4F+FontOffset),($6A+FontOffset),($4C+FontOffset),($70+FontOffset),($7D+FontOffset),($75+FontOffset),($40+FontOffset),($4A+FontOffset),($59+FontOffset),($40+FontOffset),($5A+FontOffset),($6F+FontOffset),($42+FontOffset),($6B+FontOffset),($6E+FontOffset),<($FE+FontOffset),($5A+FontOffset),($40+FontOffset),($5F+FontOffset),($4C+FontOffset),($70+FontOffset),($46+FontOffset),($69+FontOffset),($40+FontOffset),($4C+FontOffset),($70+FontOffset),($7D+FontOffset),($55+FontOffset),($42+FontOffset),($40+FontOffset),<($FE+FontOffset),($46+FontOffset),($63+FontOffset),($4C+FontOffset),($6A+FontOffset),($55+FontOffset),($42+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_3_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($8+FontOffset),($40+FontOffset),($24+FontOffset),($2F+FontOffset),($2D+FontOffset),($2E+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($29+FontOffset),($2F+FontOffset),($21+FontOffset),($22+FontOffset),($2E+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($33+FontOffset),($40+FontOffset),($31+FontOffset),($1F+FontOffset),($2C+FontOffset),($1F+FontOffset),($40+FontOffset),($1C+FontOffset),($1F+FontOffset),($1D+FontOffset),($1B+FontOffset),($2F+FontOffset),($2D+FontOffset),($1F+FontOffset),($40+FontOffset),($29+FontOffset),($20+FontOffset),<($FE+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),($31+FontOffset),($1F+FontOffset),($23+FontOffset),($2C+FontOffset),($1E+FontOffset),($40+FontOffset),($2D+FontOffset),($33+FontOffset),($27+FontOffset),($1C+FontOffset),($29+FontOffset),($26+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($33+FontOffset),($40+FontOffset),($22+FontOffset),($1B+FontOffset),($30+FontOffset),($1F+FontOffset),($40+FontOffset),($31+FontOffset),($2C+FontOffset),($23+FontOffset),($2E+FontOffset),($2E+FontOffset),($1F+FontOffset),($28+FontOffset),($40+FontOffset),($29+FontOffset),($28+FontOffset),<($FE+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($27+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_3_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($18+FontOffset),($29+FontOffset),($40+FontOffset),($2D+FontOffset),($29+FontOffset),($26+FontOffset),($29+FontOffset),($40+FontOffset),($2A+FontOffset),($1F+FontOffset),($28+FontOffset),($2D+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($26+FontOffset),($29+FontOffset),($40+FontOffset),($1F+FontOffset),($2C+FontOffset),($1B+FontOffset),($28+FontOffset),($40+FontOffset),($2A+FontOffset),($29+FontOffset),($2C+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($1F+FontOffset),($28+FontOffset),($40+FontOffset),($2D+FontOffset),($23+FontOffset),($27+FontOffset),($1C+FontOffset),($29+FontOffset),($26+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($2C+FontOffset),($1B+FontOffset),($2C+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($1F+FontOffset),($2D+FontOffset),($1D+FontOffset),($2C+FontOffset),($23+FontOffset),($2E+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($1F+FontOffset),($28+FontOffset),($40+FontOffset),<($FE+FontOffset),($1F+FontOffset),($26+FontOffset),($26+FontOffset),($29+FontOffset),($2D+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_3_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($5A+FontOffset),($6F+FontOffset),($42+FontOffset),($6B+FontOffset),($6E+FontOffset),($40+FontOffset),($56+FontOffset),($40+FontOffset),($5D+FontOffset),($6E+FontOffset),($55+FontOffset),($40+FontOffset),<($FE+FontOffset),($47+FontOffset),($4A+FontOffset),($70+FontOffset),($43+FontOffset),($40+FontOffset),($46+FontOffset),($70+FontOffset),($40+FontOffset),($41+FontOffset),($69+FontOffset),($40+FontOffset),($50+FontOffset),($70+FontOffset),($46+FontOffset),($67+FontOffset),($40+FontOffset),<($FE+FontOffset),($5F+FontOffset),($4C+FontOffset),($70+FontOffset),($46+FontOffset),($69+FontOffset),($40+FontOffset),($50+FontOffset),($70+FontOffset),($79+FontOffset),($50+FontOffset),($40+FontOffset),($54+FontOffset),($40+FontOffset),($45+FontOffset),($63+FontOffset),($79+FontOffset),($50+FontOffset),($40+FontOffset),<($FE+FontOffset),($50+FontOffset),($70+FontOffset),($49+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_4_en:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($7+FontOffset),($1B+FontOffset),($22+FontOffset),($1B+FontOffset),($22+FontOffset),($1B+FontOffset),($75+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),($31+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($22+FontOffset),($1B+FontOffset),($2E+FontOffset),($40+FontOffset),($33+FontOffset),($29+FontOffset),($2F+FontOffset),($40+FontOffset),($27+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),($23+FontOffset),($2E+FontOffset),($40+FontOffset),<($FE+FontOffset),($8+FontOffset),($40+FontOffset),($28+FontOffset),($1F+FontOffset),($30+FontOffset),($1F+FontOffset),($2C+FontOffset),($40+FontOffset),($22+FontOffset),($1F+FontOffset),($1B+FontOffset),($2C+FontOffset),($1E+FontOffset),($40+FontOffset),($1B+FontOffset),($1C+FontOffset),($29+FontOffset),($2F+FontOffset),($2E+FontOffset),($40+FontOffset),<($FE+FontOffset),($2A+FontOffset),($33+FontOffset),($26+FontOffset),($29+FontOffset),($28+FontOffset),($2D+FontOffset),($40+FontOffset),($1C+FontOffset),($1F+FontOffset),($23+FontOffset),($28+FontOffset),($21+FontOffset),($40+FontOffset),($27+FontOffset),($1B+FontOffset),($21+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($26+FontOffset),<($FE+FontOffset),($23+FontOffset),($28+FontOffset),($40+FontOffset),($27+FontOffset),($33+FontOffset),($40+FontOffset),($31+FontOffset),($22+FontOffset),($29+FontOffset),($26+FontOffset),($1F+FontOffset),($40+FontOffset),($26+FontOffset),($23+FontOffset),($20+FontOffset),($1F+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_4_es:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($9+FontOffset),($1B+FontOffset),($24+FontOffset),($1B+FontOffset),($24+FontOffset),($1B+FontOffset),($75+FontOffset),($40+FontOffset),($1B+FontOffset),($22+FontOffset),($29+FontOffset),($2C+FontOffset),($1B+FontOffset),<($FE+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($26+FontOffset),($29+FontOffset),($40+FontOffset),($27+FontOffset),($1F+FontOffset),($28+FontOffset),($1D+FontOffset),($23+FontOffset),($29+FontOffset),($28+FontOffset),($1B+FontOffset),($2D+FontOffset),($75+FontOffset),($40+FontOffset),<($FE+FontOffset),($24+FontOffset),($1B+FontOffset),($27+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),($1F+FontOffset),($2D+FontOffset),($1D+FontOffset),($2F+FontOffset),($1D+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),($2D+FontOffset),($29+FontOffset),($1C+FontOffset),($2C+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($2A+FontOffset),($23+FontOffset),($26+FontOffset),($29+FontOffset),($28+FontOffset),($1F+FontOffset),($2D+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($2D+FontOffset),($1F+FontOffset),($1B+FontOffset),($28+FontOffset),($40+FontOffset),<($FE+FontOffset),($27+FontOffset),($1B+FontOffset),($21+FontOffset),($23+FontOffset),($1D+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($1F+FontOffset),($28+FontOffset),($40+FontOffset),($2E+FontOffset),($29+FontOffset),($1E+FontOffset),($1B+FontOffset),($40+FontOffset),($27+FontOffset),($23+FontOffset),($40+FontOffset),<($FE+FontOffset),($30+FontOffset),($23+FontOffset),($1E+FontOffset),($1B+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_4_ja:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($5D+FontOffset),($5D+FontOffset),($75+FontOffset),($40+FontOffset),($4F+FontOffset),($43+FontOffset),($42+FontOffset),($44+FontOffset),($5A+FontOffset),($70+FontOffset),($75+FontOffset),($40+FontOffset),<($FE+FontOffset),($5F+FontOffset),($4C+FontOffset),($70+FontOffset),($46+FontOffset),($69+FontOffset),($40+FontOffset),($5A+FontOffset),($6F+FontOffset),($42+FontOffset),($6B+FontOffset),($6E+FontOffset),($40+FontOffset),($46+FontOffset),($70+FontOffset),($40+FontOffset),($4F+FontOffset),($6E+FontOffset),($4B+FontOffset),($70+FontOffset),($42+FontOffset),<($FE+FontOffset),($4D+FontOffset),($69+FontOffset),($40+FontOffset),($59+FontOffset),($5A+FontOffset),($40+FontOffset),($47+FontOffset),($42+FontOffset),($50+FontOffset),($40+FontOffset),($4A+FontOffset),($54+FontOffset),($40+FontOffset),($46+FontOffset),($70+FontOffset),($40+FontOffset),($55+FontOffset),($42+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_5_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($12+FontOffset),($29+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),($31+FontOffset),($40+FontOffset),($31+FontOffset),($22+FontOffset),($1B+FontOffset),($2E+FontOffset),($40+FontOffset),($31+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($1E+FontOffset),($29+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_5_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($4+FontOffset),($28+FontOffset),($2E+FontOffset),($29+FontOffset),($28+FontOffset),($1D+FontOffset),($1F+FontOffset),($2D+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($22+FontOffset),($1B+FontOffset),($1D+FontOffset),($1F+FontOffset),($27+FontOffset),($29+FontOffset),($2D+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_5_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($54+FontOffset),($70+FontOffset),($43+FontOffset),($4C+FontOffset),($66+FontOffset),($43+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_6_en:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($13+FontOffset),($22+FontOffset),($1B+FontOffset),($2E+FontOffset),($40+FontOffset),($31+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),($1B+FontOffset),($26+FontOffset),($26+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),($2A+FontOffset),($26+FontOffset),($1B+FontOffset),($28+FontOffset),($72+FontOffset),($40+FontOffset),($7+FontOffset),($29+FontOffset),($31+FontOffset),($40+FontOffset),($31+FontOffset),($29+FontOffset),($2F+FontOffset),($26+FontOffset),($1E+FontOffset),($40+FontOffset),<($FE+FontOffset),($8+FontOffset),($40+FontOffset),($25+FontOffset),($28+FontOffset),($29+FontOffset),($31+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_6_es:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($4+FontOffset),($2D+FontOffset),($1F+FontOffset),($40+FontOffset),($1F+FontOffset),($2C+FontOffset),($1B+FontOffset),($40+FontOffset),($2E+FontOffset),($29+FontOffset),($1E+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($1F+FontOffset),($26+FontOffset),($40+FontOffset),($2A+FontOffset),($26+FontOffset),($1B+FontOffset),($28+FontOffset),($72+FontOffset),($40+FontOffset),($18+FontOffset),($40+FontOffset),($1D+FontOffset),($29+FontOffset),($27+FontOffset),($29+FontOffset),($40+FontOffset),($30+FontOffset),($29+FontOffset),($33+FontOffset),($40+FontOffset),<($FE+FontOffset),($1B+FontOffset),($40+FontOffset),($2D+FontOffset),($1B+FontOffset),($1C+FontOffset),($1F+FontOffset),($2C+FontOffset),($26+FontOffset),($29+FontOffset),($40+FontOffset),($33+FontOffset),($29+FontOffset),($72+FontOffset),($40+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_6_ja:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($41+FontOffset),($50+FontOffset),($4C+FontOffset),($40+FontOffset),($5A+FontOffset),($40+FontOffset),($4C+FontOffset),($67+FontOffset),($55+FontOffset),($42+FontOffset),($66+FontOffset),($71+FontOffset),($40+FontOffset),<($FE+FontOffset),($4F+FontOffset),($6A+FontOffset),($40+FontOffset),($5A+FontOffset),($40+FontOffset),($4E+FontOffset),($70+FontOffset),($6E+FontOffset),($5C+FontOffset),($70+FontOffset),($40+FontOffset),($59+FontOffset),($40+FontOffset),($49+FontOffset),($42+FontOffset),($46+FontOffset),($48+FontOffset),($40+FontOffset),<($FE+FontOffset),($50+FontOffset),($70+FontOffset),($79+FontOffset),($50+FontOffset),($46+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_7_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($E+FontOffset),($25+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),($31+FontOffset),($1F+FontOffset),($40+FontOffset),($31+FontOffset),($23+FontOffset),($26+FontOffset),($26+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($2C+FontOffset),($33+FontOffset),($40+FontOffset),($2E+FontOffset),($29+FontOffset),($40+FontOffset),($2F+FontOffset),($2D+FontOffset),($1F+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),($2D+FontOffset),($29+FontOffset),($28+FontOffset),($21+FontOffset),($40+FontOffset),<($FE+FontOffset),($2A+FontOffset),($29+FontOffset),($31+FontOffset),($1F+FontOffset),($2C+FontOffset),($40+FontOffset),($1E+FontOffset),($23+FontOffset),($2C+FontOffset),($1F+FontOffset),($1D+FontOffset),($2E+FontOffset),($26+FontOffset),($33+FontOffset),($40+FontOffset),<($FE+FontOffset),($1B+FontOffset),($21+FontOffset),($1B+FontOffset),($23+FontOffset),($28+FontOffset),($2D+FontOffset),($2E+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),($1F+FontOffset),($28+FontOffset),($1F+FontOffset),($27+FontOffset),($23+FontOffset),($1F+FontOffset),($2D+FontOffset),($73+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_7_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($1+FontOffset),($2F+FontOffset),($1F+FontOffset),($28+FontOffset),($29+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),($30+FontOffset),($1B+FontOffset),($27+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($1B+FontOffset),($40+FontOffset),($23+FontOffset),($28+FontOffset),($2E+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($1B+FontOffset),($2C+FontOffset),($40+FontOffset),($2F+FontOffset),($2E+FontOffset),($23+FontOffset),($26+FontOffset),($23+FontOffset),($34+FontOffset),($1B+FontOffset),($2C+FontOffset),($40+FontOffset),<($FE+FontOffset),($1F+FontOffset),($26+FontOffset),($40+FontOffset),($2A+FontOffset),($29+FontOffset),($1E+FontOffset),($1F+FontOffset),($2C+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($40+FontOffset),($26+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($1D+FontOffset),($1B+FontOffset),($28+FontOffset),($1D+FontOffset),($23+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),($1E+FontOffset),($23+FontOffset),($2C+FontOffset),($1F+FontOffset),($1D+FontOffset),($2E+FontOffset),($1B+FontOffset),($27+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($1F+FontOffset),<($FE+FontOffset),($1D+FontOffset),($29+FontOffset),($28+FontOffset),($2E+FontOffset),($2C+FontOffset),($1B+FontOffset),($40+FontOffset),($26+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($1F+FontOffset),($28+FontOffset),($1F+FontOffset),($27+FontOffset),($23+FontOffset),($21+FontOffset),($29+FontOffset),($2D+FontOffset),($73+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_7_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($4C+FontOffset),($70+FontOffset),($7D+FontOffset),($75+FontOffset),($40+FontOffset),($53+FontOffset),($47+FontOffset),($40+FontOffset),($56+FontOffset),($40+FontOffset),($50+FontOffset),($42+FontOffset),($4C+FontOffset),($53+FontOffset),($40+FontOffset),<($FE+FontOffset),($43+FontOffset),($50+FontOffset),($40+FontOffset),($59+FontOffset),($40+FontOffset),($51+FontOffset),($46+FontOffset),($67+FontOffset),($40+FontOffset),($6D+FontOffset),($40+FontOffset),($51+FontOffset),($7F+FontOffset),($48+FontOffset),($4E+FontOffset),($52+FontOffset),($40+FontOffset),($56+FontOffset),($40+FontOffset),<($FE+FontOffset),($52+FontOffset),($46+FontOffset),($79+FontOffset),($53+FontOffset),($60+FontOffset),($53+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_8_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($8+FontOffset),($20+FontOffset),($40+FontOffset),($33+FontOffset),($29+FontOffset),($2F+FontOffset),($40+FontOffset),($1D+FontOffset),($1B+FontOffset),($28+FontOffset),($40+FontOffset),<($FE+FontOffset),($21+FontOffset),($1F+FontOffset),($28+FontOffset),($1F+FontOffset),($2C+FontOffset),($1B+FontOffset),($2E+FontOffset),($1F+FontOffset),($40+FontOffset),($1F+FontOffset),($28+FontOffset),($29+FontOffset),($2F+FontOffset),($21+FontOffset),($22+FontOffset),($40+FontOffset),($29+FontOffset),($20+FontOffset),($40+FontOffset),<($FE+FontOffset),($23+FontOffset),($2E+FontOffset),($40+FontOffset),($23+FontOffset),($28+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),($1D+FontOffset),($29+FontOffset),($2C+FontOffset),($2C+FontOffset),($1F+FontOffset),($1D+FontOffset),($2E+FontOffset),($40+FontOffset),<($FE+FontOffset),($20+FontOffset),($2C+FontOffset),($1F+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($28+FontOffset),($1D+FontOffset),($33+FontOffset),($75+FontOffset),($40+FontOffset),($23+FontOffset),($2E+FontOffset),($40+FontOffset),($31+FontOffset),($23+FontOffset),($26+FontOffset),($26+FontOffset),($40+FontOffset),<($FE+FontOffset),($22+FontOffset),($1B+FontOffset),($2C+FontOffset),($27+FontOffset),($29+FontOffset),($28+FontOffset),($23+FontOffset),($34+FontOffset),($1F+FontOffset),($40+FontOffset),($31+FontOffset),($23+FontOffset),($2E+FontOffset),($22+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($27+FontOffset),($29+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($2C+FontOffset),($2D+FontOffset),($22+FontOffset),($23+FontOffset),($2A+FontOffset),($74+FontOffset),($2D+FontOffset),($40+FontOffset),($1D+FontOffset),($29+FontOffset),($2C+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($1B+FontOffset),($28+FontOffset),($1E+FontOffset),($40+FontOffset),($1E+FontOffset),($23+FontOffset),($2D+FontOffset),($1B+FontOffset),($1C+FontOffset),($26+FontOffset),($1F+FontOffset),($40+FontOffset),($23+FontOffset),($2E+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_8_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($12+FontOffset),($23+FontOffset),($40+FontOffset),($2A+FontOffset),($29+FontOffset),($1E+FontOffset),($1F+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($21+FontOffset),($1F+FontOffset),($28+FontOffset),($1F+FontOffset),($2C+FontOffset),($1B+FontOffset),($2C+FontOffset),($40+FontOffset),($2D+FontOffset),($2F+FontOffset),($20+FontOffset),($23+FontOffset),($1D+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($2A+FontOffset),($29+FontOffset),($1E+FontOffset),($1F+FontOffset),($2C+FontOffset),($40+FontOffset),($1F+FontOffset),($28+FontOffset),($40+FontOffset),($26+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($20+FontOffset),($2C+FontOffset),($1F+FontOffset),($1D+FontOffset),($2F+FontOffset),($1F+FontOffset),($28+FontOffset),($1D+FontOffset),($23+FontOffset),($1B+FontOffset),($40+FontOffset),($1D+FontOffset),($29+FontOffset),($2C+FontOffset),($2C+FontOffset),($1F+FontOffset),($1D+FontOffset),($2E+FontOffset),($1B+FontOffset),($75+FontOffset),<($FE+FontOffset),($30+FontOffset),($1B+FontOffset),($40+FontOffset),($1B+FontOffset),($40+FontOffset),($22+FontOffset),($1B+FontOffset),($2C+FontOffset),($27+FontOffset),($29+FontOffset),($28+FontOffset),($23+FontOffset),($34+FontOffset),($1B+FontOffset),($2C+FontOffset),($40+FontOffset),($1D+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),<($FE+FontOffset),($1F+FontOffset),($26+FontOffset),($40+FontOffset),($28+FontOffset),($2F+FontOffset),($1D+FontOffset),($26+FontOffset),($1F+FontOffset),($29+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($40+FontOffset),($26+FontOffset),($1B+FontOffset),($40+FontOffset),($28+FontOffset),($1B+FontOffset),($30+FontOffset),($1F+FontOffset),<($FE+FontOffset),($28+FontOffset),($29+FontOffset),($1E+FontOffset),($2C+FontOffset),($23+FontOffset),($34+FontOffset),($1B+FontOffset),($40+FontOffset),($33+FontOffset),($40+FontOffset),<($FE+FontOffset),($1E+FontOffset),($1F+FontOffset),($2D+FontOffset),($22+FontOffset),($1B+FontOffset),($1C+FontOffset),($23+FontOffset),($26+FontOffset),($23+FontOffset),($2E+FontOffset),($1B+FontOffset),($2C+FontOffset),($26+FontOffset),($1B+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_8_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($50+FontOffset),($50+FontOffset),($70+FontOffset),($4C+FontOffset),($42+FontOffset),($40+FontOffset),($4C+FontOffset),($7E+FontOffset),($43+FontOffset),($5A+FontOffset),($4D+FontOffset),($43+FontOffset),($40+FontOffset),($53+FontOffset),($70+FontOffset),<($FE+FontOffset),($4C+FontOffset),($70+FontOffset),($7E+FontOffset),($79+FontOffset),($5C+FontOffset),($6F+FontOffset),($6E+FontOffset),($40+FontOffset),($51+FontOffset),($46+FontOffset),($67+FontOffset),($40+FontOffset),($6D+FontOffset),($40+FontOffset),($5C+FontOffset),($44+FontOffset),($6A+FontOffset),($50+FontOffset),($67+FontOffset),($75+FontOffset),<($FE+FontOffset),($5E+FontOffset),($70+FontOffset),($4E+FontOffset),($6E+FontOffset),($40+FontOffset),($59+FontOffset),($40+FontOffset),($4A+FontOffset),($41+FontOffset),($40+FontOffset),($54+FontOffset),($40+FontOffset),($51+FontOffset),($7F+FontOffset),($43+FontOffset),($6C+FontOffset),($40+FontOffset),($4C+FontOffset),($53+FontOffset),($40+FontOffset),<($FE+FontOffset),($61+FontOffset),($4A+FontOffset),($43+FontOffset),($40+FontOffset),($56+FontOffset),($40+FontOffset),($4D+FontOffset),($69+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_9_en:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($8+FontOffset),($40+FontOffset),($31+FontOffset),($23+FontOffset),($26+FontOffset),($26+FontOffset),($40+FontOffset),($2E+FontOffset),($2C+FontOffset),($33+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_9_es:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($15+FontOffset),($29+FontOffset),($33+FontOffset),($40+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($23+FontOffset),($28+FontOffset),($2E+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($1B+FontOffset),($2C+FontOffset),($26+FontOffset),($29+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_9_ja:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($41+FontOffset),($50+FontOffset),($4C+FontOffset),($40+FontOffset),($5A+FontOffset),($40+FontOffset),($4C+FontOffset),($66+FontOffset),($43+FontOffset),($40+FontOffset),($54+FontOffset),($40+FontOffset),<($FE+FontOffset),($4C+FontOffset),($5F+FontOffset),($4D+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_10_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($B+FontOffset),($1F+FontOffset),($2E+FontOffset),($74+FontOffset),($2D+FontOffset),($40+FontOffset),($21+FontOffset),($29+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_10_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($15+FontOffset),($1B+FontOffset),($27+FontOffset),($29+FontOffset),($2D+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_10_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($42+FontOffset),($49+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_11_en:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($C+FontOffset),($2F+FontOffset),($2D+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($26+FontOffset),($40+FontOffset),<($FE+FontOffset),($1+FontOffset),($1B+FontOffset),($2E+FontOffset),($2E+FontOffset),($26+FontOffset),($1F+FontOffset),($40+FontOffset),($2+FontOffset),($22+FontOffset),($1B+FontOffset),($28+FontOffset),($21+FontOffset),($1F+FontOffset),($71+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_11_es:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($C+FontOffset),($2F+FontOffset),($2D+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($26+FontOffset),($40+FontOffset),<($FE+FontOffset),($1+FontOffset),($1B+FontOffset),($2E+FontOffset),($2E+FontOffset),($26+FontOffset),($1F+FontOffset),($40+FontOffset),($2+FontOffset),($22+FontOffset),($1B+FontOffset),($28+FontOffset),($21+FontOffset),($1F+FontOffset),($71+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_2_11_ja:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($60+FontOffset),($7E+FontOffset),($7A+FontOffset),($4C+FontOffset),($70+FontOffset),($46+FontOffset),($69+FontOffset),($40+FontOffset),($5A+FontOffset),($70+FontOffset),($54+FontOffset),($69+FontOffset),($40+FontOffset),<($FE+FontOffset),($51+FontOffset),($44+FontOffset),($6E+FontOffset),($4C+FontOffset),($70+FontOffset),($71+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_mothership_2_0_en:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($74+FontOffset),($2D+FontOffset),($40+FontOffset),($2D+FontOffset),($29+FontOffset),($28+FontOffset),($21+FontOffset),($40+FontOffset),($31+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),<($FF+FontOffset)
cutscenes_string_mothership_2_0_es:
	 .byte ($B+FontOffset),($1B+FontOffset),($40+FontOffset),($1D+FontOffset),($1B+FontOffset),($28+FontOffset),($1D+FontOffset),($23+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($40+FontOffset),($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_mothership_2_0_ja:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($40+FontOffset),($59+FontOffset),($40+FontOffset),($43+FontOffset),($50+FontOffset),($40+FontOffset),($5A+FontOffset),($40+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_mothership_2_1_en:
	 .byte ($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),($2E+FontOffset),($40+FontOffset),($30+FontOffset),($1F+FontOffset),($2C+FontOffset),($33+FontOffset),($40+FontOffset),<($FE+FontOffset),($1F+FontOffset),($20+FontOffset),($20+FontOffset),($1F+FontOffset),($1D+FontOffset),($2E+FontOffset),($23+FontOffset),($30+FontOffset),($1F+FontOffset),($40+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_mothership_2_1_es:
	 .byte ($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),($40+FontOffset),($20+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($27+FontOffset),($2F+FontOffset),($33+FontOffset),($40+FontOffset),<($FE+FontOffset),($1F+FontOffset),($20+FontOffset),($1F+FontOffset),($1D+FontOffset),($2E+FontOffset),($23+FontOffset),($30+FontOffset),($1B+FontOffset),($40+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_mothership_2_1_ja:
	 .byte ($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),($41+FontOffset),($6E+FontOffset),($5F+FontOffset),($68+FontOffset),($40+FontOffset),($4A+FontOffset),($43+FontOffset),($46+FontOffset),($40+FontOffset),($46+FontOffset),($70+FontOffset),($40+FontOffset),($55+FontOffset),($42+FontOffset),($40+FontOffset),<($FE+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_0_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($16+FontOffset),($1F+FontOffset),($26+FontOffset),($26+FontOffset),($75+FontOffset),($40+FontOffset),($8+FontOffset),($2E+FontOffset),($40+FontOffset),<($FE+FontOffset),($2D+FontOffset),($29+FontOffset),($2F+FontOffset),($28+FontOffset),($1E+FontOffset),($1F+FontOffset),($1E+FontOffset),($40+FontOffset),($26+FontOffset),($23+FontOffset),($25+FontOffset),($1F+FontOffset),($40+FontOffset),($23+FontOffset),($2E+FontOffset),($40+FontOffset),<($FE+FontOffset),($31+FontOffset),($29+FontOffset),($2F+FontOffset),($26+FontOffset),($1E+FontOffset),($40+FontOffset),($31+FontOffset),($29+FontOffset),($2C+FontOffset),($25+FontOffset),($75+FontOffset),($40+FontOffset),($2C+FontOffset),($23+FontOffset),($21+FontOffset),($22+FontOffset),($2E+FontOffset),($72+FontOffset),($40+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_0_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($1+FontOffset),($2F+FontOffset),($1F+FontOffset),($28+FontOffset),($29+FontOffset),($75+FontOffset),($40+FontOffset),($2A+FontOffset),($1B+FontOffset),($2C+FontOffset),($1F+FontOffset),($1D+FontOffset),($23+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($23+FontOffset),($1C+FontOffset),($1B+FontOffset),($40+FontOffset),($1B+FontOffset),($40+FontOffset),($20+FontOffset),($2F+FontOffset),($28+FontOffset),($1D+FontOffset),($23+FontOffset),($29+FontOffset),($28+FontOffset),($1B+FontOffset),($2C+FontOffset),($75+FontOffset),<($FE+FontOffset),($28+FontOffset),($29+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_0_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($43+FontOffset),($5F+FontOffset),($48+FontOffset),($42+FontOffset),($48+FontOffset),($4F+FontOffset),($43+FontOffset),($40+FontOffset),($50+FontOffset),($70+FontOffset),($79+FontOffset),($50+FontOffset),($40+FontOffset),($54+FontOffset),($40+FontOffset),<($FE+FontOffset),($45+FontOffset),($63+FontOffset),($79+FontOffset),($50+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_1_en:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($0+FontOffset),($28+FontOffset),($1E+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),($31+FontOffset),($40+FontOffset),($31+FontOffset),($22+FontOffset),($1B+FontOffset),($2E+FontOffset),($72+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_1_es:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($18+FontOffset),($40+FontOffset),($1B+FontOffset),($22+FontOffset),($29+FontOffset),($2C+FontOffset),($1B+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_1_ja:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($54+FontOffset),($70+FontOffset),($43+FontOffset),($4C+FontOffset),($66+FontOffset),($43+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_2_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($C+FontOffset),($1B+FontOffset),($33+FontOffset),($1C+FontOffset),($1F+FontOffset),($40+FontOffset),($33+FontOffset),($29+FontOffset),($2F+FontOffset),($40+FontOffset),($1E+FontOffset),($23+FontOffset),($1E+FontOffset),($40+FontOffset),<($FE+FontOffset),($23+FontOffset),($2E+FontOffset),($40+FontOffset),($31+FontOffset),($2C+FontOffset),($29+FontOffset),($28+FontOffset),($21+FontOffset),($72+FontOffset),($40+FontOffset),($7+FontOffset),($29+FontOffset),($31+FontOffset),($40+FontOffset),($1E+FontOffset),($23+FontOffset),($1E+FontOffset),($40+FontOffset),<($FE+FontOffset),($33+FontOffset),($29+FontOffset),($2F+FontOffset),($40+FontOffset),($20+FontOffset),($23+FontOffset),($28+FontOffset),($1E+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),($1D+FontOffset),($29+FontOffset),($2C+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($20+FontOffset),($2C+FontOffset),($1F+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($28+FontOffset),($1D+FontOffset),($33+FontOffset),($40+FontOffset),($2E+FontOffset),($29+FontOffset),($40+FontOffset),($27+FontOffset),($1B+FontOffset),($2E+FontOffset),($1D+FontOffset),($22+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_2_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($F+FontOffset),($29+FontOffset),($2C+FontOffset),($40+FontOffset),($1B+FontOffset),($22+FontOffset),($23+FontOffset),($40+FontOffset),($26+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($22+FontOffset),($23+FontOffset),($1D+FontOffset),($23+FontOffset),($2D+FontOffset),($2E+FontOffset),($1F+FontOffset),($40+FontOffset),($27+FontOffset),($1B+FontOffset),($26+FontOffset),($72+FontOffset),($40+FontOffset),($2+FontOffset),($29+FontOffset),($27+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($22+FontOffset),($23+FontOffset),($1D+FontOffset),($23+FontOffset),($2D+FontOffset),($2E+FontOffset),($1F+FontOffset),($40+FontOffset),($2A+FontOffset),($1B+FontOffset),($2C+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($1F+FontOffset),($28+FontOffset),($1D+FontOffset),($29+FontOffset),($28+FontOffset),($2E+FontOffset),($2C+FontOffset),($1B+FontOffset),($2C+FontOffset),($40+FontOffset),($26+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($20+FontOffset),($2C+FontOffset),($1F+FontOffset),($1D+FontOffset),($2F+FontOffset),($1F+FontOffset),($28+FontOffset),($1D+FontOffset),($23+FontOffset),($1B+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($1F+FontOffset),($28+FontOffset),($23+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($23+FontOffset),($21+FontOffset),($2F+FontOffset),($1B+FontOffset),($26+FontOffset),($1B+FontOffset),($2C+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_2_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($47+FontOffset),($60+FontOffset),($40+FontOffset),($5A+FontOffset),($40+FontOffset),($5F+FontOffset),($51+FontOffset),($46+FontOffset),($70+FontOffset),($79+FontOffset),($50+FontOffset),($40+FontOffset),<($FE+FontOffset),($46+FontOffset),($63+FontOffset),($4C+FontOffset),($6A+FontOffset),($55+FontOffset),($42+FontOffset),($73+FontOffset),($40+FontOffset),($50+FontOffset),($50+FontOffset),($70+FontOffset),($4C+FontOffset),($42+FontOffset),($40+FontOffset),($4C+FontOffset),($7E+FontOffset),($43+FontOffset),($5A+FontOffset),($4D+FontOffset),($43+FontOffset),<($FE+FontOffset),($5A+FontOffset),($40+FontOffset),($54+FontOffset),($70+FontOffset),($6E+FontOffset),($55+FontOffset),($40+FontOffset),($5E+FontOffset),($43+FontOffset),($5E+FontOffset),($43+FontOffset),($40+FontOffset),($53+FontOffset),($70+FontOffset),($40+FontOffset),<($FE+FontOffset),($60+FontOffset),($52+FontOffset),($49+FontOffset),($50+FontOffset),($46+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_3_en:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($4+FontOffset),($22+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_3_es:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($4+FontOffset),($22+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_3_ja:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($44+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_4_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($18+FontOffset),($29+FontOffset),($2F+FontOffset),($40+FontOffset),($22+FontOffset),($1B+FontOffset),($1E+FontOffset),($40+FontOffset),($2E+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($27+FontOffset),($1B+FontOffset),($2E+FontOffset),($1D+FontOffset),($22+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($27+FontOffset),($29+FontOffset),($2E+FontOffset),($1F+FontOffset),($2C+FontOffset),($2D+FontOffset),($22+FontOffset),($23+FontOffset),($2A+FontOffset),($74+FontOffset),($2D+FontOffset),($40+FontOffset),($1D+FontOffset),($29+FontOffset),($2C+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($20+FontOffset),($2C+FontOffset),($1F+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($28+FontOffset),($1D+FontOffset),($33+FontOffset),($40+FontOffset),($31+FontOffset),($23+FontOffset),($2E+FontOffset),($22+FontOffset),($40+FontOffset),($33+FontOffset),($29+FontOffset),($2F+FontOffset),($2C+FontOffset),($40+FontOffset),<($FE+FontOffset),($2D+FontOffset),($29+FontOffset),($28+FontOffset),($21+FontOffset),($73+FontOffset),($40+FontOffset),($7+FontOffset),($29+FontOffset),($31+FontOffset),($40+FontOffset),($1E+FontOffset),($23+FontOffset),($1E+FontOffset),($40+FontOffset),($33+FontOffset),($29+FontOffset),($2F+FontOffset),($40+FontOffset),<($FE+FontOffset),($25+FontOffset),($28+FontOffset),($29+FontOffset),($31+FontOffset),($40+FontOffset),($31+FontOffset),($22+FontOffset),($1B+FontOffset),($2E+FontOffset),($40+FontOffset),($20+FontOffset),($2C+FontOffset),($1F+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($28+FontOffset),($1D+FontOffset),($33+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($29+FontOffset),($40+FontOffset),($2F+FontOffset),($2D+FontOffset),($1F+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_4_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($13+FontOffset),($1F+FontOffset),($28+FontOffset),($23+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($23+FontOffset),($21+FontOffset),($2F+FontOffset),($1B+FontOffset),($26+FontOffset),($1B+FontOffset),($2C+FontOffset),($40+FontOffset),($26+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($20+FontOffset),($2C+FontOffset),($1F+FontOffset),($1D+FontOffset),($2F+FontOffset),($1F+FontOffset),($28+FontOffset),($1D+FontOffset),($23+FontOffset),($1B+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($26+FontOffset),($40+FontOffset),<($FE+FontOffset),($28+FontOffset),($2F+FontOffset),($1D+FontOffset),($1F+FontOffset),($26+FontOffset),($29+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($40+FontOffset),($26+FontOffset),($1B+FontOffset),($40+FontOffset),($28+FontOffset),($1B+FontOffset),($30+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($28+FontOffset),($29+FontOffset),($1E+FontOffset),($2C+FontOffset),($23+FontOffset),($34+FontOffset),($1B+FontOffset),($40+FontOffset),($1D+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),($2E+FontOffset),($2F+FontOffset),($40+FontOffset),<($FE+FontOffset),($1D+FontOffset),($1B+FontOffset),($28+FontOffset),($1D+FontOffset),($23+FontOffset),($29+FontOffset),($28+FontOffset),($73+FontOffset),($40+FontOffset),($2+FontOffset),($29+FontOffset),($27+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($2D+FontOffset),($2F+FontOffset),($2A+FontOffset),($23+FontOffset),($2D+FontOffset),($2E+FontOffset),($1F+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($20+FontOffset),($2C+FontOffset),($1F+FontOffset),($1D+FontOffset),($2F+FontOffset),($1F+FontOffset),($28+FontOffset),($1D+FontOffset),($23+FontOffset),($1B+FontOffset),($40+FontOffset),($2F+FontOffset),($2E+FontOffset),($23+FontOffset),($26+FontOffset),($23+FontOffset),($34+FontOffset),($1B+FontOffset),($2C+FontOffset),($72+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_4_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($43+FontOffset),($50+FontOffset),($40+FontOffset),($53+FontOffset),($70+FontOffset),($40+FontOffset),($5E+FontOffset),($70+FontOffset),($4E+FontOffset),($6E+FontOffset),($40+FontOffset),($59+FontOffset),($40+FontOffset),($4A+FontOffset),($41+FontOffset),<($FE+FontOffset),($59+FontOffset),($40+FontOffset),($4C+FontOffset),($7E+FontOffset),($43+FontOffset),($5A+FontOffset),($4D+FontOffset),($43+FontOffset),($40+FontOffset),($56+FontOffset),($40+FontOffset),($41+FontOffset),($6C+FontOffset),($4E+FontOffset),($55+FontOffset),($42+FontOffset),($40+FontOffset),($54+FontOffset),($40+FontOffset),<($FE+FontOffset),($42+FontOffset),($49+FontOffset),($55+FontOffset),($46+FontOffset),($79+FontOffset),($50+FontOffset),($73+FontOffset),($40+FontOffset),($4C+FontOffset),($7E+FontOffset),($43+FontOffset),($5A+FontOffset),($4D+FontOffset),($43+FontOffset),($40+FontOffset),($5A+FontOffset),($40+FontOffset),($54+FontOffset),($70+FontOffset),($43+FontOffset),<($FE+FontOffset),($53+FontOffset),($70+FontOffset),($40+FontOffset),($6C+FontOffset),($46+FontOffset),($68+FontOffset),($5F+FontOffset),($4C+FontOffset),($50+FontOffset),($46+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_5_en:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($0+FontOffset),($22+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),($8+FontOffset),($40+FontOffset),($29+FontOffset),($28+FontOffset),($26+FontOffset),($33+FontOffset),($40+FontOffset),<($FE+FontOffset),($2D+FontOffset),($1B+FontOffset),($28+FontOffset),($21+FontOffset),($40+FontOffset),($20+FontOffset),($2C+FontOffset),($29+FontOffset),($27+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),($1C+FontOffset),($29+FontOffset),($2E+FontOffset),($2E+FontOffset),($29+FontOffset),($27+FontOffset),<($FE+FontOffset),($29+FontOffset),($20+FontOffset),($40+FontOffset),($27+FontOffset),($33+FontOffset),($40+FontOffset),($22+FontOffset),($1F+FontOffset),($1B+FontOffset),($2C+FontOffset),($2E+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_5_es:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($0+FontOffset),($22+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),($18+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($2D+FontOffset),($23+FontOffset),($27+FontOffset),($2A+FontOffset),($26+FontOffset),($1F+FontOffset),($27+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($1F+FontOffset),($40+FontOffset),($1D+FontOffset),($1B+FontOffset),($28+FontOffset),($2E+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($1D+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),($2E+FontOffset),($29+FontOffset),($1E+FontOffset),($29+FontOffset),($40+FontOffset),($27+FontOffset),($23+FontOffset),($40+FontOffset),($1D+FontOffset),($29+FontOffset),($2C+FontOffset),($1B+FontOffset),($34+FontOffset),($29+FontOffset),($28+FontOffset),($73+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_5_ja:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($41+FontOffset),($75+FontOffset),($40+FontOffset),($41+FontOffset),($50+FontOffset),($4C+FontOffset),($40+FontOffset),($5A+FontOffset),($40+FontOffset),($4A+FontOffset),($4A+FontOffset),($6B+FontOffset),($40+FontOffset),<($FE+FontOffset),($59+FontOffset),($40+FontOffset),($4F+FontOffset),($4A+FontOffset),($40+FontOffset),($46+FontOffset),($67+FontOffset),($40+FontOffset),($43+FontOffset),($79+FontOffset),($50+FontOffset),($40+FontOffset),($50+FontOffset),($70+FontOffset),($49+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_6_en:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($E+FontOffset),($1C+FontOffset),($30+FontOffset),($23+FontOffset),($29+FontOffset),($2F+FontOffset),($2D+FontOffset),($26+FontOffset),($33+FontOffset),($75+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($2C+FontOffset),($1F+FontOffset),($40+FontOffset),($23+FontOffset),($2D+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),($40+FontOffset),($31+FontOffset),($1B+FontOffset),($33+FontOffset),($40+FontOffset),($20+FontOffset),($29+FontOffset),($2C+FontOffset),($40+FontOffset),<($FE+FontOffset),($27+FontOffset),($1F+FontOffset),($40+FontOffset),($2E+FontOffset),($29+FontOffset),($40+FontOffset),($25+FontOffset),($28+FontOffset),($29+FontOffset),($31+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($27+FontOffset),($29+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($2C+FontOffset),($2D+FontOffset),($22+FontOffset),($23+FontOffset),($2A+FontOffset),($74+FontOffset),($2D+FontOffset),($40+FontOffset),($1D+FontOffset),($29+FontOffset),($2C+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($20+FontOffset),($2C+FontOffset),($1F+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($28+FontOffset),($1D+FontOffset),($33+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_6_es:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($E+FontOffset),($1C+FontOffset),($30+FontOffset),($23+FontOffset),($1B+FontOffset),($27+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($1F+FontOffset),($75+FontOffset),($40+FontOffset),<($FE+FontOffset),($28+FontOffset),($29+FontOffset),($40+FontOffset),($22+FontOffset),($1B+FontOffset),($33+FontOffset),($40+FontOffset),($20+FontOffset),($29+FontOffset),($2C+FontOffset),($27+FontOffset),($1B+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($33+FontOffset),($29+FontOffset),($40+FontOffset),($2A+FontOffset),($2F+FontOffset),($1F+FontOffset),($1E+FontOffset),($1B+FontOffset),($40+FontOffset),($2D+FontOffset),($1B+FontOffset),($1C+FontOffset),($1F+FontOffset),($2C+FontOffset),($40+FontOffset),($26+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($20+FontOffset),($2C+FontOffset),($1F+FontOffset),($1D+FontOffset),($2F+FontOffset),($1F+FontOffset),($28+FontOffset),($1D+FontOffset),($23+FontOffset),($1B+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($26+FontOffset),($40+FontOffset),<($FE+FontOffset),($28+FontOffset),($2F+FontOffset),($1D+FontOffset),($26+FontOffset),($1F+FontOffset),($29+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($40+FontOffset),($26+FontOffset),($1B+FontOffset),($40+FontOffset),($28+FontOffset),($1B+FontOffset),($30+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($28+FontOffset),($29+FontOffset),($1E+FontOffset),($2C+FontOffset),($23+FontOffset),($34+FontOffset),($1B+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_6_ja:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($63+FontOffset),($51+FontOffset),($6B+FontOffset),($6E+FontOffset),($75+FontOffset),($40+FontOffset),($5E+FontOffset),($70+FontOffset),($4E+FontOffset),($6E+FontOffset),($40+FontOffset),($59+FontOffset),($40+FontOffset),<($FE+FontOffset),($4A+FontOffset),($41+FontOffset),($40+FontOffset),($59+FontOffset),($40+FontOffset),($4C+FontOffset),($7E+FontOffset),($43+FontOffset),($5A+FontOffset),($4D+FontOffset),($43+FontOffset),($40+FontOffset),($6D+FontOffset),($40+FontOffset),($4C+FontOffset),($69+FontOffset),($40+FontOffset),($6C+FontOffset),($49+FontOffset),($40+FontOffset),<($FE+FontOffset),($55+FontOffset),($42+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_7_en:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($12+FontOffset),($29+FontOffset),($40+FontOffset),($8+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($29+FontOffset),($2F+FontOffset),($21+FontOffset),($22+FontOffset),($2E+FontOffset),($40+FontOffset),<($FE+FontOffset),($23+FontOffset),($2E+FontOffset),($40+FontOffset),($31+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),($29+FontOffset),($28+FontOffset),($1F+FontOffset),($40+FontOffset),($29+FontOffset),($20+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($29+FontOffset),($2D+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($1D+FontOffset),($1B+FontOffset),($2D+FontOffset),($1F+FontOffset),($2D+FontOffset),($40+FontOffset),($23+FontOffset),($28+FontOffset),($40+FontOffset),($31+FontOffset),($22+FontOffset),($23+FontOffset),($1D+FontOffset),($22+FontOffset),($40+FontOffset),($1B+FontOffset),($20+FontOffset),($2E+FontOffset),($1F+FontOffset),($2C+FontOffset),<($FE+FontOffset),($26+FontOffset),($23+FontOffset),($2D+FontOffset),($2E+FontOffset),($1F+FontOffset),($28+FontOffset),($23+FontOffset),($28+FontOffset),($21+FontOffset),($40+FontOffset),($2E+FontOffset),($29+FontOffset),($40+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($1D+FontOffset),($29+FontOffset),($28+FontOffset),($30+FontOffset),($23+FontOffset),($28+FontOffset),($1D+FontOffset),($23+FontOffset),($28+FontOffset),($21+FontOffset),($40+FontOffset),<($FE+FontOffset),($1F+FontOffset),($32+FontOffset),($2A+FontOffset),($26+FontOffset),($1B+FontOffset),($28+FontOffset),($1B+FontOffset),($2E+FontOffset),($23+FontOffset),($29+FontOffset),($28+FontOffset),($75+FontOffset),($40+FontOffset),<($FE+FontOffset),($1F+FontOffset),($30+FontOffset),($1F+FontOffset),($2C+FontOffset),($33+FontOffset),($2E+FontOffset),($22+FontOffset),($23+FontOffset),($28+FontOffset),($21+FontOffset),($40+FontOffset),($24+FontOffset),($2F+FontOffset),($2D+FontOffset),($2E+FontOffset),($40+FontOffset),<($FE+FontOffset),($31+FontOffset),($29+FontOffset),($2C+FontOffset),($25+FontOffset),($2D+FontOffset),($40+FontOffset),($20+FontOffset),($23+FontOffset),($28+FontOffset),($1F+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_7_es:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($0+FontOffset),($2D+FontOffset),($23+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($2A+FontOffset),($1F+FontOffset),($28+FontOffset),($2D+FontOffset),($1F+FontOffset),<($FE+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($1F+FontOffset),($2C+FontOffset),($1B+FontOffset),($40+FontOffset),($2F+FontOffset),($28+FontOffset),($29+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($40+FontOffset),($1F+FontOffset),($2D+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($1D+FontOffset),($1B+FontOffset),($2D+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($1F+FontOffset),($28+FontOffset),($40+FontOffset),($26+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($1D+FontOffset),($2F+FontOffset),($1B+FontOffset),($26+FontOffset),($1F+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($1E+FontOffset),($1F+FontOffset),($2D+FontOffset),($2A+FontOffset),($2F+FontOffset),($1F+FontOffset),($2D+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($40+FontOffset),($1F+FontOffset),($2D+FontOffset),($1D+FontOffset),($2F+FontOffset),($1D+FontOffset),($22+FontOffset),($1B+FontOffset),($2C+FontOffset),($40+FontOffset),<($FE+FontOffset),($2F+FontOffset),($28+FontOffset),($1B+FontOffset),($40+FontOffset),($1F+FontOffset),($32+FontOffset),($2A+FontOffset),($26+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($1D+FontOffset),($23+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($2D+FontOffset),($2F+FontOffset),($1F+FontOffset),($28+FontOffset),($1F+FontOffset),($40+FontOffset),($1D+FontOffset),($29+FontOffset),($28+FontOffset),($30+FontOffset),($23+FontOffset),($28+FontOffset),($1D+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($1F+FontOffset),($75+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($29+FontOffset),($1E+FontOffset),($29+FontOffset),($40+FontOffset),($20+FontOffset),($2F+FontOffset),($28+FontOffset),($1D+FontOffset),($23+FontOffset),($29+FontOffset),($28+FontOffset),($1B+FontOffset),($40+FontOffset),($33+FontOffset),($40+FontOffset),($33+FontOffset),($1B+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_7_ja:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($74+FontOffset),($4E+FontOffset),($79+FontOffset),($54+FontOffset),($48+FontOffset),($68+FontOffset),($7F+FontOffset),($48+FontOffset),($40+FontOffset),($59+FontOffset),($40+FontOffset),($41+FontOffset),($69+FontOffset),<($FE+FontOffset),($4E+FontOffset),($52+FontOffset),($62+FontOffset),($42+FontOffset),($40+FontOffset),($6D+FontOffset),($40+FontOffset),($47+FontOffset),($42+FontOffset),($50+FontOffset),($40+FontOffset),($41+FontOffset),($54+FontOffset),($40+FontOffset),($56+FontOffset),($40+FontOffset),($4D+FontOffset),($5D+FontOffset),($70+FontOffset),($53+FontOffset),<($FE+FontOffset),($5A+FontOffset),($40+FontOffset),($43+FontOffset),($5F+FontOffset),($48+FontOffset),($42+FontOffset),($48+FontOffset),($74+FontOffset),($40+FontOffset),($4F+FontOffset),($43+FontOffset),($42+FontOffset),($43+FontOffset),($40+FontOffset),($5A+FontOffset),($70+FontOffset),($41+FontOffset),($42+FontOffset),($40+FontOffset),<($FE+FontOffset),($50+FontOffset),($70+FontOffset),($79+FontOffset),($50+FontOffset),($40+FontOffset),($54+FontOffset),($40+FontOffset),($45+FontOffset),($63+FontOffset),($79+FontOffset),($50+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_8_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($8+FontOffset),($20+FontOffset),($40+FontOffset),($31+FontOffset),($1F+FontOffset),($40+FontOffset),($1E+FontOffset),($29+FontOffset),($28+FontOffset),($74+FontOffset),($2E+FontOffset),($40+FontOffset),<($FE+FontOffset),($25+FontOffset),($28+FontOffset),($29+FontOffset),($31+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),($1D+FontOffset),($29+FontOffset),($2C+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($20+FontOffset),($2C+FontOffset),($1F+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($28+FontOffset),($1D+FontOffset),($33+FontOffset),($40+FontOffset),($29+FontOffset),($2F+FontOffset),($2C+FontOffset),($40+FontOffset),($2A+FontOffset),($26+FontOffset),($1B+FontOffset),($28+FontOffset),($40+FontOffset),<($FE+FontOffset),($23+FontOffset),($2D+FontOffset),($40+FontOffset),($2F+FontOffset),($2D+FontOffset),($1F+FontOffset),($26+FontOffset),($1F+FontOffset),($2D+FontOffset),($2D+FontOffset),($73+FontOffset),($40+FontOffset),($16+FontOffset),($22+FontOffset),($1B+FontOffset),($2E+FontOffset),($40+FontOffset),($1B+FontOffset),($2C+FontOffset),($1F+FontOffset),<($FE+FontOffset),($31+FontOffset),($1F+FontOffset),($40+FontOffset),($21+FontOffset),($29+FontOffset),($23+FontOffset),($28+FontOffset),($21+FontOffset),($40+FontOffset),($2E+FontOffset),($29+FontOffset),($40+FontOffset),($1E+FontOffset),($29+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_8_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($12+FontOffset),($23+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),($40+FontOffset),($2D+FontOffset),($1B+FontOffset),($1C+FontOffset),($1F+FontOffset),($27+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($26+FontOffset),($1B+FontOffset),($40+FontOffset),($20+FontOffset),($2C+FontOffset),($1F+FontOffset),($1D+FontOffset),($2F+FontOffset),($1F+FontOffset),($28+FontOffset),($1D+FontOffset),($23+FontOffset),($1B+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($26+FontOffset),($40+FontOffset),<($FE+FontOffset),($28+FontOffset),($2F+FontOffset),($1D+FontOffset),($1F+FontOffset),($26+FontOffset),($29+FontOffset),($40+FontOffset),($28+FontOffset),($2F+FontOffset),($1F+FontOffset),($2D+FontOffset),($2E+FontOffset),($2C+FontOffset),($29+FontOffset),($40+FontOffset),($2A+FontOffset),($26+FontOffset),($1B+FontOffset),($28+FontOffset),($40+FontOffset),<($FE+FontOffset),($28+FontOffset),($29+FontOffset),($40+FontOffset),($2E+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($1F+FontOffset),($40+FontOffset),($2D+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($1E+FontOffset),($29+FontOffset),($73+FontOffset),($40+FontOffset),<($FE+FontOffset),($10+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($30+FontOffset),($1B+FontOffset),($27+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($1B+FontOffset),($40+FontOffset),($22+FontOffset),($1B+FontOffset),($1D+FontOffset),($1F+FontOffset),($2C+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_8_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($4C+FontOffset),($7E+FontOffset),($43+FontOffset),($5A+FontOffset),($4D+FontOffset),($43+FontOffset),($40+FontOffset),($6D+FontOffset),($40+FontOffset),<($FE+FontOffset),($4C+FontOffset),($67+FontOffset),($55+FontOffset),($49+FontOffset),($6A+FontOffset),($5A+FontOffset),($70+FontOffset),($75+FontOffset),($40+FontOffset),($4A+FontOffset),($59+FontOffset),($40+FontOffset),($49+FontOffset),($42+FontOffset),($46+FontOffset),($48+FontOffset),($40+FontOffset),($5A+FontOffset),($40+FontOffset),<($FE+FontOffset),($50+FontOffset),($70+FontOffset),($62+FontOffset),($40+FontOffset),($53+FontOffset),($70+FontOffset),($4D+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_9_en:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($C+FontOffset),($1B+FontOffset),($33+FontOffset),($1C+FontOffset),($1F+FontOffset),($40+FontOffset),($31+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($2D+FontOffset),($22+FontOffset),($29+FontOffset),($2F+FontOffset),($26+FontOffset),($1E+FontOffset),($40+FontOffset),($22+FontOffset),($23+FontOffset),($2C+FontOffset),($1F+FontOffset),($40+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($2D+FontOffset),($1D+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($2D+FontOffset),($2E+FontOffset),($40+FontOffset),($2E+FontOffset),($29+FontOffset),($40+FontOffset),($2D+FontOffset),($29+FontOffset),($26+FontOffset),($30+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($23+FontOffset),($2E+FontOffset),($40+FontOffset),($20+FontOffset),($29+FontOffset),($2C+FontOffset),($40+FontOffset),($2F+FontOffset),($2D+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_9_es:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($10+FontOffset),($2F+FontOffset),($23+FontOffset),($34+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($1E+FontOffset),($1F+FontOffset),($1C+FontOffset),($1F+FontOffset),($2C+FontOffset),($23+FontOffset),($1B+FontOffset),($27+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($1D+FontOffset),($29+FontOffset),($28+FontOffset),($2E+FontOffset),($2C+FontOffset),($1B+FontOffset),($2E+FontOffset),($1B+FontOffset),($2C+FontOffset),<($FE+FontOffset),($1B+FontOffset),($40+FontOffset),($2F+FontOffset),($28+FontOffset),($40+FontOffset),($1D+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($20+FontOffset),($23+FontOffset),($1D+FontOffset),($29+FontOffset),($40+FontOffset),($2A+FontOffset),($1B+FontOffset),($2C+FontOffset),($1B+FontOffset),<($FE+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($26+FontOffset),($29+FontOffset),($40+FontOffset),($2D+FontOffset),($29+FontOffset),($26+FontOffset),($2F+FontOffset),($1D+FontOffset),($23+FontOffset),($29+FontOffset),($28+FontOffset),($1F+FontOffset),($40+FontOffset),($2A+FontOffset),($29+FontOffset),($2C+FontOffset),<($FE+FontOffset),($28+FontOffset),($29+FontOffset),($2D+FontOffset),($29+FontOffset),($2E+FontOffset),($2C+FontOffset),($29+FontOffset),($2D+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_9_ja:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($6C+FontOffset),($50+FontOffset),($4C+FontOffset),($50+FontOffset),($51+FontOffset),($40+FontOffset),($59+FontOffset),($40+FontOffset),($46+FontOffset),($6C+FontOffset),($68+FontOffset),($40+FontOffset),($56+FontOffset),<($FE+FontOffset),($4F+FontOffset),($6A+FontOffset),($40+FontOffset),($6D+FontOffset),($40+FontOffset),($46+FontOffset),($42+FontOffset),($49+FontOffset),($52+FontOffset),($40+FontOffset),($4D+FontOffset),($69+FontOffset),($40+FontOffset),($56+FontOffset),($40+FontOffset),($5A+FontOffset),($40+FontOffset),<($FE+FontOffset),($46+FontOffset),($46+FontOffset),($70+FontOffset),($48+FontOffset),($4C+FontOffset),($7D+FontOffset),($40+FontOffset),($6D+FontOffset),($40+FontOffset),($64+FontOffset),($54+FontOffset),($43+FontOffset),($40+FontOffset),($46+FontOffset),($63+FontOffset),($4C+FontOffset),($6A+FontOffset),($55+FontOffset),($42+FontOffset),($73+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_10_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($E+FontOffset),($25+FontOffset),($75+FontOffset),($40+FontOffset),($8+FontOffset),($40+FontOffset),($31+FontOffset),($23+FontOffset),($26+FontOffset),($26+FontOffset),($40+FontOffset),($2E+FontOffset),($2C+FontOffset),($33+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($29+FontOffset),($40+FontOffset),($20+FontOffset),($23+FontOffset),($28+FontOffset),($1E+FontOffset),($40+FontOffset),($29+FontOffset),($28+FontOffset),($1F+FontOffset),($40+FontOffset),($31+FontOffset),($22+FontOffset),($29+FontOffset),($40+FontOffset),($31+FontOffset),($23+FontOffset),($26+FontOffset),($26+FontOffset),<($FE+FontOffset),($26+FontOffset),($23+FontOffset),($2D+FontOffset),($2E+FontOffset),($1F+FontOffset),($28+FontOffset),($40+FontOffset),($2E+FontOffset),($29+FontOffset),($40+FontOffset),($2F+FontOffset),($2D+FontOffset),($40+FontOffset),($20+FontOffset),($29+FontOffset),($2C+FontOffset),($40+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($2C+FontOffset),($1F+FontOffset),($1B+FontOffset),($2D+FontOffset),($29+FontOffset),($28+FontOffset),($1B+FontOffset),($1C+FontOffset),($26+FontOffset),($1F+FontOffset),($40+FontOffset),($2A+FontOffset),($2C+FontOffset),($23+FontOffset),($1D+FontOffset),($1F+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_10_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($4+FontOffset),($2D+FontOffset),($2E+FontOffset),($1B+FontOffset),($40+FontOffset),($1C+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($75+FontOffset),($40+FontOffset),($30+FontOffset),($29+FontOffset),($33+FontOffset),($40+FontOffset),<($FE+FontOffset),($1B+FontOffset),($40+FontOffset),($23+FontOffset),($28+FontOffset),($2E+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($1B+FontOffset),($2C+FontOffset),($40+FontOffset),($1F+FontOffset),($28+FontOffset),($1D+FontOffset),($29+FontOffset),($28+FontOffset),($2E+FontOffset),($2C+FontOffset),($1B+FontOffset),($2C+FontOffset),<($FE+FontOffset),($1B+FontOffset),($26+FontOffset),($21+FontOffset),($2F+FontOffset),($28+FontOffset),($29+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($23+FontOffset),($1F+FontOffset),($2C+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($1F+FontOffset),($2D+FontOffset),($1D+FontOffset),($2F+FontOffset),($1D+FontOffset),($22+FontOffset),($1B+FontOffset),($2C+FontOffset),($28+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($2A+FontOffset),($29+FontOffset),($2C+FontOffset),($40+FontOffset),($2F+FontOffset),($28+FontOffset),($40+FontOffset),<($FE+FontOffset),($2A+FontOffset),($2C+FontOffset),($1F+FontOffset),($1D+FontOffset),($23+FontOffset),($29+FontOffset),($40+FontOffset),($2C+FontOffset),($1B+FontOffset),($34+FontOffset),($29+FontOffset),($28+FontOffset),($1B+FontOffset),($1C+FontOffset),($26+FontOffset),($1F+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_10_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($4C+FontOffset),($70+FontOffset),($7D+FontOffset),($75+FontOffset),($40+FontOffset),($53+FontOffset),($47+FontOffset),($4E+FontOffset),($42+FontOffset),($46+FontOffset),($46+FontOffset),($48+FontOffset),($40+FontOffset),($59+FontOffset),($40+FontOffset),<($FE+FontOffset),($46+FontOffset),($46+FontOffset),($70+FontOffset),($48+FontOffset),($4C+FontOffset),($7D+FontOffset),($40+FontOffset),($6D+FontOffset),($40+FontOffset),($4B+FontOffset),($46+FontOffset),($70+FontOffset),($4C+FontOffset),($53+FontOffset),($42+FontOffset),($47+FontOffset),($5F+FontOffset),($4D+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_11_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($1+FontOffset),($2F+FontOffset),($2E+FontOffset),($40+FontOffset),($23+FontOffset),($2E+FontOffset),($40+FontOffset),($31+FontOffset),($23+FontOffset),($26+FontOffset),($26+FontOffset),($40+FontOffset),($1C+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($22+FontOffset),($1B+FontOffset),($2C+FontOffset),($1E+FontOffset),($75+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($29+FontOffset),($2D+FontOffset),($1F+FontOffset),($40+FontOffset),($21+FontOffset),($2F+FontOffset),($33+FontOffset),($2D+FontOffset),($40+FontOffset),($1B+FontOffset),($2C+FontOffset),($1F+FontOffset),<($FE+FontOffset),($1B+FontOffset),($26+FontOffset),($26+FontOffset),($40+FontOffset),($20+FontOffset),($1B+FontOffset),($27+FontOffset),($29+FontOffset),($2F+FontOffset),($2D+FontOffset),($40+FontOffset),($1B+FontOffset),($28+FontOffset),($1E+FontOffset),($40+FontOffset),<($FE+FontOffset),($27+FontOffset),($23+FontOffset),($26+FontOffset),($26+FontOffset),($29+FontOffset),($28+FontOffset),($1B+FontOffset),($23+FontOffset),($2C+FontOffset),($1F+FontOffset),($2D+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_11_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($F+FontOffset),($1F+FontOffset),($2C+FontOffset),($29+FontOffset),($40+FontOffset),($2D+FontOffset),($1F+FontOffset),($2C+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($1E+FontOffset),($23+FontOffset),($20+FontOffset),($23+FontOffset),($1D+FontOffset),($23+FontOffset),($26+FontOffset),($75+FontOffset),($40+FontOffset),($1F+FontOffset),($2D+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($1D+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($20+FontOffset),($23+FontOffset),($1D+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($2D+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($29+FontOffset),($1E+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($20+FontOffset),($1B+FontOffset),($27+FontOffset),($29+FontOffset),($2D+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($33+FontOffset),($40+FontOffset),<($FE+FontOffset),($27+FontOffset),($23+FontOffset),($26+FontOffset),($26+FontOffset),($29+FontOffset),($28+FontOffset),($1B+FontOffset),($2C+FontOffset),($23+FontOffset),($29+FontOffset),($2D+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_3_11_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($50+FontOffset),($70+FontOffset),($49+FontOffset),($54+FontOffset),($70+FontOffset),($75+FontOffset),($40+FontOffset),($61+FontOffset),($4D+FontOffset),($70+FontOffset),($46+FontOffset),($4C+FontOffset),($42+FontOffset),($40+FontOffset),<($FE+FontOffset),($4A+FontOffset),($54+FontOffset),($40+FontOffset),($50+FontOffset),($70+FontOffset),($73+FontOffset),($40+FontOffset),($46+FontOffset),($46+FontOffset),($70+FontOffset),($48+FontOffset),($4C+FontOffset),($7D+FontOffset),($40+FontOffset),($5A+FontOffset),($40+FontOffset),($60+FontOffset),($6E+FontOffset),($55+FontOffset),($40+FontOffset),<($FE+FontOffset),($65+FontOffset),($43+FontOffset),($62+FontOffset),($42+FontOffset),($40+FontOffset),($4F+FontOffset),($4C+FontOffset),($53+FontOffset),($40+FontOffset),($46+FontOffset),($58+FontOffset),($63+FontOffset),($51+FontOffset),($50+FontOffset),($70+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_0_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),($0+FontOffset),($28+FontOffset),($1E+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1B+FontOffset),($2E+FontOffset),($40+FontOffset),($23+FontOffset),($2D+FontOffset),<($FE+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),($2A+FontOffset),($26+FontOffset),($1B+FontOffset),($28+FontOffset),($73+FontOffset),($40+FontOffset),($2+FontOffset),($1B+FontOffset),($28+FontOffset),($40+FontOffset),($33+FontOffset),($29+FontOffset),($2F+FontOffset),($40+FontOffset),<($FE+FontOffset),($22+FontOffset),($1F+FontOffset),($26+FontOffset),($2A+FontOffset),($40+FontOffset),($2F+FontOffset),($2D+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_0_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),($18+FontOffset),($40+FontOffset),($1F+FontOffset),($2D+FontOffset),($1F+FontOffset),($40+FontOffset),($1F+FontOffset),($2D+FontOffset),($40+FontOffset),($1F+FontOffset),($26+FontOffset),<($FE+FontOffset),($2A+FontOffset),($26+FontOffset),($1B+FontOffset),($28+FontOffset),($73+FontOffset),($40+FontOffset),($F+FontOffset),($29+FontOffset),($1E+FontOffset),($1F+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($1B+FontOffset),($33+FontOffset),($2F+FontOffset),($1E+FontOffset),($1B+FontOffset),($2C+FontOffset),($28+FontOffset),($29+FontOffset),($2D+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_0_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($54+FontOffset),($56+FontOffset),($46+FontOffset),($48+FontOffset),($75+FontOffset),($40+FontOffset),($4F+FontOffset),($6A+FontOffset),($40+FontOffset),($5A+FontOffset),($40+FontOffset),<($FE+FontOffset),($6C+FontOffset),($50+FontOffset),($4C+FontOffset),($50+FontOffset),($51+FontOffset),($40+FontOffset),($59+FontOffset),($40+FontOffset),($49+FontOffset),($42+FontOffset),($46+FontOffset),($48+FontOffset),($40+FontOffset),($50+FontOffset),($70+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_1_en:
	 .byte ($12+FontOffset),($1D+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($2D+FontOffset),($2E+FontOffset),($7C+FontOffset),($40+FontOffset),($13+FontOffset),($22+FontOffset),($1B+FontOffset),($2E+FontOffset),($40+FontOffset),($2A+FontOffset),($26+FontOffset),($1B+FontOffset),($28+FontOffset),<($FE+FontOffset),($1E+FontOffset),($29+FontOffset),($1F+FontOffset),($2D+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),($2E+FontOffset),($40+FontOffset),($27+FontOffset),($1B+FontOffset),($25+FontOffset),($1F+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($2D+FontOffset),($26+FontOffset),($23+FontOffset),($21+FontOffset),($22+FontOffset),($2E+FontOffset),($1F+FontOffset),($2D+FontOffset),($2E+FontOffset),($40+FontOffset),($2D+FontOffset),($1F+FontOffset),($28+FontOffset),($2D+FontOffset),($1F+FontOffset),($73+FontOffset),($40+FontOffset),($6+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($1B+FontOffset),($31+FontOffset),($1B+FontOffset),($33+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_1_es:
	 .byte ($2+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($20+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($7C+FontOffset),($40+FontOffset),($4+FontOffset),($2D+FontOffset),($1F+FontOffset),($40+FontOffset),($2A+FontOffset),($26+FontOffset),($1B+FontOffset),($28+FontOffset),<($FE+FontOffset),($28+FontOffset),($29+FontOffset),($40+FontOffset),($2E+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($1F+FontOffset),($40+FontOffset),($1F+FontOffset),($26+FontOffset),($40+FontOffset),($27+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($27+FontOffset),($23+FontOffset),($28+FontOffset),($23+FontOffset),($27+FontOffset),($29+FontOffset),($40+FontOffset),($2D+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($1E+FontOffset),($29+FontOffset),($73+FontOffset),($40+FontOffset),<($FE+FontOffset),($0+FontOffset),($28+FontOffset),($1E+FontOffset),($1B+FontOffset),($2E+FontOffset),($1F+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_1_ja:
	 .byte ($46+FontOffset),($46+FontOffset),($70+FontOffset),($48+FontOffset),($4C+FontOffset),($7D+FontOffset),($7C+FontOffset),($40+FontOffset),($4F+FontOffset),($59+FontOffset),($40+FontOffset),($61+FontOffset),($50+FontOffset),($70+FontOffset),($55+FontOffset),($40+FontOffset),($49+FontOffset),($42+FontOffset),($46+FontOffset),($48+FontOffset),<($FE+FontOffset),($5A+FontOffset),($40+FontOffset),($42+FontOffset),($60+FontOffset),($40+FontOffset),($6D+FontOffset),($40+FontOffset),($55+FontOffset),($4B+FontOffset),($55+FontOffset),($42+FontOffset),($73+FontOffset),($40+FontOffset),($4B+FontOffset),($6A+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_en:
	 .byte ($12+FontOffset),($1D+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($2D+FontOffset),($2E+FontOffset),($7C+FontOffset),($40+FontOffset),($18+FontOffset),($29+FontOffset),($2F+FontOffset),($40+FontOffset),($1D+FontOffset),($1B+FontOffset),($28+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($1B+FontOffset),($25+FontOffset),($1F+FontOffset),($40+FontOffset),($1B+FontOffset),($40+FontOffset),($2D+FontOffset),($26+FontOffset),($23+FontOffset),($1E+FontOffset),($1F+FontOffset),($40+FontOffset),($2C+FontOffset),($2F+FontOffset),($26+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($27+FontOffset),($1B+FontOffset),($1E+FontOffset),($1F+FontOffset),($40+FontOffset),($29+FontOffset),($20+FontOffset),($40+FontOffset),($2A+FontOffset),($2F+FontOffset),($2C+FontOffset),($1F+FontOffset),($40+FontOffset),($21+FontOffset),($29+FontOffset),($26+FontOffset),($1E+FontOffset),($40+FontOffset),<($FE+FontOffset),($20+FontOffset),($29+FontOffset),($2C+FontOffset),($40+FontOffset),($20+FontOffset),($2C+FontOffset),($1F+FontOffset),($1F+FontOffset),($40+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),($33+FontOffset),($29+FontOffset),($2F+FontOffset),($2C+FontOffset),($40+FontOffset),($31+FontOffset),($1B+FontOffset),($33+FontOffset),<($FE+FontOffset),($29+FontOffset),($2F+FontOffset),($2E+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_es:
	 .byte ($2+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($20+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($7C+FontOffset),($40+FontOffset),($F+FontOffset),($29+FontOffset),($1E+FontOffset),($1F+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($29+FontOffset),($27+FontOffset),($1B+FontOffset),($2C+FontOffset),($40+FontOffset),($2F+FontOffset),($28+FontOffset),($1B+FontOffset),($40+FontOffset),($2C+FontOffset),($1F+FontOffset),($21+FontOffset),($26+FontOffset),($1B+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($1D+FontOffset),($1B+FontOffset),($26+FontOffset),($1D+FontOffset),($2F+FontOffset),($26+FontOffset),($29+FontOffset),($40+FontOffset),($22+FontOffset),($1F+FontOffset),($1D+FontOffset),($22+FontOffset),($1B+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($40+FontOffset),($29+FontOffset),($2C+FontOffset),($29+FontOffset),<($FE+FontOffset),($2A+FontOffset),($2F+FontOffset),($2C+FontOffset),($29+FontOffset),($40+FontOffset),($21+FontOffset),($2C+FontOffset),($1B+FontOffset),($2E+FontOffset),($23+FontOffset),($2D+FontOffset),($40+FontOffset),($1D+FontOffset),($2F+FontOffset),($1B+FontOffset),($28+FontOffset),($1E+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($2D+FontOffset),($1B+FontOffset),($26+FontOffset),($21+FontOffset),($1B+FontOffset),($2D+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_ja:
	 .byte ($46+FontOffset),($46+FontOffset),($70+FontOffset),($48+FontOffset),($4C+FontOffset),($7D+FontOffset),($7C+FontOffset),($40+FontOffset),($53+FontOffset),($70+FontOffset),($69+FontOffset),($40+FontOffset),($5F+FontOffset),($44+FontOffset),($40+FontOffset),($56+FontOffset),($75+FontOffset),($40+FontOffset),<($FE+FontOffset),($61+FontOffset),($68+FontOffset),($7F+FontOffset),($43+FontOffset),($40+FontOffset),($53+FontOffset),($70+FontOffset),($40+FontOffset),($45+FontOffset),($43+FontOffset),($4A+FontOffset),($70+FontOffset),($6E+FontOffset),($40+FontOffset),($59+FontOffset),($40+FontOffset),<($FE+FontOffset),($49+FontOffset),($42+FontOffset),($4B+FontOffset),($6E+FontOffset),($4C+FontOffset),($70+FontOffset),($7D+FontOffset),($48+FontOffset),($40+FontOffset),($6D+FontOffset),($40+FontOffset),($54+FontOffset),($79+FontOffset),($53+FontOffset),($42+FontOffset),($42+FontOffset),($40+FontOffset),<($FE+FontOffset),($53+FontOffset),($70+FontOffset),($4D+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_4_0_en:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($16+FontOffset),($22+FontOffset),($1B+FontOffset),($2E+FontOffset),($40+FontOffset),($1E+FontOffset),($23+FontOffset),($1E+FontOffset),($40+FontOffset),($2D+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($2D+FontOffset),($1B+FontOffset),($33+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_4_0_es:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($10+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($1E+FontOffset),($23+FontOffset),($24+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($1F+FontOffset),($26+FontOffset),($26+FontOffset),($1B+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_4_0_ja:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($55+FontOffset),($56+FontOffset),($40+FontOffset),($54+FontOffset),($40+FontOffset),($42+FontOffset),($79+FontOffset),($50+FontOffset),($46+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_4_1_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($3A+FontOffset),($3A+FontOffset),($36+FontOffset),($40+FontOffset),($7+FontOffset),($34+FontOffset),($73+FontOffset),($40+FontOffset),($13+FontOffset),($22+FontOffset),($1B+FontOffset),($2E+FontOffset),($40+FontOffset),($23+FontOffset),($2D+FontOffset),<($FE+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),($20+FontOffset),($2C+FontOffset),($1F+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($28+FontOffset),($1D+FontOffset),($33+FontOffset),($40+FontOffset),($33+FontOffset),($29+FontOffset),($2F+FontOffset),($40+FontOffset),<($FE+FontOffset),($27+FontOffset),($2F+FontOffset),($2D+FontOffset),($2E+FontOffset),($40+FontOffset),($2E+FontOffset),($2C+FontOffset),($33+FontOffset),($40+FontOffset),($2E+FontOffset),($29+FontOffset),($40+FontOffset),($27+FontOffset),($1B+FontOffset),($2E+FontOffset),($1D+FontOffset),($22+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_4_1_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($3A+FontOffset),($36+FontOffset),($36+FontOffset),($40+FontOffset),($7+FontOffset),($34+FontOffset),($73+FontOffset),($40+FontOffset),($4+FontOffset),($2D+FontOffset),($1B+FontOffset),($40+FontOffset),($1F+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($26+FontOffset),($1B+FontOffset),($40+FontOffset),($20+FontOffset),($2C+FontOffset),($1F+FontOffset),($1D+FontOffset),($2F+FontOffset),($1F+FontOffset),($28+FontOffset),($1D+FontOffset),($23+FontOffset),($1B+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($1E+FontOffset),($1F+FontOffset),($1C+FontOffset),($1F+FontOffset),($2D+FontOffset),($40+FontOffset),($23+FontOffset),($28+FontOffset),($2E+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($1B+FontOffset),($2C+FontOffset),($40+FontOffset),<($FE+FontOffset),($23+FontOffset),($21+FontOffset),($2F+FontOffset),($1B+FontOffset),($26+FontOffset),($1B+FontOffset),($2C+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_4_1_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($3A+FontOffset),($36+FontOffset),($36+FontOffset),($40+FontOffset),($7+FontOffset),($34+FontOffset),($73+FontOffset),($40+FontOffset),($4F+FontOffset),($59+FontOffset),($40+FontOffset),<($FE+FontOffset),($4C+FontOffset),($7E+FontOffset),($43+FontOffset),($5A+FontOffset),($4D+FontOffset),($43+FontOffset),($40+FontOffset),($56+FontOffset),($40+FontOffset),($41+FontOffset),($6C+FontOffset),($4E+FontOffset),($55+FontOffset),($42+FontOffset),($40+FontOffset),($54+FontOffset),($40+FontOffset),<($FE+FontOffset),($42+FontOffset),($49+FontOffset),($55+FontOffset),($42+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_4_2_en:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($E+FontOffset),($25+FontOffset),($71+FontOffset),($40+FontOffset),($8+FontOffset),($40+FontOffset),($31+FontOffset),($23+FontOffset),($26+FontOffset),($26+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_4_2_es:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($4+FontOffset),($2D+FontOffset),($2E+FontOffset),($1B+FontOffset),($40+FontOffset),($1C+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($71+FontOffset),($40+FontOffset),($B+FontOffset),($29+FontOffset),<($FE+FontOffset),($22+FontOffset),($1B+FontOffset),($2C+FontOffset),($1F+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_4_2_ja:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($4C+FontOffset),($70+FontOffset),($7D+FontOffset),($75+FontOffset),($40+FontOffset),($41+FontOffset),($6C+FontOffset),($4E+FontOffset),($5F+FontOffset),($4D+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_4_3_en:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($C+FontOffset),($2F+FontOffset),($2D+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($26+FontOffset),($40+FontOffset),<($FE+FontOffset),($1+FontOffset),($1B+FontOffset),($2E+FontOffset),($2E+FontOffset),($26+FontOffset),($1F+FontOffset),($40+FontOffset),($2+FontOffset),($22+FontOffset),($1B+FontOffset),($28+FontOffset),($21+FontOffset),($1F+FontOffset),($71+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_4_3_es:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($C+FontOffset),($2F+FontOffset),($2D+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($26+FontOffset),($40+FontOffset),<($FE+FontOffset),($1+FontOffset),($1B+FontOffset),($2E+FontOffset),($2E+FontOffset),($26+FontOffset),($1F+FontOffset),($40+FontOffset),($2+FontOffset),($22+FontOffset),($1B+FontOffset),($28+FontOffset),($21+FontOffset),($1F+FontOffset),($71+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_4_3_ja:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($60+FontOffset),($7E+FontOffset),($7A+FontOffset),($4C+FontOffset),($70+FontOffset),($46+FontOffset),($69+FontOffset),($40+FontOffset),($5A+FontOffset),($70+FontOffset),($54+FontOffset),($69+FontOffset),($40+FontOffset),<($FE+FontOffset),($51+FontOffset),($44+FontOffset),($6E+FontOffset),($4C+FontOffset),($70+FontOffset),($71+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_no_mothership_0_en:
	 .byte ($0+FontOffset),($20+FontOffset),($2E+FontOffset),($1F+FontOffset),($2C+FontOffset),($40+FontOffset),($1B+FontOffset),($40+FontOffset),($1C+FontOffset),($2C+FontOffset),($23+FontOffset),($1F+FontOffset),($20+FontOffset),($40+FontOffset),($27+FontOffset),($29+FontOffset),($27+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),<($FE+FontOffset),($29+FontOffset),($20+FontOffset),($40+FontOffset),($2D+FontOffset),($23+FontOffset),($26+FontOffset),($1F+FontOffset),($28+FontOffset),($1D+FontOffset),($1F+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_no_mothership_0_es:
	 .byte ($B+FontOffset),($2F+FontOffset),($1F+FontOffset),($21+FontOffset),($29+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($40+FontOffset),($2F+FontOffset),($28+FontOffset),($40+FontOffset),($1C+FontOffset),($2C+FontOffset),($1F+FontOffset),($30+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($2D+FontOffset),($23+FontOffset),($26+FontOffset),($1F+FontOffset),($28+FontOffset),($1D+FontOffset),($23+FontOffset),($29+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_no_mothership_0_ja:
	 .byte ($60+FontOffset),($4C+FontOffset),($70+FontOffset),($46+FontOffset),($42+FontOffset),($40+FontOffset),($51+FontOffset),($6E+FontOffset),($63+FontOffset),($48+FontOffset),($40+FontOffset),($59+FontOffset),($40+FontOffset),($41+FontOffset),($54+FontOffset),($53+FontOffset),($70+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),<($FF+FontOffset)
cutscenes_string_no_mothership_2_0_en:
	 .byte ($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),($27+FontOffset),($29+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($2C+FontOffset),($2D+FontOffset),($22+FontOffset),($23+FontOffset),($2A+FontOffset),($40+FontOffset),<($FE+FontOffset),($1E+FontOffset),($23+FontOffset),($2D+FontOffset),($1B+FontOffset),($2A+FontOffset),($2A+FontOffset),($1F+FontOffset),($1B+FontOffset),($2C+FontOffset),($1F+FontOffset),($1E+FontOffset),($40+FontOffset),($20+FontOffset),($2C+FontOffset),($29+FontOffset),($27+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),<($FE+FontOffset),($2D+FontOffset),($25+FontOffset),($33+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_no_mothership_2_0_es:
	 .byte ($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),($26+FontOffset),($1B+FontOffset),($40+FontOffset),($28+FontOffset),($1B+FontOffset),($30+FontOffset),($1F+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),($1E+FontOffset),($2C+FontOffset),($23+FontOffset),($34+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($1E+FontOffset),($1F+FontOffset),($2D+FontOffset),($1B+FontOffset),($2A+FontOffset),($1B+FontOffset),($2C+FontOffset),($1F+FontOffset),($1D+FontOffset),($23+FontOffset),($29+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($26+FontOffset),($40+FontOffset),<($FE+FontOffset),($20+FontOffset),($23+FontOffset),($2C+FontOffset),($27+FontOffset),($1B+FontOffset),($27+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($29+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_no_mothership_2_0_ja:
	 .byte ($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),($4F+FontOffset),($67+FontOffset),($40+FontOffset),($46+FontOffset),($67+FontOffset),($40+FontOffset),($5E+FontOffset),($70+FontOffset),($4E+FontOffset),($6E+FontOffset),($40+FontOffset),($46+FontOffset),($70+FontOffset),($40+FontOffset),<($FE+FontOffset),($47+FontOffset),($44+FontOffset),($50+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_5_0_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($71+FontOffset),($40+FontOffset),($13+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($27+FontOffset),($29+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($2C+FontOffset),($2D+FontOffset),($22+FontOffset),($23+FontOffset),($2A+FontOffset),($40+FontOffset),($23+FontOffset),($2D+FontOffset),($40+FontOffset),($21+FontOffset),($29+FontOffset),($28+FontOffset),($1F+FontOffset),($71+FontOffset),($40+FontOffset),<($FE+FontOffset),($16+FontOffset),($1F+FontOffset),($40+FontOffset),($1E+FontOffset),($23+FontOffset),($1E+FontOffset),($40+FontOffset),($23+FontOffset),($2E+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_5_0_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($71+FontOffset),($40+FontOffset),($B+FontOffset),($1B+FontOffset),($40+FontOffset),($28+FontOffset),($1B+FontOffset),($30+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($28+FontOffset),($29+FontOffset),($1E+FontOffset),($2C+FontOffset),($23+FontOffset),($34+FontOffset),($1B+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),($40+FontOffset),($1F+FontOffset),($2D+FontOffset),($2E+FontOffset),($1B+FontOffset),($40+FontOffset),($27+FontOffset),($1B+FontOffset),($2D+FontOffset),($71+FontOffset),<($FE+FontOffset),($B+FontOffset),($29+FontOffset),($40+FontOffset),($22+FontOffset),($23+FontOffset),($1D+FontOffset),($23+FontOffset),($27+FontOffset),($29+FontOffset),($2D+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_5_0_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($51+FontOffset),($7D+FontOffset),($6E+FontOffset),($71+FontOffset),($40+FontOffset),($5E+FontOffset),($70+FontOffset),($4E+FontOffset),($6E+FontOffset),($40+FontOffset),<($FE+FontOffset),($46+FontOffset),($70+FontOffset),($40+FontOffset),($47+FontOffset),($44+FontOffset),($50+FontOffset),($71+FontOffset),($40+FontOffset),($64+FontOffset),($79+FontOffset),($50+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_5_1_en:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($11+FontOffset),($1F+FontOffset),($1B+FontOffset),($26+FontOffset),($26+FontOffset),($33+FontOffset),($71+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_5_1_es:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($3+FontOffset),($1F+FontOffset),($40+FontOffset),($30+FontOffset),($1F+FontOffset),($2C+FontOffset),($1E+FontOffset),($1B+FontOffset),($1E+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_5_1_ja:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($5E+FontOffset),($6E+FontOffset),($54+FontOffset),($43+FontOffset),($56+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_5_2_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($8+FontOffset),($28+FontOffset),($1E+FontOffset),($1F+FontOffset),($1F+FontOffset),($1E+FontOffset),($71+FontOffset),($40+FontOffset),($16+FontOffset),($1F+FontOffset),($40+FontOffset),($27+FontOffset),($2F+FontOffset),($2D+FontOffset),($2E+FontOffset),<($FE+FontOffset),($1F+FontOffset),($28+FontOffset),($2D+FontOffset),($2F+FontOffset),($2C+FontOffset),($1F+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1B+FontOffset),($2E+FontOffset),($40+FontOffset),<($FE+FontOffset),($1F+FontOffset),($30+FontOffset),($1F+FontOffset),($2C+FontOffset),($33+FontOffset),($1C+FontOffset),($29+FontOffset),($1E+FontOffset),($33+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),($2E+FontOffset),($23+FontOffset),($1D+FontOffset),($1F+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($22+FontOffset),($1B+FontOffset),($2E+FontOffset),($40+FontOffset),($23+FontOffset),($2E+FontOffset),($40+FontOffset),($31+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1B+FontOffset),($28+FontOffset),($25+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($29+FontOffset),($40+FontOffset),($2F+FontOffset),($2D+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_5_2_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($3+FontOffset),($1F+FontOffset),($40+FontOffset),($30+FontOffset),($1F+FontOffset),($2C+FontOffset),($1E+FontOffset),($1B+FontOffset),($1E+FontOffset),($71+FontOffset),($40+FontOffset),<($FE+FontOffset),($3+FontOffset),($1F+FontOffset),($1C+FontOffset),($1F+FontOffset),($27+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($1B+FontOffset),($2D+FontOffset),($1F+FontOffset),($21+FontOffset),($2F+FontOffset),($2C+FontOffset),($1B+FontOffset),($2C+FontOffset),($28+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($2E+FontOffset),($29+FontOffset),($1E+FontOffset),($29+FontOffset),($40+FontOffset),($1F+FontOffset),($26+FontOffset),($40+FontOffset),($27+FontOffset),($2F+FontOffset),($28+FontOffset),($1E+FontOffset),($29+FontOffset),($40+FontOffset),($2D+FontOffset),($1F+FontOffset),<($FE+FontOffset),($1E+FontOffset),($1F+FontOffset),($40+FontOffset),($1D+FontOffset),($2F+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($1B+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($20+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($21+FontOffset),($2C+FontOffset),($1B+FontOffset),($1D+FontOffset),($23+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),($1B+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),($2D+FontOffset),($29+FontOffset),($2E+FontOffset),($2C+FontOffset),($29+FontOffset),($2D+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_5_2_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($5E+FontOffset),($6E+FontOffset),($54+FontOffset),($43+FontOffset),($40+FontOffset),($50+FontOffset),($70+FontOffset),($66+FontOffset),($71+FontOffset),($40+FontOffset),($6C+FontOffset),($50+FontOffset),($4C+FontOffset),($50+FontOffset),($51+FontOffset),<($FE+FontOffset),($59+FontOffset),($40+FontOffset),($4C+FontOffset),($6C+FontOffset),($4B+FontOffset),($70+FontOffset),($40+FontOffset),($53+FontOffset),($70+FontOffset),($4D+FontOffset),($40+FontOffset),($59+FontOffset),($5A+FontOffset),($40+FontOffset),($60+FontOffset),($6E+FontOffset),($55+FontOffset),($40+FontOffset),($56+FontOffset),($40+FontOffset),<($FE+FontOffset),($4C+FontOffset),($67+FontOffset),($4E+FontOffset),($55+FontOffset),($42+FontOffset),($40+FontOffset),($54+FontOffset),($40+FontOffset),($42+FontOffset),($49+FontOffset),($55+FontOffset),($42+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_5_3_en:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($16+FontOffset),($1F+FontOffset),($40+FontOffset),($31+FontOffset),($23+FontOffset),($26+FontOffset),($26+FontOffset),($40+FontOffset),($1C+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($2C+FontOffset),($23+FontOffset),($1D+FontOffset),($22+FontOffset),($71+FontOffset),($40+FontOffset),($C+FontOffset),($1B+FontOffset),($33+FontOffset),($1C+FontOffset),($1F+FontOffset),($40+FontOffset),($31+FontOffset),($1F+FontOffset),($40+FontOffset),($1D+FontOffset),($1B+FontOffset),($28+FontOffset),($40+FontOffset),<($FE+FontOffset),($1C+FontOffset),($1F+FontOffset),($1D+FontOffset),($29+FontOffset),($27+FontOffset),($1F+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),($28+FontOffset),($1F+FontOffset),($31+FontOffset),($40+FontOffset),<($FE+FontOffset),($2C+FontOffset),($2F+FontOffset),($26+FontOffset),($1F+FontOffset),($2C+FontOffset),($2D+FontOffset),($40+FontOffset),($29+FontOffset),($20+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($2C+FontOffset),($1F+FontOffset),($27+FontOffset),($1B+FontOffset),($23+FontOffset),($28+FontOffset),($23+FontOffset),($28+FontOffset),($21+FontOffset),($40+FontOffset),<($FE+FontOffset),($2A+FontOffset),($29+FontOffset),($2A+FontOffset),($2F+FontOffset),($26+FontOffset),($1B+FontOffset),($2E+FontOffset),($23+FontOffset),($29+FontOffset),($28+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_5_3_es:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($15+FontOffset),($1B+FontOffset),($27+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($1B+FontOffset),($40+FontOffset),($2D+FontOffset),($1F+FontOffset),($2C+FontOffset),($40+FontOffset),<($FE+FontOffset),($2C+FontOffset),($23+FontOffset),($1D+FontOffset),($29+FontOffset),($2D+FontOffset),($71+FontOffset),($40+FontOffset),($10+FontOffset),($2F+FontOffset),($23+FontOffset),($34+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($2A+FontOffset),($29+FontOffset),($1E+FontOffset),($1B+FontOffset),($27+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($1D+FontOffset),($29+FontOffset),($28+FontOffset),($30+FontOffset),($1F+FontOffset),($2C+FontOffset),($2E+FontOffset),($23+FontOffset),($2C+FontOffset),($28+FontOffset),($29+FontOffset),($2D+FontOffset),<($FE+FontOffset),($1F+FontOffset),($28+FontOffset),($40+FontOffset),($26+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($28+FontOffset),($2F+FontOffset),($1F+FontOffset),($30+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($21+FontOffset),($29+FontOffset),($1C+FontOffset),($1F+FontOffset),($2C+FontOffset),($28+FontOffset),($1B+FontOffset),($28+FontOffset),($2E+FontOffset),($1F+FontOffset),($2D+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($40+FontOffset),($26+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($2A+FontOffset),($29+FontOffset),($1C+FontOffset),($26+FontOffset),($1B+FontOffset),($1D+FontOffset),($23+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($1E+FontOffset),($1B+FontOffset),($71+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_5_3_ja:
	 .byte ($72+FontOffset),($72+FontOffset),($72+FontOffset),($7C+FontOffset),($40+FontOffset),($46+FontOffset),($58+FontOffset),($63+FontOffset),($51+FontOffset),($40+FontOffset),($56+FontOffset),($40+FontOffset),($55+FontOffset),($69+FontOffset),($71+FontOffset),($40+FontOffset),<($FE+FontOffset),($59+FontOffset),($4A+FontOffset),($79+FontOffset),($53+FontOffset),($69+FontOffset),($40+FontOffset),($4C+FontOffset),($70+FontOffset),($6E+FontOffset),($4A+FontOffset),($43+FontOffset),($40+FontOffset),($59+FontOffset),($40+FontOffset),($45+FontOffset),($43+FontOffset),($4B+FontOffset),($5F+FontOffset),($40+FontOffset),($56+FontOffset),<($FE+FontOffset),($55+FontOffset),($69+FontOffset),($40+FontOffset),($46+FontOffset),($63+FontOffset),($4C+FontOffset),($6A+FontOffset),($55+FontOffset),($42+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_5_4_en:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($18+FontOffset),($1B+FontOffset),($33+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_5_4_es:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($40+FontOffset),($18+FontOffset),($1B+FontOffset),($33+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_utaco_temple_5_4_ja:
	 .byte ($14+FontOffset),($2E+FontOffset),($1B+FontOffset),($1D+FontOffset),($29+FontOffset),($7C+FontOffset),($42+FontOffset),($44+FontOffset),($42+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_cutscene_empty_0_en:
	 .byte ($0+FontOffset),($2E+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),($26+FontOffset),($1B+FontOffset),($1C+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_cutscene_empty_0_es:
	 .byte ($4+FontOffset),($28+FontOffset),($40+FontOffset),($1F+FontOffset),($26+FontOffset),($40+FontOffset),($26+FontOffset),($1B+FontOffset),($1C+FontOffset),($29+FontOffset),($2C+FontOffset),($1B+FontOffset),($2E+FontOffset),($29+FontOffset),($2C+FontOffset),($23+FontOffset),($29+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),<($FF+FontOffset)
cutscenes_string_cutscene_empty_0_ja:
	 .byte ($49+FontOffset),($6E+FontOffset),($47+FontOffset),($7E+FontOffset),($43+FontOffset),($4C+FontOffset),($52+FontOffset),($40+FontOffset),($56+FontOffset),($53+FontOffset),($73+FontOffset),($73+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_0_en:
	 .byte ($12+FontOffset),($1D+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($2D+FontOffset),($2E+FontOffset),($7C+FontOffset),($40+FontOffset),($7+FontOffset),($1F+FontOffset),($26+FontOffset),($26+FontOffset),($29+FontOffset),($75+FontOffset),($40+FontOffset),($8+FontOffset),($40+FontOffset),<($FE+FontOffset),($1E+FontOffset),($23+FontOffset),($1E+FontOffset),($28+FontOffset),($74+FontOffset),($2E+FontOffset),($40+FontOffset),($2D+FontOffset),($1F+FontOffset),($1F+FontOffset),($40+FontOffset),($33+FontOffset),($29+FontOffset),($2F+FontOffset),($40+FontOffset),<($FE+FontOffset),($1D+FontOffset),($29+FontOffset),($27+FontOffset),($23+FontOffset),($28+FontOffset),($21+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_0_es:
	 .byte ($2+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($20+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($7C+FontOffset),($40+FontOffset),($7+FontOffset),($29+FontOffset),($26+FontOffset),($1B+FontOffset),($75+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),<($FE+FontOffset),($26+FontOffset),($29+FontOffset),($2D+FontOffset),($40+FontOffset),($30+FontOffset),($23+FontOffset),($40+FontOffset),($30+FontOffset),($1F+FontOffset),($28+FontOffset),($23+FontOffset),($2C+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_0_ja:
	 .byte ($46+FontOffset),($46+FontOffset),($70+FontOffset),($48+FontOffset),($4C+FontOffset),($7D+FontOffset),($7C+FontOffset),($40+FontOffset),($4A+FontOffset),($6E+FontOffset),($56+FontOffset),($51+FontOffset),($5A+FontOffset),($75+FontOffset),($40+FontOffset),($41+FontOffset),($55+FontOffset),($50+FontOffset),($40+FontOffset),<($FE+FontOffset),($46+FontOffset),($70+FontOffset),($40+FontOffset),($48+FontOffset),($69+FontOffset),($40+FontOffset),($59+FontOffset),($40+FontOffset),($47+FontOffset),($52+FontOffset),($70+FontOffset),($46+FontOffset),($55+FontOffset),($46+FontOffset),($79+FontOffset),($50+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_1_en:
	 .byte ($12+FontOffset),($1D+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($2D+FontOffset),($2E+FontOffset),($7C+FontOffset),($40+FontOffset),($0+FontOffset),($2D+FontOffset),($40+FontOffset),($8+FontOffset),($40+FontOffset),($2D+FontOffset),($1B+FontOffset),($23+FontOffset),($1E+FontOffset),<($FE+FontOffset),($1C+FontOffset),($1F+FontOffset),($20+FontOffset),($29+FontOffset),($2C+FontOffset),($1F+FontOffset),($75+FontOffset),($40+FontOffset),($1F+FontOffset),($30+FontOffset),($1F+FontOffset),($2C+FontOffset),($33+FontOffset),($2E+FontOffset),($22+FontOffset),($23+FontOffset),($28+FontOffset),($21+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($33+FontOffset),($40+FontOffset),($2D+FontOffset),($1B+FontOffset),($23+FontOffset),($1E+FontOffset),($40+FontOffset),($1B+FontOffset),($28+FontOffset),($1E+FontOffset),($40+FontOffset),($1E+FontOffset),($29+FontOffset),($28+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($27+FontOffset),($1B+FontOffset),($1E+FontOffset),($1F+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),($40+FontOffset),($2D+FontOffset),($1F+FontOffset),($28+FontOffset),($2D+FontOffset),($1F+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_1_es:
	 .byte ($2+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($20+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($7C+FontOffset),($40+FontOffset),($2+FontOffset),($29+FontOffset),($27+FontOffset),($29+FontOffset),($40+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($1E+FontOffset),($23+FontOffset),($1D+FontOffset),($22+FontOffset),($29+FontOffset),($40+FontOffset),($1B+FontOffset),($28+FontOffset),($2E+FontOffset),($1F+FontOffset),($2D+FontOffset),($75+FontOffset),($40+FontOffset),($2E+FontOffset),($29+FontOffset),($1E+FontOffset),($29+FontOffset),($40+FontOffset),($26+FontOffset),($29+FontOffset),<($FE+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($1E+FontOffset),($23+FontOffset),($24+FontOffset),($1F+FontOffset),($2C+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($22+FontOffset),($23+FontOffset),($1D+FontOffset),($23+FontOffset),($1F+FontOffset),($2C+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),($40+FontOffset),($2E+FontOffset),($2F+FontOffset),($30+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($2D+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($1E+FontOffset),($29+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_1_ja:
	 .byte ($46+FontOffset),($46+FontOffset),($70+FontOffset),($48+FontOffset),($4C+FontOffset),($7D+FontOffset),($7C+FontOffset),($40+FontOffset),($42+FontOffset),($4E+FontOffset),($70+FontOffset),($6E+FontOffset),($40+FontOffset),($56+FontOffset),($40+FontOffset),($6C+FontOffset),($50+FontOffset),($4C+FontOffset),($40+FontOffset),<($FE+FontOffset),($46+FontOffset),($70+FontOffset),($40+FontOffset),($42+FontOffset),($79+FontOffset),($50+FontOffset),($40+FontOffset),($54+FontOffset),($45+FontOffset),($68+FontOffset),($75+FontOffset),($40+FontOffset),($4F+FontOffset),($59+FontOffset),($40+FontOffset),($5C+FontOffset),($50+FontOffset),($68+FontOffset),($40+FontOffset),<($FE+FontOffset),($46+FontOffset),($70+FontOffset),($40+FontOffset),($42+FontOffset),($79+FontOffset),($53+FontOffset),($40+FontOffset),($4C+FontOffset),($50+FontOffset),($40+FontOffset),($63+FontOffset),($40+FontOffset),($59+FontOffset),($40+FontOffset),($4D+FontOffset),($5D+FontOffset),($70+FontOffset),($53+FontOffset),($40+FontOffset),($5A+FontOffset),<($FE+FontOffset),($42+FontOffset),($60+FontOffset),($40+FontOffset),($46+FontOffset),($70+FontOffset),($40+FontOffset),($55+FontOffset),($42+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_2_en:
	 .byte ($12+FontOffset),($1D+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($2D+FontOffset),($2E+FontOffset),($7C+FontOffset),($40+FontOffset),($13+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($27+FontOffset),($29+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($2C+FontOffset),($2D+FontOffset),($22+FontOffset),($23+FontOffset),($2A+FontOffset),($40+FontOffset),($26+FontOffset),($1F+FontOffset),($20+FontOffset),($2E+FontOffset),($40+FontOffset),($20+FontOffset),($29+FontOffset),($2C+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),($2D+FontOffset),($1B+FontOffset),($27+FontOffset),($1F+FontOffset),($40+FontOffset),($2C+FontOffset),($1F+FontOffset),($1B+FontOffset),($2D+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),($23+FontOffset),($2E+FontOffset),($40+FontOffset),<($FE+FontOffset),($1B+FontOffset),($2A+FontOffset),($2A+FontOffset),($1F+FontOffset),($1B+FontOffset),($2C+FontOffset),($1F+FontOffset),($1E+FontOffset),($40+FontOffset),($23+FontOffset),($28+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($20+FontOffset),($23+FontOffset),($2C+FontOffset),($2D+FontOffset),($2E+FontOffset),($40+FontOffset),($2E+FontOffset),($23+FontOffset),($27+FontOffset),($1F+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_2_es:
	 .byte ($2+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($20+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($7C+FontOffset),($40+FontOffset),($B+FontOffset),($1B+FontOffset),($40+FontOffset),($28+FontOffset),($1B+FontOffset),($30+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($28+FontOffset),($29+FontOffset),($1E+FontOffset),($2C+FontOffset),($23+FontOffset),($34+FontOffset),($1B+FontOffset),($40+FontOffset),($2D+FontOffset),($1F+FontOffset),($40+FontOffset),($20+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($2A+FontOffset),($29+FontOffset),($2C+FontOffset),($40+FontOffset),<($FE+FontOffset),($26+FontOffset),($1B+FontOffset),($40+FontOffset),($27+FontOffset),($23+FontOffset),($2D+FontOffset),($27+FontOffset),($1B+FontOffset),($40+FontOffset),($2C+FontOffset),($1B+FontOffset),($34+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),($2A+FontOffset),($29+FontOffset),($2C+FontOffset),($40+FontOffset),<($FE+FontOffset),($26+FontOffset),($1B+FontOffset),($40+FontOffset),($1D+FontOffset),($2F+FontOffset),($1B+FontOffset),($26+FontOffset),($40+FontOffset),($1B+FontOffset),($2A+FontOffset),($1B+FontOffset),($2C+FontOffset),($1F+FontOffset),($1D+FontOffset),($23+FontOffset),($29+FontOffset),($40+FontOffset),($1F+FontOffset),($28+FontOffset),($40+FontOffset),<($FE+FontOffset),($2A+FontOffset),($2C+FontOffset),($23+FontOffset),($27+FontOffset),($1F+FontOffset),($2C+FontOffset),($40+FontOffset),($26+FontOffset),($2F+FontOffset),($21+FontOffset),($1B+FontOffset),($2C+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_2_ja:
	 .byte ($46+FontOffset),($46+FontOffset),($70+FontOffset),($48+FontOffset),($4C+FontOffset),($7D+FontOffset),($7C+FontOffset),($40+FontOffset),($5E+FontOffset),($70+FontOffset),($4E+FontOffset),($6E+FontOffset),($40+FontOffset),($46+FontOffset),($70+FontOffset),($40+FontOffset),($47+FontOffset),($44+FontOffset),($50+FontOffset),($40+FontOffset),<($FE+FontOffset),($68+FontOffset),($65+FontOffset),($43+FontOffset),($40+FontOffset),($5A+FontOffset),($40+FontOffset),($41+FontOffset),($67+FontOffset),($6C+FontOffset),($6A+FontOffset),($50+FontOffset),($40+FontOffset),($68+FontOffset),($65+FontOffset),($43+FontOffset),($40+FontOffset),($54+FontOffset),($40+FontOffset),<($FE+FontOffset),($45+FontOffset),($55+FontOffset),($4C+FontOffset),($70+FontOffset),($40+FontOffset),($50+FontOffset),($70+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_3_en:
	 .byte ($12+FontOffset),($1D+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($2D+FontOffset),($2E+FontOffset),($7C+FontOffset),($40+FontOffset),($9+FontOffset),($2F+FontOffset),($2D+FontOffset),($2E+FontOffset),($40+FontOffset),<($FE+FontOffset),($1C+FontOffset),($1F+FontOffset),($1D+FontOffset),($1B+FontOffset),($2F+FontOffset),($2D+FontOffset),($1F+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_3_es:
	 .byte ($2+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($20+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($7C+FontOffset),($40+FontOffset),($F+FontOffset),($29+FontOffset),($2C+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($2D+FontOffset),($23+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_3_ja:
	 .byte ($46+FontOffset),($46+FontOffset),($70+FontOffset),($48+FontOffset),($4C+FontOffset),($7D+FontOffset),($7C+FontOffset),($40+FontOffset),($68+FontOffset),($65+FontOffset),($43+FontOffset),($40+FontOffset),($5A+FontOffset),($40+FontOffset),($55+FontOffset),($42+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_4_en:
	 .byte ($12+FontOffset),($1D+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($2D+FontOffset),($2E+FontOffset),($7C+FontOffset),($40+FontOffset),($12+FontOffset),($29+FontOffset),($75+FontOffset),($40+FontOffset),($2E+FontOffset),($22+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($23+FontOffset),($27+FontOffset),($2A+FontOffset),($29+FontOffset),($2C+FontOffset),($2E+FontOffset),($1B+FontOffset),($28+FontOffset),($2E+FontOffset),($40+FontOffset),($26+FontOffset),($1F+FontOffset),($2D+FontOffset),($2D+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),($2E+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($2C+FontOffset),($1F+FontOffset),($27+FontOffset),($1F+FontOffset),($27+FontOffset),($1C+FontOffset),($1F+FontOffset),($2C+FontOffset),($40+FontOffset),($22+FontOffset),($1F+FontOffset),($2C+FontOffset),($1F+FontOffset),($40+FontOffset),($23+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($22+FontOffset),($1B+FontOffset),($2E+FontOffset),($40+FontOffset),($8+FontOffset),($40+FontOffset),($31+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),($2C+FontOffset),($23+FontOffset),($21+FontOffset),($22+FontOffset),($2E+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_4_es:
	 .byte ($2+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($20+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($7C+FontOffset),($40+FontOffset),($0+FontOffset),($2D+FontOffset),($23+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($75+FontOffset),<($FE+FontOffset),($26+FontOffset),($1B+FontOffset),($40+FontOffset),($26+FontOffset),($1F+FontOffset),($1D+FontOffset),($1D+FontOffset),($23+FontOffset),($29+FontOffset),($28+FontOffset),($40+FontOffset),<($FE+FontOffset),($23+FontOffset),($27+FontOffset),($2A+FontOffset),($29+FontOffset),($2C+FontOffset),($2E+FontOffset),($1B+FontOffset),($28+FontOffset),($2E+FontOffset),($1F+FontOffset),($40+FontOffset),($2A+FontOffset),($1B+FontOffset),($2C+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($2C+FontOffset),($1F+FontOffset),($1D+FontOffset),($29+FontOffset),($2C+FontOffset),($1E+FontOffset),($1B+FontOffset),($2C+FontOffset),($40+FontOffset),($1F+FontOffset),($2D+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($33+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($1F+FontOffset),($28+FontOffset),($23+FontOffset),($1B+FontOffset),($40+FontOffset),($2C+FontOffset),($1B+FontOffset),($34+FontOffset),($29+FontOffset),($28+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_4_ja:
	 .byte ($46+FontOffset),($46+FontOffset),($70+FontOffset),($48+FontOffset),($4C+FontOffset),($7D+FontOffset),($7C+FontOffset),($40+FontOffset),($4F+FontOffset),($6A+FontOffset),($4C+FontOffset),($70+FontOffset),($7D+FontOffset),($75+FontOffset),($40+FontOffset),<($FE+FontOffset),($4C+FontOffset),($70+FontOffset),($7E+FontOffset),($43+FontOffset),($66+FontOffset),($43+FontOffset),($55+FontOffset),($40+FontOffset),($6A+FontOffset),($79+FontOffset),($4D+FontOffset),($6E+FontOffset),($40+FontOffset),($5A+FontOffset),($7C+FontOffset),($40+FontOffset),($6C+FontOffset),($50+FontOffset),($4C+FontOffset),($40+FontOffset),<($FE+FontOffset),($5A+FontOffset),($40+FontOffset),($4E+FontOffset),($42+FontOffset),($46+FontOffset),($48+FontOffset),($40+FontOffset),($50+FontOffset),($70+FontOffset),($79+FontOffset),($50+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_5_en:
	 .byte ($12+FontOffset),($1D+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($2D+FontOffset),($2E+FontOffset),($7C+FontOffset),($40+FontOffset),($0+FontOffset),($28+FontOffset),($1E+FontOffset),($40+FontOffset),<($FE+FontOffset),($2D+FontOffset),($1D+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($1D+FontOffset),($1F+FontOffset),($40+FontOffset),($31+FontOffset),($23+FontOffset),($28+FontOffset),($2D+FontOffset),($40+FontOffset),($1B+FontOffset),($21+FontOffset),($1B+FontOffset),($23+FontOffset),($28+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_5_es:
	 .byte ($2+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($20+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($7C+FontOffset),($40+FontOffset),($18+FontOffset),($40+FontOffset),($26+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($1D+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($1D+FontOffset),($23+FontOffset),($1B+FontOffset),($40+FontOffset),($30+FontOffset),($2F+FontOffset),($1F+FontOffset),($26+FontOffset),($30+FontOffset),($1F+FontOffset),($40+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($21+FontOffset),($1B+FontOffset),($28+FontOffset),($1B+FontOffset),($2C+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_5_ja:
	 .byte ($46+FontOffset),($46+FontOffset),($70+FontOffset),($48+FontOffset),($4C+FontOffset),($7D+FontOffset),($7C+FontOffset),($40+FontOffset),($63+FontOffset),($43+FontOffset),($42+FontOffset),($51+FontOffset),($54+FontOffset),($70+FontOffset),($40+FontOffset),($46+FontOffset),($46+FontOffset),($70+FontOffset),($48+FontOffset),($40+FontOffset),<($FE+FontOffset),($59+FontOffset),($40+FontOffset),($46+FontOffset),($51+FontOffset),($40+FontOffset),($50+FontOffset),($70+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_6_en:
	 .byte ($12+FontOffset),($1D+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($2D+FontOffset),($2E+FontOffset),($7C+FontOffset),($40+FontOffset),($6+FontOffset),($29+FontOffset),($29+FontOffset),($1E+FontOffset),($40+FontOffset),<($FE+FontOffset),($28+FontOffset),($23+FontOffset),($21+FontOffset),($22+FontOffset),($2E+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_6_es:
	 .byte ($2+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($20+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($7C+FontOffset),($40+FontOffset),($1+FontOffset),($2F+FontOffset),($1F+FontOffset),($28+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($28+FontOffset),($29+FontOffset),($1D+FontOffset),($22+FontOffset),($1F+FontOffset),($2D+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_the_lab_2_6_ja:
	 .byte ($46+FontOffset),($46+FontOffset),($70+FontOffset),($48+FontOffset),($4C+FontOffset),($7D+FontOffset),($7C+FontOffset),($40+FontOffset),($45+FontOffset),($64+FontOffset),($4D+FontOffset),($60+FontOffset),($55+FontOffset),($4B+FontOffset),($42+FontOffset),($73+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_cutscene_empty_2_0_en:
	 .byte ($12+FontOffset),($1D+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($2D+FontOffset),($2E+FontOffset),($7C+FontOffset),($40+FontOffset),($0+FontOffset),($26+FontOffset),($20+FontOffset),($2C+FontOffset),($1F+FontOffset),($1E+FontOffset),($71+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_cutscene_empty_2_0_es:
	 .byte ($2+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($20+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($7C+FontOffset),($40+FontOffset),($0+FontOffset),($26+FontOffset),($20+FontOffset),($2C+FontOffset),($1F+FontOffset),($1E+FontOffset),($71+FontOffset),($71+FontOffset),<($FF+FontOffset)
cutscenes_string_cutscene_empty_2_0_ja:
	 .byte ($46+FontOffset),($46+FontOffset),($70+FontOffset),($48+FontOffset),($4C+FontOffset),($7D+FontOffset),($7C+FontOffset),($40+FontOffset),($41+FontOffset),($69+FontOffset),($5C+FontOffset),($6A+FontOffset),($79+FontOffset),($54+FontOffset),($70+FontOffset),($71+FontOffset),($71+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_cutscene_empty_2_1_en:
	 .byte ($12+FontOffset),($1D+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($2D+FontOffset),($2E+FontOffset),($7C+FontOffset),($40+FontOffset),($7+FontOffset),($29+FontOffset),($31+FontOffset),($40+FontOffset),($27+FontOffset),($1B+FontOffset),($28+FontOffset),($33+FontOffset),($40+FontOffset),<($FE+FontOffset),($2E+FontOffset),($23+FontOffset),($27+FontOffset),($1F+FontOffset),($2D+FontOffset),($40+FontOffset),($8+FontOffset),($40+FontOffset),($22+FontOffset),($1B+FontOffset),($30+FontOffset),($1F+FontOffset),($40+FontOffset),($2E+FontOffset),($29+FontOffset),($40+FontOffset),($2E+FontOffset),($1F+FontOffset),($26+FontOffset),($26+FontOffset),<($FE+FontOffset),($33+FontOffset),($29+FontOffset),($2F+FontOffset),($40+FontOffset),($2E+FontOffset),($29+FontOffset),($40+FontOffset),($1E+FontOffset),($29+FontOffset),($28+FontOffset),($74+FontOffset),($2E+FontOffset),($40+FontOffset),($26+FontOffset),($1F+FontOffset),($2E+FontOffset),($40+FontOffset),<($FE+FontOffset),($2A+FontOffset),($1F+FontOffset),($29+FontOffset),($2A+FontOffset),($26+FontOffset),($1F+FontOffset),($40+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($1F+FontOffset),($2C+FontOffset),($40+FontOffset),($22+FontOffset),($1F+FontOffset),($2C+FontOffset),($1F+FontOffset),($71+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_cutscene_empty_2_1_es:
	 .byte ($2+FontOffset),($23+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($23+FontOffset),($20+FontOffset),($23+FontOffset),($1D+FontOffset),($1B+FontOffset),($7C+FontOffset),($40+FontOffset),($2+FontOffset),($2F+FontOffset),($1B+FontOffset),($28+FontOffset),($2E+FontOffset),($1B+FontOffset),($2D+FontOffset),($40+FontOffset),<($FE+FontOffset),($30+FontOffset),($1F+FontOffset),($1D+FontOffset),($1F+FontOffset),($2D+FontOffset),($40+FontOffset),($2E+FontOffset),($1F+FontOffset),($40+FontOffset),($2E+FontOffset),($1F+FontOffset),($28+FontOffset),($21+FontOffset),($29+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),<($FE+FontOffset),($1E+FontOffset),($1F+FontOffset),($1D+FontOffset),($23+FontOffset),($2C+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($28+FontOffset),($29+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($23+FontOffset),($1F+FontOffset),($2C+FontOffset),($29+FontOffset),($40+FontOffset),<($FE+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($1E+FontOffset),($1F+FontOffset),($24+FontOffset),($1F+FontOffset),($2D+FontOffset),($40+FontOffset),($2B+FontOffset),($2F+FontOffset),($1F+FontOffset),($40+FontOffset),($26+FontOffset),($1B+FontOffset),($40+FontOffset),<($FE+FontOffset),($21+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($1F+FontOffset),($40+FontOffset),($1F+FontOffset),($28+FontOffset),($2E+FontOffset),($2C+FontOffset),($1F+FontOffset),($40+FontOffset),($1B+FontOffset),($1D+FontOffset),($1B+FontOffset),($71+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
cutscenes_string_cutscene_empty_2_1_ja:
	 .byte ($46+FontOffset),($46+FontOffset),($70+FontOffset),($48+FontOffset),($4C+FontOffset),($7D+FontOffset),($7C+FontOffset),($40+FontOffset),($50+FontOffset),($70+FontOffset),($6A+FontOffset),($40+FontOffset),($63+FontOffset),($40+FontOffset),<($FE+FontOffset),($5A+FontOffset),($42+FontOffset),($67+FontOffset),($4E+FontOffset),($53+FontOffset),($48+FontOffset),($6A+FontOffset),($55+FontOffset),($42+FontOffset),($40+FontOffset),($54+FontOffset),($40+FontOffset),($55+FontOffset),($6E+FontOffset),($54+FontOffset),($70+FontOffset),($63+FontOffset),($40+FontOffset),<($FE+FontOffset),($42+FontOffset),($79+FontOffset),($50+FontOffset),($40+FontOffset),($46+FontOffset),($71+FontOffset),($72+FontOffset),($40+FontOffset),<($FF+FontOffset)
;Strings lists:
cutscenes_strings_list_def_destroyed_city_en_lo:
	.byte <cutscenes_string_destroyed_city_0_en
	.byte <cutscenes_string_destroyed_city_1_en
	.byte <cutscenes_string_destroyed_city_2_en
	.byte 0
cutscenes_strings_list_def_destroyed_city_en_hi:
	.byte >cutscenes_string_destroyed_city_0_en
	.byte >cutscenes_string_destroyed_city_1_en
	.byte >cutscenes_string_destroyed_city_2_en
	.byte 0
cutscenes_strings_list_def_destroyed_city_es_lo:
	.byte <cutscenes_string_destroyed_city_0_es
	.byte <cutscenes_string_destroyed_city_1_es
	.byte <cutscenes_string_destroyed_city_2_es
	.byte 0
cutscenes_strings_list_def_destroyed_city_es_hi:
	.byte >cutscenes_string_destroyed_city_0_es
	.byte >cutscenes_string_destroyed_city_1_es
	.byte >cutscenes_string_destroyed_city_2_es
	.byte 0
cutscenes_strings_list_def_destroyed_city_ja_lo:
	.byte <cutscenes_string_destroyed_city_0_ja
	.byte <cutscenes_string_destroyed_city_1_ja
	.byte <cutscenes_string_destroyed_city_2_ja
	.byte 0
cutscenes_strings_list_def_destroyed_city_ja_hi:
	.byte >cutscenes_string_destroyed_city_0_ja
	.byte >cutscenes_string_destroyed_city_1_ja
	.byte >cutscenes_string_destroyed_city_2_ja
	.byte 0
cutscenes_strings_list_def_mothership_en_lo:
	.byte <cutscenes_string_mothership_0_en
	.byte <cutscenes_string_mothership_1_en
	.byte 0
cutscenes_strings_list_def_mothership_en_hi:
	.byte >cutscenes_string_mothership_0_en
	.byte >cutscenes_string_mothership_1_en
	.byte 0
cutscenes_strings_list_def_mothership_es_lo:
	.byte <cutscenes_string_mothership_0_es
	.byte <cutscenes_string_mothership_1_es
	.byte 0
cutscenes_strings_list_def_mothership_es_hi:
	.byte >cutscenes_string_mothership_0_es
	.byte >cutscenes_string_mothership_1_es
	.byte 0
cutscenes_strings_list_def_mothership_ja_lo:
	.byte <cutscenes_string_mothership_0_ja
	.byte <cutscenes_string_mothership_1_ja
	.byte 0
cutscenes_strings_list_def_mothership_ja_hi:
	.byte >cutscenes_string_mothership_0_ja
	.byte >cutscenes_string_mothership_1_ja
	.byte 0
cutscenes_strings_list_def_destroyed_city_2_en_lo:
	.byte <cutscenes_string_destroyed_city_2_0_en
	.byte <cutscenes_string_destroyed_city_2_1_en
	.byte 0
cutscenes_strings_list_def_destroyed_city_2_en_hi:
	.byte >cutscenes_string_destroyed_city_2_0_en
	.byte >cutscenes_string_destroyed_city_2_1_en
	.byte 0
cutscenes_strings_list_def_destroyed_city_2_es_lo:
	.byte <cutscenes_string_destroyed_city_2_0_es
	.byte <cutscenes_string_destroyed_city_2_1_es
	.byte 0
cutscenes_strings_list_def_destroyed_city_2_es_hi:
	.byte >cutscenes_string_destroyed_city_2_0_es
	.byte >cutscenes_string_destroyed_city_2_1_es
	.byte 0
cutscenes_strings_list_def_destroyed_city_2_ja_lo:
	.byte <cutscenes_string_destroyed_city_2_0_ja
	.byte <cutscenes_string_destroyed_city_2_1_ja
	.byte 0
cutscenes_strings_list_def_destroyed_city_2_ja_hi:
	.byte >cutscenes_string_destroyed_city_2_0_ja
	.byte >cutscenes_string_destroyed_city_2_1_ja
	.byte 0
cutscenes_strings_list_def_temple_ruins_en_lo:
	.byte <cutscenes_string_temple_ruins_0_en
	.byte 0
cutscenes_strings_list_def_temple_ruins_en_hi:
	.byte >cutscenes_string_temple_ruins_0_en
	.byte 0
cutscenes_strings_list_def_temple_ruins_es_lo:
	.byte <cutscenes_string_temple_ruins_0_es
	.byte 0
cutscenes_strings_list_def_temple_ruins_es_hi:
	.byte >cutscenes_string_temple_ruins_0_es
	.byte 0
cutscenes_strings_list_def_temple_ruins_ja_lo:
	.byte <cutscenes_string_temple_ruins_0_ja
	.byte 0
cutscenes_strings_list_def_temple_ruins_ja_hi:
	.byte >cutscenes_string_temple_ruins_0_ja
	.byte 0
cutscenes_strings_list_def_utaco_temple_en_lo:
	.byte <cutscenes_string_utaco_temple_0_en
	.byte <cutscenes_string_utaco_temple_1_en
	.byte <cutscenes_string_utaco_temple_2_en
	.byte <cutscenes_string_utaco_temple_3_en
	.byte <cutscenes_string_utaco_temple_4_en
	.byte <cutscenes_string_utaco_temple_5_en
	.byte <cutscenes_string_utaco_temple_6_en
	.byte <cutscenes_string_utaco_temple_7_en
	.byte <cutscenes_string_utaco_temple_8_en
	.byte 0
cutscenes_strings_list_def_utaco_temple_en_hi:
	.byte >cutscenes_string_utaco_temple_0_en
	.byte >cutscenes_string_utaco_temple_1_en
	.byte >cutscenes_string_utaco_temple_2_en
	.byte >cutscenes_string_utaco_temple_3_en
	.byte >cutscenes_string_utaco_temple_4_en
	.byte >cutscenes_string_utaco_temple_5_en
	.byte >cutscenes_string_utaco_temple_6_en
	.byte >cutscenes_string_utaco_temple_7_en
	.byte >cutscenes_string_utaco_temple_8_en
	.byte 0
cutscenes_strings_list_def_utaco_temple_es_lo:
	.byte <cutscenes_string_utaco_temple_0_es
	.byte <cutscenes_string_utaco_temple_1_es
	.byte <cutscenes_string_utaco_temple_2_es
	.byte <cutscenes_string_utaco_temple_3_es
	.byte <cutscenes_string_utaco_temple_4_es
	.byte <cutscenes_string_utaco_temple_5_es
	.byte <cutscenes_string_utaco_temple_6_es
	.byte <cutscenes_string_utaco_temple_7_es
	.byte <cutscenes_string_utaco_temple_8_es
	.byte 0
cutscenes_strings_list_def_utaco_temple_es_hi:
	.byte >cutscenes_string_utaco_temple_0_es
	.byte >cutscenes_string_utaco_temple_1_es
	.byte >cutscenes_string_utaco_temple_2_es
	.byte >cutscenes_string_utaco_temple_3_es
	.byte >cutscenes_string_utaco_temple_4_es
	.byte >cutscenes_string_utaco_temple_5_es
	.byte >cutscenes_string_utaco_temple_6_es
	.byte >cutscenes_string_utaco_temple_7_es
	.byte >cutscenes_string_utaco_temple_8_es
	.byte 0
cutscenes_strings_list_def_utaco_temple_ja_lo:
	.byte <cutscenes_string_utaco_temple_0_ja
	.byte <cutscenes_string_utaco_temple_1_ja
	.byte <cutscenes_string_utaco_temple_2_ja
	.byte <cutscenes_string_utaco_temple_3_ja
	.byte <cutscenes_string_utaco_temple_4_ja
	.byte <cutscenes_string_utaco_temple_5_ja
	.byte <cutscenes_string_utaco_temple_6_ja
	.byte <cutscenes_string_utaco_temple_7_ja
	.byte <cutscenes_string_utaco_temple_8_ja
	.byte 0
cutscenes_strings_list_def_utaco_temple_ja_hi:
	.byte >cutscenes_string_utaco_temple_0_ja
	.byte >cutscenes_string_utaco_temple_1_ja
	.byte >cutscenes_string_utaco_temple_2_ja
	.byte >cutscenes_string_utaco_temple_3_ja
	.byte >cutscenes_string_utaco_temple_4_ja
	.byte >cutscenes_string_utaco_temple_5_ja
	.byte >cutscenes_string_utaco_temple_6_ja
	.byte >cutscenes_string_utaco_temple_7_ja
	.byte >cutscenes_string_utaco_temple_8_ja
	.byte 0
cutscenes_strings_list_def_utaco_temple_2_en_lo:
	.byte <cutscenes_string_utaco_temple_2_0_en
	.byte <cutscenes_string_utaco_temple_2_1_en
	.byte <cutscenes_string_utaco_temple_2_2_en
	.byte <cutscenes_string_utaco_temple_2_3_en
	.byte <cutscenes_string_utaco_temple_2_4_en
	.byte <cutscenes_string_utaco_temple_2_5_en
	.byte <cutscenes_string_utaco_temple_2_6_en
	.byte <cutscenes_string_utaco_temple_2_7_en
	.byte <cutscenes_string_utaco_temple_2_8_en
	.byte <cutscenes_string_utaco_temple_2_9_en
	.byte <cutscenes_string_utaco_temple_2_10_en
	.byte <cutscenes_string_utaco_temple_2_11_en
	.byte 0
cutscenes_strings_list_def_utaco_temple_2_en_hi:
	.byte >cutscenes_string_utaco_temple_2_0_en
	.byte >cutscenes_string_utaco_temple_2_1_en
	.byte >cutscenes_string_utaco_temple_2_2_en
	.byte >cutscenes_string_utaco_temple_2_3_en
	.byte >cutscenes_string_utaco_temple_2_4_en
	.byte >cutscenes_string_utaco_temple_2_5_en
	.byte >cutscenes_string_utaco_temple_2_6_en
	.byte >cutscenes_string_utaco_temple_2_7_en
	.byte >cutscenes_string_utaco_temple_2_8_en
	.byte >cutscenes_string_utaco_temple_2_9_en
	.byte >cutscenes_string_utaco_temple_2_10_en
	.byte >cutscenes_string_utaco_temple_2_11_en
	.byte 0
cutscenes_strings_list_def_utaco_temple_2_es_lo:
	.byte <cutscenes_string_utaco_temple_2_0_es
	.byte <cutscenes_string_utaco_temple_2_1_es
	.byte <cutscenes_string_utaco_temple_2_2_es
	.byte <cutscenes_string_utaco_temple_2_3_es
	.byte <cutscenes_string_utaco_temple_2_4_es
	.byte <cutscenes_string_utaco_temple_2_5_es
	.byte <cutscenes_string_utaco_temple_2_6_es
	.byte <cutscenes_string_utaco_temple_2_7_es
	.byte <cutscenes_string_utaco_temple_2_8_es
	.byte <cutscenes_string_utaco_temple_2_9_es
	.byte <cutscenes_string_utaco_temple_2_10_es
	.byte <cutscenes_string_utaco_temple_2_11_es
	.byte 0
cutscenes_strings_list_def_utaco_temple_2_es_hi:
	.byte >cutscenes_string_utaco_temple_2_0_es
	.byte >cutscenes_string_utaco_temple_2_1_es
	.byte >cutscenes_string_utaco_temple_2_2_es
	.byte >cutscenes_string_utaco_temple_2_3_es
	.byte >cutscenes_string_utaco_temple_2_4_es
	.byte >cutscenes_string_utaco_temple_2_5_es
	.byte >cutscenes_string_utaco_temple_2_6_es
	.byte >cutscenes_string_utaco_temple_2_7_es
	.byte >cutscenes_string_utaco_temple_2_8_es
	.byte >cutscenes_string_utaco_temple_2_9_es
	.byte >cutscenes_string_utaco_temple_2_10_es
	.byte >cutscenes_string_utaco_temple_2_11_es
	.byte 0
cutscenes_strings_list_def_utaco_temple_2_ja_lo:
	.byte <cutscenes_string_utaco_temple_2_0_ja
	.byte <cutscenes_string_utaco_temple_2_1_ja
	.byte <cutscenes_string_utaco_temple_2_2_ja
	.byte <cutscenes_string_utaco_temple_2_3_ja
	.byte <cutscenes_string_utaco_temple_2_4_ja
	.byte <cutscenes_string_utaco_temple_2_5_ja
	.byte <cutscenes_string_utaco_temple_2_6_ja
	.byte <cutscenes_string_utaco_temple_2_7_ja
	.byte <cutscenes_string_utaco_temple_2_8_ja
	.byte <cutscenes_string_utaco_temple_2_9_ja
	.byte <cutscenes_string_utaco_temple_2_10_ja
	.byte <cutscenes_string_utaco_temple_2_11_ja
	.byte 0
cutscenes_strings_list_def_utaco_temple_2_ja_hi:
	.byte >cutscenes_string_utaco_temple_2_0_ja
	.byte >cutscenes_string_utaco_temple_2_1_ja
	.byte >cutscenes_string_utaco_temple_2_2_ja
	.byte >cutscenes_string_utaco_temple_2_3_ja
	.byte >cutscenes_string_utaco_temple_2_4_ja
	.byte >cutscenes_string_utaco_temple_2_5_ja
	.byte >cutscenes_string_utaco_temple_2_6_ja
	.byte >cutscenes_string_utaco_temple_2_7_ja
	.byte >cutscenes_string_utaco_temple_2_8_ja
	.byte >cutscenes_string_utaco_temple_2_9_ja
	.byte >cutscenes_string_utaco_temple_2_10_ja
	.byte >cutscenes_string_utaco_temple_2_11_ja
	.byte 0
cutscenes_strings_list_def_mothership_2_en_lo:
	.byte <cutscenes_string_mothership_2_0_en
	.byte <cutscenes_string_mothership_2_1_en
	.byte 0
cutscenes_strings_list_def_mothership_2_en_hi:
	.byte >cutscenes_string_mothership_2_0_en
	.byte >cutscenes_string_mothership_2_1_en
	.byte 0
cutscenes_strings_list_def_mothership_2_es_lo:
	.byte <cutscenes_string_mothership_2_0_es
	.byte <cutscenes_string_mothership_2_1_es
	.byte 0
cutscenes_strings_list_def_mothership_2_es_hi:
	.byte >cutscenes_string_mothership_2_0_es
	.byte >cutscenes_string_mothership_2_1_es
	.byte 0
cutscenes_strings_list_def_mothership_2_ja_lo:
	.byte <cutscenes_string_mothership_2_0_ja
	.byte <cutscenes_string_mothership_2_1_ja
	.byte 0
cutscenes_strings_list_def_mothership_2_ja_hi:
	.byte >cutscenes_string_mothership_2_0_ja
	.byte >cutscenes_string_mothership_2_1_ja
	.byte 0
cutscenes_strings_list_def_utaco_temple_3_en_lo:
	.byte <cutscenes_string_utaco_temple_3_0_en
	.byte <cutscenes_string_utaco_temple_3_1_en
	.byte <cutscenes_string_utaco_temple_3_2_en
	.byte <cutscenes_string_utaco_temple_3_3_en
	.byte <cutscenes_string_utaco_temple_3_4_en
	.byte <cutscenes_string_utaco_temple_3_5_en
	.byte <cutscenes_string_utaco_temple_3_6_en
	.byte <cutscenes_string_utaco_temple_3_7_en
	.byte <cutscenes_string_utaco_temple_3_8_en
	.byte <cutscenes_string_utaco_temple_3_9_en
	.byte <cutscenes_string_utaco_temple_3_10_en
	.byte <cutscenes_string_utaco_temple_3_11_en
	.byte 0
cutscenes_strings_list_def_utaco_temple_3_en_hi:
	.byte >cutscenes_string_utaco_temple_3_0_en
	.byte >cutscenes_string_utaco_temple_3_1_en
	.byte >cutscenes_string_utaco_temple_3_2_en
	.byte >cutscenes_string_utaco_temple_3_3_en
	.byte >cutscenes_string_utaco_temple_3_4_en
	.byte >cutscenes_string_utaco_temple_3_5_en
	.byte >cutscenes_string_utaco_temple_3_6_en
	.byte >cutscenes_string_utaco_temple_3_7_en
	.byte >cutscenes_string_utaco_temple_3_8_en
	.byte >cutscenes_string_utaco_temple_3_9_en
	.byte >cutscenes_string_utaco_temple_3_10_en
	.byte >cutscenes_string_utaco_temple_3_11_en
	.byte 0
cutscenes_strings_list_def_utaco_temple_3_es_lo:
	.byte <cutscenes_string_utaco_temple_3_0_es
	.byte <cutscenes_string_utaco_temple_3_1_es
	.byte <cutscenes_string_utaco_temple_3_2_es
	.byte <cutscenes_string_utaco_temple_3_3_es
	.byte <cutscenes_string_utaco_temple_3_4_es
	.byte <cutscenes_string_utaco_temple_3_5_es
	.byte <cutscenes_string_utaco_temple_3_6_es
	.byte <cutscenes_string_utaco_temple_3_7_es
	.byte <cutscenes_string_utaco_temple_3_8_es
	.byte <cutscenes_string_utaco_temple_3_9_es
	.byte <cutscenes_string_utaco_temple_3_10_es
	.byte <cutscenes_string_utaco_temple_3_11_es
	.byte 0
cutscenes_strings_list_def_utaco_temple_3_es_hi:
	.byte >cutscenes_string_utaco_temple_3_0_es
	.byte >cutscenes_string_utaco_temple_3_1_es
	.byte >cutscenes_string_utaco_temple_3_2_es
	.byte >cutscenes_string_utaco_temple_3_3_es
	.byte >cutscenes_string_utaco_temple_3_4_es
	.byte >cutscenes_string_utaco_temple_3_5_es
	.byte >cutscenes_string_utaco_temple_3_6_es
	.byte >cutscenes_string_utaco_temple_3_7_es
	.byte >cutscenes_string_utaco_temple_3_8_es
	.byte >cutscenes_string_utaco_temple_3_9_es
	.byte >cutscenes_string_utaco_temple_3_10_es
	.byte >cutscenes_string_utaco_temple_3_11_es
	.byte 0
cutscenes_strings_list_def_utaco_temple_3_ja_lo:
	.byte <cutscenes_string_utaco_temple_3_0_ja
	.byte <cutscenes_string_utaco_temple_3_1_ja
	.byte <cutscenes_string_utaco_temple_3_2_ja
	.byte <cutscenes_string_utaco_temple_3_3_ja
	.byte <cutscenes_string_utaco_temple_3_4_ja
	.byte <cutscenes_string_utaco_temple_3_5_ja
	.byte <cutscenes_string_utaco_temple_3_6_ja
	.byte <cutscenes_string_utaco_temple_3_7_ja
	.byte <cutscenes_string_utaco_temple_3_8_ja
	.byte <cutscenes_string_utaco_temple_3_9_ja
	.byte <cutscenes_string_utaco_temple_3_10_ja
	.byte <cutscenes_string_utaco_temple_3_11_ja
	.byte 0
cutscenes_strings_list_def_utaco_temple_3_ja_hi:
	.byte >cutscenes_string_utaco_temple_3_0_ja
	.byte >cutscenes_string_utaco_temple_3_1_ja
	.byte >cutscenes_string_utaco_temple_3_2_ja
	.byte >cutscenes_string_utaco_temple_3_3_ja
	.byte >cutscenes_string_utaco_temple_3_4_ja
	.byte >cutscenes_string_utaco_temple_3_5_ja
	.byte >cutscenes_string_utaco_temple_3_6_ja
	.byte >cutscenes_string_utaco_temple_3_7_ja
	.byte >cutscenes_string_utaco_temple_3_8_ja
	.byte >cutscenes_string_utaco_temple_3_9_ja
	.byte >cutscenes_string_utaco_temple_3_10_ja
	.byte >cutscenes_string_utaco_temple_3_11_ja
	.byte 0
cutscenes_strings_list_def_the_lab_en_lo:
	.byte <cutscenes_string_the_lab_0_en
	.byte <cutscenes_string_the_lab_1_en
	.byte <cutscenes_string_the_lab_2_en
	.byte 0
cutscenes_strings_list_def_the_lab_en_hi:
	.byte >cutscenes_string_the_lab_0_en
	.byte >cutscenes_string_the_lab_1_en
	.byte >cutscenes_string_the_lab_2_en
	.byte 0
cutscenes_strings_list_def_the_lab_es_lo:
	.byte <cutscenes_string_the_lab_0_es
	.byte <cutscenes_string_the_lab_1_es
	.byte <cutscenes_string_the_lab_2_es
	.byte 0
cutscenes_strings_list_def_the_lab_es_hi:
	.byte >cutscenes_string_the_lab_0_es
	.byte >cutscenes_string_the_lab_1_es
	.byte >cutscenes_string_the_lab_2_es
	.byte 0
cutscenes_strings_list_def_the_lab_ja_lo:
	.byte <cutscenes_string_the_lab_0_ja
	.byte <cutscenes_string_the_lab_1_ja
	.byte <cutscenes_string_the_lab_2_ja
	.byte 0
cutscenes_strings_list_def_the_lab_ja_hi:
	.byte >cutscenes_string_the_lab_0_ja
	.byte >cutscenes_string_the_lab_1_ja
	.byte >cutscenes_string_the_lab_2_ja
	.byte 0
cutscenes_strings_list_def_utaco_temple_4_en_lo:
	.byte <cutscenes_string_utaco_temple_4_0_en
	.byte <cutscenes_string_utaco_temple_4_1_en
	.byte <cutscenes_string_utaco_temple_4_2_en
	.byte <cutscenes_string_utaco_temple_4_3_en
	.byte 0
cutscenes_strings_list_def_utaco_temple_4_en_hi:
	.byte >cutscenes_string_utaco_temple_4_0_en
	.byte >cutscenes_string_utaco_temple_4_1_en
	.byte >cutscenes_string_utaco_temple_4_2_en
	.byte >cutscenes_string_utaco_temple_4_3_en
	.byte 0
cutscenes_strings_list_def_utaco_temple_4_es_lo:
	.byte <cutscenes_string_utaco_temple_4_0_es
	.byte <cutscenes_string_utaco_temple_4_1_es
	.byte <cutscenes_string_utaco_temple_4_2_es
	.byte <cutscenes_string_utaco_temple_4_3_es
	.byte 0
cutscenes_strings_list_def_utaco_temple_4_es_hi:
	.byte >cutscenes_string_utaco_temple_4_0_es
	.byte >cutscenes_string_utaco_temple_4_1_es
	.byte >cutscenes_string_utaco_temple_4_2_es
	.byte >cutscenes_string_utaco_temple_4_3_es
	.byte 0
cutscenes_strings_list_def_utaco_temple_4_ja_lo:
	.byte <cutscenes_string_utaco_temple_4_0_ja
	.byte <cutscenes_string_utaco_temple_4_1_ja
	.byte <cutscenes_string_utaco_temple_4_2_ja
	.byte <cutscenes_string_utaco_temple_4_3_ja
	.byte 0
cutscenes_strings_list_def_utaco_temple_4_ja_hi:
	.byte >cutscenes_string_utaco_temple_4_0_ja
	.byte >cutscenes_string_utaco_temple_4_1_ja
	.byte >cutscenes_string_utaco_temple_4_2_ja
	.byte >cutscenes_string_utaco_temple_4_3_ja
	.byte 0
cutscenes_strings_list_def_no_mothership_en_lo:
	.byte <cutscenes_string_no_mothership_0_en
	.byte 0
cutscenes_strings_list_def_no_mothership_en_hi:
	.byte >cutscenes_string_no_mothership_0_en
	.byte 0
cutscenes_strings_list_def_no_mothership_es_lo:
	.byte <cutscenes_string_no_mothership_0_es
	.byte 0
cutscenes_strings_list_def_no_mothership_es_hi:
	.byte >cutscenes_string_no_mothership_0_es
	.byte 0
cutscenes_strings_list_def_no_mothership_ja_lo:
	.byte <cutscenes_string_no_mothership_0_ja
	.byte 0
cutscenes_strings_list_def_no_mothership_ja_hi:
	.byte >cutscenes_string_no_mothership_0_ja
	.byte 0
cutscenes_strings_list_def_no_mothership_2_en_lo:
	.byte <cutscenes_string_no_mothership_2_0_en
	.byte 0
cutscenes_strings_list_def_no_mothership_2_en_hi:
	.byte >cutscenes_string_no_mothership_2_0_en
	.byte 0
cutscenes_strings_list_def_no_mothership_2_es_lo:
	.byte <cutscenes_string_no_mothership_2_0_es
	.byte 0
cutscenes_strings_list_def_no_mothership_2_es_hi:
	.byte >cutscenes_string_no_mothership_2_0_es
	.byte 0
cutscenes_strings_list_def_no_mothership_2_ja_lo:
	.byte <cutscenes_string_no_mothership_2_0_ja
	.byte 0
cutscenes_strings_list_def_no_mothership_2_ja_hi:
	.byte >cutscenes_string_no_mothership_2_0_ja
	.byte 0
cutscenes_strings_list_def_utaco_temple_5_en_lo:
	.byte <cutscenes_string_utaco_temple_5_0_en
	.byte <cutscenes_string_utaco_temple_5_1_en
	.byte <cutscenes_string_utaco_temple_5_2_en
	.byte <cutscenes_string_utaco_temple_5_3_en
	.byte <cutscenes_string_utaco_temple_5_4_en
	.byte 0
cutscenes_strings_list_def_utaco_temple_5_en_hi:
	.byte >cutscenes_string_utaco_temple_5_0_en
	.byte >cutscenes_string_utaco_temple_5_1_en
	.byte >cutscenes_string_utaco_temple_5_2_en
	.byte >cutscenes_string_utaco_temple_5_3_en
	.byte >cutscenes_string_utaco_temple_5_4_en
	.byte 0
cutscenes_strings_list_def_utaco_temple_5_es_lo:
	.byte <cutscenes_string_utaco_temple_5_0_es
	.byte <cutscenes_string_utaco_temple_5_1_es
	.byte <cutscenes_string_utaco_temple_5_2_es
	.byte <cutscenes_string_utaco_temple_5_3_es
	.byte <cutscenes_string_utaco_temple_5_4_es
	.byte 0
cutscenes_strings_list_def_utaco_temple_5_es_hi:
	.byte >cutscenes_string_utaco_temple_5_0_es
	.byte >cutscenes_string_utaco_temple_5_1_es
	.byte >cutscenes_string_utaco_temple_5_2_es
	.byte >cutscenes_string_utaco_temple_5_3_es
	.byte >cutscenes_string_utaco_temple_5_4_es
	.byte 0
cutscenes_strings_list_def_utaco_temple_5_ja_lo:
	.byte <cutscenes_string_utaco_temple_5_0_ja
	.byte <cutscenes_string_utaco_temple_5_1_ja
	.byte <cutscenes_string_utaco_temple_5_2_ja
	.byte <cutscenes_string_utaco_temple_5_3_ja
	.byte <cutscenes_string_utaco_temple_5_4_ja
	.byte 0
cutscenes_strings_list_def_utaco_temple_5_ja_hi:
	.byte >cutscenes_string_utaco_temple_5_0_ja
	.byte >cutscenes_string_utaco_temple_5_1_ja
	.byte >cutscenes_string_utaco_temple_5_2_ja
	.byte >cutscenes_string_utaco_temple_5_3_ja
	.byte >cutscenes_string_utaco_temple_5_4_ja
	.byte 0
cutscenes_strings_list_def_cutscene_empty_en_lo:
	.byte <cutscenes_string_cutscene_empty_0_en
	.byte 0
cutscenes_strings_list_def_cutscene_empty_en_hi:
	.byte >cutscenes_string_cutscene_empty_0_en
	.byte 0
cutscenes_strings_list_def_cutscene_empty_es_lo:
	.byte <cutscenes_string_cutscene_empty_0_es
	.byte 0
cutscenes_strings_list_def_cutscene_empty_es_hi:
	.byte >cutscenes_string_cutscene_empty_0_es
	.byte 0
cutscenes_strings_list_def_cutscene_empty_ja_lo:
	.byte <cutscenes_string_cutscene_empty_0_ja
	.byte 0
cutscenes_strings_list_def_cutscene_empty_ja_hi:
	.byte >cutscenes_string_cutscene_empty_0_ja
	.byte 0
cutscenes_strings_list_def_the_lab_2_en_lo:
	.byte <cutscenes_string_the_lab_2_0_en
	.byte <cutscenes_string_the_lab_2_1_en
	.byte <cutscenes_string_the_lab_2_2_en
	.byte <cutscenes_string_the_lab_2_3_en
	.byte <cutscenes_string_the_lab_2_4_en
	.byte <cutscenes_string_the_lab_2_5_en
	.byte <cutscenes_string_the_lab_2_6_en
	.byte 0
cutscenes_strings_list_def_the_lab_2_en_hi:
	.byte >cutscenes_string_the_lab_2_0_en
	.byte >cutscenes_string_the_lab_2_1_en
	.byte >cutscenes_string_the_lab_2_2_en
	.byte >cutscenes_string_the_lab_2_3_en
	.byte >cutscenes_string_the_lab_2_4_en
	.byte >cutscenes_string_the_lab_2_5_en
	.byte >cutscenes_string_the_lab_2_6_en
	.byte 0
cutscenes_strings_list_def_the_lab_2_es_lo:
	.byte <cutscenes_string_the_lab_2_0_es
	.byte <cutscenes_string_the_lab_2_1_es
	.byte <cutscenes_string_the_lab_2_2_es
	.byte <cutscenes_string_the_lab_2_3_es
	.byte <cutscenes_string_the_lab_2_4_es
	.byte <cutscenes_string_the_lab_2_5_es
	.byte <cutscenes_string_the_lab_2_6_es
	.byte 0
cutscenes_strings_list_def_the_lab_2_es_hi:
	.byte >cutscenes_string_the_lab_2_0_es
	.byte >cutscenes_string_the_lab_2_1_es
	.byte >cutscenes_string_the_lab_2_2_es
	.byte >cutscenes_string_the_lab_2_3_es
	.byte >cutscenes_string_the_lab_2_4_es
	.byte >cutscenes_string_the_lab_2_5_es
	.byte >cutscenes_string_the_lab_2_6_es
	.byte 0
cutscenes_strings_list_def_the_lab_2_ja_lo:
	.byte <cutscenes_string_the_lab_2_0_ja
	.byte <cutscenes_string_the_lab_2_1_ja
	.byte <cutscenes_string_the_lab_2_2_ja
	.byte <cutscenes_string_the_lab_2_3_ja
	.byte <cutscenes_string_the_lab_2_4_ja
	.byte <cutscenes_string_the_lab_2_5_ja
	.byte <cutscenes_string_the_lab_2_6_ja
	.byte 0
cutscenes_strings_list_def_the_lab_2_ja_hi:
	.byte >cutscenes_string_the_lab_2_0_ja
	.byte >cutscenes_string_the_lab_2_1_ja
	.byte >cutscenes_string_the_lab_2_2_ja
	.byte >cutscenes_string_the_lab_2_3_ja
	.byte >cutscenes_string_the_lab_2_4_ja
	.byte >cutscenes_string_the_lab_2_5_ja
	.byte >cutscenes_string_the_lab_2_6_ja
	.byte 0
cutscenes_strings_list_def_cutscene_empty_2_en_lo:
	.byte <cutscenes_string_cutscene_empty_2_0_en
	.byte <cutscenes_string_cutscene_empty_2_1_en
	.byte 0
cutscenes_strings_list_def_cutscene_empty_2_en_hi:
	.byte >cutscenes_string_cutscene_empty_2_0_en
	.byte >cutscenes_string_cutscene_empty_2_1_en
	.byte 0
cutscenes_strings_list_def_cutscene_empty_2_es_lo:
	.byte <cutscenes_string_cutscene_empty_2_0_es
	.byte <cutscenes_string_cutscene_empty_2_1_es
	.byte 0
cutscenes_strings_list_def_cutscene_empty_2_es_hi:
	.byte >cutscenes_string_cutscene_empty_2_0_es
	.byte >cutscenes_string_cutscene_empty_2_1_es
	.byte 0
cutscenes_strings_list_def_cutscene_empty_2_ja_lo:
	.byte <cutscenes_string_cutscene_empty_2_0_ja
	.byte <cutscenes_string_cutscene_empty_2_1_ja
	.byte 0
cutscenes_strings_list_def_cutscene_empty_2_ja_hi:
	.byte >cutscenes_string_cutscene_empty_2_0_ja
	.byte >cutscenes_string_cutscene_empty_2_1_ja
	.byte 0
cutscenes_strings_en_lo_lo:
	.byte <cutscenes_strings_list_def_destroyed_city_en_lo
	.byte <cutscenes_strings_list_def_mothership_en_lo
	.byte <cutscenes_strings_list_def_destroyed_city_2_en_lo
	.byte <cutscenes_strings_list_def_temple_ruins_en_lo
	.byte <cutscenes_strings_list_def_utaco_temple_en_lo
	.byte 0
	.byte <cutscenes_strings_list_def_utaco_temple_2_en_lo
	.byte 0
	.byte <cutscenes_strings_list_def_mothership_2_en_lo
	.byte <cutscenes_strings_list_def_utaco_temple_3_en_lo
	.byte <cutscenes_strings_list_def_the_lab_en_lo
	.byte <cutscenes_strings_list_def_utaco_temple_4_en_lo
	.byte 0
	.byte 0
	.byte <cutscenes_strings_list_def_no_mothership_en_lo
	.byte <cutscenes_strings_list_def_no_mothership_2_en_lo
	.byte <cutscenes_strings_list_def_utaco_temple_5_en_lo
	.byte <cutscenes_strings_list_def_cutscene_empty_en_lo
	.byte <cutscenes_strings_list_def_the_lab_2_en_lo
	.byte <cutscenes_strings_list_def_cutscene_empty_2_en_lo
	.byte 0
cutscenes_strings_en_lo_hi:
	.byte >cutscenes_strings_list_def_destroyed_city_en_lo
	.byte >cutscenes_strings_list_def_mothership_en_lo
	.byte >cutscenes_strings_list_def_destroyed_city_2_en_lo
	.byte >cutscenes_strings_list_def_temple_ruins_en_lo
	.byte >cutscenes_strings_list_def_utaco_temple_en_lo
	.byte 0
	.byte >cutscenes_strings_list_def_utaco_temple_2_en_lo
	.byte 0
	.byte >cutscenes_strings_list_def_mothership_2_en_lo
	.byte >cutscenes_strings_list_def_utaco_temple_3_en_lo
	.byte >cutscenes_strings_list_def_the_lab_en_lo
	.byte >cutscenes_strings_list_def_utaco_temple_4_en_lo
	.byte 0
	.byte 0
	.byte >cutscenes_strings_list_def_no_mothership_en_lo
	.byte >cutscenes_strings_list_def_no_mothership_2_en_lo
	.byte >cutscenes_strings_list_def_utaco_temple_5_en_lo
	.byte >cutscenes_strings_list_def_cutscene_empty_en_lo
	.byte >cutscenes_strings_list_def_the_lab_2_en_lo
	.byte >cutscenes_strings_list_def_cutscene_empty_2_en_lo
	.byte 0
cutscenes_strings_en_hi_lo:
	.byte <cutscenes_strings_list_def_destroyed_city_en_hi
	.byte <cutscenes_strings_list_def_mothership_en_hi
	.byte <cutscenes_strings_list_def_destroyed_city_2_en_hi
	.byte <cutscenes_strings_list_def_temple_ruins_en_hi
	.byte <cutscenes_strings_list_def_utaco_temple_en_hi
	.byte 0
	.byte <cutscenes_strings_list_def_utaco_temple_2_en_hi
	.byte 0
	.byte <cutscenes_strings_list_def_mothership_2_en_hi
	.byte <cutscenes_strings_list_def_utaco_temple_3_en_hi
	.byte <cutscenes_strings_list_def_the_lab_en_hi
	.byte <cutscenes_strings_list_def_utaco_temple_4_en_hi
	.byte 0
	.byte 0
	.byte <cutscenes_strings_list_def_no_mothership_en_hi
	.byte <cutscenes_strings_list_def_no_mothership_2_en_hi
	.byte <cutscenes_strings_list_def_utaco_temple_5_en_hi
	.byte <cutscenes_strings_list_def_cutscene_empty_en_hi
	.byte <cutscenes_strings_list_def_the_lab_2_en_hi
	.byte <cutscenes_strings_list_def_cutscene_empty_2_en_hi
	.byte 0
cutscenes_strings_en_hi_hi:
	.byte >cutscenes_strings_list_def_destroyed_city_en_hi
	.byte >cutscenes_strings_list_def_mothership_en_hi
	.byte >cutscenes_strings_list_def_destroyed_city_2_en_hi
	.byte >cutscenes_strings_list_def_temple_ruins_en_hi
	.byte >cutscenes_strings_list_def_utaco_temple_en_hi
	.byte 0
	.byte >cutscenes_strings_list_def_utaco_temple_2_en_hi
	.byte 0
	.byte >cutscenes_strings_list_def_mothership_2_en_hi
	.byte >cutscenes_strings_list_def_utaco_temple_3_en_hi
	.byte >cutscenes_strings_list_def_the_lab_en_hi
	.byte >cutscenes_strings_list_def_utaco_temple_4_en_hi
	.byte 0
	.byte 0
	.byte >cutscenes_strings_list_def_no_mothership_en_hi
	.byte >cutscenes_strings_list_def_no_mothership_2_en_hi
	.byte >cutscenes_strings_list_def_utaco_temple_5_en_hi
	.byte >cutscenes_strings_list_def_cutscene_empty_en_hi
	.byte >cutscenes_strings_list_def_the_lab_2_en_hi
	.byte >cutscenes_strings_list_def_cutscene_empty_2_en_hi
	.byte 0
cutscenes_strings_es_lo_lo:
	.byte <cutscenes_strings_list_def_destroyed_city_es_lo
	.byte <cutscenes_strings_list_def_mothership_es_lo
	.byte <cutscenes_strings_list_def_destroyed_city_2_es_lo
	.byte <cutscenes_strings_list_def_temple_ruins_es_lo
	.byte <cutscenes_strings_list_def_utaco_temple_es_lo
	.byte 0
	.byte <cutscenes_strings_list_def_utaco_temple_2_es_lo
	.byte 0
	.byte <cutscenes_strings_list_def_mothership_2_es_lo
	.byte <cutscenes_strings_list_def_utaco_temple_3_es_lo
	.byte <cutscenes_strings_list_def_the_lab_es_lo
	.byte <cutscenes_strings_list_def_utaco_temple_4_es_lo
	.byte 0
	.byte 0
	.byte <cutscenes_strings_list_def_no_mothership_es_lo
	.byte <cutscenes_strings_list_def_no_mothership_2_es_lo
	.byte <cutscenes_strings_list_def_utaco_temple_5_es_lo
	.byte <cutscenes_strings_list_def_cutscene_empty_es_lo
	.byte <cutscenes_strings_list_def_the_lab_2_es_lo
	.byte <cutscenes_strings_list_def_cutscene_empty_2_es_lo
	.byte 0
cutscenes_strings_es_lo_hi:
	.byte >cutscenes_strings_list_def_destroyed_city_es_lo
	.byte >cutscenes_strings_list_def_mothership_es_lo
	.byte >cutscenes_strings_list_def_destroyed_city_2_es_lo
	.byte >cutscenes_strings_list_def_temple_ruins_es_lo
	.byte >cutscenes_strings_list_def_utaco_temple_es_lo
	.byte 0
	.byte >cutscenes_strings_list_def_utaco_temple_2_es_lo
	.byte 0
	.byte >cutscenes_strings_list_def_mothership_2_es_lo
	.byte >cutscenes_strings_list_def_utaco_temple_3_es_lo
	.byte >cutscenes_strings_list_def_the_lab_es_lo
	.byte >cutscenes_strings_list_def_utaco_temple_4_es_lo
	.byte 0
	.byte 0
	.byte >cutscenes_strings_list_def_no_mothership_es_lo
	.byte >cutscenes_strings_list_def_no_mothership_2_es_lo
	.byte >cutscenes_strings_list_def_utaco_temple_5_es_lo
	.byte >cutscenes_strings_list_def_cutscene_empty_es_lo
	.byte >cutscenes_strings_list_def_the_lab_2_es_lo
	.byte >cutscenes_strings_list_def_cutscene_empty_2_es_lo
	.byte 0
cutscenes_strings_es_hi_lo:
	.byte <cutscenes_strings_list_def_destroyed_city_es_hi
	.byte <cutscenes_strings_list_def_mothership_es_hi
	.byte <cutscenes_strings_list_def_destroyed_city_2_es_hi
	.byte <cutscenes_strings_list_def_temple_ruins_es_hi
	.byte <cutscenes_strings_list_def_utaco_temple_es_hi
	.byte 0
	.byte <cutscenes_strings_list_def_utaco_temple_2_es_hi
	.byte 0
	.byte <cutscenes_strings_list_def_mothership_2_es_hi
	.byte <cutscenes_strings_list_def_utaco_temple_3_es_hi
	.byte <cutscenes_strings_list_def_the_lab_es_hi
	.byte <cutscenes_strings_list_def_utaco_temple_4_es_hi
	.byte 0
	.byte 0
	.byte <cutscenes_strings_list_def_no_mothership_es_hi
	.byte <cutscenes_strings_list_def_no_mothership_2_es_hi
	.byte <cutscenes_strings_list_def_utaco_temple_5_es_hi
	.byte <cutscenes_strings_list_def_cutscene_empty_es_hi
	.byte <cutscenes_strings_list_def_the_lab_2_es_hi
	.byte <cutscenes_strings_list_def_cutscene_empty_2_es_hi
	.byte 0
cutscenes_strings_es_hi_hi:
	.byte >cutscenes_strings_list_def_destroyed_city_es_hi
	.byte >cutscenes_strings_list_def_mothership_es_hi
	.byte >cutscenes_strings_list_def_destroyed_city_2_es_hi
	.byte >cutscenes_strings_list_def_temple_ruins_es_hi
	.byte >cutscenes_strings_list_def_utaco_temple_es_hi
	.byte 0
	.byte >cutscenes_strings_list_def_utaco_temple_2_es_hi
	.byte 0
	.byte >cutscenes_strings_list_def_mothership_2_es_hi
	.byte >cutscenes_strings_list_def_utaco_temple_3_es_hi
	.byte >cutscenes_strings_list_def_the_lab_es_hi
	.byte >cutscenes_strings_list_def_utaco_temple_4_es_hi
	.byte 0
	.byte 0
	.byte >cutscenes_strings_list_def_no_mothership_es_hi
	.byte >cutscenes_strings_list_def_no_mothership_2_es_hi
	.byte >cutscenes_strings_list_def_utaco_temple_5_es_hi
	.byte >cutscenes_strings_list_def_cutscene_empty_es_hi
	.byte >cutscenes_strings_list_def_the_lab_2_es_hi
	.byte >cutscenes_strings_list_def_cutscene_empty_2_es_hi
	.byte 0
cutscenes_strings_ja_lo_lo:
	.byte <cutscenes_strings_list_def_destroyed_city_ja_lo
	.byte <cutscenes_strings_list_def_mothership_ja_lo
	.byte <cutscenes_strings_list_def_destroyed_city_2_ja_lo
	.byte <cutscenes_strings_list_def_temple_ruins_ja_lo
	.byte <cutscenes_strings_list_def_utaco_temple_ja_lo
	.byte 0
	.byte <cutscenes_strings_list_def_utaco_temple_2_ja_lo
	.byte 0
	.byte <cutscenes_strings_list_def_mothership_2_ja_lo
	.byte <cutscenes_strings_list_def_utaco_temple_3_ja_lo
	.byte <cutscenes_strings_list_def_the_lab_ja_lo
	.byte <cutscenes_strings_list_def_utaco_temple_4_ja_lo
	.byte 0
	.byte 0
	.byte <cutscenes_strings_list_def_no_mothership_ja_lo
	.byte <cutscenes_strings_list_def_no_mothership_2_ja_lo
	.byte <cutscenes_strings_list_def_utaco_temple_5_ja_lo
	.byte <cutscenes_strings_list_def_cutscene_empty_ja_lo
	.byte <cutscenes_strings_list_def_the_lab_2_ja_lo
	.byte <cutscenes_strings_list_def_cutscene_empty_2_ja_lo
	.byte 0
cutscenes_strings_ja_lo_hi:
	.byte >cutscenes_strings_list_def_destroyed_city_ja_lo
	.byte >cutscenes_strings_list_def_mothership_ja_lo
	.byte >cutscenes_strings_list_def_destroyed_city_2_ja_lo
	.byte >cutscenes_strings_list_def_temple_ruins_ja_lo
	.byte >cutscenes_strings_list_def_utaco_temple_ja_lo
	.byte 0
	.byte >cutscenes_strings_list_def_utaco_temple_2_ja_lo
	.byte 0
	.byte >cutscenes_strings_list_def_mothership_2_ja_lo
	.byte >cutscenes_strings_list_def_utaco_temple_3_ja_lo
	.byte >cutscenes_strings_list_def_the_lab_ja_lo
	.byte >cutscenes_strings_list_def_utaco_temple_4_ja_lo
	.byte 0
	.byte 0
	.byte >cutscenes_strings_list_def_no_mothership_ja_lo
	.byte >cutscenes_strings_list_def_no_mothership_2_ja_lo
	.byte >cutscenes_strings_list_def_utaco_temple_5_ja_lo
	.byte >cutscenes_strings_list_def_cutscene_empty_ja_lo
	.byte >cutscenes_strings_list_def_the_lab_2_ja_lo
	.byte >cutscenes_strings_list_def_cutscene_empty_2_ja_lo
	.byte 0
cutscenes_strings_ja_hi_lo:
	.byte <cutscenes_strings_list_def_destroyed_city_ja_hi
	.byte <cutscenes_strings_list_def_mothership_ja_hi
	.byte <cutscenes_strings_list_def_destroyed_city_2_ja_hi
	.byte <cutscenes_strings_list_def_temple_ruins_ja_hi
	.byte <cutscenes_strings_list_def_utaco_temple_ja_hi
	.byte 0
	.byte <cutscenes_strings_list_def_utaco_temple_2_ja_hi
	.byte 0
	.byte <cutscenes_strings_list_def_mothership_2_ja_hi
	.byte <cutscenes_strings_list_def_utaco_temple_3_ja_hi
	.byte <cutscenes_strings_list_def_the_lab_ja_hi
	.byte <cutscenes_strings_list_def_utaco_temple_4_ja_hi
	.byte 0
	.byte 0
	.byte <cutscenes_strings_list_def_no_mothership_ja_hi
	.byte <cutscenes_strings_list_def_no_mothership_2_ja_hi
	.byte <cutscenes_strings_list_def_utaco_temple_5_ja_hi
	.byte <cutscenes_strings_list_def_cutscene_empty_ja_hi
	.byte <cutscenes_strings_list_def_the_lab_2_ja_hi
	.byte <cutscenes_strings_list_def_cutscene_empty_2_ja_hi
	.byte 0
cutscenes_strings_ja_hi_hi:
	.byte >cutscenes_strings_list_def_destroyed_city_ja_hi
	.byte >cutscenes_strings_list_def_mothership_ja_hi
	.byte >cutscenes_strings_list_def_destroyed_city_2_ja_hi
	.byte >cutscenes_strings_list_def_temple_ruins_ja_hi
	.byte >cutscenes_strings_list_def_utaco_temple_ja_hi
	.byte 0
	.byte >cutscenes_strings_list_def_utaco_temple_2_ja_hi
	.byte 0
	.byte >cutscenes_strings_list_def_mothership_2_ja_hi
	.byte >cutscenes_strings_list_def_utaco_temple_3_ja_hi
	.byte >cutscenes_strings_list_def_the_lab_ja_hi
	.byte >cutscenes_strings_list_def_utaco_temple_4_ja_hi
	.byte 0
	.byte 0
	.byte >cutscenes_strings_list_def_no_mothership_ja_hi
	.byte >cutscenes_strings_list_def_no_mothership_2_ja_hi
	.byte >cutscenes_strings_list_def_utaco_temple_5_ja_hi
	.byte >cutscenes_strings_list_def_cutscene_empty_ja_hi
	.byte >cutscenes_strings_list_def_the_lab_2_ja_hi
	.byte >cutscenes_strings_list_def_cutscene_empty_2_ja_hi
	.byte 0
