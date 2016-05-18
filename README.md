# Bullet Fontana #

This is an edit of [StarShipFontata](https://github.com/AidanDelaney/StarshipFontana) to create a bullet hell like game. Written in C++ 11 with SDL Libraries.

## Story ##
The evil b’Kuhn has stolen the code to Earth’s defence system.
With this code he can, at any time, defeat the entire human race.
Only one woman is brave enough to go after b’Kuhn. Will she be
Earth’s hero? Puzzle your way though the universe in the company
of Commander Fontana in **Bullet Fontana**.

## Installation ##
You will have to have the SDL development libraries installed on
your system.  The easiest way to compile is to use a command-line

```bash
$ g++ -c -std=c++11 src/*.cpp
$ g++ -o starship *.o -lSDL2 -lSDL2_image -lSDL2_tff
```

which will produce an executable file called "starship" in the
top-level directory.  To execute this file do the following

`$ ./starship`
 
from the top-level directory.  The game will expect to find the
`assets` directory under its current working directory.

## Credits ##
The font in this game is from [FontSquirrel](https://www.fontsquirrel.com) Under the [SIL 1.1](https://www.fontsquirrel.com/license/Anonymous-Pro)
