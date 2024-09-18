# SMP Clock Speed Measurement Tool

Posted by lidnariq [on the nesdev forum][p].

[p]: https://forums.nesdev.org/viewtopic.php?t=24610

> In past years, people have taken note that the clock for the sound generator
> inside the SNES uses a low-quality clock source, and it tends to run fast.
> People have previously measured this, but the needed precision measurement
> devices are rare and expensive. Additionally, although there's a well-
> documented thermal coefficient modulating the frequency of PZT resonators,
> previous measurements couldn't find this here.
>
> Fortunately, every SNES already has a fairly precise clock source in it: the
> main system oscillator must provide a fairly accurate, precise, and consistent
> clock speed or else the colors will be noticeably wrong; at 400ppm of error
> the left and right sides of the screen would be noticeably the wrong hue.
> Additionally, quartz crystals are usually accurate to within 30ppm without any
> assistance such as a OCXO or GPSDO.
>
> The attached program uses the S-CPU running in FastROM to measure how fast the
> S-SMP takes to run a specific delay loop. It then does a bunch of math to
> convert that count into 1- a number of microseconds, and 2- the measured speed
> of the S-SMP, and it also keeps track of and displays the fastest and slowest
> speeds seen.
>
> Both I and several other people have tested on NTSC hardware. It *should* work
> in PAL, but I have not had anyone test yet.
>
> My personal SNES cold boots with its DSP DAC at roughly 32030Hz, and very
> slowly speeds up as it gets warmer.
>
> Source is included. Program is licensed under zlib, since I liberally copied
> from Pino's lorom-template.
>
> Specific code routines of possible interest:
> long division of 37-bit number by 17- or 18-bit number
> "Double dabble" 24-bit to 8-nybble BCD conversion

UnDisbeliever [followed up][p2] with measurements taken on their consoles:

> - 2/1/3 3-chip SFC (my daily driver): 32147Hz cold, 32152Hz after 25 minutes
> - 1-chip SFC: 32036Hz cold, 32043Hz after 25 minutes
> - 1/1/1 SFC (a with broken PPU): 32067Hz cold
> - 1/1/1 SFC: 32080Hz cold, 32090Hz after 25 minutes
> - PAL 1-chip SNES: 32083Hz cold, 32091Hz after 25 minutes

[p2]: https://forums.nesdev.org/viewtopic.php?p=287980#p287980

...and then [again][p3] later:

> We measured the S-APU clock speed on our PAL 1-CHIP SNES with my brother's
> oscilloscope. It read 32.08kHz cold, rising to 32.09kHz after a bit, matching
> the output of smpspeed.sfc (32083Hz -> 32086Hz).

[p3]: https://forums.nesdev.org/viewtopic.php?p=288054#p288054
