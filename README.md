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

Going to try to install a windows kit
https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk

Installed it with the defaults, used the installer rather than the ISO for now.
Doesn't seem to work, going to try installing visual studio
(I get the following error:

clang.exe: warning: unable to find a Visual Studio installation; try running Clang from a developer command prompt [-Wmsvc-not-found]
In file included from gltest.c:1:
glfw-3.2.1.bin.WIN64/include\GLFW/glfw3.h:171:12: fatal error: 'GL/gl.h' file not found
  #include <GL/gl.h>
)

There is a copy of that file available from the installed windows kit: C:\Program Files (x86)\Windows Kits\10\Include\10.0.17763.0\um
Adding that directory failed at 
C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/um\GL/gl.h:22:10: fatal error: 'winapifamily.h' file not found
#include <winapifamily.h>

I added in those directories with a Makefile.local, uninstalled and reinstalled clang, then got an error where the program was not executable:

On my desktop where it's not working:
(output of clang -v):
 "link.exe" -out:gltest.exe -defaultlib:libcmt "-libpath:lib\\amd64" "-libpath:C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.17763.0\\ucrt\\x64" "-libpath:C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.17763.0\\um\\x64" -libpath:glfw-3.2.1.bin.WIN64/lib-vc2015 -nologo glfw3dll.lib glfw3.lib opengl32.lib "C:\\Users\\David\\AppData\\Local\\Temp\\gltest-06b875.o"
clang.exe: error: unable to execute command: program not executable
clang.exe: error: linker command failed with exit code 1 (use -v to see invocation)
make: * [gltest.exe] Error 1
($ where link.exe)
C:\Program Files\Git\usr\bin\link.exe

On my laptop where it is working:
(output of clang -v):
"C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\BuildTools\\VC\\Tools\\MSVC\\14.14.26428\\bin\\HostX64\\x64\\link.exe
$ where link.exe
C:\Program Files\Git\usr\bin\link.exe

It looks like I had an already-installed visual studio code on this computer. I'm going to try installing visual studio code on the other computer and see if it works

