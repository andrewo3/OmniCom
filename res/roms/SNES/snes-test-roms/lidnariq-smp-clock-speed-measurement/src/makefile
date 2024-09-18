#!/usr/bin/make -f
#
# Makefile for LoROM template
# Copyright 2014-2015 Damian Yerrick
#
# Copying and distribution of this file, with or without
# modification, are permitted in any medium without royalty
# provided the copyright notice and this notice are preserved.
# This file is offered as-is, without any warranty.
#

# These are used in the title of the SFC program and the zip file.
title = smpspeed

# Space-separated list of asm files without .s extension
# (use a backslash to continue on the next line)
objlist = \
	snesheader main blarggapu spcimage

AS65 = ca65
LD65 = ld65
CFLAGS65 = 

# .PHONY means these targets aren't actual filenames
.PHONY: all clean

all: $(title).sfc

clean:
	-rm *.o 

# Rules for ROM
objlisto = $(foreach o,$(objlist),$(o).o)

$(title).map $(title).dbg $(title).sfc: snes32K.cfg $(objlisto)
	$(LD65) -o $(title).sfc -vm --dbgfile $(title).dbg -m $(title).map -C $^

%.o: %.s snes.inc global.inc
	$(AS65) $(CFLAGS65) $< -g -o $@

# Rules for CHR data
%.chr2 %.chr4 %.chr8: %.gif
	gif2chrs8 $<

main.o: main.s snes.inc global.inc
