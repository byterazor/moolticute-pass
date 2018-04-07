SUBDIRS = libmoolticute-c

bin_PROGRAMS=mpass

mpass_SOURCES  = src/get.c
mpass_SOURCES += yaap/src/argparse.c
mpass_SOURCES += src/main.c
mpass_SOURCES += src/ls.c
mpass_SOURCES += src/info.c

if HAVE_X11
  mpass_SOURCES+=src/x11_clipboard.c
endif

mpass_LDADD = libmoolticute-c/libmoolticute.a
