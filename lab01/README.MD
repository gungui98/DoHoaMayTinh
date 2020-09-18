# Lab01 - note

## Installation note
### For windows:
Follow this note for setup codeblock : [wiki](http://wiki.codeblocks.org/index.php/Using_FreeGlut_with_Code::Blocks)
Download glut from this [link](https://www.transmissionzero.co.uk/files/software/development/GLUT/freeglut-MinGW-3.0.0-1.mp.zip)
Extract zip file, we get ```freeglut``` with 3 sub folder ```bin, include, lib```
From code block project, select "build options", select "search directory"
Add ```path/to/freeglut/include``` to Compiler
Add ```path/to/freeglut/lib``` to Linker (If you use 64bit, then select ```path/to/freeglut/lib/x64```)
Copy ```path/to/freeglut/bin/freeglut.dll``` to ```C:\Windows\System32``` (If you use 64bit, then select ```path/to/freeglut/bin/x64/freeglut.dll```)
Try to build and run project

### For Ubuntu

``` sudo apt install g++ cmake freeglut3 freeglut3-dev```

## To compile

``` g++ Lab01.cpp -lglut -lGL -o lab01.o```

## To run
``` ./lab01.o```
