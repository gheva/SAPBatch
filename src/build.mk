include $(strip ${BLDFILES})/functions.mk

ROOT := ..
DIR :=

SUBDIRS += wavfile
SUBDIRS += tapers
SUBDIRS += yin
SUBDIRS += fft
SUBDIRS += apps

# vim: set noexpandtab :

