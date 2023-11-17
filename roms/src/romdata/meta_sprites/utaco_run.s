utaco_run_PATTERN_OFFSET=0
utaco_run_frames: .byte 6
utaco_run_bytes_per_frame: .byte 144
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
utaco_run_frame_0:    ;(X,Y,PATTERN_ID,PAL)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.byte 0, 0, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 0, (utaco_run_PATTERN_OFFSET+0), 0
.byte 16, 0, (utaco_run_PATTERN_OFFSET+1), 0
.byte 24, 0, (utaco_run_PATTERN_OFFSET+2), 0
.byte 32, 0, (utaco_run_PATTERN_OFFSET+3), 0
.byte 40, 0, (utaco_run_PATTERN_OFFSET+0), 0
.byte 0, 8, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 8, (utaco_run_PATTERN_OFFSET+0), 0
.byte 16, 8, (utaco_run_PATTERN_OFFSET+4), 0
.byte 24, 8, (utaco_run_PATTERN_OFFSET+5), 0
.byte 32, 8, (utaco_run_PATTERN_OFFSET+6), 0
.byte 40, 8, (utaco_run_PATTERN_OFFSET+7), 0
.byte 0, 16, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 16, (utaco_run_PATTERN_OFFSET+8), 0
.byte 16, 16, (utaco_run_PATTERN_OFFSET+9), 0
.byte 24, 16, (utaco_run_PATTERN_OFFSET+10), 1
.byte 32, 16, (utaco_run_PATTERN_OFFSET+11), 2
.byte 40, 16, (utaco_run_PATTERN_OFFSET+12), 0
.byte 0, 24, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 24, (utaco_run_PATTERN_OFFSET+13), 1
.byte 16, 24, (utaco_run_PATTERN_OFFSET+14), 2
.byte 24, 24, (utaco_run_PATTERN_OFFSET+15), 3
.byte 32, 24, (utaco_run_PATTERN_OFFSET+16), 3
.byte 40, 24, (utaco_run_PATTERN_OFFSET+17), 0
.byte 0, 32, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 32, (utaco_run_PATTERN_OFFSET+18), 3
.byte 16, 32, (utaco_run_PATTERN_OFFSET+19), 3
.byte 24, 32, (utaco_run_PATTERN_OFFSET+20), 3
.byte 32, 32, (utaco_run_PATTERN_OFFSET+0), 0
.byte 40, 32, (utaco_run_PATTERN_OFFSET+0), 0
.byte 0, 40, (utaco_run_PATTERN_OFFSET+21), 0
.byte 8, 40, (utaco_run_PATTERN_OFFSET+22), 3
.byte 16, 40, (utaco_run_PATTERN_OFFSET+23), 0
.byte 24, 40, (utaco_run_PATTERN_OFFSET+24), 0
.byte 32, 40, (utaco_run_PATTERN_OFFSET+0), 0
.byte 40, 40, (utaco_run_PATTERN_OFFSET+0), 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
utaco_run_frame_1:    ;(X,Y,PATTERN_ID,PAL)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.byte 0, 0, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 0, (utaco_run_PATTERN_OFFSET+0), 0
.byte 16, 0, (utaco_run_PATTERN_OFFSET+25), 0
.byte 24, 0, (utaco_run_PATTERN_OFFSET+26), 0
.byte 32, 0, (utaco_run_PATTERN_OFFSET+27), 0
.byte 40, 0, (utaco_run_PATTERN_OFFSET+0), 0
.byte 0, 8, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 8, (utaco_run_PATTERN_OFFSET+28), 0
.byte 16, 8, (utaco_run_PATTERN_OFFSET+29), 0
.byte 24, 8, (utaco_run_PATTERN_OFFSET+30), 0
.byte 32, 8, (utaco_run_PATTERN_OFFSET+31), 0
.byte 40, 8, (utaco_run_PATTERN_OFFSET+32), 0
.byte 0, 16, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 16, (utaco_run_PATTERN_OFFSET+33), 0
.byte 16, 16, (utaco_run_PATTERN_OFFSET+34), 0
.byte 24, 16, (utaco_run_PATTERN_OFFSET+35), 2
.byte 32, 16, (utaco_run_PATTERN_OFFSET+36), 2
.byte 40, 16, (utaco_run_PATTERN_OFFSET+37), 0
.byte 0, 24, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 24, (utaco_run_PATTERN_OFFSET+38), 1
.byte 16, 24, (utaco_run_PATTERN_OFFSET+39), 3
.byte 24, 24, (utaco_run_PATTERN_OFFSET+40), 3
.byte 32, 24, (utaco_run_PATTERN_OFFSET+41), 2
.byte 40, 24, (utaco_run_PATTERN_OFFSET+42), 0
.byte 0, 32, (utaco_run_PATTERN_OFFSET+43), 0
.byte 8, 32, (utaco_run_PATTERN_OFFSET+44), 3
.byte 16, 32, (utaco_run_PATTERN_OFFSET+45), 3
.byte 24, 32, (utaco_run_PATTERN_OFFSET+46), 3
.byte 32, 32, (utaco_run_PATTERN_OFFSET+47), 0
.byte 40, 32, (utaco_run_PATTERN_OFFSET+48), 0
.byte 0, 40, (utaco_run_PATTERN_OFFSET+49), 0
.byte 8, 40, (utaco_run_PATTERN_OFFSET+50), 0
.byte 16, 40, (utaco_run_PATTERN_OFFSET+0), 0
.byte 24, 40, (utaco_run_PATTERN_OFFSET+51), 3
.byte 32, 40, (utaco_run_PATTERN_OFFSET+52), 0
.byte 40, 40, (utaco_run_PATTERN_OFFSET+0), 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
utaco_run_frame_2:    ;(X,Y,PATTERN_ID,PAL)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.byte 0, 0, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 0, (utaco_run_PATTERN_OFFSET+0), 0
.byte 16, 0, (utaco_run_PATTERN_OFFSET+0), 0
.byte 24, 0, (utaco_run_PATTERN_OFFSET+53), 0
.byte 32, 0, (utaco_run_PATTERN_OFFSET+54), 0
.byte 40, 0, (utaco_run_PATTERN_OFFSET+0), 0
.byte 0, 8, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 8, (utaco_run_PATTERN_OFFSET+0), 0
.byte 16, 8, (utaco_run_PATTERN_OFFSET+55), 0
.byte 24, 8, (utaco_run_PATTERN_OFFSET+5), 0
.byte 32, 8, (utaco_run_PATTERN_OFFSET+56), 0
.byte 40, 8, (utaco_run_PATTERN_OFFSET+57), 0
.byte 0, 16, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 16, (utaco_run_PATTERN_OFFSET+8), 0
.byte 16, 16, (utaco_run_PATTERN_OFFSET+58), 0
.byte 24, 16, (utaco_run_PATTERN_OFFSET+59), 1
.byte 32, 16, (utaco_run_PATTERN_OFFSET+60), 1
.byte 40, 16, (utaco_run_PATTERN_OFFSET+61), 0
.byte 0, 24, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 24, (utaco_run_PATTERN_OFFSET+62), 0
.byte 16, 24, (utaco_run_PATTERN_OFFSET+63), 2
.byte 24, 24, (utaco_run_PATTERN_OFFSET+64), 3
.byte 32, 24, (utaco_run_PATTERN_OFFSET+65), 3
.byte 40, 24, (utaco_run_PATTERN_OFFSET+66), 0
.byte 0, 32, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 32, (utaco_run_PATTERN_OFFSET+67), 0
.byte 16, 32, (utaco_run_PATTERN_OFFSET+68), 3
.byte 24, 32, (utaco_run_PATTERN_OFFSET+69), 3
.byte 32, 32, (utaco_run_PATTERN_OFFSET+70), 3
.byte 40, 32, (utaco_run_PATTERN_OFFSET+71), 0
.byte 0, 40, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 40, (utaco_run_PATTERN_OFFSET+72), 0
.byte 16, 40, (utaco_run_PATTERN_OFFSET+73), 0
.byte 24, 40, (utaco_run_PATTERN_OFFSET+74), 0
.byte 32, 40, (utaco_run_PATTERN_OFFSET+0), 0
.byte 40, 40, (utaco_run_PATTERN_OFFSET+0), 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
utaco_run_frame_3:    ;(X,Y,PATTERN_ID,PAL)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.byte 0, 0, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 0, (utaco_run_PATTERN_OFFSET+0), 0
.byte 16, 0, (utaco_run_PATTERN_OFFSET+75), 0
.byte 24, 0, (utaco_run_PATTERN_OFFSET+76), 0
.byte 32, 0, (utaco_run_PATTERN_OFFSET+77), 0
.byte 40, 0, (utaco_run_PATTERN_OFFSET+0), 0
.byte 0, 8, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 8, (utaco_run_PATTERN_OFFSET+0), 0
.byte 16, 8, (utaco_run_PATTERN_OFFSET+78), 0
.byte 24, 8, (utaco_run_PATTERN_OFFSET+79), 0
.byte 32, 8, (utaco_run_PATTERN_OFFSET+80), 0
.byte 40, 8, (utaco_run_PATTERN_OFFSET+81), 0
.byte 0, 16, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 16, (utaco_run_PATTERN_OFFSET+82), 0
.byte 16, 16, (utaco_run_PATTERN_OFFSET+83), 0
.byte 24, 16, (utaco_run_PATTERN_OFFSET+84), 1
.byte 32, 16, (utaco_run_PATTERN_OFFSET+85), 1
.byte 40, 16, (utaco_run_PATTERN_OFFSET+86), 0
.byte 0, 24, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 24, (utaco_run_PATTERN_OFFSET+87), 0
.byte 16, 24, (utaco_run_PATTERN_OFFSET+88), 3
.byte 24, 24, (utaco_run_PATTERN_OFFSET+89), 3
.byte 32, 24, (utaco_run_PATTERN_OFFSET+90), 3
.byte 40, 24, (utaco_run_PATTERN_OFFSET+91), 0
.byte 0, 32, (utaco_run_PATTERN_OFFSET+92), 0
.byte 8, 32, (utaco_run_PATTERN_OFFSET+93), 3
.byte 16, 32, (utaco_run_PATTERN_OFFSET+94), 3
.byte 24, 32, (utaco_run_PATTERN_OFFSET+95), 3
.byte 32, 32, (utaco_run_PATTERN_OFFSET+0), 0
.byte 40, 32, (utaco_run_PATTERN_OFFSET+0), 0
.byte 0, 40, (utaco_run_PATTERN_OFFSET+96), 0
.byte 8, 40, (utaco_run_PATTERN_OFFSET+97), 0
.byte 16, 40, (utaco_run_PATTERN_OFFSET+98), 0
.byte 24, 40, (utaco_run_PATTERN_OFFSET+0), 0
.byte 32, 40, (utaco_run_PATTERN_OFFSET+0), 0
.byte 40, 40, (utaco_run_PATTERN_OFFSET+0), 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
utaco_run_frame_4:    ;(X,Y,PATTERN_ID,PAL)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.byte 0, 0, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 0, (utaco_run_PATTERN_OFFSET+0), 0
.byte 16, 0, (utaco_run_PATTERN_OFFSET+25), 0
.byte 24, 0, (utaco_run_PATTERN_OFFSET+26), 0
.byte 32, 0, (utaco_run_PATTERN_OFFSET+27), 0
.byte 40, 0, (utaco_run_PATTERN_OFFSET+0), 0
.byte 0, 8, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 8, (utaco_run_PATTERN_OFFSET+28), 0
.byte 16, 8, (utaco_run_PATTERN_OFFSET+29), 0
.byte 24, 8, (utaco_run_PATTERN_OFFSET+30), 0
.byte 32, 8, (utaco_run_PATTERN_OFFSET+31), 0
.byte 40, 8, (utaco_run_PATTERN_OFFSET+32), 0
.byte 0, 16, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 16, (utaco_run_PATTERN_OFFSET+99), 1
.byte 16, 16, (utaco_run_PATTERN_OFFSET+100), 2
.byte 24, 16, (utaco_run_PATTERN_OFFSET+35), 2
.byte 32, 16, (utaco_run_PATTERN_OFFSET+101), 1
.byte 40, 16, (utaco_run_PATTERN_OFFSET+102), 0
.byte 0, 24, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 24, (utaco_run_PATTERN_OFFSET+103), 2
.byte 16, 24, (utaco_run_PATTERN_OFFSET+104), 3
.byte 24, 24, (utaco_run_PATTERN_OFFSET+105), 3
.byte 32, 24, (utaco_run_PATTERN_OFFSET+106), 3
.byte 40, 24, (utaco_run_PATTERN_OFFSET+107), 0
.byte 0, 32, (utaco_run_PATTERN_OFFSET+108), 0
.byte 8, 32, (utaco_run_PATTERN_OFFSET+109), 3
.byte 16, 32, (utaco_run_PATTERN_OFFSET+110), 3
.byte 24, 32, (utaco_run_PATTERN_OFFSET+111), 3
.byte 32, 32, (utaco_run_PATTERN_OFFSET+112), 0
.byte 40, 32, (utaco_run_PATTERN_OFFSET+0), 0
.byte 0, 40, (utaco_run_PATTERN_OFFSET+113), 0
.byte 8, 40, (utaco_run_PATTERN_OFFSET+114), 0
.byte 16, 40, (utaco_run_PATTERN_OFFSET+115), 0
.byte 24, 40, (utaco_run_PATTERN_OFFSET+116), 0
.byte 32, 40, (utaco_run_PATTERN_OFFSET+117), 0
.byte 40, 40, (utaco_run_PATTERN_OFFSET+0), 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
utaco_run_frame_5:    ;(X,Y,PATTERN_ID,PAL)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.byte 0, 0, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 0, (utaco_run_PATTERN_OFFSET+0), 0
.byte 16, 0, (utaco_run_PATTERN_OFFSET+1), 0
.byte 24, 0, (utaco_run_PATTERN_OFFSET+118), 0
.byte 32, 0, (utaco_run_PATTERN_OFFSET+119), 0
.byte 40, 0, (utaco_run_PATTERN_OFFSET+0), 0
.byte 0, 8, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 8, (utaco_run_PATTERN_OFFSET+0), 0
.byte 16, 8, (utaco_run_PATTERN_OFFSET+120), 0
.byte 24, 8, (utaco_run_PATTERN_OFFSET+121), 0
.byte 32, 8, (utaco_run_PATTERN_OFFSET+122), 0
.byte 40, 8, (utaco_run_PATTERN_OFFSET+7), 0
.byte 0, 16, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 16, (utaco_run_PATTERN_OFFSET+123), 0
.byte 16, 16, (utaco_run_PATTERN_OFFSET+124), 0
.byte 24, 16, (utaco_run_PATTERN_OFFSET+125), 1
.byte 32, 16, (utaco_run_PATTERN_OFFSET+126), 1
.byte 40, 16, (utaco_run_PATTERN_OFFSET+127), 0
.byte 0, 24, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 24, (utaco_run_PATTERN_OFFSET+128), 3
.byte 16, 24, (utaco_run_PATTERN_OFFSET+129), 3
.byte 24, 24, (utaco_run_PATTERN_OFFSET+130), 3
.byte 32, 24, (utaco_run_PATTERN_OFFSET+131), 1
.byte 40, 24, (utaco_run_PATTERN_OFFSET+132), 0
.byte 0, 32, (utaco_run_PATTERN_OFFSET+0), 0
.byte 8, 32, (utaco_run_PATTERN_OFFSET+133), 0
.byte 16, 32, (utaco_run_PATTERN_OFFSET+134), 3
.byte 24, 32, (utaco_run_PATTERN_OFFSET+135), 3
.byte 32, 32, (utaco_run_PATTERN_OFFSET+136), 0
.byte 40, 32, (utaco_run_PATTERN_OFFSET+0), 0
.byte 0, 40, (utaco_run_PATTERN_OFFSET+137), 0
.byte 8, 40, (utaco_run_PATTERN_OFFSET+138), 0
.byte 16, 40, (utaco_run_PATTERN_OFFSET+139), 3
.byte 24, 40, (utaco_run_PATTERN_OFFSET+0), 0
.byte 32, 40, (utaco_run_PATTERN_OFFSET+0), 0
.byte 40, 40, (utaco_run_PATTERN_OFFSET+0), 0
utaco_run_frametable_lo:
	 .byte <utaco_run_frame_0
	 .byte <utaco_run_frame_1
	 .byte <utaco_run_frame_2
	 .byte <utaco_run_frame_3
	 .byte <utaco_run_frame_4
	 .byte <utaco_run_frame_5
utaco_run_frametable_hi:
	 .byte >utaco_run_frame_0
	 .byte >utaco_run_frame_1
	 .byte >utaco_run_frame_2
	 .byte >utaco_run_frame_3
	 .byte >utaco_run_frame_4
	 .byte >utaco_run_frame_5
