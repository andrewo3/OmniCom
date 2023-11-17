
.include "src/globals.inc"
.include "src/songdata_3.inc"

      
.segment "SBANK_3"
songdata3_musicdata:
	.incbin "romdata/music/third_stage_music.bin"
	
.include "romdata/third_stage_beats.s"

		
	
