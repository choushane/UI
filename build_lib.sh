#!/bin/bash
TOOLCHAIN_PATH=/opt/cross-project/arm/linaro/
CROSS_COMPILER=arm-linux-gnueabihf

PATH=$TOOLCHAIN_PATH/bin:$PATH

# variable used to configure each lib
INSTALL_PATH=$PWD/SDL/arm
HOST=arm-linux
CC=$CROSS_COMPILER-gcc
CXX=$CROSS_COMPILER-g++

#`-- install_UI
#    |-- SDL
#    |   |-- SDL-1.2.15
#    |   |-- SDL_image-1.2.12
#    |   |-- SDL_ttf-2.0.11
#    |   |-- arm
#    |   |-- freetype-2.5.2
#    |   |-- libpng-1.6.9
#    |   `-- zlib-1.2.8

LIB_FILE=(
	"freetype-2.5.2.tar.bz2"
	"libpng-1.6.9.tar.gz"
	"SDL-1.2.15.tar.gz"
	"SDL_image-1.2.12.tar.gz"
	"SDL_ttf-2.0.11.tar.gz"
	"zlib-1.2.8.tar.gz"
)

# set -v will print each commands..
set -v

cd SDL/
for file in ${LIB_FILE[*]}
do
	echo "---------- untar $file ----------"
	tar xvf $file
done

echo "---------- patch SDL-1.2.15 ----------"
patch -p0 < SDL-1.2.15.patch
cd -

mkdir -p $INSTALL_PATH

echo "========== build SDL-1.2.15 =========="
cd SDL/SDL-1.2.15/
./configure --prefix=$INSTALL_PATH --host=$HOST --disable-audio --disable-joystick --disable-cdrom --disable-cpuinfo --disable-assembly --disable-oss --disable-alsa --disable-esd --disable-pulseaudio --disable-arts --disable-nas --disable-diskaudio --disable-dummyaudio --disable-mintaudio --disable-nasm --disable-altivec  --disable-video-x11 --disable-video-directfb --disable-dga --disable-video-dga --disable-video-x11-dgamouse --disable-video-x11-vm --disable-video-x11-xv --disable-video-x11-xinerama  --disable-video-x11-xme --disable-video-x11-xrandr --disable-video-photon --disable-video-cocoa --disable-video-ps2gs --disable-video-ps3 --disable-video-svga --disable-video-vgl --disable-video-wscons --disable-video-xbios --disable-video-gem --disable-video-dummy --disable-video-opengl --disable-osmesa-shared --disable-screensaver --disable-input-events --disable-input-tslib --disable-stdio-redirect --disable-directx --disable-sdl-dlopen --disable-atari-ldg CC=$CC CXX=$CXX
make 
make install
cd -

echo "========== build zlib-1.2.8 =========="
cd SDL/zlib-1.2.8/
CC=$CC ./configure --prefix=$INSTALL_PATH
make
make install
cd -

echo "========== build libpng-1.6.9 =========="
cd SDL/libpng-1.6.9/
./configure --prefix=$INSTALL_PATH --host=$HOST LDFLAGS=-L$INSTALL_PATH/lib CPPFLAGS=-I$INSTALL_PATH/include CC=$CC CXX=$CXX
make
make install
cd -

echo "========== build SDL_image-1.2.12 =========="
cd SDL/SDL_image-1.2.12/
./configure --prefix=$INSTALL_PATH --host=$HOST --disable-bmp --disable-gif --disable-jpg --disable-jpg-shared --disable-lbm --disable-pcx --disable-png-shared --disable-pnm --disable-tga --disable-tif --disable-tif-shared --disable-xcf --disable-xpm --disable-xv --disable-webp --disable-webp-shared CC=$CC CXX=$CXX CPPFLAGS=-I$INSTALL_PATH/include PKG_CONFIG_PATH=$INSTALL_PATH/lib/pkgconfig
make
make install
cd -

echo "========== build freetype-2.5.2 =========="
cd SDL/freetype-2.5.2/
./configure --prefix=$INSTALL_PATH --host=$HOST --without-png CC=$CC CXX=$CXX
make
make install
cd -

echo "========== build SDL_ttf-2.0.11 =========="
cd SDL/SDL_ttf-2.0.11/
./configure --prefix=$INSTALL_PATH --host=$HOST CC=$CC CXX=$CXX PKG_CONFIG_PATH=$INSTALL_PATH/lib/pkgconfig --with-freetype-prefix=$INSTALL_PATH
make
make install
cd -
