CC?=gcc
PKG_CONFIG?=pkg-config

CFLAGS?=-g -O2 -Wall
LDFLAGS?=-lm  -lrt -lasound -ljack -lpthread -lportaudio


PREFIX?=/usr/local
DESTDIR?=


CLIB = 
# arch hardening
#CPPFLAGS+=-D_FORTIFY_SOURCE=2
#CFLAGS+=-march=x86-64 -mtune=generic -O2 -pipe -fno-plt
#LDFLAGS+=-Wl,-O1,--sort-common,--as-needed,-z,relro,-z,now

# arch & debian hardening workaround
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
        LDFLAGS+=-Wl,--no-as-needed
endif


#CFLAGS+=$(shell $(PKG_CONFIG) GL --cflags)
#LDFLAGS+=$(shell $(PKG_CONFIG) GL --libs)

#CFLAGS+=$(shell $(PKG_CONFIG) math --cflags)
#LDFLAGS+=$(shell $(PKG_CONFIG) math --libs)

#CFLAGS+=$(shell $(PKG_CONFIG) libportaudio --cflags)
#LDFLAGS+=$(shell $(PKG_CONFIG) libportaudio --libs)


all: main

install-deps:
	mkdir -p lib

	curl https://files.portaudio.com/archives/pa_stable_v190600_20161030.tgz | tar -zx -C lib
	cd lib/portaudio && ./configure && $(MAKE) -j
.PHONY: install-deps
