# How to compile:

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
