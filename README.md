This is my attempt to implement Troy Schrapel's HD44780 LCD emulator for Windows.

At the moment, I haven't implemented anything related to graphics. It just renders the output as spaces and
asterisks using 120 x 32 characters for a 20x4 display (5+1 x 8).

As of 7/11, I think I managed to successfully add SFML to the CLion project with some *major* help and inspiration
from https://github.com/eXpl0it3r/cmake-sfml-project/tree/copy_dlls , so now I can finally start rendering
the emulated LCD itself graphically.