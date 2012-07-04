
STKDIR=../stk-4.4.3

CC=g++
CXXFLAGS=-I$(STKDIR)/include -DHAVE_GETTIMEOFDAY -D__MACOSX_CORE__ -D__LITTLE_ENDIAN__
LDFLAGS=-L$(STKDIR)/src
LDLIBS=-lstk -framework CoreAudio -framework CoreFoundation -framework CoreMidi

stkprogram: stkprogram.o

crtsine: crtsine.o