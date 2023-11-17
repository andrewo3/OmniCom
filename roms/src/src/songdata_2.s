
.include "src/globals.inc"
.include "src/songdata_2.inc"

      
.segment "SBANK_2"
songdata2_musicdata:
	.incbin "romdata/music/second_stage_music.bin"
	
.include "romdata/second_stage_beats.s"

		
	