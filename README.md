# learnopengl
Repository for following along with the tutorial for https://learnopengl.com/

# dependencies
Windows 64 bit (only tested on windows 10)

Following should be on the PATH:
* Gnu make for windows
* clang executables (e.g. clang++.exe)

Seem to need some kind of msvc install. Clang gives a warning if it's not there
and attempts to import GL/gl.h fail. I'm going to check on my other computer to
figure out what exactly is needed.

On my laptop, it's pulling the gl.h from:
C:\Program Files (x86)\Windows Kits\10\Include\10.0.17134.0\um\gl\gl.h

