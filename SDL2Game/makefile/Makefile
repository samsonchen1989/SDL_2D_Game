################################################################
# A simple Makefile tested in
# Ubuntu 10.04.4 LTS
# Gcc version 4.4.3
# Make sure you install SDL2,SDL2_Image and zlib before compile
################################################################
PROG        = sdlgame
CC          = g++
CCFLAGS     = -I /usr/local/include/ -I /usr/local/include/SDL2/ -lSDL2 -lSDL2_image -lz
OBJS        = ../*.cpp ../base64/*.cpp ../tinyxml/*.cpp

all:     $(PROG)

$(PROG): $(OBJS)
	     $(CC) $(CCFLAGS) -o $(PROG) $(OBJS) -lstdc++ && \
	     cp -R ../assets ./
