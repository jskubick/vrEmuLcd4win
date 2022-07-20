Implementation of Troy Schrapel's HD44780 LCD emulator for Windows.

Two of the files (VrEmuLcd.h and .c) were taken verbatim from https://github.com/visrealm/vrEmuLcd and haven't
been changed.

I know that in theory, I should have probably included his two files as a Git submodule instead of just copying 
them to my project... but to be honest, I don't actually know _how_. More precisely... I know how to pull the 
files into my repo, but I don't know how to partially sever the link so SmartGit won't pester me thereafter
about not having write access to the other person's remote repo (where the files in the submodule came from).

### Warning

This is *literally* the first meaningful non-Arduino C++ development project I've ever attempted. Absolutely,
positively **do not** regard it as an example of good (let alone "best") C++ practices. 

### Note:

On 7/20/22, I got frustrated & severed the submodule link to hd447780-formatter that was living in ./formatter
and (semi-temporarily?) replaced it with a normal subdirectory so I can move forward instead of being stalled 
& sidetracked indefinitely trying to get past this submodule problem :-(