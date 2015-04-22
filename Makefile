# You can adjust the toolchain as you need
TOOLCHAIN_PATH = /opt/cross-project/arm/linaro/

CROSS_COMPILER = arm-linux-gnueabihf
CXX= $(TOOLCHAIN_PATH)/bin/$(CROSS_COMPILER)-g++
STRIP=$(TOOLCHAIN_PATH)/bin/$(CROSS_COMPILER)-strip

# ./SDL/arm/bin/sdl-config --static-libs --cflags
SDL_LIBS=-LSDL/arm/lib -Wl,-rpath,SDL/arm/lib -lSDL -lpthread -lm -lpthread
SDL_INCLUDE = -ISDL/arm/include

CXXFLAGS = -Wall -c $(SDL_INCLUDE)
#CXXFLAGS += -DUPDATE_8198C_FW
LDFLAGS = -lSDL_ttf -lSDL_image -lfreetype -lpng16 -lz $(SDL_LIBS)

EXE = animation
TAR = customer.tar

all: res $(EXE)
	mkdir -p tmp/res/
	cp $(EXE) tmp/
	cp res/*.png res/*.ttf tmp/res/
	cd tmp/ && tar cvf ../$(TAR) *
	rm -rf tmp/

res:
	make -C ./res

$(EXE): install_UI.o
	$(CXX) $< $(LDFLAGS) -static  -o $@
	$(STRIP) $(EXE)

install_UI.o: install_UI.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	make clean -C ./res
	rm *.o && rm $(EXE) $(TAR) init.exe

init: init.o
	$(CXX) $< $(LDFLAGS) -static -o $@.exe
	$(STRIP) $@.exe

.PHONY: res
