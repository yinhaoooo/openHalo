#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for src/backend/adapter/mysql
#
# IDENTIFICATION
#    src/backend/adapter/mysql/Makefile
#
#-------------------------------------------------------------------------

subdir = src/backend/adapter/mysql
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global

OBJS = \
	adapter.o \
	errorConvertor.o \
	pwdEncryptDecrypt.o \
    netTransceiver.o \
    systemVar.o \
	userLogonAuth.o \
    uuidShort.o

include $(top_srcdir)/src/backend/common.mk
