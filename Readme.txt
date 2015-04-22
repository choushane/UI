install_UI.cpp is a UI program based on SDL-1.2.15, which is used to display install progress during firmware installation.
init.cpp is just a test program also based on SDL-1.2.15.

1) to build install_UI.exe, following the steps below:
   # modify TOOLCHAIN_PATH & CROSS_COMPILER in build_lib.sh & Makefile, if necessary.
   $ ./build_lib.sh
   $ ./make

   a) ./build_lib.sh will untar each files under SDL/, cd to each directory, then using the following commands to build each of them. The generated library/header files will be installed under SDL/arm/.
      - configure [parameters depends on each library]
      - make
      - make install

   b) ./make will create install_UI.exe & customer.tar.
      - install_UI.exe is the executable binary used to display update progress. It will open .png and .ttf files under "/tmp/installUI/res/", which is to meet the requirement of the install_a. The path can be changed to "res/" by #define STAND_ALONE_AP in install_UI.cpp.
      - customer.tar is the required file that should be included into install.img.

2) to clean the installed libraries produced by build_lib.sh, using the following commands:
   $ ./rm_lib.sh

3) to build the test program(init.cpp), using the following commands. It will create init.exe
   $ ./make init
   init.exe will open .png and .ttf files under "res/".
   

the tree structure will be like following after ./build_lib.sh:
`-- install_UI
    |-- SDL
    |   |-- SDL-1.2.15
    |   |-- SDL_image-1.2.12
    |   |-- SDL_ttf-2.0.11
    |   |-- arm
    |   |-- freetype-2.5.2
    |   |-- libpng-1.6.9
    |   `-- zlib-1.2.8
    `-- res
