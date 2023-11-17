robotfindskitten 1.2NES
original version by Leonard Richardson
Nintendo version by Scott Holder
2014

This is a port of the Zen Simulation robotfindskitten. Your goal is to move around and observe objects by touching them to determine whether they are kitten or not.

The game ends when robotfindskitten.

You can read more information about the game itself at http://www.robotfindskitten.org . I have a simple project page up at http://iamscott.net/rfk , though you probably already knew that if you downloaded it from there.

Included in this archive you'll find:

rfk.nes - A compiled ROM of the game.
COPYING - The license of the game. It's GPL. Find the source on the site.

It's pretty simple, but it was intended as a way for me to get my feet wet in NES development. The code is not necessarily the prettiest or best as I'm a complete noob to NES programming, and only have some experience with C. It mostly works though.

I have verified this works on real hardware, but there's a corruption issue with the attribute table that leads to some stray colors in the non-kitten object descriptions. It's still 100% playable however, and seems to work fine in emulators, so I'm releasing it anyway. It may have something to do with the title palette descriptions I borrowed from one of the examples.

If you're interested in making a cartridge of this, you'll need an NROM-256 board, or an NROM-128 board converted to -256 specs. This is trivial to do; look it up. You'll find NROM boards in a lot of the very early games. "Baseball", "Golf", those sorts. It uses an NROM-256 standard 32Kb PRG-ROM and 8Kb CHR-ROM, so you don't need any esoteric setups to make a cartridge. If you don't know what this paragraph means, don't worry about it :)

If you're going to make and sell repros of this for some reason, all I ask is that you drop me a line and tell me a bit about yourself. I'm more interested to know that people are interested in this than anything else. Maybe buy me a local beer or two from your area.

Have fun!
Scott Holder
rfk@iamscott.net