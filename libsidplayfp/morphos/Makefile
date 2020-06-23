CXX=ppc-morphos-g++-9
CC=ppc-morphos-gcc-9
PERL=perl
STRIP=ppc-morphos-strip
RANLIB=ppc-morphos-ranlib
RM=rm -rf
MKDIR=mkdir -p
CP=cp

PREFIX=/usr/local

CPPFLAGS = -I../src/builders/residfp-builder \
-I../src/builders/residfp-builder/residfp \
-I../src/builders/resid-builder \
-I../src/builders/resid-builder/resid \
-I../src \
-I../src/sidtune \
-I. \
-Ios-include \
-Iinclude \
-Iinclude/resid \
-Iinclude/residfp \
-std=c++11 \
-DHAVE_CONFIG_H \
-O2 \
-noixemul

CFLAGS= -I. \
-Ios-include \
-O2 \
-noixemul

LIBRARY_LDFLAGS = -L.\
-nostartfiles \
-noixemul

LDFLAGS = \
-noixemul

MD5SRC = \
../src/utils/MD5/MD5.cpp

libsidplayfp_a_SOURCES = \
../src/EventScheduler.cpp \
../src/player.cpp \
../src/psiddrv.cpp \
../src/mixer.cpp \
../src/reloc65.cpp \
../src/sidemu.cpp \
../src/c64/c64.cpp \
../src/c64/mmu.cpp \
../src/c64/VIC_II/mos656x.cpp \
../src/c64/CPU/mos6510.cpp \
../src/c64/CPU/mos6510debug.cpp \
../src/c64/CIA/interrupt.cpp \
../src/c64/CIA/mos6526.cpp \
../src/c64/CIA/SerialPort.cpp \
../src/c64/CIA/timer.cpp \
../src/c64/CIA/tod.cpp \
../src/sidplayfp/sidplayfp.cpp \
../src/sidplayfp/sidbuilder.cpp \
../src/sidplayfp/SidConfig.cpp \
../src/sidplayfp/SidInfo.cpp \
../src/sidplayfp/SidTune.cpp \
../src/sidplayfp/SidTuneInfo.cpp \
../src/sidtune/MUS.cpp \
../src/sidtune/p00.cpp \
../src/sidtune/prg.cpp \
../src/sidtune/PSID.cpp \
../src/sidtune/SidTuneBase.cpp \
../src/sidtune/SidTuneTools.cpp \
../src/utils/iniParser.cpp \
../src/utils/md5Factory.cpp \
../src/utils/SidDatabase.cpp \
$(MD5SRC)

libsidplayfp_a_EXTRAS = \
../src/psiddrv.bin \
../src/poweron.bin

libsidplayfp_a_LDFLAGS = -version-info $(LIBSIDPLAYVERSION) $(LDFLAGS)

libsidplayfp_a_HEADERS = \
../src/sidplayfp/siddefs.h \
../src/sidplayfp/SidConfig.h \
../src/sidplayfp/SidInfo.h \
../src/sidplayfp/SidTuneInfo.h \
../src/sidplayfp/sidbuilder.h \
../src/sidplayfp/sidplayfp.h \
../src/sidplayfp/SidTune.h \
../src/utils/SidDatabase.h

libsidplayfp_a_HEADERS2 = \
../src/sidplayfp/sidversion.h

libresidfp_a_CPP_SOURCES = \
../src/builders/residfp-builder/residfp/Dac.cpp \
../src/builders/residfp-builder/residfp/Integrator.cpp \
../src/builders/residfp-builder/residfp/EnvelopeGenerator.cpp \
../src/builders/residfp-builder/residfp/ExternalFilter.cpp \
../src/builders/residfp-builder/residfp/FilterModelConfig.cpp \
../src/builders/residfp-builder/residfp/Filter.cpp \
../src/builders/residfp-builder/residfp/Filter6581.cpp \
../src/builders/residfp-builder/residfp/Filter8580.cpp \
../src/builders/residfp-builder/residfp/FilterModelConfig8580.cpp \
../src/builders/residfp-builder/residfp/Integrator8580.cpp \
../src/builders/residfp-builder/residfp/OpAmp.cpp \
../src/builders/residfp-builder/residfp/SID.cpp \
../src/builders/residfp-builder/residfp/Spline.cpp \
../src/builders/residfp-builder/residfp/WaveformCalculator.cpp \
../src/builders/residfp-builder/residfp/WaveformGenerator.cpp \
../src/builders/residfp-builder/residfp/resample/SincResampler.cpp

libresidfp_a_CC_SOURCES = \
../src/builders/residfp-builder/residfp/version.cc

#=========================================================
# resid

libresid_a_CPPFLAGS = \
$(CPPFLAGS) \
-DVERSION=\"1.0-pre2\"

libresid_a_CC_SOURCES = \
../src/builders/resid-builder/resid/dac.cc \
../src/builders/resid-builder/resid/sid.cc \
../src/builders/resid-builder/resid/voice.cc \
../src/builders/resid-builder/resid/wave.cc \
../src/builders/resid-builder/resid/envelope.cc \
../src/builders/resid-builder/resid/filter.cc \
../src/builders/resid-builder/resid/extfilt.cc \
../src/builders/resid-builder/resid/pot.cc \
../src/builders/resid-builder/resid/version.cc

libresid_a_DAT_SOURCES = \
$(libresid_a_DATA:.dat=.h)

libresid_a_DATA = \
../src/builders/resid-builder/resid/wave6581_PST.dat \
../src/builders/resid-builder/resid/wave6581_PS_.dat \
../src/builders/resid-builder/resid/wave6581_P_T.dat \
../src/builders/resid-builder/resid/wave6581__ST.dat \
../src/builders/resid-builder/resid/wave8580_PST.dat \
../src/builders/resid-builder/resid/wave8580_PS_.dat \
../src/builders/resid-builder/resid/wave8580_P_T.dat \
../src/builders/resid-builder/resid/wave8580__ST.dat

#=========================================================
# builders
libsidplayfp_residfp_a_HEADERS = \
../src/builders/residfp-builder/residfp.h

libsidplayfp_residfp_a_SOURCES = \
../src/builders/residfp-builder/residfp-builder.cpp \
../src/builders/residfp-builder/residfp-emu.cpp

libsidplayfp_residfp_a_LIBADD = \
../src/builders/residfp-builder/residfp/libresidfp.a


libsidplayfp_resid_a_HEADERS = \
../src/builders/resid-builder/resid.h

libsidplayfp_resid_a_SOURCES = \
../src/builders/resid-builder/resid-builder.cpp \
../src/builders/resid-builder/resid-emu.cpp

libsidplayfp_resid_a_LIBADD = \
../src/builders/resid-builder/resid/libresid.a

#=========================================================
# libstilview
libstilview_a_SOURCES = \
../src/utils/STILview/stil.cpp

libstilview_a_LDFLAGS = -version-info $(LIBSTILVIEWVERSION) $(LDFLAGS)

libstilview_a_HEADERS = \
../src/utils/STILview/stil.h \
../src/utils/STILview/stildefs.h

extra_SOURCES = \
$(libresid_DATA:.dat=.h)

a65_SOURCES = \
../src/psiddrv.a65 \
../src/sidtune/sidplayer1.a65 \
../src/sidtune/sidplayer2.a65

LIBRARY_SOURCES = \
library.c \
libsections.c

LIBRARY_CPP_SOURCES = \
playsidcpp.cpp

libplaysidfp_a_SOURCES= \
lib_shared.c

#----------------------

all: playsidfp.library libplaysidfp.a example

libsidplayfp_a_CPPFLAGS = $(CPPFLAGS)
libsidplayfp_resid_a_CPPFLAGS = $(CPPFLAGS)
libresidfp_a_CPPFLAGS = $(CPPFLAGS)
libsidplayfp_residfp_a_CPPFLAGS = $(CPPFLAGS)
LIBRARY_CPPFLAGS = $(CPPFLAGS) -Istatic-include

libsidplayfp_a_OBJECTS = $(libsidplayfp_a_SOURCES:.cpp=.o)

libresid_a_OBJECTS = $(libresid_a_CC_SOURCES:.cc=.o)
libsidplayfp_resid_a_OBJECTS = $(libsidplayfp_resid_a_SOURCES:.cpp=.o)

libresidfp_a_CPP_OBJECTS = $(libresidfp_a_CPP_SOURCES:.cpp=.o)
libresidfp_a_CC_OBJECTS = $(libresidfp_a_CC_SOURCES:.cc=.o)
libsidplayfp_residfp_a_OBJECTS = $(libsidplayfp_residfp_a_SOURCES:.cpp=.o)

libplaysidfp_a_OBJECTS = $(libplaysidfp_a_SOURCES:.c=.o)
LIBRARY_OBJECTS = $(LIBRARY_SOURCES:.c=.o)
LIBRARY_CPP_OBJECTS = $(LIBRARY_CPP_SOURCES:.cpp=.o)

a65_OBJECTS = $(a65_SOURCES:.a65=.bin)

$(libsidplayfp_a_OBJECTS): %.o: %.cpp
	$(CXX) $(libsidplayfp_a_CPPFLAGS) -c $< -o $@

$(libresid_a_OBJECTS): %.o: %.cc
	$(CXX) $(libresid_a_CPPFLAGS) -c $< -o $@

$(libsidplayfp_resid_a_OBJECTS): %.o: %.cpp
	$(CXX) $(libsidplayfp_resid_a_CPPFLAGS) -c $< -o $@

$(libresidfp_a_CPP_OBJECTS): %.o: %.cpp
	$(CXX) $(libresidfp_a_CPPFLAGS) -c $< -o $@

$(libresidfp_a_CC_OBJECTS): %.o: %.cc
	$(CXX) $(libresidfp_a_CPPFLAGS) -c $< -o $@

$(libsidplayfp_residfp_a_OBJECTS): %.o: %.cpp
	$(CXX) $(libsidplayfp_residfp_a_CPPFLAGS) -c $< -o $@

$(libplaysidfp_a_OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBRARY_OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBRARY_CPP_OBJECTS): %.o: %.cpp
	$(CXX) $(LIBRARY_CPPFLAGS) -c $< -o $@

libresid.a: $(libresid_a_OBJECTS)

libresidfp.a: $(libresidfp_a_CPP_OBJECTS) $(libresidfp_a_CC_OBJECTS)

libsidplayfp-resid.a: libresid.a $(libsidplayfp_resid_a_OBJECTS)

libsidplayfp-residfp.a: libresidfp.a $(libsidplayfp_residfp_a_OBJECTS)

libsidplayfp.a: $(a65_OBJECTS) libsidplayfp-residfp.a libsidplayfp-resid.a $(libsidplayfp_a_OBJECTS)
	$(AR) -crv $@ $(libsidplayfp_a_OBJECTS) $(libsidplayfp_residfp_a_OBJECTS) $(libresidfp_a_CPP_OBJECTS) $(libresidfp_a_CC_OBJECTS) $(libsidplayfp_resid_a_OBJECTS) $(libresid_a_OBJECTS)
	$(RANLIB) $@
	$(MKDIR) static-include/sidplayfp/builders
	$(CP) $(libsidplayfp_a_HEADERS) static-include/sidplayfp/
	$(CP) include/sidplayfp/sidversion.h static-include/sidplayfp/
	$(CP) $(libsidplayfp_residfp_a_HEADERS) static-include/sidplayfp/builders/
	$(CP) $(libsidplayfp_resid_a_HEADERS) static-include/sidplayfp/builders/

libplaysidfp.a: $(libplaysidfp_a_OBJECTS)
	$(AR) -crv $@ $(libplaysidfp_a_OBJECTS)
	$(RANLIB) $@

gen_includes:
	$(MKDIR) os-include/proto os-include/ppcinline os-include/inline
	cvinclude.pl --fd=os-include/fd/playsidfp_lib.fd --clib=os-include/clib/playsidfp_protos.h --inline=os-include/ppcinline/playsidfp.h --proto=os-include/proto/playsidfp.h --vbccinlines=os-include/inline/playsidfp_protos.h

playsidfp.library: libsidplayfp.a $(LIBRARY_OBJECTS) $(LIBRARY_CPP_OBJECTS)
	$(CXX) $(LIBRARY_LDFLAGS) -o $@.db $(LIBRARY_OBJECTS) $(LIBRARY_CPP_OBJECTS) libsidplayfp.a -lc -lm
	$(STRIP) -o $@ --remove-section=-comment $@.db

example: example.c libplaysidfp.a
	$(CC) -noixemul -Ios-include -L. -o $@ example.c -lplaysidfp

#=========================================================
# Recreate psiddrv.bin, needs xa65
$(a65_OBJECTS): %.bin: %.a65
	o65file=`echo $@ | sed 's/bin/o65/'`;\
	xa -R -G $< -o $$o65file &&\
	od -v -An -w8 -tx1 $$o65file | sed -re 's/[[:alnum:]]+/0x&,/g' > $@ &&\
	${RM} $$o65file

#=========================================================
# For libresid JVJV: TODO
.dat.h:
	$(PERL) ../src/builders/resid-builder/resid/samp2src.pl $* $< $@

install: sidplayfp.library
	$(CP) libplaysidfp.a $(PREFIX)/lib/
	$(CP) libsidplayfp.a $(PREFIX)/lib/
	$(MKDIR) $(PREFIX)/include/sidplayfp/builders
	$(CP) $(libsidplayfp_static_a_HEADERS) $(PREFIX)/include/sidplayfp/
	$(CP) include/sidplayfp/sidversion.h $(PREFIX)/include/sidplayfp/
	$(CP) $(libsidplayfp_residfp_a_HEADERS) $(PREFIX)/include/sidplayfp/builders/
	$(CP) $(libsidplayfp_resid_a_HEADERS) $(PREFIX)/include/sidplayfp/builders/

uninstall:
	$(RM) $(PREFIX)/lib/libplaysidfp.a
	$(RM) $(PREFIX)/lib/libsidplayfp.a
	$(RM) $(PREFIX)/include/sidplayfp

clean:
	$(RM) example playsidfp.library* libplaysidfp.a libsidplayfp.a libsidplayfp-residfp.a libsidplayfp-resid.a libresidfp.a libresid.a $(LIBRARY_OBJECTS) $(LIBRARY_CPP_OBJECTS) $(libplaysidfp_a_OBJECTS) $(libsidplayfp_a_OBJECTS) $(libresid_a_OBJECTS) $(libsidplayfp_resid_a_OBJECTS) $(libresidfp_a_CPP_OBJECTS) $(libresidfp_a_CC_OBJECTS) $(libsidplayfp_residfp_a_OBJECTS) $(a65_OBJECTS) static-include

