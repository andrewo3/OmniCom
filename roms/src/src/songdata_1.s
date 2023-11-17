
.include "src/globals.inc"
.include "src/songdata_1.inc"

.segment "SBANK_1"

songdata1_musicdata:
	.incbin "romdata/music/first_stage_music.bin"

.include "romdata/first_stage_beats.s"
  	

