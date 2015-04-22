#
# FreeType 2 configuration rules templates for Unix + configure
#


# Copyright 1996-2000, 2002, 2004, 2006, 2008, 2013 by
# David Turner, Robert Wilhelm, and Werner Lemberg.
#
# This file is part of the FreeType project, and may only be used, modified,
# and distributed under the terms of the FreeType project license,
# LICENSE.TXT.  By continuing to use, modify, or distribute this file you
# indicate that you have read the license and understand and accept it
# fully.

SHELL := /bin/bash

TOP_DIR := $(shell cd $(TOP_DIR); pwd)

DELETE := rm -f
DELDIR := rmdir
CAT    := cat
SEP    := /

# this is used for `make distclean' and `make install'
OBJ_BUILD ?= $(BUILD_DIR)

# don't use `:=' here since the path stuff will be included after this file
#
FTSYS_SRC = $(BASE_DIR)/ftsystem.c

INSTALL         := /usr/bin/install -c
INSTALL_DATA    := ${INSTALL} -m 644
INSTALL_PROGRAM := ${INSTALL}
INSTALL_SCRIPT  := ${INSTALL}
MKINSTALLDIRS   := $(BUILD_DIR)/mkinstalldirs

DISTCLEAN += $(OBJ_BUILD)/config.cache    \
             $(OBJ_BUILD)/config.log      \
             $(OBJ_BUILD)/config.status   \
             $(OBJ_BUILD)/unix-def.mk     \
             $(OBJ_BUILD)/unix-cc.mk      \
             $(OBJ_BUILD)/ftconfig.h      \
             $(OBJ_BUILD)/freetype-config \
             $(OBJ_BUILD)/freetype2.pc    \
             $(LIBTOOL)                   \
             $(OBJ_BUILD)/Makefile


# Standard installation variables.
#
prefix       := /home/shane/TAS/install_UI/SDL/arm
exec_prefix  := ${prefix}
libdir       := ${exec_prefix}/lib
bindir       := ${exec_prefix}/bin
includedir   := ${prefix}/include
datarootdir  := ${prefix}/share
datadir      := ${datarootdir}

version_info := 17:1:11

# Variables needed for `freetype-config' and `freetype.pc'.
#
FT2_EXTRA_LIBS     := 
LIBBZ2             := 
LIBZ               := -lz
LIBPNG             := 
build_libtool_libs := 
ft_version         := 17.1.11

# The directory where all library files are placed.
#
# By default, this is the same as $(OBJ_DIR); however, this can be changed
# to suit particular needs.
#
LIB_DIR := $(OBJ_DIR)

# The BASE_SRC macro lists all source files that should be included in
# src/base/ftbase.c.  When configure sets up CFLAGS to build ftmac.c,
# ftmac.c should be added to BASE_SRC.
ftmac_c := 

# The SYSTEM_ZLIB macro is defined if the user wishes to link dynamically
# with its system wide zlib. If SYSTEM_ZLIB is 'yes', the zlib part of the
# ftgzip module is not compiled in.
SYSTEM_ZLIB := yes


# The NO_OUTPUT macro is appended to command lines in order to ignore
# the output of some programs.
#
NO_OUTPUT := 2> /dev/null


# To support calls like
#
#   configure --includedir='${libdir}'/freetype2/include
#
# we generate `freetype-config' and `freetype.pc' at compile time so that
# those variables are properly expanded.

$(OBJ_BUILD)/freetype-config: $(TOP_DIR)/builds/unix/freetype-config.in
	rm -f $@ $@.tmp
	sed -e 's|%FT2_EXTRA_LIBS%|$(FT2_EXTRA_LIBS)|' \
	    -e 's|%LIBBZ2%|$(LIBBZ2)|' \
	    -e 's|%LIBZ%|$(LIBZ)|' \
	    -e 's|%LIBPNG%|$(LIBPNG)|' \
	    -e 's|%build_libtool_libs%|$(build_libtool_libs)|' \
	    -e 's|%exec_prefix%|$(exec_prefix)|' \
	    -e 's|%ft_version%|$(ft_version)|' \
	    -e 's|%includedir%|$(includedir)|' \
	    -e 's|%libdir%|$(libdir)|' \
	    -e 's|%prefix%|$(prefix)|' \
	    $< \
	    > $@.tmp
	chmod +x $@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@

$(OBJ_BUILD)/freetype2.pc: $(TOP_DIR)/builds/unix/freetype2.in
	rm -f $@ $@.tmp
	sed -e 's|%FT2_EXTRA_LIBS%|$(FT2_EXTRA_LIBS)|' \
	    -e 's|%LIBBZ2%|$(LIBBZ2)|' \
	    -e 's|%LIBZ%|$(LIBZ)|' \
	    -e 's|%LIBPNG%|$(LIBPNG)|' \
	    -e 's|%build_libtool_libs%|$(build_libtool_libs)|' \
	    -e 's|%exec_prefix%|$(exec_prefix)|' \
	    -e 's|%ft_version%|$(ft_version)|' \
	    -e 's|%includedir%|$(includedir)|' \
	    -e 's|%libdir%|$(libdir)|' \
	    -e 's|%prefix%|$(prefix)|' \
	    $< \
	    > $@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@

all: $(OBJ_BUILD)/freetype-config \
     $(OBJ_BUILD)/freetype2.pc

# EOF