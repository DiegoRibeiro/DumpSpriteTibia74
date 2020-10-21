# How to compile:

## Get the submodule
git submodule init
git submodule update

## Windows:

Require mingw32-make, gcc, g++:

`mkdir build && cd build`

`cmake -G "MinGW Makefiles" ../`

`mingw32-make all`

Optional:
`mingw32-make install`

## Linux

`mkdir build && cd build`

`cmake ../`

`make all`

Optional:
`make install`

## Notes

So far i have tested only on tibia.spr from 7.4/7.7 version. 
