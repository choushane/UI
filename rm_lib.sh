#!/bin/bash

#`-- install_UI
#    |-- SDL
#    |   |-- SDL-1.2.15
#    |   |-- SDL_image-1.2.12
#    |   |-- SDL_ttf-2.0.11
#    |   |-- arm
#    |   |-- freetype-2.5.2
#    |   |-- libpng-1.6.9
#    |   `-- zlib-1.2.8

LIB_DIR=(
	"freetype-2.5.2"
	"libpng-1.6.9"
	"SDL-1.2.15"
	"SDL_image-1.2.12"
	"SDL_ttf-2.0.11"
	"zlib-1.2.8"
	"arm"
)

cd SDL/
for dir in ${LIB_DIR[*]}
do
	rm -rf $dir
done
cd -
